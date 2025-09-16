# Meeting - 15 September 2025

## Progress report

- MemoryController struct now contains two fields: memory, directory.

- Added 2 food for thought questions: comparing mutex vs MSI, and asking how we achieved concurrency without threads. I just realized I forgot to push them. Will push them after the meeting.

- During the last meeting, we wondered how to test that handling requests for different memory blocks is fully concurrent. 
    The answer was simply to assert that when the memory controller receives a message pertaining to one block, it doesn't send out messages pertaining to a different block.

- Added basic script to remove solution from code.

- Started writing new async lab based on a [rust-course-fei lesson](https://github.com/Kobzol/rust-course-fei/blob/main/lessons/10/exercises/src/lib.rs). I like the lesson because it naturally pushes students toward async design patterns.
    - The state for each client is waiting on many things at once: timeout timer, messages from other users to forward, messages from the current user. Waiting on multiple arbitrary things is a strength of async (due to cancellability and cheapness to spawn multiple tasks).

- I think this is much better than my previous ["server-locking-lab"](https://gitlab.cs.washington.edu/cse334/server_locking_lab/). I still haven't pushed the new async lab to GitLab, as I'm still starting to work on it.

## TODO

- Make synthesis questions clearer.
- Add question that leads to moesi.
- Keep working on async lab.
