# Async

Thread example:

```rust
use std::thread;
use std::time::Duration;

fn say_hi() {
    for i in 1..5 {
        println!("hi number {i} from the spawned thread!");
        thread::sleep(Duration::from_millis(1));
    }
}

fn main() {
    let handle = thread::spawn(say_hi);

    for i in 1..5 {
        println!("hi number {i} from the main thread!");
        thread::sleep(Duration::from_millis(1));
    }

    handle.join().unwrap();
}
```

Async example:

```rust
use tokio::time::{sleep, Duration};

async fn say_hi() {
    for i in 1..5 {
        println!("hi number {i} from the spawned task!");
        sleep(Duration::from_millis(1)).await;
    }
}

#[tokio::main]
async fn main() {
    let handle = tokio::spawn(say_hi());

    for i in 1..5 {
        println!("hi number {i} from the main task!");
        sleep(Duration::from_millis(1)).await;
    }

    handle.await.unwrap();
}
```

All `async fn` will immediately return a `Future` instead of the listed return value.

- A `Future` will only output the listed return value once the function finishes.

- A `Future` is a `struct` state-machine which holds the current state of the function in it.
    Instead of variables being stored on the stack, they're stored as fields in the `Future`.

- A `Future` may hold other `Future`s if it called `await` on them in the code.

- A `Future` doesn't run on its own. It needs to be polled to completion by repetitively calling `poll()` on it.
    Each `poll()` will run the future as far as possible, also running `Future`s contained within it.
    If it doesn't finish, it configures a waker that will wake the caller of `poll()` when the future is ready to be called again.

Thankfully we don't need to call `poll()` ourselves, because we're using [tokio](https://docs.rs/tokio/), which does this for us.
Tokio repeatedly calls `poll()` on the `Future` returned by `main()`, and any `Future`s passed to `tokio::spawn`.


## Why Async?

That's a lot of complexity, for what?
Async gives us three benefits:

TODO

### Reduced context switch time

The async executor (like tokio) just keeps calling `poll()` so there's no need for context switch overhead like saving registers and things like that.

### Reduced memory usage per task

As mentioned earlier, each `Future` is a state machine which holds variables and other `Future`s as fields. This means that `async` functions actually don't have stacks! This is very helpful when running a server with millions of tasks.

### Cancellation

Futures can be cancelled by simply `drop()`ing them. Rust guarantees this will call `drop()` on all the contained fields, cleanly cancelling the future.

On the other hand, threads are very difficult to safely cancel in Rust. TODO

## Why not Async?

Of course, there are reasons why we don't use async everywhere.

IO-bound vs CPU-bound. C/C++ interop. TODO