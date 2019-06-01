#include <iostream>
#include <iostream>
#include <string>
#include <stack>
#include "Module.h"

using namespace std;

const map<char, unsigned int> MathExpression::operators_pri = MathExpression::create_map();

string MathExpression::get_expression(string str) {
	string output;
	stack<char> operators;

	for (unsigned int i = 0, len =str.size(); i < len; i++) {
	
		if (this->is_delimiter(str[i])) { 
			continue; 
		}

		if (isdigit(str[i])) {
			string number;

			while (isdigit(str[i])) {
				number += str[i];
				i++;

				if (i == len) break;
			}

			i--;
			output += number + ' ';
			continue;
		}

		if (this->is_operator(str[i])) {
			
			if (str[i] == '(') {
				operators.push(str[i]);
			}
			else if (str[i] == ')') {

				char out = operators.top();
				operators.pop();

				while (out != '(') {
					output += out << ' ';
					out = operators.top();
					operators.pop();
				}
			}
			else {
				if (operators.size() > 0 &&
					this->get_priority(str[i]) <= this->get_priority(operators.top())) {
					output += operators.top() << ' ';
					operators.pop();
				}

				operators.push(str[i]);
			}
		}
	}

	while (!operators.empty()) {
		output += operators.top() << ' ';
		operators.pop();
	}

	return output;
}

float MathExpression::postfix_calc(string str) {
	stack<float> temp;

	for (unsigned int i = 0, len = str.size(); i < len; i++) {
		
		if (isdigit(str[i])) {
			string number;

			while (isdigit(str[i])) {
				number += str[i];
				i++;

				if (i == len) {
					break;
				}
			}

			cout << number << endl;

			i--;
			temp.push(stoi(number));
			continue;
		}
		else if (this->is_operator(str[i])) {

			float a = temp.top(); temp.pop();
			float b = temp.top(); temp.pop();

			cout << a << " " << b << endl;

			float res = 0;

			switch (str[i]) {
			case '+': res = a + b;
				break;
			case '-': res = b - a;
				break;
			case '*': res = a * b;
				break;
			case '/': res = b / a;
				break;
			case '^': res = pow(b, a);
				break;
			}

			temp.push(res);
		}
	}

	return temp.top();
}