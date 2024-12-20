#pragma once

#include "base.h"

template <typename T>
concept Number = is_arithmetic_v<T>;


// ���� ��� -----------------------------------------------------------------------

template<class K, Number V>
class graph_matrix
{
public:
	graph_matrix() {}

	void insert_node(const K& node);
	void delete_node(const K& node);

	void set_link(const K& from, const K& to, const V& weight);
	V is_adjacent(const K& from, const K& to);

	void print();

	template<class F>
	void bfs(const K& start, F func);

	template<class F>
	void dfs_use_stack(const K& start, F func);

	template<class F>
	void dfs_use_recursion(const K& start, F func);

	unordered_map<K, pair<K, V>> dijkstra(K start);

private:
	vector<vector<V>> data;
	unordered_map<K, int> label;

	template<class F>
	void dfs_recursion(const K& start, F func, set<K>& visited);
};

template<class K, Number V>
void graph_matrix<K, V>::insert_node(const K& node)
{
	if (label.find(node) != label.end()) return;

	label[node] = data.size();

	for (auto& v : data)
	{
		v.push_back(V());
	}
	vector<V> temp(data.size() + 1);
	data.push_back(temp);
}

template<class K, Number V>
void graph_matrix<K, V>::delete_node(const K& node)
{
	if (label.find(node) == label.end()) return;

	int index = label[node];

	label.erase(node);

	data.erase(data.begin() + index);

	for (auto& v : data)
	{
		v.erase(v.begin() + index);
	}

	for (auto& i : label)
	{
		if (i.second > index) --(i.second);
	}
}

template<class K, Number V>
void graph_matrix<K, V>::set_link(const K& from, const K& to, const V& weight)
{
	data[label[from]][label[to]] = weight;
}

template<class K, Number V>
V graph_matrix<K, V>::is_adjacent(const K& from, const K& to)
{
	return data[label[from]][label[to]];
}

template<class K, Number V>
void graph_matrix<K, V>::print()
{
	for (auto& k : label)
	{
		cout.width(10);
		cout << k.first << " : ";
		for (const auto& v : data[k.second])
		{
			cout << v << " ";
		}
		cout << endl;
	}
}


template<class K, Number V>
template<class F>
void graph_matrix<K, V>::bfs(const K& start, F func)
{
	set<K> visited;
	queue<K> q;
	q.push(start);

	//visited.size() != label.size() �� �����ʴ� ����
	// �����ִ� ����� ���� ��ü��庸�� ���� �� ���� �����̴�
	// ���� Ž���� ���ϸ� �ڿ����� q�� ��� �Ǿ� !q.empty()�� �ڿ�������
	while (!q.empty())
	{
		K cur_node = q.front();
		func(cur_node);
		visited.insert(cur_node);

		for (auto& v : label)
		{
			if (is_adjacent(cur_node, v.first) > V() && visited.find(v.first) == visited.end())
			{//�����ϰ� �湮 �� �� ����
				q.push(v.first);
			}
		}

		q.pop();
	}
}

template<class K, Number V>
template<class F>
void graph_matrix<K, V>::dfs_use_stack(const K& start, F func)
{
	set<K> visited;
	stack<K> s;
	s.push(start);

	//visited.size() != label.size() �� �����ʴ� ����
	// �����ִ� ����� ���� ��ü��庸�� ���� �� ���� �����̴�
	// ���� Ž���� ���ϸ� �ڿ����� s�� ��� �Ǿ� !s.empty()�� �ڿ������� 
	while (!s.empty())
	{
		K cur_node = s.top();
		if (visited.find(cur_node) == visited.end())
		{//Ž���� ���� ���� ��忡 ���� (����̹Ƿ� �б����� ���� �ߺ� ����� �� �ִ�)
			func(cur_node);
			visited.insert(cur_node);
		}

		bool no_node_to_go = true;
		for (auto& v : label)
		{
			if (is_adjacent(cur_node, v.first) > V() && visited.find(v.first) == visited.end())
			{//�����ϰ� visited�� ������ -> �湮 ���� ���� ���� �߰�
				s.push(v.first);
				no_node_to_go = false;
			}
		}
		if (no_node_to_go)
		{//������尡 ���ų�, ��� �湮�� �����
			s.pop();
		}
	}
}

template<class K, Number V>
template<class F>
void graph_matrix<K, V>::dfs_use_recursion(const K& start, F func)
{
	set<K> visited;
	dfs_recursion(start, func, visited);
}

