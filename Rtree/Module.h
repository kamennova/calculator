#include <iostream>
#include <string>
#include <stack>
#include <map>

using namespace std;

class MathExpression {
private: static const map<char, unsigned int> operators_pri;

		 static map<char, unsigned int> create_map() {
			 map<char, unsigned int> m;

			 m['('] = 0;
			 m[')'] = 0;
			 m['+'] = 2;
			 m['-'] = 3;
			 m['*'] = 4;
			 m['/'] = 4;
			 m['^'] = 5;

			 return m;
		 };

			 static bool is_delimiter(char s) {
				 return (s == '=' || s == ' ');
			 };

			 static bool is_operator(char s) {
				 return (MathExpression::operators_pri.find(s) != MathExpression::operators_pri.end());
			 };

			 static unsigned int get_priority(char op) {
				 return MathExpression::operators_pri.find(op)->second;
			 }

		public:

			float calculate(string input) {
				string output = this->get_expression(input);
				cout << "Output: " << output << endl;

				float result = this->postfix_calc(output);

				cout << "Result: " << result << endl;

				return result;
			};

			string get_expression(string str);
			float postfix_calc(string str);
};