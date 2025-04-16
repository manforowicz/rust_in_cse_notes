# Meeting - 9 April 2025

## Deadlock Lab Requirements

One of the things I think students should learn in lab 4 or 5 is avoiding deadlock.

Deadlock is only possible when there's more than 1 lock, so this lab should require multiple locks (fine-grained locking).

We shouldn't just arbitrarily require students to use fine-grained locking.
Instead, our test suite should only pass if students used fine-grained locking under the hood.

Well, fine-grained locking has 1 detectable advantage over global locking: performance.
But how do we structure the lab to make the performance improvement of fine-grained locking actually noticable?

For example, the access pattern "lock, update, unlock" is unlikely to be a bottleneck.
On the other hand, the pattern "lock, wait for something, update, unlock" could cause a bottleneck, since locks are held for more time.

So we should structure the lab to require waiting for something while holding multiple locks.
What we wait for must depend on all the locked values themselves, because otherwise we could just wait without holding the locks.
Here are some general things that code can wait on:
- Waiting for a CPU-bound workload to finish.
- Waiting for a file read/write.
- Waiting for a network read/write.

However, to make it easy for our test suite to control this waiting,
I think we should abstract this by having the library wait for
a user to call a library function.

## Mutable Set Server Lab

What if the students implement a server that has a set of objects, and handles requests.
Each client can request to read or modify any subset of these objects.
But what if a user wants to read, and then write something that depends on what they just read?
If they do this as 2 separate operations, we'll have a TOCTOU bug.
Therefore, the server should support a combined operation, like:

1. Client requests to do an operation on a certain subset of objects.
2. Server acquires locks and responds with the current values of those objects.
3. Client sends what they'd like to write to those objects.
4. Server completes operation, releases locks, and responds with success.

Hopefully this lab design forces multiple locks to be held while waiting for the client response.
And hopefully a global lock would make this noticably slower.

Now I need to turn these "objects" and "requests" into a plausible real-world application.


## Cuckoo Hash Notes

I left some notes on Cuckoo hash:
<https://github.com/thisisyiqing/cuckoo/pull/1>


## Other things

I found a free book about locks and atomics in Rust:
<https://marabos.nl/atomics/>

It could partially serve as a course textbook.


## Notes on Oleg's Lab

When I was a student in CSE 333, I wasn't a huge fan of having to write serialization/deserialization code when there are already many existing libraries that do this!

Minimize things that aren't unique to Rust (parsing?)
Do we have a list of concepts we want to cover (that are unique to Rust)?
- Box
- Iterator
- Closure
- Pattern matching
- &mut and &

People often say (and there's even an article about this): don't learn Rust by writing data structures. Data structures are difficult with ownership, and aren't representative of normal programs you'd write.

Tom's idea: Give them existing code, and have them modify it. But don't make it just "fill in the blanks"

Tom: Students can circumvent test cases by writing some trivial code.

Tom: Some kind of interface in the test code that allowed you to suspend a thread. Forced wait while holding.
Tom: Cornell class where students hand packets to a work queue, which only passed the tests if it was parallelized.

## Other live notes

Maybe we don't need to wait while holding locks and the performance difference will be visible either way?

Do we need to teach explicit lifetimes?

Add test to loom that sees if it catches 