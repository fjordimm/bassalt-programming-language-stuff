
////////////////////////////////////////////////////////////

///// C Includes /////

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

////////////////////////////////////////////////////////////

///// Global Key-Value Incrementer For Locks-and-keys /////

static int64_t BASSALT_KEYINCRVAL = 0;

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
	int64_t KEY;
	_Point_ OBJ;
} _Point_LOCKANDKEY;

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
	
	printf("=== Ending Program ===\n");

	return 0;
}

////////////////////////////////////////////////////////////
