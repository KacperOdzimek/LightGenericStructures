/*
    T macro pattern
        [instance name], [stored type], [stored type destructor (opt)]
*/

#include "generic.h"

#ifndef T
    #error No "T" macro defined at the time of inclusion. Note T macros are undef at the end of every data structure header.
#endif

#ifndef A
    #error No "A" macro defined at the time of inclusion. Note A macros are undef at the end of every data structure header.
#endif

/*
    Unpack and Helpers
*/

#define INSTANCE   LGS_FIRST(T)
#define STORED     LGS_SECOND(T)
#define DESTRUCTOR LGS_THIRD(T)

/*
    Typedef
*/

// Doubly linked list (dlist) node
#define dlist_node(inst) LGS_INST(dlist_node, inst)

// Double-linked list (dlist)
// O(n) memory complexity
#define dlist(inst) LGS_INST(dlist, inst)

typedef struct dlist_node(INSTANCE) {
    STORED                priv_obj;
    struct dlist_node(INSTANCE)* priv_prev;
    struct dlist_node(INSTANCE)* priv_next;
} dlist_node(INSTANCE);

typedef struct dlist(INSTANCE) {
    size_t          priv_size;
    dlist_node(INSTANCE)*  priv_first;
    dlist_node(INSTANCE)*  priv_last;
} dlist(INSTANCE);

/*
    Zero / Destruction
*/

// Makes uninitialized memory proper 0-initialized empty list
// Does not free anything
#define dlist_zero(inst) LGS_INST(dlist_zero, inst)

LGS_API(void) dlist_zero(INSTANCE)(dlist(INSTANCE)* tar) {
    tar->priv_size  = 0;
    tar->priv_first = NULL;
    tar->priv_last  = NULL;
}


// Free allocated memory, destroys owned objects
#define dlist_destroy(inst) LGS_INST(dlist_destroy, inst)

LGS_API(void) dlist_destroy(INSTANCE)(dlist(INSTANCE)* tar) {
    dlist_node(INSTANCE)* cur = tar->priv_first;
    while (cur) {
        dlist_node(INSTANCE)* next = cur->priv_next;
        DESTRUCTOR(&cur->priv_obj); // destroy object
        LGS_FREE(cur); // free node
        cur = next;
    }

    dlist_zero(INSTANCE)(tar);
}

/*
    Query
*/

// Returns list size
// O(1)
#define dlist_size(inst) LGS_INST(dlist_size, inst)

LGS_API(size_t) dlist_size(INSTANCE)(const dlist(INSTANCE)* tar) {
    return tar->priv_size;
}

/*
    Node Operations
*/

// Returns pointer to the first node in the list
// NULL if list is empty
// O(1)
#define dlist_first(inst) LGS_INST(dlist_first, inst)

LGS_API(dlist_node(INSTANCE)*) dlist_first(INSTANCE)(const dlist(INSTANCE)* tar) {
    return tar->priv_first;
}


// Returns pointer to the last node in the list
// NULL if list is empty
// O(1)
#define dlist_last(inst) LGS_INST(dlist_last, inst)

LGS_API(dlist_node(INSTANCE)*) dlist_last(INSTANCE)(const dlist(INSTANCE)* tar) {
    return tar->priv_last;
}

// Given pointer to a list's node, returns pointer to the next list's node
// Returns NULL if given pointer was last node in the list
// Returns NULL if passed NULL
// O(1)
#define dlist_next(inst) LGS_INST(dlist_next, inst)

LGS_API(dlist_node(INSTANCE)*) dlist_next(INSTANCE)(dlist_node(INSTANCE)* n) {
    return n ? n->priv_next : NULL;
}


// Given pointer to a list's node, returns pointer to the previous list's node
// Returns NULL if given pointer was first node in the list
// Returns NULL if passed NULL
// O(1)
#define dlist_prev(inst) LGS_INST(dlist_prev, inst)

LGS_API(dlist_node(INSTANCE)*) dlist_prev(INSTANCE)(dlist_node(INSTANCE)* n) {
    return n ? n->priv_prev : NULL;
}


// Returns pointer to the object stored inside the given node
// Do not invalidate the object, as the destructor (if provided) will be called on it sooner or later
// O(1)
#define dlist_access(inst) LGS_INST(dlist_access, inst)

LGS_API(STORED*) dlist_access(INSTANCE)(dlist_node(INSTANCE)* n) {
    return &n->priv_obj;
}


