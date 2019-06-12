#include <iostream>
#include <iostream>
#include <string>
#include <stack>
#include "Module.h"

using namespace std;

const map<char, unsigned int> MathExpression::operators_pri = MathExpression::create_map();

string MathExpression::get_postfix_str(string str) {
	string output;
	stack<char> operators;

	for (unsigned int i = 0, len =str.size(); i < len; i++) {
	
		if (MathExpression::is_delimiter(str[i])) { 
			continue; 
		}

		if (isdigit(str[i]) || isalpha(str[i])) { // variable name or number
			string number;

			while ((isdigit(str[i]) || isalpha(str[i])) && i < len) {
				number += str[i];
				i++;
			}

			i--;
			output += number + ' ';
		} else if (MathExpression::is_operator(str[i])) {
			
			if (str[i] == '(') {
				operators.push(str[i]);
			}
			else if (str[i] == ')') {

				char out = operators.top();
				operators.pop();

				while (out != '(') {
					output += out;
					output += ' ';
					out = operators.top();
					operators.pop();
				}
			}
			else {
				if (operators.size() > 0 &&
					MathExpression::get_priority(str[i]) <= MathExpression::get_priority(operators.top())) {
					output += operators.top();
					output += ' ';
					operators.pop();
				}

				operators.push(str[i]);
			}
		}
	}

	while (!operators.empty()) {
		output += operators.top();
		output += ' ';
		operators.pop();
	}

	return output;
} 

string MathExpression::reverse_exp(string line) {
	string reversed, 
		var_name;

	for (int i = line.size() - 1; i >= 0; i--) {
		if (isalpha(line[i])) {
			while (i >= 0 && (isalpha(line[i]))) {
				var_name += line[i];
				i--;
			}

			i++;
			reverse(var_name.begin(), var_name.end());
			reversed += var_name;
			var_name.erase();
		} else if (isdigit(line[i])) {
			while (i >= 0 && (isdigit(line[i]) || line[i] == ',' || line[i] == '.')) {
				var_name += line[i];
				i--;
			}

			i++;
			reverse(var_name.begin(), var_name.end());
			reversed += var_name;
			var_name.erase();
		}
		else if (line[i] == '(') {
			reversed += ')';
		}
		else if (line[i] == ')') {
			reversed += '(';
		}
		else {
			reversed += line[i];
		}
	}

	return reversed;
}

string MathExpression::get_prefix_str(string str) {
	string reversed = MathExpression::reverse_exp(str);

	string post = MathExpression::get_postfix_str(reversed);
	post = MathExpression::reverse_exp(post);

	return post;
} 

