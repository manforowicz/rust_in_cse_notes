# Meeting - 21 May 2025

- Added list of helpful Rust courses to [async-notes](https://gitlab.cs.washington.edu/cse334/async-notes)

- Added todo-list of things to improve in async-notes. Should I convert it into a slide deck?

- [server_locking_lab](https://gitlab.cs.washington.edu/cse334/server_locking_lab) notes:
    - Matthew asked for an idiomatic use-case for async. I think server's are great for this. We can test to see how many connections they can handle concurrently. Theoretically, that's what the lab is already doing.
    - I can make it lean more into async by taking [rust-course-fei's](https://github.com/Kobzol/rust-course-fei/tree/main/lessons) approach of easing students in to the concept by first having them use an event-loop. Most code is reusable.
    - Concurrent servers in Java often use event loops because there's no async/await syntax.


- Yay, Tom said I'll receive a summer TA offer for working on the course.