template<class K, Number V>
template<class F>
void graph_matrix<K, V>::dfs_recursion(const K& start, F func, set<K>& visited)
{
	//�湮�� ���� ����
	if (visited.find(start) != visited.end()) return;

	func(start);
	visited.insert(start);

	for (auto& v : label)
	{
		if (is_adjacent(start, v.first) > V() && visited.find(v.first) == visited.end())
		{//�����ϰ� visited�� ������ -> �湮 ���� ��� ��� ����
			dfs_recursion(v.first, func, visited);
		}
	}
}

template<class K, Number V>
unordered_map<K, pair<K, V>> graph_matrix<K, V>::dijkstra(K start)
{
	vector<V> dist(label.size(), 10000);
	unordered_map<K, K> result_node(label.size());
	set<K> visited;
	dist[label[start]] = 0;
	K cur_node = start;

	for (int _ = 1; _ < label.size(); _ += 1)
	{
		//��� ���� �Ÿ��迭���� ���� ���� ��
		for (auto& v : label)
		{
			if (visited.find(v.first) == visited.end()) cur_node = v.first;
		}
		for (auto& v : label)
		{
			if (dist[v.second] < dist[label[cur_node]] && visited.find(v.first) == visited.end()) cur_node = v.first;
		}
		//�湮ó��
		visited.insert(cur_node);

		//������ ����� �Ÿ��̿�, ����
		for (auto& v : label)
		{
			//�湮������ �ǳʶ�
			if (visited.find(v.first) != visited.end()) continue;

			//�������ϸ� �ǳʶ�
			V temp = is_adjacent(cur_node, v.first);
			if (temp == 0) continue;

			//������ ����
			temp += dist[label[cur_node]];
			if (temp < dist[label[v.first]])
			{
				dist[label[v.first]] = temp;
				result_node[v.first] = cur_node;
			}
		}
	}

	unordered_map<K, pair<K, V>> result;
	for (auto& v : label)
	{
		result[v.first] = pair{ result_node[v.first], dist[label[v.first]] };
	}
	return result;
}

// ���� ��� -----------------------------------------------------------------------

// ���� ����Ʈ -----------------------------------------------------------------------

template<class K, Number V>
class graph_list
{
public:
	graph_list() {}

	void insert_node(const K& node);
	void delete_node(const K& node);

	void set_link(const K& from, const K& to, const V& weight);
	V is_adjacent(const K& from, const K& to);

	void print();

	template<class F>
	void bfs(const K& start, F func);

	template<class F>
	void dfs_use_stack(const K& start, F func);

	template<class F>
	void dfs_use_recursion(const K& start, F func);

	unordered_map<K, pair<K, V>> dijkstra(K start);

private:
	vector<vector<pair<K, V>>> data;
	unordered_map<K, int> label;

	template<class F>
	void dfs_recursion(const K& start, F func, set<K>& visited);
};

template<class K, Number V>
void graph_list<K, V>::insert_node(const K& node)
{
	if (label.find(node) != label.end()) return;

	label[node] = data.size();
	
	data.push_back(vector<pair<K, V>>{});
}

template<class K, Number V>
void graph_list<K, V>::delete_node(const K& node)
{
	if (label.find(node) == label.end()) return;

	int index = label[node];

	label.erase(node);

	data.erase(data.begin() + index);

	for (auto& v : data)
	{
		vector<vector<pair<K, V>>::iterator> del_iter;
		for (auto i = v.begin(); i != v.end(); ++i)
		{
			if (i->first == node) del_iter.push_back(i);
		}
		for (auto i : del_iter)
		{
			v.erase(i);
		}
	}

	for (auto& i : label)
	{
		if (i.second > index) i.second -= 1;
	}
}

template<class K, Number V>
void graph_list<K, V>::set_link(const K& from, const K& to, const V& weight)
{
	for (auto& v : data[label[from]])
	{
		if (v.first == to)
		{
			v.second = weight;
			return;
		}
	}

	data[label[from]].push_back(pair{ to, weight });
}

template<class K, Number V>
V graph_list<K, V>::is_adjacent(const K& from, const K& to)
{
	for (auto& v : data[label[from]])
	{
		if (v.first == to) return v.second;
	}
}

template<class K, Number V>
void graph_list<K, V>::print()
{
	for (auto& v : label)
	{
		cout << v.first << " : ";
		for (auto& u : data[v.second])
		{
			cout << "[" << u.first << ":" << u.second << "] ";
		}
		cout << endl;
	}
}

