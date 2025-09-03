# Meeting - 2 July 2025


## Updates

- [Mio](https://docs.rs/mio/) (Rust's cross-platform epoll-equivalent) is more clunky due to lack of file descriptors, which requires having a hashmap from tokens to connections ([example](https://github.com/tokio-rs/mio/blob/master/examples/tcp_server.rs)). But this isn't that big of a deal.
- [rust-course-fei](https://github.com/Kobzol/rust-course-fei/blob/main/lessons/09/exercises/src/lib.rs) encourages just using Rust's epoll wrapper.

- For async lab, hard to create a design that forces spawning many async tasks. Instead, we can add timeout requirements, etc. which higlight other benefits of async like cancellation (select!, timeout, etc.).

- I don't know much about cache coherence, so I'll have to be guided along the way.

## Meeting Notes

- MSI protocol (the equivalent of Rust's ).
- Extension is MOESI protocol.
- 

- CPU is in the test suite (as opposed to student implementation).
- We only really need students to implement a single cache line to get the point across.
- We might provide DRAM which has timings. DRAM plugs into test suite to check if students are doing the right thing.
- We provide the queues that link up the caches.
- Maybe asks students to expose the current state of the cache line in each cache.
- Each cache serves requests in the order they came in.
- We should give our own "epoll-like" API that doesn't actually use sockets. This might allow us to plug Loom into this lab.
- We provide global clock code. Every cycle it calls a "tick" function for each cache.

Topology:
- Every CPU has one cache. CPUs cannot talk to each other but caches can talk to each other.
- Only a single layer of caches. So each CPU has its cache, and then it's memory.
- Maybe non-uniform topology where delays between caches are different.

- Case: two CPUs try to write to a single cache line at the same time.
- Maybe have two cache lines so that we can implement a lock on top of this simulator. We might need atomics for this.

- CPUs only issue read and writes requests.
- See if adding delays add extra complexity.
- Cache lines are 64 bytes, but CPU writes are only 8 bytes. No race conditions. But cache false sharing.
- Cache should stay in the M state, rather than re-broadcasting every time.
- We might need a "tick()" function (unlike distributed systems, we do have access to real time here). Look into if that makes a difference here. Try the lab without any notion of time.
- Demonstrate helpfulness of Rust's sum-types.
- Intermediate state of broadcasting and waiting for messages to return.
- Messages can be arbitrarily delayed maybe up to a boundary.

Messages:
- Invalidate request
- Invalidate response
- Memory read/write request/response
- Read request to see if a value is already in a different cache for performance.

https://en.wikipedia.org/wiki/MSI_protocol

https://en.wikipedia.org/wiki/MOESI_protocol
