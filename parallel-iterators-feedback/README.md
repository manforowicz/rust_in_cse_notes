# Parallel Iterators Lab Feedback

Sorry for taking so long to provide feedback on this lab.

I pushed my implementation into a `marcin-solution` folder in `main`.

As usual, feel free to ignore any of these.

- Rename "parallel-iterators" repository to something like "thread-pool".

- I think the cache coherence lab should come before the thread pool lab.
  I think the cache coherence lab uses an easier subset of the thread pool lab's concepts.
  So I think it'd be beneficial to put the cache coherence lab first to sequentially ease students in.

- Students are expected to have a queue per worker rather than a shared queue.
  Do tests check this, or will we manually grade this?

- Somewhere we'll want to tell students why we need traits instead of C-style
  function pointers when using closures, and why we need separate `FnOnce`, `FnMut`, and `Fn`.
  This doesn't need to be in the readme - it could be in slides or an appendix.

- Does the `RF` type parameter stand for "return function"? I like just `R` for "return" more,
  but I'm bikeshedding.

- I would remove the separate `ThreadPool.stop()`, and have it all done in `drop()`.
  Having a separate `stop()` means `ThreadPool` has 2 states: running and stopped.
  Combining everything in `drop()` means any valid `ThreadPool` object must be running, I think.

- Make it clear that dropping/stopping the threadpool won't cause worker threads to stop
  midway through doing a job. If I understand correctly, the worker thread will only
  notice the stop command when the job function it's working on completes and returns.

- I learned that Rust's thread locals are confusing. They look like normal statics, but
  they allow for lazy initialization,
  and they have some [special methods](https://doc.rust-lang.org/std/thread/struct.LocalKey.html#impl-LocalKey%3CCell%3CT%3E%3E)
  available when they contain `Cell` or `RefCell`.
  I'd warn students about this.
  I think we should provide a `thread_local!` declaration to students, and let them change it if they want.

- I think we should provide students with something like `type Work = Box<dyn FnOnce() + Send + 'static>`
  or they might get stuck.

- Tests would be quicker to understand if they had more comments, and if assertions had messages.

- In the readme, state more explicitly when `get_result()` should return `None`, for clarity.

- In the readme work queues section, I'd explicitly contrast the multi-queue "locking is not inherently slow"
  to the naive implementation of a single queue with a single lock.

- As an experiment, I tried changing my worker implementation to steal work from adjacent queues in decreasing (instead of increasing) order.
  All the tests still passed. I suppose that's fine, because this is a minor implementation detail. And I'm not sure how we'd test this anyway.

- As another experiment, I tried swapping the behavior of `StealMode::OneAtATime` and `StealMode::TakeHalf` in my worker code.
  All tests still passed. I'm not sure if this is easily testable though.

Those are the main things I can think of, otherwise the lab looks great!