// Returns read only pointer to the object stored inside the given node
// O(1)
#define dlist_const_access(inst) LGS_INST(dlist_const_access, inst)

LGS_API(const STORED*) dlist_const_access(INSTANCE)(dlist_node(INSTANCE)* n) {
    return &n->priv_obj;
}

/*
    Operations
*/

// Inserts new node with given value before the "before" node
// If "before" node is NULL new node will be now the last one 
// (it gets pushed before exclusive list end)
// Takes ownership of object at success
// May fail allocation, returns NULL on fail, valid node pointer otherwise, O(1)
#define dlist_push_before(inst) LGS_INST(dlist_push_before, inst)

LGS_API(dlist_node(INSTANCE)*) dlist_push_before(INSTANCE)(
    dlist(INSTANCE)*        tar, 
    dlist_node(INSTANCE)*   before, 
    STORED                  value
) {
    dlist_node(INSTANCE)* new_node = (dlist_node(INSTANCE)*)LGS_ALLOC(sizeof(dlist_node(INSTANCE)));
    if (!new_node) return NULL;

    new_node->priv_obj = value;

    // Insert at end if before == NULL
    if (before == NULL) {
        new_node->priv_prev = tar->priv_last;
        new_node->priv_next = NULL;

        if (tar->priv_last) tar->priv_last->priv_next = new_node;
        else                tar->priv_first = new_node;

        tar->priv_last = new_node;
    }
    // Insert before given list node
    else {    
        new_node->priv_prev = before->priv_prev;
        new_node->priv_next = before;

        if (before->priv_prev) before->priv_prev->priv_next = new_node;
        else                   tar->priv_first = new_node;

        before->priv_prev = new_node;
    }

    tar->priv_size++;
    return new_node;
}


// Inserts new node with given value after the "after" node
// If "after" node is NULL new node will be now the first one
// (it gets pushed after exclusive list begin)
// Takes ownership of object at success
// May fail allocation, returns NULL on fail, node valid pointer otherwise, O(1)
#define dlist_push_after(inst) LGS_INST(dlist_push_after, inst)

LGS_API(dlist_node(INSTANCE)*) dlist_push_after(INSTANCE)(
    dlist(INSTANCE)*        tar, 
    dlist_node(INSTANCE)*   after, 
    STORED                  value
) {
    dlist_node(INSTANCE)* new_node = (dlist_node(INSTANCE)*)LGS_ALLOC(sizeof(dlist_node(INSTANCE)));
    if (!new_node) return NULL;

    new_node->priv_obj = value;

    // Insert at begin if after == NULL
    if (after == NULL) {
        new_node->priv_prev = NULL;
        new_node->priv_next = tar->priv_first;

        if (tar->priv_first) tar->priv_first->priv_prev = new_node;
        else                 tar->priv_last = new_node; // dlist was empty

        tar->priv_first = new_node;
    }
    else {
        // Insert after given node
        new_node->priv_prev = after;
        new_node->priv_next = after->priv_next;

        if (after->priv_next) after->priv_next->priv_prev = new_node;
        else                  tar->priv_last = new_node;  // inserting after last node

        after->priv_next = new_node;
    }

    tar->priv_size++;
    return new_node;
}


// Erases given element from the list
// If out == NULL destructor will be called on contained object
// Otherwise *out = element, caller gets ownership over object and destructor will not be called
// O(1)
#define dlist_pop(inst) LGS_INST(dlist_pop, inst)

LGS_API(void) dlist_pop(INSTANCE)(dlist(INSTANCE)* tar, dlist_node(INSTANCE)* n, STORED* out) {
    // get rid of object
    if (out) *out = n->priv_obj;
    else DESTRUCTOR(&n->priv_obj);

    // Relink previous node
    if (n->priv_prev) n->priv_prev->priv_next = n->priv_next;
    else              tar->priv_first = n->priv_next; // n was first node
    
    // Relink next node
    if (n->priv_next) n->priv_next->priv_prev = n->priv_prev;
    else              tar->priv_last = n->priv_prev; // n was last node

    // Decrease size and free
    tar->priv_size--;
    LGS_FREE(n);
}


// Clears list
// O(1), with destructors O(n)
#define dlist_clear(inst) LGS_INST(dlist_clear, inst)

LGS_API(void) dlist_clear(INSTANCE)(dlist(INSTANCE)* tar) {
    dlist_destroy(INSTANCE)(tar); // apparently the same
}

#undef INSTANCE
#undef STORED
#undef DESTRUCTOR

// consume parameters
#undef T
#undef A
