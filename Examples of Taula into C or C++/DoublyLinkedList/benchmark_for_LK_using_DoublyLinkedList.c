
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
	fprintf(stderr, "Tried to dereference an invalid weak LK pointer!\n");
	exit(1);
}

////////////////////////////////////////////////////////////

///// Global LK key constants /////

#define _BASSALT_LK_INVALIDKEY 0
static uint64_t _BASSALT_LK_HEADER; // will be initialized in main
static uint64_t _BASSALT_LK_KEYVAL; // will be initialized in main, and will be incremented each time it is used

////////////////////////////////////////////////////////////

/*************************************************************************/
/******************************** With LK ********************************/
/*************************************************************************/

////////////////////////////////////////////////////////////

///// Definition for the LK pointer tuple for 'WLKNode' /////

typedef struct
{
	uint64_t _CKEY;
	struct __WLKNode_LOCKANDKEY* _ADDR;
} WLKNode_LPTR;

///// Definition for struct 'WLKNode' /////

typedef struct
{
	int32_t data;
	WLKNode_LPTR prev;
	WLKNode_LPTR next;
} WLKNode;

///// Definition for the LK object struct for 'WLKNode' /////

typedef struct __WLKNode_LOCKANDKEY
{
	uint64_t _KEY;
	WLKNode _OBJ;
} WLKNode_LOCKANDKEY;

///// Definition for the implicit destructor for 'WLKNode' /////

