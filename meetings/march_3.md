# Meeting - 3 March 2025

## Motivation of Rust's ownership:
```c
// Language: C

// Caller is responsible for calling free()
// on the returned char*.
// That is, the caller is now the "owner" of that char*.
char* write_a_book(void) { /* ... */ }

char* book = write_a_book();
// How can we force the user to actually free this?
// Enter: Rust's ownership system!
free(book);
```

## Motivation of Rust's mutable/immutable:
- Like `const` in C.
- A binary tree `get()` could return a reference to an int.
    If a user modifies it, that'd break an invariant.

## Motivation of only one mutable reference:
- You want the guarantee, that while you hold an immutable reference, it won't disappear or be invalidated:

```rust,ignore
// Language: Rust
let elem: &u32 = list.get_last(); // immutable ref
list.pop(); // mutable ref
println!("{elem}"); // invalid, won't compile
```

## Motivation of Rust's lifetimes

- We have a struct which holds a pointer to some `malloc()`ed data.
- If we `free()` the data, the struct's pointer will be invalid.
- We must ensure the data lives at least as long as the struct.
- Rust lifetimes guarantee this.
- Rust function example: `'a` specifies the lifetime of return type. Arguments must live for at least `'a`.

## Feedback
- Specify why Rust mutex wraps data, instead of being separate like in Java/C++.
    Draw out a memory diagram here.

- Introduce async step-by-step, to make students feel like they're discovering it on their own.

