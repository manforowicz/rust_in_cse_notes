# Threads

Rust lets you use normal OS threads, just like pthreads in C.
Press play to run:

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