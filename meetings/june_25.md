# Meeting - 25 June 2025

- Question: should event loop assignment be in C or Rust?
    - C is more real-world-applicable, because in Rust you'd just use `async` with a runtime like [Tokio](https://docs.rs/tokio/).

- Question: which non-blocking API should I use? epoll or io_uring?
    - epoll is "reactor/poll-based"
        - you wait until an operation can be done, and then you do it yourself in non-blocking mode.
        - epoll is conceptually more similar to Rust's [`Waker`](https://doc.rust-lang.org/stable/std/task/struct.Waker.html) and [`poll()`](https://doc.rust-lang.org/stable/std/future/trait.Future.html#tymethod.poll) model.
    - io_uring is "proactor/continuation-based"
        - you submit an operation, and wait on a queue of operations completed by the OS.
        - Is slightly more performant, and more modern than epoll.
    - I implemented echo servers using both APIs in the [event-loop-lab](https://gitlab.cs.washington.edu/cse334/event-loop-lab) repository.
 
- [Interesting article](https://www.usenix.org/legacy/events/hotos03/tech/full_papers/vonbehren/vonbehren.pdf) I found suggests just spawning a thread per connection on modern systems which can handle tens of thousands of threads. I [verified](https://github.com/manforowicz/rust_in_cse_notes/blob/main/c_playground/thread_stress_test.c) that 15,000 threads only uses 125MB on my computer.
    - This [blog post](https://kobzol.github.io/rust/2025/01/15/async-rust-is-about-concurrency.html) explores the benefits that "cancellation" offers. Specifically, you can set a timeout on any async function call, without knowing what's inside. We could have the async lab lean into that cancellation aspect.

- Random questions:
    - Do we have any assignments that will cover primitives like condition variables?
    - Should we also include signals and signal handlers as a method sometimes used in concurrency?
    - 

- Ideas for things I can work on:
    - Write readme for event-loop lab, polish, and add skeleton code.
    - Rewrite the async server lab to include key async use-cases like timeouts and [select](https://docs.rs/tokio/latest/tokio/macro.select.html).
    - Make slides for event loops and async.
    - 