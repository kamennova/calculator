#include <iostream>
#include <ctime>
#include "Module.h"

using namespace std;

int main() {

	string exp;

	cout << "Enter math expression: ";
	getline(cin, exp);

	MathExpression calculator;
	calculator.calculate(exp);

	system("pause");

	return 1;
}