
////////////////////////////////////////////////////////////

///// C Includes /////

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

////////////////////////////////////////////////////////////

///// Bassalt Helper Functions /////

void BASSALT_PRINTERROR_DEREFBADWLPTR()
{
	printf("Tried to dereference an invalid weak lock-and-key pointer!\n");
}

////////////////////////////////////////////////////////////

///// Global Key Incrementer For Locks-and-keys /////

#define BASSALT_LOCKSANDKEYS_NULLKEY 0
static uint64_t BASSALT_LOCKSANDKEYS_KEYINCR = 1;

////////////////////////////////////////////////////////////

///// Definition for Struct 'Point' /////

typedef struct
{
	float _x_;
	float _y_;
} _Point_;

///// Definition for Lock-and-key Struct for 'Point' /////

typedef struct
{
	uint64_t KEY;
	_Point_ OBJ;
} _Point_LOCKANDKEY;

///// Definition for Weak Lock-and-key Pointer Tuple for 'Point' /////

typedef struct
{
	uint64_t CKEY;
	_Point_LOCKANDKEY* PTR;
} _Point_LOCKANDKEY_WLPTR;

////////////////////////////////////////////////////////////

///// Definition for Function 'PrintPoint(Point& p)' /////

void _PrintPoint_(_Point_* _p_)
{
	printf("Point(%f, %f)\n", _p_->_x_, _p_->_y_);
}

////////////////////////////////////////////////////////////

///// Main Function /////

int main(void)
{
	printf("=== Starting Program ===\n");

	_Point_LOCKANDKEY* _a_ = (_Point_LOCKANDKEY*)malloc(sizeof(_Point_LOCKANDKEY));
		_Point_ TEMP0;
			TEMP0._x_ = 3.0f;
			TEMP0._y_ = 9.0f;
		_a_->KEY = BASSALT_LOCKSANDKEYS_KEYINCR++;
		_a_->OBJ = TEMP0;

	_Point_LOCKANDKEY_WLPTR _b_;
		_b_.CKEY = _a_->KEY;
		_b_.PTR = _a_;
	
	printf("a = ");
	_PrintPoint_(&(_a_->OBJ));
	printf("b = ");
	if (_b_.CKEY == _b_.PTR->KEY) // checks for invalid wlptr (possible optimization: the compiler doesn't have to do this here since it knows the object of _a_ couldn't have been deleted)
		_PrintPoint_(&(_b_.PTR->OBJ));
		else BASSALT_PRINTERROR_DEREFBADWLPTR();
	
	printf("\n");
	_a_->OBJ._x_ = 777.0f;
	_a_->OBJ._y_ += 0.1f;

	printf("a = ");
	_PrintPoint_(&(_a_->OBJ));
	printf("b = ");
	if (_b_.CKEY == _b_.PTR->KEY) // checks for invalid wlptr (possible optimization: the compiler doesn't have to do this here since it knows the object of _a_ couldn't have been deleted)
		_PrintPoint_(&(_b_.PTR->OBJ));
		else BASSALT_PRINTERROR_DEREFBADWLPTR();

	printf("\n");
	if (_b_.CKEY != _b_.PTR->KEY) BASSALT_PRINTERROR_DEREFBADWLPTR(); // check for invalid wlptr (possible optimization: the compiler doesn't have to do this here since it knows the object of _a_ couldn't have been deleted)
	_b_.PTR->OBJ._x_ = 444.0f;
	_b_.PTR->OBJ._y_ += 0.08f;

	printf("a = ");
	_PrintPoint_(&(_a_->OBJ));
	printf("b = ");
	if (_b_.CKEY == _b_.PTR->KEY) // checks for invalid wlptr (possible optimization: the compiler doesn't have to do this here since it knows the object of _a_ couldn't have been deleted)
		_PrintPoint_(&(_b_.PTR->OBJ));
		else BASSALT_PRINTERROR_DEREFBADWLPTR();

	printf("\n");
	_a_->KEY = BASSALT_LOCKSANDKEYS_NULLKEY;
	free(_a_);
	_a_ = NULL;

	printf("a cannot be printed due to compiler errors\n");
	printf("b = ");
	if (_b_.CKEY == _b_.PTR->KEY) // checks for invalid wlptr
		_PrintPoint_(&(_b_.PTR->OBJ));
		else BASSALT_PRINTERROR_DEREFBADWLPTR();
	
	printf("\n");
	_a_ = (_Point_LOCKANDKEY*)malloc(sizeof(_Point_LOCKANDKEY));
		_Point_ TEMP1;
			TEMP1._x_ = 15.0f;
			TEMP1._y_ = 30.0f;
		_a_->KEY = BASSALT_LOCKSANDKEYS_KEYINCR++;
		_a_->OBJ = TEMP1;
	
	printf("a = ");
	_PrintPoint_(&(_a_->OBJ));
	printf("b = ");
	if (_b_.CKEY == _b_.PTR->KEY) // checks for invalid wlptr (possible optimization: the compiler doesn't have to do this here since it knows the object of _a_ couldn't have been deleted)
		_PrintPoint_(&(_b_.PTR->OBJ));
		else BASSALT_PRINTERROR_DEREFBADWLPTR();

	printf("=== Ending Program ===\n");

	// Implicit memory freeing
	free(_a_);

	return 0;
}

////////////////////////////////////////////////////////////
