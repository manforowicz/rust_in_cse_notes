# Send and Sync

TODO

```rust
use std::thread;
use std::time::Duration;

fn say_hi()

fn main() {
    let handle1 = thread::spawn(move || {
        println!("Thread {} is running", i);
        thread::sleep(Duration::from_secs(1));
        println!("Thread {} is done", i);
    });

    for handle in handles {
        handle.join().unwrap();
    }
}
```