#import "slide-template.typ": *
#import "@preview/diagraph:0.3.1": raw-render

#show: project.with()
// #enable-handout-mode(true)

#slide[
  = Ordered Locking

  #align(center)[
    CSE 334
  ]
]

#slide[
  == Deadlock

  - Happens when there's a set of threads where

    - each thread waits for a resource held by another thread.

  #show: later

  #toolbox.side-by-side[
    === No-deadlock interleaving
    #toolbox.side-by-side[
      ```rs
      // thread 1
      a.lock();
      b.lock();
      drop(a);
      drop(b);
      ```
    ][
      ```rs
      // thread 2




      b.lock();
      a.lock();
      drop(a);
      drop(b);
      ```
    ]
  ][
    === Deadlock interleaving
    #toolbox.side-by-side[
      ```rs
      // thread 1
      a.lock();

      b.lock();

      drop(a);
      drop(b);
      ```
    ][
      ```rs
      // thread 2

      b.lock();

      a.lock();


      drop(a);
      drop(b);
      ```
    ]
  ]
  #show: later

  - Tricky to debug. What if it only deadlocks 1% of the time?
]


#slide[
  == What code is deadlock-prone?

  - Let's draw a graph, where each path represents a possible order of locking.

  #show: later

  #toolbox.side-by-side[
    #toolbox.side-by-side[
      ```rs
      // thread 1
      a.lock();
      b.lock();
      ```
    ][
      ```rs
      // thread 2
      b.lock();
      a.lock();
      ```
    ]
  ][
    #raw-render(```
      digraph {
        node [fontsize=15]
        edge [fontsize=15]
        layout="dot";
        rankdir="TB";
        unlocked;
        { rank = same; "lock a"; "lock b"}
        unlocked -> "lock a" [color=blue, xlabel="thread 1", fontcolor=blue];
        "lock a" -> "lock b" [color=blue];
        unlocked -> "lock b" [color=red label="thread 2" fontcolor=red];
        "lock b" -> "lock a" [color=red]
      }
    ```)
  ]

  #show: later
  - What property of this graph tells us this can deadlock?
  #show: later
  - The cycle!
  #show: later
  - What rules can programmers follow to prevent cycles?
  #show: later
]

#slide[
  == Lock Ordering

  - We can assign the locks an order.

  #show: later

    #raw-render(```
      digraph {
        node [fontsize=15]
        edge [fontsize=15]
        layout="dot";
        rankdir="LR";
        splines=true;
        edge[weight=1];
        unlocked -> "lock 1" -> "lock 2" -> "lock 3";

        edge[weight=0];
        unlocked:n -> "lock 2":nw;
        unlocked:ne -> "lock 3":nw;
        "lock 1":n -> "lock 3":nw;
      }
    ```)


  Hello

  ```rust
  fn hi() {
    println!("SUP");
  }
  ```
]
