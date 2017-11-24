{
	const int const_num = 1;
	vec2 testVec1;
	ivec4 testVec2;
	ivec4 testVec3 = ivec4(0, 1, 2, 3);
	int num = testVec[2];
	bool check;
	ivec4 tempVec;
	float tempFloat;
	int tempInt;
	float final;

	if (num == 1 || num == 0) {
		check = true;
		tempVec = testVec2 * testVec3;
		testVec1[0] = num * ( - const_num + 1);
		tempFloat = rsq(testVec2[1]);
		tempInt = dp3(testVec2, testVec3);
		
	} else {
		check = false;
	}

	final = rsq(tempFloat);
}
