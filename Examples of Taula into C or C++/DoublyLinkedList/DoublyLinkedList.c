
////////////////////////////////////////////////////////////

///// C Includes /////

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

////////////////////////////////////////////////////////////

///// Bassalt helper functions /////

void _BASSALT_ERROR_NullPtr()
{
	fprintf(stderr, "Tried to dereference a null pointer!\n");
	exit(1);
}

void _BASSALT_ERROR_MallocFailed()
{
	fprintf(stderr, "Standard function malloc() failed!\n");
	exit(1);
}

void _BASSALT_ERROR_BadLptr()
{
	fprintf(stderr, "Tried to dereference an invalid weak lock-and-key pointer!\n");
	exit(1);
}

////////////////////////////////////////////////////////////

/*
The term 'LK' refers to the lock-and-key method.
LK key generation inspired by section 5.2 of https://www.cs.rochester.edu/u/jzhou41/papers/checkedc.pdf

Bit allotment for LK keys (stored in a uint64_t):
 - [63:48] for the randomly-generated global LK key header
 - [47:0] for the key value
*/

///// Global LK key constants /////

#define _BASSALT_LK_INVALIDKEY 0
static uint64_t _BASSALT_LK_HEADER; // will be initialized in main
static uint64_t _BASSALT_LK_KEYVAL; // will be initialized in main, and will be incremented each time it is used

////////////////////////////////////////////////////////////

///// Definition for the LK pointer tuple for 'Node' /////

typedef struct
{
	uint64_t _CKEY;
	struct __Node_LOCKANDKEY* _ADDR;
} Node_LPTR;

///// Definition for struct 'Node' /////

typedef struct
{
	int32_t data;
	Node_LPTR prev;
	Node_LPTR next;
} Node;

///// Definition for the LK object struct for 'Node' /////

typedef struct __Node_LOCKANDKEY
{
	uint64_t _KEY;
	Node _OBJ;
} Node_LOCKANDKEY;

///// Definition for the ___________ for 'Node' /////

////////////////////////////////////////////////////////////

///// Definition for the function 'PrintNode(Node!& node)' /////

void PrintNode(Node* node)
{
	printf("Node(");
	/*Memory Safety Check*/ if (node == NULL) _BASSALT_ERROR_NullPtr();
	printf(node->prev._ADDR == NULL ? "o " : "* ");
	printf("%i", node->data);
	printf(node->next._ADDR == NULL ? " o" : " *");
	printf(")");
}

////////////////////////////////////////////////////////////

///// Definition for the struct of class 'List' /////

typedef struct
{
	int32_t count;
	Node_LPTR head;
	Node_LPTR tail;
} List;

///// Definition for the constructor of 'List' /////

void Node_CONSTRUCTOR(List* _THIS)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();
	_THIS->count = 0;
	_THIS->head._CKEY = _BASSALT_LK_INVALIDKEY;
	_THIS->head._ADDR = NULL;
	_THIS->tail._CKEY = _BASSALT_LK_INVALIDKEY;
	_THIS->tail._ADDR = NULL;
}

///// Definition for List.append(int val) /////

void Node_append(List* _THIS, int32_t val)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();

	Node_LPTR newNode;
		Node_LOCKANDKEY* _TEMP0 = (Node_LOCKANDKEY*)malloc(sizeof(Node_LOCKANDKEY));
			/*Memory Safety Check*/ if (_TEMP0 == NULL) _BASSALT_ERROR_MallocFailed();
			Node _TEMP1;
				_TEMP1.data = val;
				_TEMP1.prev._CKEY = _BASSALT_LK_INVALIDKEY;
				_TEMP1.prev._ADDR = NULL;
				_TEMP1.next._CKEY = _BASSALT_LK_INVALIDKEY;
				_TEMP1.next._ADDR = NULL;
			_TEMP0->_KEY = _BASSALT_LK_KEYVAL++;
			_TEMP0->_OBJ = _TEMP1;
		newNode._CKEY = _TEMP0->_KEY;
		newNode._ADDR = _TEMP0;
	
	if (_THIS->count == 0)
	{
		_THIS->head = newNode;
		_THIS->tail = _THIS->head;
	}
	else if (_THIS->count == 1)
	{
		newNode._ADDR->_OBJ.prev = _THIS->head;
		
		/*Memory Safety Check*/ if (_THIS->head._ADDR == NULL || _THIS->head._CKEY != _THIS->head._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		_THIS->head._ADDR->_OBJ.next = newNode;
		_THIS->tail = _THIS->head._ADDR->_OBJ.next;
	}
	else
	{
		newNode._ADDR->_OBJ.prev = _THIS->tail;

		/*Memory Safety Check*/ if (_THIS->tail._ADDR == NULL || _THIS->tail._CKEY != _THIS->tail._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		_THIS->tail._ADDR->_OBJ.next = newNode;
		_THIS->tail = _THIS->tail._ADDR->_OBJ.next;
	}

	_THIS->count++;
}

///// Definition for List.removeLast() /////

void Node_removeLast(List* _THIS)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();

	
}

///// Definition for List.printFrontToBack() /////

void Node_printFrontToBack(List* _THIS)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();

	printf("List (%i) front-to-back:\n", _THIS->count);

	Node_LPTR n = _THIS->head;
	while (n._ADDR != NULL)
	{
		printf("  ");
		/*Memory Safety Check*/ if (n._ADDR == NULL || n._CKEY != n._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		PrintNode(&(n._ADDR->_OBJ));
		printf("\n");
		
		n = n._ADDR->_OBJ.next;
	}
}

///// Definition for List.printBackToFront() /////

void Node_printBackToFront(List* _THIS)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();

	printf("List (%i) back-to-front:\n", _THIS->count);

	Node_LPTR n = _THIS->tail;
	while (n._ADDR != NULL)
	{
		printf("  ");
		/*Memory Safety Check*/ if (n._ADDR == NULL || n._CKEY != n._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		PrintNode(&(n._ADDR->_OBJ));
		printf("\n");
		
		n = n._ADDR->_OBJ.prev;
	}
}

////////////////////////////////////////////////////////////

///// Main function /////

int main(void)
{
	// Global LK initialization
	srand(time(NULL));
	rand();
	_BASSALT_LK_HEADER = (uint64_t)rand() << 48uL;
	_BASSALT_LK_KEYVAL = _BASSALT_LK_HEADER | ((uint64_t)rand() << 16uL) | (uint64_t)rand();

	printf("=== Starting Program ===\n");
	printf("\n");

	List a;
	Node_CONSTRUCTOR(&a);
	
	Node_append(&a, 1);
	Node_printFrontToBack(&a);
	Node_printBackToFront(&a);
	printf("\n");

	Node_append(&a, 2);
	Node_printFrontToBack(&a);
	Node_printBackToFront(&a);
	printf("\n");

	Node_append(&a, 3);
	Node_printFrontToBack(&a);
	Node_printBackToFront(&a);
	printf("\n");

	List b;
	Node_CONSTRUCTOR(&b);
	Node_append(&b, 11);
	Node_append(&b, 12);
	Node_append(&b, 13);
	Node_append(&b, 14);
	Node_append(&b, 15);
	Node_printFrontToBack(&b);
	Node_printBackToFront(&b);
	printf("\n");

	printf("=== Ending Program ===\n");

	// TODO: destructors, and LK destructors must set the object's _KEY to _BASSALT_LK_INVALIDKEY

	return 0;
}

////////////////////////////////////////////////////////////