float MathExpression::postfix_calc(string str) {
	stack<float> temp;

	for (unsigned int i = 0, len = str.size(); i < len; i++) {
		
		if (isdigit(str[i])) {
			string number;

			while (isdigit(str[i]) && i < len) {
				number += str[i];
				i++;
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

float MathExpression::get_val(string var) {
	return isdigit(var[0]) ? stof(var) : 
		this->var_map.find(var)->second;
}

string MathExpression::calc(string oper, string a, string b) {
	char oper_char = oper[0];
	float a_val,
		b_val,
		if_val;

	a_val = isdigit(a[0]) ? stof(a) : this->var_map.find(a)->second;
	b_val = isdigit(b[0]) ? stof(b) : this->var_map.find(b)->second;

	switch (oper_char) {
	case '+': return to_string(a_val + b_val);
	case '-': return to_string(a_val - b_val);
	case '*': return to_string(a_val * b_val);
	case '/': return to_string(a_val / b_val);
	case '^': return to_string(pow(a_val, b_val));
	default: return 0;
	}
}

//---

void SyntaxTree::build(string exp) {

	unsigned int start = MathExpression::is_delimiter(exp[0]) ? 1 : 0;
	this->root->data = exp[start];

	for (unsigned int i = start+1, len = exp.size(); i < len; i++) {
		if (MathExpression::is_delimiter(exp[i])) {
			continue;
		}

		if (MathExpression::is_operator(exp[i])) {
			this->insert_operator(exp[i]);
		}
		else { // variable or number
			string val;
			val += exp[i];
			i++;

			while (i < len && !MathExpression::is_delimiter(exp[i])) {
				val += exp[i];
				i++;
			}
			i--;

			this->insert_val(val);
		}
	}
}

// insert as child in current node, if is right child, move up until operator node lacks 1 child
void SyntaxTree::insert_val(string var) {
	bool is_left = !this->curr->has_left_child();

	this->curr->insert_child(is_left, var);

	if (!is_left && this->curr->parent != NULL) {
		SyntaxNode * temp = this->curr->parent;

		while (temp != NULL && temp->has_both_children()) {
			temp = temp->parent;
		}

		this->curr = temp;
	}

}

void SyntaxTree::insert_operator(char op) {
	bool is_left = !this->curr->has_left_child();
	string data; data += op;

	this->curr = this->curr->insert_child(is_left , data);
};

float SyntaxTree::calc(MathExpression *exp) {
	return stof(this->root->calc_step(exp));
}

void SyntaxTree::optimize() {
	this->root->optimize_search();
}

void SyntaxNode::optimize_search() {
	if (this == NULL) return;

	if (!this->has_left_child()) { // reached leaf node
		this->optimize_step();
	}
	else {
		this->left->optimize_search();
		this->right->optimize_search();
	}
}

// optimization cases: * 0, + 0, - 0, 0^, ^0, * 1, / 1, 1^, ^1
void SyntaxNode::optimize_step() {
	bool is_left = this->is_left_child();

	if (this->data == "0") {
		if (this->parent->data == "*" || this->parent->data == "/") {
			cout << "Optimizaton case 0* or 0/ \n";
			this->parent->set_to_num("0");
		}
		else if (this->parent->data == "+") {
			cout << "Optimizaton case + 0 \n";
			this->parent->replace_self(!is_left); // replace parent with other child
		}
		else if (this->parent->data == "-") {
			if (!is_left) {
				cout << "Optimizaton case x-0 = x \n";
				this->parent->replace_self(!is_left);
			}
		}
		else if (this->parent->data == "^" ) {
			if (is_left) {
				cout << "Optimizaton case 0^x = 0 \n";
				this->parent->replace_self(is_left);
			}
			else {
				cout << "Optimizaton case x^0 = 0 \n";
				this->parent->set_to_num("1");
			}
		}
		else if (this->parent->data == "?") {
			this->parent->replace_self_level2(false, false);
		}
	}
	else if (this->data == "1") {
		if (this->parent->data == "*" || (this->parent->data == "/" && !is_left)) {
			cout << "Optimizaton case 1*x or x/1 = x \n";
			this->parent->replace_self(!is_left);
		}
		else if (this->parent->data == "^") {
			cout << "Optimizaton case 1^x = 1 or x^1 = x \n";
				this->parent->replace_self(true);
		}
	}	
}

void SyntaxNode::set_to_num(string num = "0") {
	this->left = NULL;
	this->right = NULL;
	this->data = num;

	this->optimize_step();
}

void SyntaxNode::replace_self(bool is_left) {
	SyntaxNode * child = is_left ? this->left : this->right;

	this->data = child->data;
	this->left = child->left;
	this->right = child->right;

	child = NULL;
	this->optimize_step();
}

void SyntaxNode::replace_self_level2(bool is_left, bool level2_is_left) {
	SyntaxNode * child = is_left ? this->left : this->right;
	child = level2_is_left ? child->left : child->right;

	this->data = child->data;
	this->left = child->left;
	this->right = child->right;

	child->parent = NULL;
	if (level2_is_left) { child->parent->right = NULL; }
	else {
		child->parent->left = NULL;
	}
	child = NULL;
}

//---

string SyntaxNode::calc_step(MathExpression* exp) {
	if (this->data == "?") {
		cout << this->left->data << endl;
		//if(MathExpression::is_operator(this->left))

		if (exp->get_val(this->left->calc_step(exp)) == 0) {
			return this->right->right->calc_step(exp);
		}

		return  this->right->left->calc_step(exp);
	} else if (this->has_left_child()) {
		return exp->calc(this->data, this->left->calc_step(exp), this->right->calc_step(exp));
	}

	return this->data;
}

bool SyntaxNode::is_left_child() {
	return (this->parent == NULL) || (this == this->parent->left);
};

bool SyntaxNode::has_left_child() {
	return this->left != NULL;
};

bool SyntaxNode::has_both_children() {
	return (this->left != NULL) && (this->right != NULL);
}

SyntaxNode * SyntaxNode::insert_child(bool is_left, string data) {
	SyntaxNode *child = new SyntaxNode(data, this);

	if (is_left) {
		this->left = child;
	}
	else {
		this->right = child;
	}

	return child;
}

//---

/* Read all lines, if line is simple expression (a = 7;), add to variables map
 else if var involves other variables save as string
use all variables in last line, build syntax tree with last line
 */
string FileReader::get_exp(string filename, MathExpression * exp) {
	ifstream inFile(filename);
	string line,
		prev,
		var_name,
		value;
		
	bool is_simple;

	getline(inFile, prev);

	while (!inFile.eof()) {
		getline(inFile, line);

		var_name = this->get_var(prev, value, is_simple);

		if (is_simple) {
			exp->var_map[var_name] = stof(value);
		}
		else {
			exp->var_exp_map[var_name] = value;
		}

		value.erase();
		prev = line;
	}

	cout << "Calculating " << line << "..." << endl;

	return this->get_exp_str(line, exp);
}

string FileReader::get_var(string line, string &val_str, bool &is_simple) {
	string  var_name;
	unsigned int i = 0,
		len = line.size();
	is_simple = true;

	// getting variable name
	while (i < len && !MathExpression::is_delimiter(line[i])) {
		var_name += line[i];
		i++;
	}

	i++;

	if (line[i] == '=') {
		while (i < len - 1) {
			if (MathExpression::is_delimiter(line[i])) {
				i++;
				continue;
			}

			if (!isdigit(line[i]) && !(line[i] == ',' || line[i] == '.')) { // expression contains other variables or operators
				is_simple = false;
			}

			val_str += line[i];
			i++;
		}
	}

	return var_name;
}

string FileReader::get_exp_str(string line, MathExpression* exp) {
	string exp_str,
		var_name;

	for (unsigned int i = 0, len = line.size(); i < len - 1; i++) {

		if (isalpha(line[i])) {
			while (isalpha(line[i]) && i < len-1) {
				var_name += line[i];
				i++;
			}
			i--;

			if (exp->var_exp_map.find(var_name) != exp->var_exp_map.end()) {
				exp_str += '(' + exp->var_exp_map.find(var_name)->second + ')';
			}
			else {
				exp_str += var_name;
			}

			var_name.erase();
		}
		else {
			exp_str += line[i];
		}
	}

	return exp_str;
}