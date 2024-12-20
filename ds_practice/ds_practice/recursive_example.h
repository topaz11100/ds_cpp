#pragma once

#include "base.h"

void print_number_place(int n)
{
	if (n < 10)
	{
		cout << n << endl;
		return;
	}
	print_number_place(n / 10);
	cout << n % 10 << endl;
}

void example_9_1(int n)
{
	cout << n << endl;
	if (n > 1)
	{
		example_9_1(n - 1);
	}
	cout << n << endl;
}

void example_practice_9_1_8(string s)
{
	if (s == "") return;
	cout << s[s.size() - 1] << endl;
	s.pop_back();
	example_practice_9_1_8(s);
}

void exercise_1(int thre, int n = 1)
{
	if (n > thre) return;
	for (int i = 0; i < n; i += 1) cout << " ";
	cout << "This was written by call number " << n << endl;
	exercise_1( thre, n + 1 );
	for (int i = 0; i < n; i += 1) cout << " ";
	cout << "This ALSO written by call number " << n << endl;
}

void exercise_2(const string& prefix, int level)
{
	if (level == 0)
	{
		cout << prefix << endl;
		return;
	}
	for (int i = 1; i < 10; i += 1)
	{
		exercise_2(prefix + "." + char('0' + i), level - 1);
	}
}

template <class T>
void power_backtrack(vector<vector<T>>& to, vector<T>& cur, const vector<T>& v, int i)
{
	to.push_back(cur);

	for (; i < v.size(); i += 1)
	{
		cur.push_back(v[i]);
		power_backtrack(to, cur, v, i + 1);
		cur.pop_back();
	}
}


template <class T>
void power(vector<vector<T>>& to, const vector<T>& v)
{
	vector<T> cur;
	power_backtrack(to, cur, v, 0);
}

template <class T>
void print_vecvec(const vector<vector<T>>& v)
{
	for (auto& vv : v)
	{
		for (auto& vvv : vv)
		{
			cout << vvv << " ";
		}
		cout << endl;
	}
}



