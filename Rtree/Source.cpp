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

/*p = 3^bc + 4*5 - a;
q = a+a-0*65+bc*2;
p+q;*/