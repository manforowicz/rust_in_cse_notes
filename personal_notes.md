In C, the owner is "the one responsible for calling free()"

Calling free() is a good motivation for Rust's ownership.

Example of mutable/immutable of iterating over a vector, and removing elements inside the loop.

--------------
A specific example is a pop() function of a linked list in C.

It returns a pointer to the element that was pop()ed. We tell the user to free() it in a doc comment, but how could we force them to do it.


----
Lifetimes:

We have a struct which holds a pointer to some malloc()ed data.

We must get rid of the struct before calling free() on the data.




-----
Introduce specific steps for why Mutex in Rust is locking data, whereas Mutex in Java/C++ is separate from the data.
Maybe just draw out a memory diagram of what exactly the Mutex is holding.

----

Have some more principled motivated way of introducing async.