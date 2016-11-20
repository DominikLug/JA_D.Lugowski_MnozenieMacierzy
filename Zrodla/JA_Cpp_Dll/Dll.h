#pragma once
class MyClass {
public:
	static __declspec(dllexport) void Foo( double **tabA, double **tabB, double **tabC, int startX, int stopX, int maxX, int maxY, int maxZ);

};