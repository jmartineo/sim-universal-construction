/// @file queue-stack.h
/// @brief This file provides some very simple definitions used by all the provided
/// stack and queue implementations. More specifically, the Node struct is defined 
/// for all stack and queues that are represented with a linked-list of nodes.
/// Moreover, this file provides error-codes definitions for operations on stacks and queues
/// (i.e., error codes for failing to dequeue an element from a queue, error code for failing
/// to pop an element from a stack, etc.).
#ifndef _QUEUE_STACK_H_
#define _QUEUE_STACK_H_

#include <limits.h>
#include <system.h>

typedef struct Node {
    Object val;
    volatile struct Node *next;
} Node;

#ifdef INT64_T_OBJECT
#define GUARD_VALUE     LONG_MIN
#define EMPTY_QUEUE     (GUARD_VALUE + 1)
#define EMPTY_STACK     (GUARD_VALUE + 1)
#define ENQUEUE_SUCCESS 0
#define ENQUEUE_FAIL    -1
#define PUSH_SUCCESS    0
#define PUSH_FAIL       -1

#elif defined(VOID_PTR_OBJECT)
#define GUARD_VALUE     ((void*)0xdeadbeef)
#define EMPTY_QUEUE     ((void*)0xdeadbeef + 1)
#define EMPTY_STACK     ((void*)0xdeadbeef + 1)
#define ENQUEUE_SUCCESS 0
#define ENQUEUE_FAIL    -1
#define PUSH_SUCCESS    0
#define PUSH_FAIL       -1
#endif


#endif