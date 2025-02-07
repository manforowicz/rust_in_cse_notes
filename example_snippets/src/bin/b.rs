use std::thread::spawn;

fn main() {
    println!("Hello, world!");

    let mut counter = 0;
    

    let handle = spawn(|| func(&mut counter));

    handle.join();

    counter += 1;

    println!("Counter is: {counter}");
}

fn func(counter: &mut u32) {
    *counter += 1;
}