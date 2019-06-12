#include <iostream>
#include <ctime>
#include "Module.h"

using namespace std;

const string FILENAME = "file.txt";

int main() {
	string exp;

	MathExpression* expression = new MathExpression;
	FileReader my_reader;

	string exp_str = my_reader.get_exp(FILENAME, expression);	
	expression->calculate(exp_str); 
	system("pause");

	return 1;
}