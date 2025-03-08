# Data Race Prevention

Recall this program from the previous page:

```rust
use std::thread;
use std::time::Duration;
use std::sync::Arc;

fn thread(data: Arc<Vec<u32>>) {
    println!("Data during thread: {data:?}");
}

fn start_threads() {
    let data_ptr0 = Arc::new(vec![1, 2, 3]);

    let data_ptr1 = data_ptr0.clone();
    let data_ptr2 = data_ptr0.clone();

    thread::spawn(|| thread(data_ptr1));
    thread::spawn(|| thread(data_ptr2));
}

fn main() {
    start_threads();
    thread::sleep(Duration::from_millis(10));
}
```

Let's try to change it, so that each thread modifies `data`:

```rust,compile_fail
use std::thread;
use std::time::Duration;
use std::sync::Arc;

fn thread(data: Arc<Vec<u32>>) {
    data.push(4);
    println!("Now the data is: {data:?}.");
}

fn start_threads() {
    let data_ptr0 = Arc::new(vec![1, 2, 3]);

    let data_ptr1 = data_ptr0.clone();
    let data_ptr2 = data_ptr0.clone();

    thread::spawn(|| thread(data_ptr1));
    thread::spawn(|| thread(data_ptr2));
}

fn main() {
    start_threads();
    thread::sleep(Duration::from_millis(10));
}
```

This could cause a data race,
and doesn't compile because it would violate the borrow checker's **aliasing rule**:
> At any time, a value may be referred to by
> - one or more shared ("immutable") references, *or*
> - exactly one exclusive ("mutable") reference.

Each `thread()` must have a shared ("immutable") reference to `data`,
but `.push()` requires a mutable reference.

Is there some way for a `thread()` to temporarily get a mutable reference to `data`, as long as
no other threads reference it at the same time? Yes!

We wrap `data` in a mutex, and give each thread a shared ("immutable") reference
to that `Mutex` (still using `Arc`).

Then, when a thread wants to access `data`, it calls `lock()` on the `Mutex`.
`lock()` returns a `MutexGuard`, which is just a mutable smart pointer to `data`.

The only thing special about `MutexGuard`, is that once it's dropped,
the `Mutex` automatically unlocks.

We can rewrite this code using mutexes to prevent the data race:

```rust
use std::thread;
use std::time::Duration;
use std::sync::{Arc, Mutex};

fn thread(data: Arc<Mutex<Vec<u32>>>) {
    let mut guard = data.lock().unwrap();
    guard.push(4);
    println!("Now the data is: {guard:?}.");
    // Automatically releases the lock when `guard` drops.
}

fn start_threads() {
    let data_ptr0 = Arc::new(Mutex::new(vec![1, 2, 3]));

    let data_ptr1 = data_ptr0.clone();
    let data_ptr2 = data_ptr0.clone();

    thread::spawn(|| thread(data_ptr1));
    thread::spawn(|| thread(data_ptr2));
}

fn main() {
    start_threads();
    thread::sleep(Duration::from_millis(10));
}
```