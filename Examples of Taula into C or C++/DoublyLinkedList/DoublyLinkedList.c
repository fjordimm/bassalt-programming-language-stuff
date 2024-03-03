
////////////////////////////////////////////////////////////

///// C Includes /////

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

////////////////////////////////////////////////////////////

///// Bassalt Helper Functions /////

void _BASSALT_PRINTERROR_DEREFBADWLPTR_()
{
	printf("Tried to dereference an invalid weak lock-and-key pointer!\n");
}

////////////////////////////////////////////////////////////

///// Global Key Incrementer For Locks-and-keys /////

#define _BASSALT_LOCKSANDKEYS_NULLKEY_ 0
static uint64_t _BASSALT_LOCKSANDKEYS_KEYINCR_ = 1;

////////////////////////////////////////////////////////////

///// Definition for Struct 'Node' /////

typedef struct Node_INNERSTRUCT
{
	int data;
	struct Node_INNERSTRUCT* prev;
	struct Node_INNERSTRUCT* next;
} Node;

///// Definition for Lock-and-key Struct for 'Node' /////

typedef struct
{
	uint64_t _KEY_;
	Node _OBJ_;
} Node_LOCKANDKEY;

///// Definition for Weak Lock-and-key Pointer Tuple for 'Node' /////

typedef struct
{
	uint64_t _CKEY_;
	Node_LOCKANDKEY* _PTR_;
} _Point_LOCKANDKEY_WLPTR;

////////////////////////////////////////////////////////////