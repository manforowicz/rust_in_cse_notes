# Dangling Pointer Prevention

Let's say we want multiple threads to have a reference to the same `data`.
So `start_threads()` gives each thread a shared ("immutable")
reference to `data`.
Let's see if it compiles:

```rust,compile_fail
use std::thread;
use std::time::Duration;

fn thread(data: &Vec<u32>) {
    println!("Data during thread: {data:?}");
}

fn start_threads() {
    let data = vec![1, 2, 3];
    thread::spawn(|| thread(&data));
    thread::spawn(|| thread(&data));
}

fn main() {
    start_threads();
    thread::sleep(Duration::from_millis(10));
}
```

This doesn't compile because it violates the borrow checker's **lifetime rule**:

> "A reference cannot outlive a value it borrows."

That's because `data` is implicitly `drop()`ed at the end of `start_threads()`, but
the `thread()`s still have references it.

In C, this would be a "dangling pointer" or "use after free" bug.

## Arc

We want `drop(data)` to only be called once no one has a reference to it.
We can do this by wrapping `data` in an `Arc` (atomic reference counter).

Each thread will own an `Arc` smart pointer, all pointing to the same `data`.

When `Arc` is `clone()`ed, its reference count is incremented.
When it's `drop()`ed, its reference count is decremented.

Once its reference count reaches zero, `drop(data)` is called.

Why `Arc` instead of `Rc`?
`Rc` is not `Send`, so it can't be moved between threads.
That's because the reference counter of `Rc` is non-atomic.

We can rewrite our code like so:

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
