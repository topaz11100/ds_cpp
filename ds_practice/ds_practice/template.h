#pragma once

#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>

using namespace std;

template <class T>
T sum(const T& a, const T& b) {
	return a + b;
}

template <class T>
class vector {
	vector();
	void push_back(const T& d);
	void reserve(int n);
	T& operator[](int n);

	class iterator;

private:
	T* data;
	int len;
	int cap;
};

template <class T>
vector<T>::vector() {
	data = new T[4]();
	len = 0;
	cap = 4;
}

template <class T>
void vector<T>::reserve(int n) {
	if (n < len) return;
	new_data = new T[n];
	copy(data, data + len, new_data);
	delete data;
	data = new_data;
	cap = n;
}

template <class T>
void vector<T>::push_back(const T& d) {
	data[len] = d;
	length += 1;
}

template <class T>
T& vector<T>::operator[](int n) {
	return data[n];
}

class student {
public:
	student(int id = -1, string name = "", double score = -1);
	void setid(int id) { this->id = id; }
	void setname(string name) { memcpy(this->name, name.c_str(), max_len); }
	void setscore(double score) { this->score = score; }
	int getid() { return this->id; }
	string getname() { return string(this->name); }
	double getscore() { return this->score; }
	static const int max_len = 32;
private:
	int id;
	char name[max_len + 1];
	double score;
};
student::student(int id, string name, double score) {
	this->id = id;
	memcpy(this->name, name.c_str(), max_len);
	this->score = score;
}

/*
* �ʱ�
�迭������������
�����迭�κ��͸��鶧�ؾ��Ұ͵�
������
bag and set���� ����

�Ǳ�
bag and set ���տ��걸��
���Ϳ����°ű���
�������̺귯���̿�
������Է� �� ó���ϱ�
06_Template list ���� 2�� �����ϱ�

*/








