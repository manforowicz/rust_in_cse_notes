# Meeting - 16 April 2025

## Server Locking Lab

I made a [server_locking_lab](https://gitlab.cs.washington.edu/cse334/server_locking_lab) repository in the GitLab group.

I don't want to just tell students to "use fine grained locking" when I can instead
write external tests that won't pass until this is done.

I structured this lab, so that I can write test cases that enforce these learning goals:
- Fine-grained locking.
- Ordered locking.
- Async over threads.

This lab can be cleanly divided into different phases, each with their own test cases:
1. Implement single-threaded server with no locking.
2. Add locking, and spawn a thread for each new request.
3. Use a threadpool.
4. Use single-threaded async.


## Cuckoo Hash

I update my [pull request](https://github.com/thisisyiqing/cuckoo/pull/1)
to enforce lock ordering, and fix a race condition bug.

I didn't have time to write more loom tests.

## To-Do

- Keep working on this lab.
- If I have time, try Matthew's lab and send a feedback message.
- Send Yiqing a message with feedback about slides: https://docs.google.com/presentation/d/14myOhXS0392NV5tz7ZTAjgLYsCNwOfFQ9r81QP-l46E/edit#slide=id.p
- Add more Cuckoo tests, and maybe run [flamegraph](https://nnethercote.github.io/perf-book/profiling.html) on it.