# Meeting - 7 March 2025

## Exhaustive interleaving testing

Can use [Loom](https://docs.rs/loom/) to exhaustively test all possible interleavings.
- Must use Loom's concurrency primitives.
- Can swap std primitives using some module and cfg trickery.

## Work in progress

- I want to introduce async using this motivation: https://without.boats/blog/why-async-rust/

Two axes of coroutines: stackful vs stackless, pre-emptive vs cooperative

## Rust Concurrency Project Goals

- Granular locking (aka multiple locks) with deadlock risk.
    Maybe let students use my ordered locking library I showed last time.
- Some CPU-bound workloads. Demonstrates threads, and pre-emption.
- Some IO-bound workloads. Demonstrates async and event-loops.
- Actually interesting. Maybe backend for a website?

Maybe students can build cookoo hash table for this?

## Rust Concurrency Project Ideas

### Proxy with cache


For the cache, how do we force more granularity than just: `Mutex<HashMap>`. 

What if we have one cache for Javascript, one cache for HTML, and one cache for files?
We can motivate them by having different expiration times for each of these content types.
Like images don't change often, but maybe HTML does.
And we have a task that periodically flushes items from each of the caches.

And then we can also have multiple locks for logs like access date, access IP, etc.

For simplicity, maybe the "proxy" can run on students' laptops, but still proxying browser requests.


### Collaborative online text editor

How can we force more granularity than `Mutex<String>`?

What if it's a spreadsheet, and each row and column or cell somehow have their own lock?

Or a collaborative image painter, where each user selects rectangles they currently "own" and freely draw things there.

But still 

We provide them with the javascript front-end. They write the server's API functions.

## Notes

Tom: separating semantics of things like critical sections, with the tools that can fulfill them (like spinlock vs sleeplock).

We shouldn't assume that people will be using Rust.
We should give them a mental framework accessible to various languages.

Async as an example of more waiting happening at the same time, as opposed to a bunch of threads blocking.
Lightweight fibers/futures are a way of having more waiting happening at the same time.

Saving progress of work, so we can go back to it later.

I should first explain event loops, then callbacks, then Rust async/await.

Syntax that looks imperative, but is actually callbacks.

Continue to utilize precious thread stack space when waiting on IO.

Async is all about waiting on many things at once.

Async system calls, select(), poll(), etc.