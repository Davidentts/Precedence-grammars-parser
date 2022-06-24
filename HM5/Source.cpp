#include <iostream>
#include <stack>
#include <map>
#include<string>
#include"Header.h"

const unsigned int size = 9;

char start_str[] = "  ";
std::stack <char> raw_str;
std::stack <char> processed_str;
std::map<std::string, char> grammar;
std::map<char, int> mtrx_dict;

int relation_matrix[size][size] = {
	{0,0,0,0,0,0,0,0,2},
	{1,3,3,1,3,3,3,3,2},
	{2,2,2,2,2,2,2,2,2},
	{0,0,0,0,0,0,0,0,2},
	{0,0,1,0,3,3,3,0,2},
	{0,0,1,0,3,3,3,0,2},
	{2,2,2,2,2,2,2,2,2},
	{0,0,0,1,3,0,0,3,2},
	{3,3,3,3,3,3,3,3,0}
};

/*
* 0 - null
* 1 - =
* 2 - >
* 3 - <
*/

int main()
{
	bool correct_string = false;
	while(!correct_string)
	{
		std::cout << "Enter a line with '#' at the begining and end: ";
		std::cin >> start_str;
		if (start_str[0] != '#' && start_str[strlen(start_str) - 1] != '#')
		{
			std::cout << "Mistake!\nInvalid string format. Try again\n";
		}
		else correct_string = true;
	}

	init();
	shift_or_convolution();
	//is_derivable();
	return 0;
}

void init()
{
	for(int i = strlen(start_str)-1; i > 0; i--)
	{
		raw_str.push(start_str[i]);
	}
	processed_str.push(start_str[0]);

	grammar["CS"]	= 'S';
	grammar["CB"]	= 'S';
	grammar["C"]	= 'S';
	grammar["A"]	= 'C';
	grammar["aA"]	= 'A';
	grammar["bA"]	= 'A';
	grammar["c"]	= 'A';
	grammar["dB"]	= 'B';
	grammar["a"]	= 'B';
	grammar["d"]	= 'B';

	mtrx_dict['S'] = 0;
	mtrx_dict['C'] = 1;
	mtrx_dict['A'] = 2;
	mtrx_dict['B'] = 3;
	mtrx_dict['a'] = 4;
	mtrx_dict['b'] = 5;
	mtrx_dict['c'] = 6;
	mtrx_dict['d'] = 7;
	mtrx_dict['#'] = 8;
}

void shift_or_convolution()
{
	int rel = relation();
	if (rel == 1 || rel == 3)
	{
		processed_str.push(raw_str.top());
		raw_str.pop();
		shift_or_convolution();
	}
	else
		convolution();
}

void convolution()
{
	char top_processed = processed_str.top();

	std::string base;
	base += top_processed;

	processed_str.pop();
	while (relation(top_processed) != 3)
	{
		top_processed = processed_str.top();
		processed_str.pop();
		std::string old_base = base;
		base = top_processed;
		base += old_base;
	}
	processed_str.push(which_rule(base));

	if (processed_str.top() != 'S' || processed_str.size() > 2)
		shift_or_convolution();
	else
		end_or_next();
}

void end_or_next()
{
	if (raw_str.top() == '#')
		success_or_failed();
	else
		shift_or_convolution();
}

void success_or_failed()
{
	char top = processed_str.top();
	processed_str.pop();
	if (top == 'S' && processed_str.top() == '#')
		std::cout << "SUCCESS!\nThe chain does not belong to the language";
	else
		error();
}

int relation()
{
	auto right = mtrx_dict.find(raw_str.top());
	auto left = mtrx_dict.find(processed_str.top());

	if (left == mtrx_dict.end() || right == mtrx_dict.end())
		error();

	if (relation_matrix[left->second][right->second] == 0)
		error();

	std::cout << "relation = " << relation_matrix[left->second][right->second] << "\n";

	return relation_matrix[left->second][right->second];
}

int relation(char top)
{
	auto left = mtrx_dict.find(processed_str.top());
	auto right = mtrx_dict.find(top);
	if (left == mtrx_dict.end() || right == mtrx_dict.end())
		error();

	std::cout << "relation = " << relation_matrix[left->second][right->second] << "\n";

	return relation_matrix[left->second][right->second];
}

char which_rule(std::string form)
{
	auto rule = grammar.find(form);
	if (rule == grammar.end())
		error();

	return rule->second;
}

void error()
{
	std::cout << "FAILED!\nThe chain does not belong to the language!";
	exit(0);
}