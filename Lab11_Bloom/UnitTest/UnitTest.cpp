#include "stdafx.h"
#include <iostream>
#include "..\Engine\Physics.h"
#include <assert.h>

int main()
{
	bool testingPassed = Physics::UnitTest();
	
	if (!testingPassed) {
		return -1;
	}

	return 0;

}