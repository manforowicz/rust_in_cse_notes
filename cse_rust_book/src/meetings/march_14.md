# Meeting - 14 March 2025

## Loom Follow-up

I don't think the first example in the [Loom docs](https://docs.rs/loom)
is unsafe, because there is no `unsafe` keyword.
I think atmoics are safe because they don't allow data races or other undefined behavior.
The Rust Reference has a [definition of what it means by undefined behavior](https://doc.rust-lang.org/reference/behavior-considered-undefined.html).

The `loom::model` takes a function as a parameter, and calls that given function many times.
Each time it tweaks some global settings of its locks and atomics, to cause a different ordering.

## Loom in Cukoo hash

Testing Cukoo with Loom should be doable in a few hours!
We would first need to write some tests with assertions.

## First Rust lab

For the first Rust lab, do you think something like the [multithreaded server in the Rust book](https://doc.rust-lang.org/book/ch21-03-graceful-shutdown-and-cleanup.html) would work, with some adaptation?
That server only uses one mutex, to essentially convert an mpsc to an mpmc to hand out tasks to the thread pool.
We could add more mutexes to make it harder?

Hmm, maybe we can have them write an iterator that spews out network requests?
Maybe this would be a good lead-in to Async?

## Second Rust Lab

I think it would be a good idea to use an ordered-locking API to teach deadlock-prevention.


## What I'll work on

I didn't get much done this week due to algorithms, electromagnetism, and TAing.
Here are some things I'd like to try to do before the next meeting:
- Write some test cases for [cukoo hash table](https://github.com/thisisyiqing/cuckoo) and run them through Loom,
    to get a better sense for the tool.
- In the previous meeting, I got a lot of good feedback on how to introduce the concept of async.
    I'll try to create a presentation, or at least an outline of a presentation that implements these ideas,
    using the [Why Async Rust](https://without.boats/blog/why-async-rust/) approach.
- Next quarter I'm taking 12 credits + TA'ing, so hopefully I'll have more time to work on this!
- PS: Is there any possibility of turning my [TCP-hole-punching file transfer project](https://github.com/manforowicz/gday) into research (whatever that means ;)?
- 

## Priority items

Prioritize making labs over slides right now.