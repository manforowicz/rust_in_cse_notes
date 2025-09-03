# Meeting - 30 July 2025

## Email summary

My 1st email's question is resolved by combining the memory and directory device.
It was caused by ambiguity over when the directory should do cache-to-cache transfers vs. fetching from memory.

My 2nd email's question is resolved by combining the I->M and S->M messages for simplicity.
It was caused by ambiguity about what happens when a cache sends S->M, and is then receives an invalidate message.

My 3rd email's questions were also resolved: no leases, and only one cache level.

## Current architecture

Current architecture of this lab is a memory+directory device that talks to caches,
and each cache talks to a processor which is given commands by the test suite.

## Executor

Initially, I thought it would be cool to run real multi-threaded algorithms, where each thread is a CPU, on top of the cache coherence simulator.
But your message clarified that's not a good idea because it requires atomic commands and knowledge of memory consistency,
which greatly increases complexity.
So I'll keep the whole thing single threaded.

How the "executor" works:

The `SystemState` struct contains memory, cache, and processor objects, as well as in-flight messages.
It has a `deliverMsg` method that takes a messages, calls the right object's handler, and appends the resulting messages to the in-flight buffer.
As a side note, `deliverMsg` is kind of ugly because there are multiple message types, so it uses a match.

We could make a simple executor that always pops the top message and calls `deliverMsg` method of the state.

But the model checker does a DFS where each node is a state, and each vertex is which message of that state is sent next.

## Vacation note

I'll visit family in Poland from August 6 to August 29.

## Meeting Notes

There's a special crate, probably call "enum derive" that could make the message enums less annoying. It takes the best of both worlds of dynamic dispatch and enums.

Good to have option to run linear code instead of model checker if we want to.

Maybe use dynamic dispatch with special hash crate.

Generic-parametrizing the size of blocks would be a bit tricky. We should have a constant for number of bytes in a block, and remove the magic number 64 in code.

u32 write function should just panic if the block size is smaller than 4 bytes.

Should allow multiple states to be preserved across states in the model checker.

Progress bar or estimated time for model checker.

There are other things to test (like excessive number of messages) than just commands issued to CPUs.

Get other statistics out of executor.run() and run assertions on those.

Frontier of states could be nice, but catch_unwind without line numbers would also be nice. Either ways is fine. Would be nice for there to be 2 modes of execution. Model checker yes and no. Could be somewhat combined if I want.

Maybe a mode where you can program a specific message delivery order to reproduce a single bug. "Give me the current message queue, and let me choose one to send".

Mode where you inject messages directly.

Or mode where failed model checker outputs log file, and then that specific execution sequence can be reproduced linearly.

Be creative and maybe make a unified solution for sequential and model-checking tests.

Build a readme, and separate out the skeleton from the code to implement.

Memory is a misnomer in this project cause it's the memory controller not the memory itself?

Matthew says it would be nice to have a separate memory device with only read and write. That's because more in-flight messages means more concurrency complexity.
He says we can make it more deterministic by specifying the order in which the directory should try fetching the existing block from CPUs on an I->S. Or having it always fetch from memory would also be fine.
