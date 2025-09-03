# Meeting - 3 September 2025

## What I've been up to

Returned from Warsaw, Bydgoszcz, Amsterdam on Friday evening.

Implemented the suggestion from our previous meeting to have an executor object
with a "frontier" of states that expands as commands are sent to it.
More readable and tracable than my previous approach of passing an array of commands.

Made the executor catch panics and cycles, and print what messages led up to it,
along with line numbers where they were sent.

Wasn't able to switch to dynamic dispatch as discussed.
That would require message downcasting to extract specific payloads.
Downcasting requires the Any trait which could confuse students.
So I chose to stick to an enum of messages.
Or rather (for readability), an enum per sending device, and then an enum of enums
for the executor to handle things internally.

Wrote most of a readme, with:
- background on cache coherence
- lab overview
- instructions
- advice

Update on separating the memory+directory device.
I played around with doing that, but noticed it might be adding unnecessary complexity.
Concurrency is already well-demonstrated by serving requests for multiple blocks independently,
and operations like sending (and waiting on) invalidate requests to many caches at the same time.
If I understand correctly, in real hardware the directory is usually stored in memory, and handled by the memory controller.


## TODO

How can we test that the memory will continue serving other blocks, even if there are long delays for a different block.

Getting full test coverage is easy (as checked by llvm-cov). I'll write more tests in general, considering edge cases,
and also write partial credit tests that are deliberately designed to pass partial implementations.

Improve the readme, add comments, and improve error messages.

This lab should be ready to playtest by Friday morning.

