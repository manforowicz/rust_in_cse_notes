#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "coroutine.h"

// Here, I will give you some skeleton code that you may find useful.
// You can change it, but our solution code did not.

struct Routine;
typedef struct Routine Routine;
struct Routine {
    // The stack pointer for when this routine is not running.
    // Remember, x86 stacks grow down and pop up.
    // It is important that this field is first.
    uint64_t saved_stack_pointer;
    // The stack used by this routine.
    uint64_t private_stack[STACK_ENTRIES];
    // The channel associated with this routine.
    Channel* channel;
    // The next routine in the linked list.
    Routine* next_in_queue;

    // YOUR CODE HERE

    // The user-provided function that this routine will run.
    FuncPtr func;

    // Incoming or outgoing queue msg
    SendableData msg;
};

// In order to keep track of what coroutines are available to run,
// we need a list of "ready" coroutines.
// This is commonly called the "ready queue" in operating systems.
// You will also need queues of routines elsewhere in this project,
//  so we provide an implementation of a queue below.

struct RoutineQueue {
    Routine* head;
    Routine* tail;
};
typedef struct RoutineQueue RoutineQueue;

// Note: in this context, `static` means private to this file.
// C is weird and `static` has many different uses.
static void enqueue(RoutineQueue* q, Routine* r) {
    if (q->tail != NULL) {
        q->tail->next_in_queue = r;
    }
    q->tail = r;
    r->next_in_queue = NULL;
    if (q->head == NULL) {
        q->head = r;
    }
}

static Routine* dequeue(RoutineQueue* q) {
    Routine* ret = q->head;
    if (ret == NULL) {
        return NULL;
    }
    q->head = ret->next_in_queue;
    if (q->tail == ret) {
        q->tail = NULL;
    }
    return ret;
}

// Here is the ready queue, initialized to be empty:
RoutineQueue readyQ = {NULL, NULL};

// Additionally, it is necessary to keep track of the currently running routine.
// This is set to NULL but you should set it on coroutine_init!

// Note that main() is a special coroutine:
//  when main returns, we do not get control, so the entire program ends,
//  unlike a normal coroutine.
// It does not have its own associated channel, either.
// Finally, it should use the OS-provided stack.
// Does this mean that main() will have a Routine struct?
Routine* current_routine = NULL;

void coroutine_init() {
    // YOUR CODE HERE

    // Create a dummy main routine, with an unused stack
    current_routine = (Routine*) calloc(1, sizeof(Routine));
}

// This is defined in routineSwitch.S. Be sure you understand it!
// You will want to build a wrapper function around this.
extern void routineSwitch(Routine* next);


bool yield_and_schedule(RoutineQueue* queue) {
    Routine* ready_routine = dequeue(&readyQ);
    // If there's no routine ready to run, return false
    if (ready_routine == NULL) {
        return false;
    }
    // Queue the current routine to be scheduled again
    if (queue != NULL) {
        enqueue(queue, current_routine);
    }
    
    // Switch to the ready routine
    routineSwitch(ready_routine);
    return true;
}

// Pointer to a coroutine that needs to be garbage-collected.
// Null if there's none.
// Routine* zombie_routine = NULL;

void coroutine_entrypoint() {
    // Run the provided function
    SendableData return_val = (current_routine->func)(current_routine->channel);
    while (true) {
        send(current_routine->channel, return_val);
    }

    // Clean up:

    // // First garbage collect zombie coroutines
    // if (zombie_routine != NULL) {
    //     free(zombie_routine);
    // }
    // zombie_routine = current_routine;
    // yield_and_schedule(NULL);
    // // We should never reach this assert
    // assert(false);
}

bool yield() {
    // YOUR CODE HERE
    return yield_and_schedule(&readyQ);
}

struct Channel {
    // YOUR CODE HERE
    RoutineQueue waiting_senders;
    RoutineQueue waiting_receivers;
};

Channel* spawn(FuncPtr func) {
    // YOUR CODE HERE

    Routine* routine = (Routine*) malloc(sizeof(Routine));
    
    routine->saved_stack_pointer = (uint64_t) &(routine->private_stack[STACK_ENTRIES-1-6]);
    routine->private_stack[STACK_ENTRIES-1] = (uint64_t) &coroutine_entrypoint;
    routine->channel = new_channel();
    routine->next_in_queue = NULL;
    routine->func = func;
    routine->msg = NULL;
    
    enqueue(&readyQ, routine);

    return routine->channel;
}

Channel* new_channel() {
    // YOUR CODE HERE
    return (Channel*) calloc(1, sizeof(Channel));
}

void send(Channel* ch, SendableData value) {
    // YOUR CODE HERE

    Routine* receiver = dequeue(&ch->waiting_receivers);

    if (receiver == NULL) {
        current_routine->msg = value;
        // Wait until someone receives our value
        if (!yield_and_schedule(&ch->waiting_senders)) {
            exit(1);
        }
    
    } else {
        receiver->msg = value;
        enqueue(&readyQ, receiver);
    }
}

SendableData receive(Channel *ch) {
    // YOUR CODE HERE
    
    Routine* sender = dequeue(&ch->waiting_senders);

    if (sender == NULL) {
        // Wait until someone sends us a value
        if (!yield_and_schedule(&ch->waiting_receivers)) {
            exit(1);
        }
        return current_routine->msg;
    } else {
        enqueue(&readyQ, sender);
        return sender->msg;
    }
}