template<class K, Number V>
template<class F>
void graph_list<K, V>::bfs(const K& start, F func)
{
	set<K> visited;
	queue<K> q;
	q.push(start);
	
	//visited.size() != label.size() �� �����ʴ� ����
	// �����ִ� ����� ���� ��ü��庸�� ���� �� ���� �����̴�
	// ���� Ž���� ���ϸ� �ڿ����� q�� ��� �Ǿ� !q.empty()�� �ڿ�������
	while (!q.empty())
	{
		K cur_node = q.front();
		func(cur_node);
		visited.insert(cur_node);

		for (auto& v : data[label[cur_node]])
		{
			// visited�� ������ -> �湮 �� ���� ť�� ���� ����
			if (visited.find(v.first) != visited.end()) continue;
			q.push(v.first);
		}
		
		q.pop();
	}
}

template<class K, Number V>
template<class F>
void graph_list<K, V>::dfs_use_stack(const K& start, F func)
{
	set<K> visited;
	stack<K> s;
	s.push(start);
	
	//visited.size() != label.size() �� �����ʴ� ����
	// �����ִ� ����� ���� ��ü��庸�� ���� �� ���� �����̴�
	// ���� Ž���� ���ϸ� �ڿ����� s�� ��� �Ǿ� !s.empty()�� �ڿ������� 
	while (!s.empty())
	{
		K cur_node = s.top();
		if (visited.find(cur_node) == visited.end())
		{//Ž���� ���� ���� ��忡 ���� (����̹Ƿ� �б����� ���� �ߺ� ����� �� �ִ�)
			func(cur_node);
			visited.insert(cur_node);
		}

		bool no_node_to_go = true;
		for (auto& v : data[label[cur_node]])
		{
			if (visited.find(v.first) == visited.end())
			{//visited�� ������ -> �湮 ���� ���� ���� �߰�
				s.push(v.first);
				no_node_to_go = false;
			}
		}
		if (no_node_to_go)
		{//������尡 ���ų�, ��� �湮�� �����
			s.pop();
		}
	}
}

template<class K, Number V>
template<class F>
void graph_list<K, V>::dfs_use_recursion(const K& start, F func)
{
	set<K> visited;
	dfs_recursion(start, func, visited);
}

template<class K, Number V>
template<class F>
void graph_list<K, V>::dfs_recursion(const K& start, F func, set<K>& visited)
{
	//�湮�� ���� ����
	if (visited.find(start) != visited.end()) return;

	func(start);
	visited.insert(start);

	for (auto& v : data[label[start]])
	{
		if (visited.find(v.first) == visited.end())
		{//visited�� ������ -> �湮 ���� ��� ��� ����
			dfs_recursion(v.first, func, visited);
		}
	}
}

template<class K, Number V>
unordered_map<K, pair<K, V>> graph_list<K, V>::dijkstra(K start)
{
	vector<V> dist(label.size(), 10000);
	unordered_map<K, K> result_node(label.size());
	set<K> visited;
	dist[label[start]] = 0;
	K cur_node = start;

	for (int _ = 1; _ < label.size(); _ += 1)
	{
		//��� ���� �Ÿ��迭���� ���� ���� ��
		for (auto& v : label)
		{
			if (visited.find(v.first) == visited.end()) cur_node = v.first;
		}
		for (auto& v : label)
		{
			if (dist[v.second] < dist[label[cur_node]] && visited.find(v.first) == visited.end()) cur_node = v.first;
		}
		//�湮ó��
		visited.insert(cur_node);

		//������ ����� �Ÿ��̿�, ����
		for (auto& v : label)
		{
			//�湮������ �ǳʶ�
			if (visited.find(v.first) != visited.end()) continue;

			//�������ϸ� �ǳʶ�
			V temp = is_adjacent(cur_node, v.first);
			if (temp == 0) continue;

			//������ ����
			temp += dist[label[cur_node]];
			if (temp < dist[label[v.first]])
			{
				dist[label[v.first]] = temp;
				result_node[v.first] = cur_node;
			}
		}
	}

	unordered_map<K, pair<K, V>> result;
	for (auto& v : label)
	{
		result[v.first] = pair{ result_node[v.first], dist[label[v.first]] };
	}
	return result;
}

// ���� ����Ʈ -----------------------------------------------------------------------


// Ž�� �� �� �Լ� ------------------------------------------------------------------

template<class P>
void print_node(const P& node)
{
	cout << node << " ";
}

// Ž�� �� �� �Լ� ------------------------------------------------------------------
