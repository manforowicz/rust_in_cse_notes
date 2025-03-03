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

Let's try to modify it, so that each thread modifies `data`:

```rust,no_compile
use std::thread;
use std::time::Duration;
use std::sync::Arc;

fn thread(data: Arc<Vec<u32>>) {
    data.push(4);
    println!("Now the data is: {guard:?}.");
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

This doesn't compile because it would violate the borrow checker's **aliasing rule**:
> - One or more shared ("immutable") references to a value, *or*
> - Exactly one exclusive ("mutable") reference to a value.

Each `thread()` has a reference to `data`, which means all the reference must be "immutable".

But `.push()` expects a mutable reference.

The compiler just saved us from a data race.

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