#pragma once

template <class C>
void cout_print(const C& container)
{
	for (auto& v : container)
	{
		cout << v << " ";
	}
	cout << endl;
}

template <class I, class V>
I linear_search(const V& v, const I& start, const I& end)
{
	for (auto i = start; i != end; ++i)
	{
		if (*i == v) return i;
	}
	return end;
}

template <class I, class V>
I binary_search(const V& v, const I& start, const I& end)
{//[start, end)
	if (start >= end) return end;
	auto mid = start + (end - start) / 2;
	if (v == *mid) return mid;
	else if (v < *mid) return binary_search(v, start, mid);
	else if (v > *mid) return binary_search(v, ++mid, end);
}

template <class I>
void select_sort(I start, I end)
{
	if (start == end) return;

	I s = start, i = start;
	auto min = *start;
	for (; i != end; ++i)
	{
		if (*i < min)
		{
			min = *i;
			s = i;
		}
	}
	swap(*s, *start);

	select_sort(++start, end);
}

template <class I>
void insert_sort_rec(I start, I end, I zero)
{
	if (start == end) return;

	/*
	I i = zero;
	for (; i != start; ++i)
	{
		if (*i > *start) break;
	}
	*/

	I i = binary_search(*start, zero, start);

	auto v = *start;
	I next = start, pre = start;
	for (; next != i; --next)
	{
		--pre;
		*next = *pre;
	}
	*i = v;

	insert_sort_rec(++start, end, zero);
}

template <class I>
void insert_sort(I start, I end)
{
	insert_sort_rec(++start, end, start);
}