void WLKNode_IMPLDESTRUCTOR(WLKNode* _P);
void WLKNode_IMPLDESTRUCTOR(WLKNode* _P)
{
	if (_P->next._ADDR != NULL)
	{
		/*Memory Safety Check*/ // if (_P->next._ADDR == NULL) _BASSALT_ERROR_NullPtr();
		/*Memory Safety Check*/ if (_P->next._CKEY != _P->next._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		WLKNode_IMPLDESTRUCTOR(&(_P->next._ADDR->_OBJ));
		_P->next._ADDR->_KEY = _BASSALT_LK_INVALIDKEY;
		free(_P->next._ADDR);
		_P->next._CKEY = _BASSALT_LK_INVALIDKEY;
		_P->next._ADDR = NULL;
	}
}

////////////////////////////////////////////////////////////

///// Definition for the function 'PrintWLKNode(WLKNode!& node)' /////

void PrintWLKNode(WLKNode* node)
{
	printf("Node(");
	/*Memory Safety Check*/ if (node == NULL) _BASSALT_ERROR_NullPtr();
	printf(node->prev._ADDR == NULL ? "o " : "* ");
	printf("%i", node->data);
	printf(node->next._ADDR == NULL ? " o" : " *");
	printf(")");
}

////////////////////////////////////////////////////////////

///// Definition for the struct of class 'WLKList' /////

typedef struct
{
	int32_t count;
	WLKNode_LPTR head;
	WLKNode_LPTR tail;
} WLKList;

///// Definition for the constructor of 'WLKList' /////

void WLKList_CONSTRUCTOR(WLKList* _THIS)
{
	_THIS->count = 0;
	_THIS->head._CKEY = _BASSALT_LK_INVALIDKEY;
	_THIS->head._ADDR = NULL;
	_THIS->tail._CKEY = _BASSALT_LK_INVALIDKEY;
	_THIS->tail._ADDR = NULL;
}

///// Definition for the implicit destructor for 'WLKList' /////

void WLKList_IMPLDESTRUCTOR(WLKList* _P)
{
	if (_P->head._ADDR != NULL)
	{
		/*Memory Safety Check*/ // if (_P->head._ADDR == NULL) _BASSALT_ERROR_NullPtr();
		/*Memory Safety Check*/ if (_P->head._CKEY != _P->head._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		WLKNode_IMPLDESTRUCTOR(&(_P->head._ADDR->_OBJ));
		_P->head._ADDR->_KEY = _BASSALT_LK_INVALIDKEY;
		free(_P->head._ADDR);
		_P->head._CKEY = _BASSALT_LK_INVALIDKEY;
		_P->head._ADDR = NULL;
	}
}

///// Definition for WLKList.append(int val) /////

void WLKList_append(WLKList* _THIS, int32_t val)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();

	WLKNode_LPTR newNode;
		WLKNode_LOCKANDKEY* _TEMP0 = (WLKNode_LOCKANDKEY*)malloc(sizeof(WLKNode_LOCKANDKEY));
			/*Memory Safety Check*/ if (_TEMP0 == NULL) _BASSALT_ERROR_MallocFailed();
			WLKNode _TEMP1;
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
		
		/*Memory Safety Check*/ if (_THIS->head._ADDR == NULL) _BASSALT_ERROR_NullPtr();
		/*Memory Safety Check*/ // if (_THIS->head._CKEY != _THIS->head._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		_THIS->head._ADDR->_OBJ.next = newNode;
		_THIS->tail = _THIS->head._ADDR->_OBJ.next;
	}
	else
	{
		newNode._ADDR->_OBJ.prev = _THIS->tail;

		/*Memory Safety Check*/ if (_THIS->tail._ADDR == NULL) _BASSALT_ERROR_NullPtr();
		/*Memory Safety Check*/ if (_THIS->tail._CKEY != _THIS->tail._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		_THIS->tail._ADDR->_OBJ.next = newNode;
		_THIS->tail = _THIS->tail._ADDR->_OBJ.next;
	}

	_THIS->count++;
}

///// Definition for WLKList.removeLast() /////

void WLKList_removeLast(WLKList* _THIS)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();

	/*Memory Safety Check*/ if (_THIS->tail._ADDR == NULL) _BASSALT_ERROR_NullPtr();
	/*Memory Safety Check*/ if (_THIS->tail._CKEY != _THIS->tail._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
	_THIS->tail = _THIS->tail._ADDR->_OBJ.prev;
	/*Memory Safety Check*/ if (_THIS->tail._ADDR == NULL) _BASSALT_ERROR_NullPtr();
	/*Memory Safety Check*/ if (_THIS->tail._CKEY != _THIS->tail._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
	/*Memory Safety Check*/ if (_THIS->tail._ADDR->_OBJ.next._ADDR == NULL) _BASSALT_ERROR_NullPtr();
	/*Memory Safety Check*/ // if (_THIS->tail._ADDR->_OBJ.next._CKEY != _THIS->tail._ADDR->_OBJ.next._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
	WLKNode_IMPLDESTRUCTOR(&(_THIS->tail._ADDR->_OBJ.next._ADDR->_OBJ));
	_THIS->tail._ADDR->_OBJ.next._ADDR->_KEY = _BASSALT_LK_INVALIDKEY;
	free(_THIS->tail._ADDR->_OBJ.next._ADDR);
	_THIS->tail._ADDR->_OBJ.next._CKEY = _BASSALT_LK_INVALIDKEY;
	_THIS->tail._ADDR->_OBJ.next._ADDR = NULL;

	_THIS->count--;
}

///// Definition for WLKList.printFrontToBack() /////

void WLKList_printFrontToBack(WLKList* _THIS)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();

	printf("List (%i) front-to-back:\n", _THIS->count);

	WLKNode_LPTR n = _THIS->head;
	while (n._ADDR != NULL)
	{
		printf("  ");
		/*Memory Safety Check*/ if (n._ADDR == NULL) _BASSALT_ERROR_NullPtr();
		/*Memory Safety Check*/ if (n._CKEY != n._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		PrintWLKNode(&(n._ADDR->_OBJ));
		printf("\n");
		
		n = n._ADDR->_OBJ.next;
	}
}

///// Definition for WLKList.printBackToFront() /////

void WLKList_printBackToFront(WLKList* _THIS)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();

	printf("List (%i) back-to-front:\n", _THIS->count);

	WLKNode_LPTR n = _THIS->tail;
	while (n._ADDR != NULL)
	{
		printf("  ");
		/*Memory Safety Check*/ if (n._ADDR == NULL) _BASSALT_ERROR_NullPtr();
		/*Memory Safety Check*/ if (n._CKEY != n._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		PrintWLKNode(&(n._ADDR->_OBJ));
		printf("\n");
		
		n = n._ADDR->_OBJ.prev;
	}
}

////////////////////////////////////////////////////////////

/*************************************************************************/
/********************************* No LK *********************************/
/*************************************************************************/

////////////////////////////////////////////////////////////

///// Definition for the LK pointer tuple for 'NLKNode' /////

typedef struct
{
	uint64_t _CKEY;
	struct __NLKNode_LOCKANDKEY* _ADDR;
} NLKNode_LPTR;

///// Definition for struct 'NLKNode' /////

typedef struct
{
	int32_t data;
	NLKNode_LPTR prev;
	NLKNode_LPTR next;
} NLKNode;

///// Definition for the LK object struct for 'NLKNode' /////

typedef struct __NLKNode_LOCKANDKEY
{
	uint64_t _KEY;
	NLKNode _OBJ;
} NLKNode_LOCKANDKEY;

///// Definition for the implicit destructor for 'NLKNode' /////

void NLKNode_IMPLDESTRUCTOR(NLKNode* _P);
void NLKNode_IMPLDESTRUCTOR(NLKNode* _P)
{
	if (_P->next._ADDR != NULL)
	{
		/*Memory Safety Check*/ // if (_P->next._ADDR == NULL) _BASSALT_ERROR_NullPtr();
		/*Memory Safety Check*/ if (_P->next._CKEY != _P->next._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		NLKNode_IMPLDESTRUCTOR(&(_P->next._ADDR->_OBJ));
		_P->next._ADDR->_KEY = _BASSALT_LK_INVALIDKEY;
		free(_P->next._ADDR);
		_P->next._CKEY = _BASSALT_LK_INVALIDKEY;
		_P->next._ADDR = NULL;
	}
}

////////////////////////////////////////////////////////////

///// Definition for the function 'PrintNLKNode(NLKNode!& node)' /////

void PrintNLKNode(NLKNode* node)
{
	printf("Node(");
	/*Memory Safety Check*/ if (node == NULL) _BASSALT_ERROR_NullPtr();
	printf(node->prev._ADDR == NULL ? "o " : "* ");
	printf("%i", node->data);
	printf(node->next._ADDR == NULL ? " o" : " *");
	printf(")");
}

////////////////////////////////////////////////////////////

///// Definition for the struct of class 'NLKList' /////

typedef struct
{
	int32_t count;
	NLKNode_LPTR head;
	NLKNode_LPTR tail;
} NLKList;

///// Definition for the constructor of 'NLKList' /////

void NLKList_CONSTRUCTOR(NLKList* _THIS)
{
	_THIS->count = 0;
	_THIS->head._CKEY = _BASSALT_LK_INVALIDKEY;
	_THIS->head._ADDR = NULL;
	_THIS->tail._CKEY = _BASSALT_LK_INVALIDKEY;
	_THIS->tail._ADDR = NULL;
}

///// Definition for the implicit destructor for 'NLKList' /////

void NLKList_IMPLDESTRUCTOR(NLKList* _P)
{
	if (_P->head._ADDR != NULL)
	{
		/*Memory Safety Check*/ // if (_P->head._ADDR == NULL) _BASSALT_ERROR_NullPtr();
		/*Memory Safety Check*/ if (_P->head._CKEY != _P->head._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		NLKNode_IMPLDESTRUCTOR(&(_P->head._ADDR->_OBJ));
		_P->head._ADDR->_KEY = _BASSALT_LK_INVALIDKEY;
		free(_P->head._ADDR);
		_P->head._CKEY = _BASSALT_LK_INVALIDKEY;
		_P->head._ADDR = NULL;
	}
}

///// Definition for NLKList.append(int val) /////

void NLKList_append(NLKList* _THIS, int32_t val)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();

	NLKNode_LPTR newNode;
		NLKNode_LOCKANDKEY* _TEMP0 = (NLKNode_LOCKANDKEY*)malloc(sizeof(NLKNode_LOCKANDKEY));
			/*Memory Safety Check*/ if (_TEMP0 == NULL) _BASSALT_ERROR_MallocFailed();
			NLKNode _TEMP1;
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
		
		/*Memory Safety Check*/ if (_THIS->head._ADDR == NULL) _BASSALT_ERROR_NullPtr();
		/*Memory Safety Check*/ // if (_THIS->head._CKEY != _THIS->head._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		_THIS->head._ADDR->_OBJ.next = newNode;
		_THIS->tail = _THIS->head._ADDR->_OBJ.next;
	}
	else
	{
		newNode._ADDR->_OBJ.prev = _THIS->tail;

		/*Memory Safety Check*/ if (_THIS->tail._ADDR == NULL) _BASSALT_ERROR_NullPtr();
		/*Memory Safety Check*/ if (_THIS->tail._CKEY != _THIS->tail._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		_THIS->tail._ADDR->_OBJ.next = newNode;
		_THIS->tail = _THIS->tail._ADDR->_OBJ.next;
	}

	_THIS->count++;
}

///// Definition for NLKList.removeLast() /////

void NLKList_removeLast(NLKList* _THIS)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();

	/*Memory Safety Check*/ if (_THIS->tail._ADDR == NULL) _BASSALT_ERROR_NullPtr();
	/*Memory Safety Check*/ if (_THIS->tail._CKEY != _THIS->tail._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
	_THIS->tail = _THIS->tail._ADDR->_OBJ.prev;
	/*Memory Safety Check*/ if (_THIS->tail._ADDR == NULL) _BASSALT_ERROR_NullPtr();
	/*Memory Safety Check*/ if (_THIS->tail._CKEY != _THIS->tail._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
	/*Memory Safety Check*/ if (_THIS->tail._ADDR->_OBJ.next._ADDR == NULL) _BASSALT_ERROR_NullPtr();
	/*Memory Safety Check*/ // if (_THIS->tail._ADDR->_OBJ.next._CKEY != _THIS->tail._ADDR->_OBJ.next._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
	NLKNode_IMPLDESTRUCTOR(&(_THIS->tail._ADDR->_OBJ.next._ADDR->_OBJ));
	_THIS->tail._ADDR->_OBJ.next._ADDR->_KEY = _BASSALT_LK_INVALIDKEY;
	free(_THIS->tail._ADDR->_OBJ.next._ADDR);
	_THIS->tail._ADDR->_OBJ.next._CKEY = _BASSALT_LK_INVALIDKEY;
	_THIS->tail._ADDR->_OBJ.next._ADDR = NULL;

	_THIS->count--;
}

///// Definition for NLKList.printFrontToBack() /////

void NLKList_printFrontToBack(NLKList* _THIS)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();

	printf("List (%i) front-to-back:\n", _THIS->count);

	NLKNode_LPTR n = _THIS->head;
	while (n._ADDR != NULL)
	{
		printf("  ");
		/*Memory Safety Check*/ if (n._ADDR == NULL) _BASSALT_ERROR_NullPtr();
		/*Memory Safety Check*/ if (n._CKEY != n._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		PrintNLKNode(&(n._ADDR->_OBJ));
		printf("\n");
		
		n = n._ADDR->_OBJ.next;
	}
}

///// Definition for NLKList.printBackToFront() /////

void NLKList_printBackToFront(NLKList* _THIS)
{
	/*Memory Safety Check*/ if (_THIS == NULL) _BASSALT_ERROR_NullPtr();

	printf("List (%i) back-to-front:\n", _THIS->count);

	NLKNode_LPTR n = _THIS->tail;
	while (n._ADDR != NULL)
	{
		printf("  ");
		/*Memory Safety Check*/ if (n._ADDR == NULL) _BASSALT_ERROR_NullPtr();
		/*Memory Safety Check*/ if (n._CKEY != n._ADDR->_KEY) _BASSALT_ERROR_BadLptr();
		PrintNLKNode(&(n._ADDR->_OBJ));
		printf("\n");
		
		n = n._ADDR->_OBJ.prev;
	}
}

/*************************************************************************/
/**************************** Main function ******************************/
/*************************************************************************/

////////////////////////////////////////////////////////////

int main(void)
{
	// Global LK initialization
	srand(time(NULL));
	rand();
	_BASSALT_LK_HEADER = (uint64_t)rand() << 48uL;
	_BASSALT_LK_KEYVAL = _BASSALT_LK_HEADER | ((uint64_t)rand() << 16uL) | (uint64_t)rand();

	printf("=== Starting Program ===\n");
	printf("\n");

	NLKList a;
	NLKList_CONSTRUCTOR(&a);
	
	NLKList_append(&a, 1);
	NLKList_printFrontToBack(&a);
	NLKList_printBackToFront(&a);
	printf("\n");

	NLKList_append(&a, 2);
	NLKList_printFrontToBack(&a);
	NLKList_printBackToFront(&a);
	printf("\n");

	NLKList_append(&a, 3);
	NLKList_printFrontToBack(&a);
	NLKList_printBackToFront(&a);
	printf("\n");

	NLKList b;
	NLKList_CONSTRUCTOR(&b);
	NLKList_append(&b, 11);
	NLKList_append(&b, 12);
	NLKList_append(&b, 13);
	NLKList_append(&b, 14);
	NLKList_append(&b, 15);
	NLKList_printFrontToBack(&b);
	NLKList_printBackToFront(&b);
	printf("\n");

	NLKList_removeLast(&b);
	NLKList_printFrontToBack(&b);
	NLKList_printBackToFront(&b);
	printf("\n");

	NLKList_removeLast(&b);
	NLKList_printFrontToBack(&b);
	NLKList_printBackToFront(&b);
	printf("\n");

	printf("=== Ending Program ===\n");

	// Implicit destructors
	NLKList_IMPLDESTRUCTOR(&a);
	NLKList_IMPLDESTRUCTOR(&b);

	return 0;
}

/*
int main(void)
{
	// Global LK initialization
	srand(time(NULL));
	rand();
	_BASSALT_LK_HEADER = (uint64_t)rand() << 48uL;
	_BASSALT_LK_KEYVAL = _BASSALT_LK_HEADER | ((uint64_t)rand() << 16uL) | (uint64_t)rand();

	printf("=== Starting Program ===\n");
	printf("\n");

	WLKList a;
	WLKList_CONSTRUCTOR(&a);
	
	WLKList_append(&a, 1);
	WLKList_printFrontToBack(&a);
	WLKList_printBackToFront(&a);
	printf("\n");

	WLKList_append(&a, 2);
	WLKList_printFrontToBack(&a);
	WLKList_printBackToFront(&a);
	printf("\n");

	WLKList_append(&a, 3);
	WLKList_printFrontToBack(&a);
	WLKList_printBackToFront(&a);
	printf("\n");

	WLKList b;
	WLKList_CONSTRUCTOR(&b);
	WLKList_append(&b, 11);
	WLKList_append(&b, 12);
	WLKList_append(&b, 13);
	WLKList_append(&b, 14);
	WLKList_append(&b, 15);
	WLKList_printFrontToBack(&b);
	WLKList_printBackToFront(&b);
	printf("\n");

	WLKList_removeLast(&b);
	WLKList_printFrontToBack(&b);
	WLKList_printBackToFront(&b);
	printf("\n");

	WLKList_removeLast(&b);
	WLKList_printFrontToBack(&b);
	WLKList_printBackToFront(&b);
	printf("\n");

	printf("=== Ending Program ===\n");

	// Implicit destructors
	WLKList_IMPLDESTRUCTOR(&a);
	WLKList_IMPLDESTRUCTOR(&b);

	return 0;
}
*/

////////////////////////////////////////////////////////////