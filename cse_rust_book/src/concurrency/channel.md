# Channel

As we've already learned, eon way for threads to communicate with each other
is by having access to shared data, using `Arc<Mutex<Data>>`.

Another way is through channels. `mpsc` stands for "Multiple Producer Single Consumer",
which means that there can be many `Sender`s but only one `Receiver`.

Channels in Rust are similar to channels in other languages:

```rust
use std::sync::mpsc;

fn main() {
    let (tx, rx) = mpsc::channel();

    tx.send(10).unwrap();
    tx.send(20).unwrap();

    println!("Received: {:?}", rx.recv());
    println!("Received: {:?}", rx.recv());

    let tx2 = tx.clone();
    tx2.send(30).unwrap();
    println!("Received: {:?}", rx.recv());
}
```