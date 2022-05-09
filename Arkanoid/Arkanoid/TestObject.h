#pragma once
#include "Object.h";
class TestObject
{
public:
	TestObject(float x, float y);
	~TestObject();

	Object* parent = new Object();
	Object* child = new Object();

};

