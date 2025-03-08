# Ordered Locking

Does Rust prevent deadlocks?
Let's see if this obviously deadlock-prone code compiles:

```rust,no_run
use std::thread;
use std::sync::{Arc, Mutex};

fn main() {
    let data1_ptr1 = Arc::new(Mutex::new(vec![1, 2]));
    let data1_ptr2 = data1_ptr1.clone();

    let data2_ptr1 = Arc::new(Mutex::new(vec![3, 4]));
    let data2_ptr2 = data2_ptr1.clone();

    let handle = thread::spawn(move || {
        let guard1 = data1_ptr1.lock().unwrap();
        let guard2 = data2_ptr1.lock().unwrap();
        println!("Thread has data: {guard1:?}, {guard2:?}");
    });

    let guard1 = data2_ptr2.lock().unwrap();
    let guard2 = data1_ptr2.lock().unwrap();
    println!("Program has data: {guard1:?}, {guard2:?}");

    handle.join().unwrap();
}
```

Oh no, it compiles!
By default, Rust doesn't prevent deadlocks.

How can we identify code that causes deadlocks?
We can draw a graph, where each vertex corresponds to holding a mutex.
Each edge represents holding some mutex, and then acquiring another mutex.
Deadlocks are possible whenever there's a cycle in this graph.

We can eliminate cycles in this graph by limiting what order mutexes are acquired in.
Specifically, we can assign each mutex a number, and ensure that mutexes of smaller numbers
cannot be locked while holding mutexes of greater numbers.

Turns out, we can write a library that uses Rust's type system to enforce this at compile time:

```rust
mod ordered_locking {
    use std::{marker::PhantomData, sync::MutexGuard};

    /// Proof that `L` is the greatest Mutex held by this thread/task.
    ///
    /// Pass into [`Mutex::lock`] to prove you can acquire the lock.
    pub struct LockProof<'a, const L: i64> {
        /// Tells the compiler that this [`LockProof`]
        /// lives for lifetime `'a`.
        _dummy: PhantomData<&'a ()>,
    }

    impl LockProof<'_, { i64::MIN }> {
        /// Returns a proof that you're holding no locks.
        ///
        /// Only call at the start of each concurrent task/thread
        /// when you're actually holding no locks.
        pub fn unlocked_state() -> Self {
            Self {
                _dummy: PhantomData,
            }
        }
    }

    /// Mutex which enforces a locking order.
    pub struct Mutex<T, const L: i64> {
        mutex: std::sync::Mutex<T>,
    }

    impl<T, const L: i64> Mutex<T, L> {
        /// Create a new ordered mutex in the unlocked state.
        pub fn new(t: T) -> Self {
            Self {
                mutex: std::sync::Mutex::new(t),
            }
        }

        /// Lock the mutex.
        pub fn lock<'a, const PREV_L: i64>(
            &'a self,
            _proof: &'a mut LockProof<PREV_L>,
        ) -> (MutexGuard<'a, T>, LockProof<'a, L>) {
            const {
                assert!(
                    PREV_L < L,
                    "Mutexes must be locked in strictly increasing ID order."
                )
            }
            (
                self.mutex
                    .lock()
                    .expect("Mutex can't be locked because it was poisoned."),
                LockProof {
                    _dummy: PhantomData,
                },
            )
        }
    }
}

use ordered_locking::{LockProof, Mutex};

fn main() {
    let mut unlocked_proof = LockProof::unlocked_state();
    let mutex1: Mutex<u32, 1> = Mutex::new(32);
    let mutex2: Mutex<u32, 2> = Mutex::new(32);

    let (mut guard1, mut proof1) = mutex1.lock(&mut unlocked_proof);
    let (mut guard2, proof2) = mutex2.lock(&mut proof1);

    *guard1 += 5;
    *guard2 += 5;
}
```
