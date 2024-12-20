#pragma once

#include "base.h"

// 힙을 이용한 우선순위 큐 구현 ------------------------------------
template<class V>
class heap
{
public:
	static enum class mode
	{
		max, min
	};
	typedef heap<V>::mode M;

	heap(M m);

	void push(const V& v);
	V pop();

	const V& top() const { return data[0]; }
	int size() const { return data.size(); }

	void print() const;

public:
	vector<V> data;
	mode m;

	void up_heap(int i);
	void down_heap(int i);
};
template<class V>
heap<V>::heap(M m)
{
	this->m = m;
}
template<class V>
void heap<V>::push(const V& v)
{
	data.push_back(v);
	up_heap(size() - 1);
}
template<class V>
V heap<V>::pop()
{
	V result{ data[0] };
	swap(data[0], data[size() - 1]);
	data.pop_back();
	down_heap(0);
	return result;
}
template<class V>
void heap<V>::up_heap(int i)
{
	if (i == 0) return;
	int p = (i - 1) / 2;
	if (m == M::max && data[p] >= data[i])
	{
		return;
	}
	if (m == M::min && data[p] <= data[i])
	{
		return;
	}
	swap(data[p], data[i]);
	up_heap(p);
}
template<class V>
void heap<V>::down_heap(int i)
{
	int l = 2 * i + 1, r = 2 * i + 2, p;
	if (l >= size()) return;
	else if (r >= size()) r = l;

	if (m == M::max)
	{
		if (data[l] >= data[r]) p = l;
		else p = r;
		if (data[i] > data[p]) return;
	}
	if (m == M::min)
	{
		if (data[l] <= data[r]) p = l;
		else p = r;
		if (data[i] < data[p]) return;
	}
	swap(data[p], data[i]);
	down_heap(p);
}
template<class V>
void heap<V>::print() const
{
	for (auto& x : data)
	{
		cout << x << " ";
	}
	cout << endl;
}
// 힙을 이용한 우선순위 큐 구현 ------------------------------------
