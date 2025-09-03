# Meeting - 8 July 2025

## My understanding of the MSI simulator

- The case where two caches want to enter the M state simultaneously is resolved by bus arbitration.
  That is, only 1 message can be sent on the bus per tick.

- When some caches are in the S state, and a cache is switching from the I to S state, it'll send a BusRd.
  No other cache will reply (because to prevent flooding the bus, only caches in the M state can provide cached blocks).
  After hearing silence for a few ticks, the memory contorller will reply to the BusRd message.
  We'll provide this memory controller to the students.
  So delays need to be at least somewhat deterministic.

- It seems that MSI/MESI are defined in terms of a snoopy architecture, but they can also be done with a directory-based architecture.
  I think this lab can stick with snoopy for simplicity and since snoopy with "snoop-filters" is often used in practice.

- Each cache will have a `tick(&mut self, bus: &mut Bus, cpu: &mut CPU)` method that is called on every tick.
  The `Bus` will have hold one value at a time, and have `recv()` and `try_send()` methods.

- I think in C++ accessing shared memory without atomics/locks is undefined behavior.
  So I wonder why standard non-atomic cache coherence requirements aren't way more relaxed, since atomics will have to be used anyway.
  - Answer: False sharing. Read-only sharing is very common (especially with things like libc). We still have to guarantee cache coherence for data, when locking, even though accessing the data isn't done with special instructions.

## Async lab thoughts

- Spawning tasks to do one thing after a timer. Demonstrates cheapness.
- Having timeouts on waiting 

## Meeting notes

- Snoopy protocols are inefficient compared to directory-based approaches.
- Tom advocates for directory-based approach.
- Tree of directory servers all the way down is how it's implemented in modern systems.
- Groups of 4 CPUs on a shared bus connected to a directory, with then directories all the way down.
- Directory would be implemented as a `HashMap<MemoryBlock, State>`.
- Acks are more common in directory-based systems.
- Tom says we should abstract this more from hardware. Don't bother ourselves with ticks.
- Tomosulo's algorithm for logical vs. hardware registers.

