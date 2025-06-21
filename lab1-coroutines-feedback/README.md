# lab1-coroutines

Marcin's thoughts while completing this lab.

This lab took me about 6 hours. I like how brief yet mentally-challenging this lab is.
Most of these notes are nitpicks and some of them might not make sense.

## General

I struggled figuring out how to pass the new channel to the spawned function.
Initially I thought I should manually set the first argument register (rdi) to the channel,
but then I peaked at the solution and noticed I don't need to write any assembly.
So instead I created a `coroutine_entrypoint()` function which calls the user-provided function.
But then I wondered how to pass the user-provided function pointer to `coroutine_entrypoint()`.
Then I realized I should put that function pointer in a field in the `Routine` struct, so that `coroutine_entrypoint()` can access it.
I think the skeleton code `Routine` struct should already have a `FuncPtr` field to make this part easier.

I had each coroutine that returns mark itself as a zombie, so the next coroutine to return could `free()` it.
But then I realized that reaping of coroutines and channels is not required.
The requirement of having a channel send the coroutine's return value an unlimited number of times makes freeing more difficult.
Honestly, this requirement was a bit strange to me, but I guess it makes sense.

## Readme

- Mention that nothing needs to be `free()`ed (or make reaping a requirement).

- Mention that students won't need to write assembly (`routineSwitch.s` is enough).

- Clarify that `make results/<testname>` means to run `make t00`, not `make results/t00`.

- I initially misinterpreted the "FYI" as saying that our `yield()` has restrictions on which coroutines it can and can't switch to. I would reword this.

- "realistic parallel problem via functional decomposition": technically it's not parallel but concurrent. I'd just say the test should be a realistic use of coroutines.

- I'm curious what the answer to the "What type of data is not sendable?" question. Ah, maybe anything that's on the stack?

- In the first code example in the readme, `main()` doesn't start with `coroutine_init()`. Make sure all the code snippets would roughly compile.

- I initially thought that the two ends of each channel get different `Channel*` pointers.
  This made me question whether the channel should be unidirectional or bidirectional.
  But then I realized that both ends share a single `Channel*` pointer by reading the `new_channel()` interface.
  Maybe briefly mention this.

## `coroutine.h`

- Some things are explicitly marked `extern` while others aren't.
- I haven't seen a function pointer typedef in C before, so maybe add a note like "don't worry about the typedef syntax".

## `routineSwitch.S`

- Line 15: I would add a comment saying that `current_routine(%rip)` simply accesses the memory pointed to by `current_routine`,
because CSE 351 students haven't seen assembly syntax that uses C variables and don't know about RIP-relative addressing.
I'd suggest adding a block comment with C-style pseudocode for what the assembly does.
- Add a comment stating we get away saving only "imporant" registers, because those are the callee-saved registers from the perspective of `routineSwitch`.

## `coroutine.c`

- In the `Routine` struct, why is it `uint64_t` instead of the more idiomatic `void*`.
- I noticed one function had 5 spaces of indent instead of 4. Run the code through a formatter.
- I was unsure why the stack in `Routine` is called `private_stack` rather than just `stack`.

- Why do we have a separate `struct Routine;` line but don't have that for `RoutineQueue`.
- In `struct Routine`, I think "It is important that this field is first" is a little cryptic.
I'd just specify that the `saved_stack_pointer` is modified in `routineSwitch.S`.
- Line 16: Students might wonder why the stack array isn't just an array of bytes. Maybe briefly mention alignment in the comment?

## Tests

- Add a make command to run all tests.
- I noticed there are some TODOs to be addressed in the makefile.
- I'd comment the makefile just to demonstrate best practices.
- I'd add more warning flags to tests such as `-wall`.
- Tests t01, t14, t15, t19, t20 are broken.
