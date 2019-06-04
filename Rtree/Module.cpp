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

		if (isdigit(str[i])) {
			string number;

			while (isdigit(str[i]) && i < len) {
				number += str[i];
				i++;
			}

			i--;
			output += number + ' ';
			continue;
		}

		if (MathExpression::is_operator(str[i])) {
			
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

void MathExpression::reverse_exp(string &str) {
	reverse(str.begin(), str.end());

	for (unsigned int i = 0, len = str.size(); i < len; i++) {
		if (str[i] == '(') {
			str[i] = ')';
		}
		else if (str[i] == ')') {
			str[i] = '(';
		}
	}
}

string MathExpression::get_prefix_str(string str) {
	MathExpression::reverse_exp(str);

	string post = MathExpression::get_postfix_str(str);
	cout << post << endl;
	reverse(post.begin(), post.end());

	return post;
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

float MathExpression::calc(string oper, float a, float b) {
	char oper_char = oper[0];

	switch (oper_char) {
	case '+': return a + b;
	case '-': return a - b; 
	case '*': return a * b;
	case '/': return a / b; 
	case '^': return pow(a, b);
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

float SyntaxTree::calc() {
	float res = this->root->calc_step();
	cout << "Result: " << res << endl;

	return res;
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
			this->parent->set_to_num("0");
		}
		else if (this->parent->data == "+") {
			this->parent->replace_self(!is_left); // replace parent with other child
		}
		else if (this->parent->data == "-") {
			if (!is_left) {
				this->parent->replace_self(!is_left);
			}
		}
		else if (this->parent->data == "^" ) {
			if (is_left) {
				this->parent->replace_self(is_left);
			}
			else {
				this->parent->set_to_num("1");
			}
		}
	}
	else if (this->data == "1") {
		if (this->parent->data == "*" || (this->parent->data == "/" && !is_left)) {
			this->parent->replace_self(!is_left);
		}
		else if (this->parent->data == "^") {
			//if (is_left) {
				this->parent->replace_self(true);
			//}
			//else {
				//this->parent->replace_self(!is)
			//}
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
}

//---

float SyntaxNode::calc_step() {
	if (this->has_left_child()) {
		return MathExpression::calc(this->data, this->left->calc_step(), this->right->calc_step());
	}

	return stoi(this->data);
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