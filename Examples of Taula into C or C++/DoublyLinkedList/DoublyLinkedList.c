
////////////////////////////////////////////////////////////

///// C Includes /////

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

////////////////////////////////////////////////////////////

///// Bassalt Helper Functions /////

void _BASSALT_ERROR_BADWLPTR()
{
	fprintf(stderr, "Tried to dereference an invalid weak lock-and-key pointer!\n");
	exit(1);
}

////////////////////////////////////////////////////////////

///// Global Key Incrementer For Locks-and-keys /////

#define _BASSALT_LOCKSANDKEYS_NULLKEY 0
static uint64_t _BASSALT_LOCKSANDKEYS_KEYINCR = 1;

////////////////////////////////////////////////////////////

///// Definition for Weak Lock-and-key Pointer Tuple for 'Node' /////

typedef struct
{
	uint64_t _CKEY;
	struct __Node_LOCKANDKEY* _PTR;
} Node_WLPTR;

///// Definition for Struct 'Node' /////

typedef struct
{
	int32_t data;
	Node_WLPTR prev;
	struct __Node_LOCKANDKEY* next;
} Node;

///// Definition for Lock-and-key Struct for 'Node' /////

typedef struct __Node_LOCKANDKEY
{
	uint64_t _KEY;
	Node _OBJ;
} Node_LOCKANDKEY;

////////////////////////////////////////////////////////////

///// Definition for Function 'PrintNode(Node!& node)' /////

void PrintNode(Node* node)
{
	printf("Node(");
	/*Memory Safety Check*/ if (node->prev._CKEY != node->prev._PTR->_KEY) _BASSALT_ERROR_BADWLPTR();
	printf(node->prev._PTR == NULL ? "o " : "* ");
	printf("%i", node->data);
	printf(node->next == NULL ? " o" : " *");
	printf(")");
}

////////////////////////////////////////////////////////////

///// Definition for Struct of Class 'List' /////

typedef struct
{
	int32_t count;
	Node_LOCKANDKEY* head;
	Node_WLPTR tail;
} List;

///// Definition for Constructor of 'List' /////

void Node_CONSTRUCTOR(List* _SELF)
{
	_SELF->count = 0;
	_SELF->head = NULL;
	_SELF->tail._CKEY = _BASSALT_LOCKSANDKEYS_NULLKEY;
	_SELF->tail._PTR = NULL;
}

///// Definition for List.append(int val) /////

void Node_append(List* _THIS, int32_t val)
{
	Node_LOCKANDKEY* newNode = (Node_LOCKANDKEY*)malloc(sizeof(Node_LOCKANDKEY));
		Node _TEMP0;
			_TEMP0.data = val;
			_TEMP0.prev._CKEY = _BASSALT_LOCKSANDKEYS_NULLKEY;
			_TEMP0.prev._PTR = NULL;
			_TEMP0.next = NULL;
		newNode->_KEY = _BASSALT_LOCKSANDKEYS_KEYINCR++;
		newNode->_OBJ = _TEMP0;
	
	if (_THIS->count == 0)
	{
		_THIS->head = newNode;
		_THIS->tail._CKEY = _THIS->head->_KEY;
		_THIS->tail._PTR = _THIS->head;

		// TODO: add all the Memory Safety Checks
	}
	else if (_THIS->count == 1)
	{
		newNode->_OBJ.prev._CKEY = _THIS->head->_KEY;
		newNode->_OBJ.prev._PTR = _THIS->head;

		_THIS->head->_OBJ.next = newNode;
		_THIS->tail._CKEY = _THIS->head->_OBJ.next->_KEY;
		_THIS->tail._PTR = _THIS->head->_OBJ.next;
	}
	else
	{
		newNode->_OBJ.prev._CKEY = _THIS->tail._CKEY;
		newNode->_OBJ.prev._PTR = _THIS->tail._PTR;

		_THIS->tail._PTR->_OBJ.next = newNode;
		_THIS->tail._CKEY = _THIS->tail._PTR->_OBJ.next->_KEY;
		_THIS->tail._PTR = _THIS->tail._PTR->_OBJ.next;
	}

	_THIS->count++;
}

///// Definition for List.print() /////

void Node_print(List* _THIS)
{
	printf("List (%i)\n", _THIS->count);

	Node_WLPTR n;
		n._CKEY = _THIS->head->_KEY;
		n._PTR = _THIS->head;
	while (n._PTR != NULL)
	{
		printf("  ");
		PrintNode(&(n._PTR->_OBJ));
		printf("\n");
		
		n._CKEY = n._PTR->_OBJ.next->_KEY;
		n._PTR = n._PTR->_OBJ.next;
	}
}

////////////////////////////////////////////////////////////

///// Main Function /////

int main(void)
{
	printf("=== Starting Program ===\n");

	List a;
	Node_CONSTRUCTOR(&a);
	
	Node_append(&a, 21);
	Node_print(&a);

	printf("=== Ending Program ===\n");

	// TODO: destructors

	return 0;
}

////////////////////////////////////////////////////////////