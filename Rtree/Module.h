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

			 static unsigned int get_priority(char op) {
				 return MathExpression::operators_pri.find(op)->second;
			 }

		public:

			static bool is_delimiter(char s) {
				return (s == '=' || s == ' ');
			};

			static bool is_operator(char s) {
				return (MathExpression::operators_pri.find(s) != MathExpression::operators_pri.end());
			};

			float calculate(string input) {
				string output = this->get_postfix_str(input);
				cout << "Output: " << output << endl;

				float result = this->postfix_calc(output);

				cout << "Result: " << result << endl;

				return result;
			};

			static float calc(string op, float a, float b);

			static void reverse_exp(string &str);
			static string get_prefix_str(string str);
			static string get_postfix_str(string str);
			float postfix_calc(string str);
};

class SyntaxNode {
public: string data;
		SyntaxNode* left;
		SyntaxNode* right;
		SyntaxNode* parent;

		SyntaxNode(string n_data = "", SyntaxNode * n_parent = NULL, SyntaxNode *n_left = NULL, SyntaxNode *n_right = NULL) {
			 left = n_left;
			 right = n_right;
			 data = n_data;
			 parent = n_parent;
		 };

		SyntaxNode* insert_child(bool is_left, string data);
		 bool is_left_child();
		 bool has_both_children();
		 bool has_left_child();
		 float calc_step();
		 void optimize_step();
		 void set_to_num(string num);
		 void replace_self(bool is_left);
		 void optimize_search();
};

class SyntaxTree {
	public:	SyntaxNode* root,
					* curr;

		SyntaxTree() {
			SyntaxNode* temp = new SyntaxNode();
			this->root = temp;
			curr = root;
		};

		void build(string exp);
		void insert_operator(char op);
		void insert_val(string var);
		float calc();
		void optimize();
};