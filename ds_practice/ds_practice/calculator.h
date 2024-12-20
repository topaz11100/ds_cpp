#pragma once

#include "stack.h"

const string Lp  = "(";
const string Rp  = ")";
const string Add = "+";
const string Sub = "-";
const string Mul = "*";
const string Div = "/";

class Calculator {
public:
	void input(istream& is);
	bool correct_parenthesis();
	double postfix_eval();
	void to_postfix();
	double eval();
	void print() { for (auto& a : origin) cout << a; }
private:
	vector<string> origin;
	Stack_linklist<string> oper;
	Stack_linklist<double> num;

	bool operator_priority(const string& input);
};

void Calculator::input(istream& is)
{
	origin.clear();
	string temp;
	getline(is, temp);
	stringstream ss{ temp };
	
	for (string word; ss >> word;)
	{
		origin.push_back(word);
	}
}

bool Calculator::correct_parenthesis()
{
	for (const string& s : origin)
	{
		if (s == Lp) oper.push(s);
		else if (s == Rp)
		{
			try { oper.pop(); }
			catch (const out_of_range& e) { return false; }
		}
	}
	return oper.empty();
}

double Calculator::postfix_eval()
{
	for (const string& s : origin)
	{
		try
		{
			num.push(stod(s));
		}
		catch (const invalid_argument& e)
		{
			double r = num.pop(), l = num.pop();
			if		(s == Add) num.push(l + r);
			else if (s == Sub) num.push(l - r);
			else if (s == Mul) num.push(l * r);
			else if (s == Div) num.push(l / r);
		}
	}
	return num.pop();
}

void Calculator::to_postfix()
{
	vector<string> temp;
	for (const string& s : origin)
	{
		try
		{
			stod(s);
			temp.push_back(s);
		}
		catch (const invalid_argument& e)
		{
			if (s == Rp)
			{
				while (oper.top() != Lp) temp.push_back(oper.pop());
				oper.pop();
			}
			else
			{
				while (operator_priority(s))
				{
					temp.push_back(oper.pop());
				}
				oper.push(s);
			}
		}
	}
	while (!oper.empty()) temp.push_back(oper.pop());
	origin.swap(temp);
}

double Calculator::eval()
{
	if (!correct_parenthesis()) { cout << "error ()"; return 0; }
	to_postfix();
	return postfix_eval();
}

bool Calculator::operator_priority(const string& input)
{
	//연산자스택에 그냥 넣는상황 - false, 빼서 넣어야하는 상황 - true
	if (oper.empty()) return false;
	string top = oper.top();
	if (top == Lp || input == Lp) return false;
	if (top == input) return true;
	if (top == "+" && input == "-") return true;
	if (top == "+" && input == "*") return false;
	if (top == "+" && input == "/") return false;
	if (top == "-" && input == "+") return true;
	if (top == "-" && input == "*") return false;
	if (top == "-" && input == "/") return false;
	else return true;
}