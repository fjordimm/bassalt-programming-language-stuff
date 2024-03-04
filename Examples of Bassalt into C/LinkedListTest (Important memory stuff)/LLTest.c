
////////////////////////////////////////////////////////////

///// C Includes /////

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

////////////////////////////////////////////////////////////

///// Definition for Struct 'Node' /////

typedef struct INNERSTRUCT_Node
{
	int32_t FIELD__data__;
	struct INNERSTRUCT_Node* FIELD__next__;
} STRUCT__Node__;

////////////////////////////////////////////////////////////

///// Internal Destructors for 'STRUCT__Node__' /////

static void (*FUNCPTR_DESTRUCTOR_Node)(STRUCT__Node__*); // function pointer for which destructor is to be used for 'Node'

void DESTRUCTOR_Node_NORECTIFYING(STRUCT__Node__* VAR__node__);
void DESTRUCTOR_Node_NORECTIFYING(STRUCT__Node__* VAR__node__)
{
	printf("DESTRUCTOR FOR 'Node' CALLED ON Node(%i): no rectifying\n", VAR__node__->FIELD__data__);

	if (VAR__node__->FIELD__next__) { (*FUNCPTR_DESTRUCTOR_Node)(VAR__node__->FIELD__next__); }
	free(VAR__node__);
}

static STRUCT__Node__** RECTIFYEE_Node__cur__;
void DESTRUCTOR_Node_RECTIFY_cur(STRUCT__Node__* VAR__node__);
void DESTRUCTOR_Node_RECTIFY_cur(STRUCT__Node__* VAR__node__)
{
	printf("DESTRUCTOR FOR 'Node' CALLED ON Node(%i): rectifying cur\n", VAR__node__->FIELD__data__);

	if (VAR__node__->FIELD__next__) { (*FUNCPTR_DESTRUCTOR_Node)(VAR__node__->FIELD__next__); }
	free(VAR__node__);

	if (*RECTIFYEE_Node__cur__ == VAR__node__)
	{
		*RECTIFYEE_Node__cur__ = NULL;
		FUNCPTR_DESTRUCTOR_Node = &DESTRUCTOR_Node_NORECTIFYING;
	}
}

////////////////////////////////////////////////////////////

///// Definition for Function 'RecPrintStack(Node&)' /////

void FUNC__RecPrintStack__(STRUCT__Node__* VAR__node__);
void FUNC__RecPrintStack__(STRUCT__Node__* VAR__node__)
{
	if (VAR__node__)
	{
		printf("  Node(%i),\n", VAR__node__->FIELD__data__);
		FUNC__RecPrintStack__(VAR__node__->FIELD__next__);
	}
	else
	{
		printf("  NullNode\n");
	}
}

////////////////////////////////////////////////////////////

///// Main Function /////

int main(void)
{
	printf("=== Starting Program ===\n");

	STRUCT__Node__* VAR__head__ = (STRUCT__Node__*) malloc(sizeof(STRUCT__Node__));
		VAR__head__->FIELD__data__ = 1;
		VAR__head__->FIELD__next__ = NULL;
	STRUCT__Node__* VAR__cur__ = VAR__head__;
		RECTIFYEE_Node__cur__ = &VAR__cur__;
	for (int VAR_i = 2; VAR_i < 16; VAR_i++)
	{
		STRUCT__Node__* TEMP0 = (STRUCT__Node__*) malloc(sizeof(STRUCT__Node__));
			TEMP0->FIELD__data__ = VAR_i;
			TEMP0->FIELD__next__ = NULL;
		VAR__cur__->FIELD__next__ = TEMP0;

		VAR__cur__ = VAR__cur__->FIELD__next__;
	}
	
	printf("Stack:\n");
	FUNC__RecPrintStack__(VAR__head__);

	if (VAR__cur__)
	{ printf("cur = Node(%i)\n", VAR__cur__->FIELD__data__); }
	else
	{ printf("cur is null\n"); }
	printf("\n");

	FUNCPTR_DESTRUCTOR_Node = &DESTRUCTOR_Node_RECTIFY_cur;
	(*FUNCPTR_DESTRUCTOR_Node)(VAR__head__->FIELD__next__->FIELD__next__->FIELD__next__);
	VAR__head__->FIELD__next__->FIELD__next__->FIELD__next__ = NULL;
	printf("\n");

	printf("Stack:\n");
	FUNC__RecPrintStack__(VAR__head__);

	if (VAR__cur__)
	{ printf("cur = Node(%i)\n", VAR__cur__->FIELD__data__); }
	else
	{ printf("cur is null\n"); }
	printf("\n");

	printf("=== Ending Program ===\n");

	FUNCPTR_DESTRUCTOR_Node = &DESTRUCTOR_Node_NORECTIFYING;
	(*FUNCPTR_DESTRUCTOR_Node)(VAR__head__);
	VAR__head__ = NULL;
	printf("\n");

	return 0;
}

////////////////////////////////////////////////////////////
