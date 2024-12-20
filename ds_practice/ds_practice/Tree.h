#include "base.h"
#include <queue>

// ����Ʈ�� ��� Ŭ���� -------------------------------------
template <class V>
struct bin_tree_node
{
	bin_tree_node(const V& d = V(),
				  bin_tree_node* l = nullptr,
				  bin_tree_node* r = nullptr);
	V data;
	bin_tree_node* left;
	bin_tree_node* right;
	
	bool is_leaf();
};
template <class V>
bin_tree_node<V>::bin_tree_node(const V& d, bin_tree_node* l, bin_tree_node* r)
{
	data = d;
	left = l;
	right = r;
}
template <class V>
bool bin_tree_node<V>::is_leaf()
{
	return left == nullptr && right == nullptr;
}
// ����Ʈ�� ��� Ŭ���� -------------------------------------

// Ʈ�� �Լ� ���� ------------------------------------------
template <class V>
int depth(bin_tree_node<V>* root)
{
	if (root == nullptr) return -1;
	return 1 + max(depth(root->left), depth(root->right));
}

template <class V>
void tree_clear(bin_tree_node<V>*& root)
{
	if (root == nullptr) return;

	tree_clear(root->left);
	tree_clear(root->right);
	delete root;
	root = nullptr;
}

template <class V>
bin_tree_node<V>* tree_copy(const bin_tree_node<V>* from)
{
	if (from == nullptr) return nullptr;

	return new bin_tree_node<V>{ from->data, tree_copy(from->left), tree_copy(from->right) };
}

template <class F, class V>
void pre_trav(bin_tree_node<V>* root, F func)
{
	if (root == nullptr) return;

	func(root);
	pre_trav(root->left, func);
	pre_trav(root->right, func);
}

template <class F, class V>
void in_trav(bin_tree_node<V>* root, F func)
{
	if (root == nullptr) return;

	in_trav(root->left, func);
	func(root);
	in_trav(root->right, func);
}

template <class F, class V>
void post_trav(bin_tree_node<V>* root, F func)
{
	if (root == nullptr) return;

	post_trav(root->left, func);
	post_trav(root->right, func);
	func(root);
}

template <class F, class V>
void level_trav(bin_tree_node<V>* root, F func)
{
	if (root == nullptr) return;

	queue <bin_tree_node<V>*> q{};
	q.push(root);

	while (!q.empty())
	{
		bin_tree_node<V>* cur_node = q.front();
		func(cur_node);
		q.pop();

		if (cur_node->left != nullptr)  q.push(cur_node->left);
		if (cur_node->right != nullptr) q.push(cur_node->right);
	}
}

template <class V>
void print_element(bin_tree_node<V>* p)
{
	cout << p->data << " ";
}
// Ʈ�� �Լ� ���� ------------------------------------------

// ���� �˻� Ʈ�� Ŭ���� ------------------------------------------
template <class V>
class bst
{
public:
	bst();
	~bst();

	V& find(const V& v) { return find_p(v, root); }
	void push(const V& v) { push_p(v, root); }
	bool erase_one(const V& v) { return erase_one_p(v, root); }

	bool empty() const { return size() == 0; }
	int size() const { return length; }

	void print() { level_trav(root, print_element<V>); cout << endl; }

private:
	bin_tree_node<V>* root;
	int length;

	V& find_p(const V& v, bin_tree_node<V>* r);
	void push_p(const V& v, bin_tree_node<V>*& r);
	bool erase_one_p(const V& v, bin_tree_node<V>*& r);
	void erase_max(V& v, bin_tree_node<V>*& r);
};
template <class V>
bst<V>::bst()
{
	root = nullptr;
	length = 0;
}
template <class V>
bst<V>::~bst()
{
	tree_clear(root);
	length = 0;
}
template <class V>
V& bst<V>::find_p(const V& v, bin_tree_node<V>* r)
{
	if (r == nullptr) return NULL;
	if (v == r->data) return r->data;

	else if (v < r->data) return find_p(v, r->left);
	else if (v > r->data) return find_p(v, r->right);
}
template <class V>
void bst<V>::push_p(const V& v, bin_tree_node<V>*& r)
{
	if (r == nullptr)
	{
		r = new bin_tree_node<V>{ v };
		length += 1;
		return;
	}

	if (v <= r->data)
	{
		push_p(v, r->left);
	}
	else
	{
		push_p(v, r->right);
	}
	
}

template <class V>
bool bst<V>::erase_one_p(const V& v, bin_tree_node<V>*& r)
{
	if (r == nullptr) return false;

	if		(v < r->data) return erase_one_p(v, r->left);
	else if (v > r->data) return erase_one_p(v, r->right);
	else
	{
		// r�� ������ �Ѱ����Ƿ� r ������ ���ܰ� �μ��� ���� -> �θ��� ������ ���� ����
		if (r->is_leaf()) // �� ��� -> �׳� ����
		{
			delete r;
			r = nullptr;
		}
		else if (r->left == nullptr) // �����ʸ� �ִ� ��� -> ������ �ڽ��� ��Ʈ�� �ű�� �ȴ�
		{
			bin_tree_node<V>* temp = r;
			r = r->right;
			delete temp;
		}
		else // ���� �ڽ��� �ִ� ���
		{
			// ���� ����Ʈ������ �ִ밪���� �ٲٰ� �ٲ��� �� ��� ����
			erase_max(r->data, r->left);	
		}

		length -= 1; return true;
	}
}
template <class V>
void bst<V>::erase_max(V& v, bin_tree_node<V>*& r)
{
	if (r->is_leaf())//�� ��� -> �ڽ��� �ִ밪, �� �ܰ迡 �� �ְ� �ڱ� ����
	{
		v = r->data;
		delete r;
		r = nullptr;
	}
	else if (r->right == nullptr) // ���� �ڽĸ� �ִ� ���
	{// �ڽ��� �ִ밪�̹Ƿ� ���� �� ����, �ڽ� ���� ����, ����
		v = r->data;//������
		bin_tree_node<V>* del = r;//������ ���� �ּ� ����
		r = r->left;//���� �ڽ����� ����
		delete del;//���� ��� ����
	}
	else //������ �ڽĵ� �ִ°�� - ���
	{//�ڽİ��踸 �ٲ�� ���, v�� �״�� �δ� ������ ��Ʈ�� ���� ����Ʈ���� �ִ�� �ٲٱ� ����
		erase_max(v, r->right);
	}
}
// ���� �˻� Ʈ�� Ŭ���� ------------------------------------------
