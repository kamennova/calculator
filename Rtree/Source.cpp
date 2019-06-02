#include <iostream>
#include <ctime>
#include "Module.h"

using namespace std;

int main() {

	string exp;

	cout << "Enter math expression: ";
	getline(cin, exp);

	MathExpression calculator;
	cout << "Infix: " << calculator.get_prefix_str(exp) << endl;

	//SyntaxTree my_tree;
	//my_tree.build(exp);

	//calculator.calculate(exp);

	system("pause");

	return 1;
}