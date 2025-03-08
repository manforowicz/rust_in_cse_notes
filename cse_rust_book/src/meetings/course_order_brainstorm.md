# Course Brainstorming

Marcin's notes about a concurrency course with Rust.

## Why we should go from high-level to low-level

Initially I favored starting at a low-level with C, and building from scratch primitives used in later higher-level Rust labs.
At first glance, this seems like a natural progression.

However, I changed my mind. In order to write bug-free low-level C concurrency primitives, you need to already have a well-developed concurrency mindset. Otherwise, you'll struggle to spot edge cases early, and not understand why certain design choices are made.

Developing that concurrency mindset in low-level C is difficult, because of undefined behavior, edge cases, etc.
Doing so is easier in Rust, because Rust has a friendly compiler that nudges you away from mistakes, and prevents you from having to debug them later on.

## Progression

Therefore, I think the course's progression should go like:

1. Learn concurrency in Rust. This should utilize chapters from the [official Rust book](https://doc.rust-lang.org/book/).
    1. Installation, compilation, basics (chapters 1-2)
    2. Ownership (chapter 4)
    3. Pattern-matching (chapter 6)
    4. Error handling (chapter 9)
    5. Iterators & closures (chapter 13)
    6. Concurrency, using threads (chapter 16)
    7. Putting it all together, in a simple multi-threaded Rust server (chapter 20).
    By now, students should have a very robust understanding of threads, mutexes, atomic vs non-atomic, and reference counters.

2. Introduce parallelizable iterator operations like fold, for each, etc.
    1. Students write one of these operations by hand, using threads, which further trains them in concurrency.
    2. Students re-write it using the higher-level [Rayon](https://docs.rs/rayon/) abstraction.

3. Now that they've developed a concurrency mindset, students can learn how concurrency works under the hood, using C.
    1. In C, implement the concurrency primitives used in the previous phase.
    2. In C, implement user-level threads.
    3. In C, implement an event loop. Motivated as a lower-overhead alternative to threads.

4. Async Rust introduced as syntactic sugar for event-loops.
    1. Write a ~100 line async executor, just like [Pollster](https://docs.rs/pollster/).
        This should introduce students to futures, polling, and wakers.
    2. Rewrite the Rust server from earlier, to use async, using [Tokio](https://docs.rs/tokio) as the executor.
        Show that now it uses much less memory per task!

5. Overview of concurrency in other languages.
    1. What is async in Javascript? How is it different from async in Rust?
    2. What are green threads in Go?

