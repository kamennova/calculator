#include <iostream>
#include <ctime>
#include "Module.h"

using namespace std;

int main() {
	string exp;

	cout << "Enter math expression: ";
	getline(cin, exp);

	MathExpression calculator;
	string prefix = calculator.get_prefix_str(exp);
	cout << "Infix: " << prefix << endl;

	SyntaxTree my_tree;
	my_tree.build(prefix);
	my_tree.optimize();
	my_tree.calc();

	system("pause");

	return 1;
}