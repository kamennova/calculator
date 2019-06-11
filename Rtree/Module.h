#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <fstream>

using namespace std;

class SyntaxNode;
class SyntaxTree;
class MathExpression;

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
		string calc_step(MathExpression* exp);
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
		float calc(MathExpression* exp);
		float get_val(string a);
		void optimize();
};

class MathExpression {
private: static const map<char, unsigned int> operators_pri;
		 //SyntaxTree tree;

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
			MathExpression() {
				//SyntaxTree(this) tree;
			};

			map<string, float> var_map;
			map<string, string> var_exp_map;

			static bool is_delimiter(char s) {
				return (s == '=' || s == ' ');
			};

			static bool is_operator(char s) {
				return (MathExpression::operators_pri.find(s) != MathExpression::operators_pri.end());
			};

			float calculate(string input) {
				float result;
				string prefix = this->get_prefix_str(input);
				cout << "Prefix notation: " << prefix << endl;

				SyntaxTree my_tree;
				my_tree.build(prefix);
				my_tree.optimize();

				result = my_tree.calc(this);
				cout << "Result: " << result << endl;

				return result;
			};

			string calc(string op, string a, string b);

			static string reverse_exp(string line);
			static string get_prefix_str(string str);
			static string get_postfix_str(string str);
			float postfix_calc(string str);
};

class FileReader {
public: FileReader() {
				};

		// returns expression
		string get_exp(string filename, MathExpression * exp);
		string get_var(string line, string &val_str, bool &is_simple);
		string get_exp_str(string line, MathExpression* exp);
};