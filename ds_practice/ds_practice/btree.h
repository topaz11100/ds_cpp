#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue> // B-Ʈ�� ����� ���� ť ���

using namespace std;

template <typename T>
class BTreeNode {
public: // ������ �ܼ�ȭ�� ���� ĸ��ȭ�� ����
    BTreeNode* parent; // �θ� ���
    vector<BTreeNode*> children; // �ڽ� ��� ����
    vector<T> dataset; // ������ ����

    BTreeNode(BTreeNode* init_parent = nullptr,
        vector<BTreeNode*> init_children = vector<BTreeNode*>(),
        vector<T> init_dataset = vector<T>()) {
        parent = init_parent;
        children = init_children;
        dataset = init_dataset;
    };

    bool is_leaf() {
        return children.size() == 0;
    }

    void print_data() {
        cout << "[ ";
        for (int i = 0; i < dataset.size(); i++) cout << dataset[i] << " ";
        cout << "]";
    }

    void loose_insert(T data) {
        dataset.push_back(data);
        sort(dataset.begin(), dataset.end());
    }
    // �ڽ� ��� ���� �Լ�
    BTreeNode* split_child(int index) {
        // �ڽ� ����� �������� �߾Ӱ��� ������
        int mid = children[index]->dataset.size() / 2;
        loose_insert(children[index]->dataset[mid]);

        // �ڽ� ��� ����
        BTreeNode* left = new BTreeNode(children[index]->parent);
        BTreeNode* right = new BTreeNode(children[index]->parent);

        // ������ ����
        for (int i = 0; i < children[index]->dataset.size(); i++) {
            if (i < mid) {
                left->dataset.push_back(children[index]->dataset[i]);
            }
            else if (i > mid) {
                right->dataset.push_back(children[index]->dataset[i]);
            }
        }

        // �ļ� ��� ����
        if (!children[index]->is_leaf()) {
            for (int i = 0; i < children[index]->children.size(); i++) {
                if (i <= mid) {
                    left->children.push_back(children[index]->children[i]);
                    children[index]->children[i]->parent = left;
                }
                else if (i > mid) {
                    right->children.push_back(children[index]->children[i]);
                    children[index]->children[i]->parent = right;
                }
            }
        }

        BTreeNode* temp = children[index];
        children[index] = left;
        children.insert(children.begin() + index + 1, right);
        delete temp;

        return right->parent;
    }

    void loose_erase(T data) {
        auto it = find(dataset.begin(), dataset.end(), data);
        dataset.erase(it);
    }

    // index��° �ڽ� ���� (index + 1)��° �ڽ� ��带 ����
    void merge_children(int index) {
        for (int i = 0; i < children[index + 1]->dataset.size(); i++) {
            children[index]->dataset.push_back(children[index + 1]->dataset[i]);
        }
        if (!children[index]->is_leaf()) {
            for (int i = 0; i < children[index + 1]->children.size(); i++) {
                children[index]->children.push_back(children[index + 1]->children[i]);
            }
        }
        children.erase(children.begin() + index + 1);
    }
};



template <typename T>
class BTree {
private:
    const int M;
    BTreeNode<T>* root_ptr;

    // ������ Ž���� ���� ��� �Լ�
    BTreeNode<T>* search_recursion(BTreeNode<T>* node, T data) {
        if (node->is_leaf()) {
            // <algorithm>�� �ִ� std::find �Լ��� �̿�
            auto it = find(node->dataset.begin(), node->dataset.end(), data);
            // it == dataset.end()��� ���°�
            if (it != node->dataset.end()) return node;
            else return nullptr;
        }
        if (data < node->dataset[0])
            return search_recursion(node->children[0], data);
        else if (data == node->dataset[0])
            return node;

        for (int i = 1; i < node->dataset.size(); i++) {
            if (node->dataset[i - 1] < data && data < node->dataset[i])
                return search_recursion(node->children[i], data);
            else if (data == node->dataset[i])
                return node;
        }
        if (data > node->dataset.back())
            return search_recursion(node->children.back(), data);
    }


    // �����͸� ���� �����ϱ⿡ ������ ��带 Ž��
    BTreeNode<T>* find_insert_point(BTreeNode<T>* node, T data) {
        if (node->is_leaf())
            return node;
        if (data <= node->dataset[0])
            return find_insert_point(node->children[0], data);
        for (int i = 1; i < node->dataset.size(); i++)
            if (node->dataset[i - 1] < data && data <= node->dataset[i])
                return find_insert_point(node->children[i], data);
        if (data > node->dataset.back())
            return find_insert_point(node->children.back(), data);
    }
    // �������� ������ �ִ� ����(M-1)�� �ʰ��� �� Ʈ�� ���� ����
    void fix_excess(BTreeNode<T>* node) {
        if (node->dataset.size() < M)
            return;
        if (node == root_ptr) {
            // ��Ʈ ����� �������� ������ �ʰ��� ���
            // ���� ��Ʈ�� �ڽ����� ������ ���ο� ��带 ����
            BTreeNode<T>* new_root = new BTreeNode<T>(nullptr, vector<BTreeNode<T>*>(1, node));
            node->parent = new_root;
            // ������ ��Ʈ�� �����ϰ�, ���� ���� ��带 ���ο� ��Ʈ�� ����
            root_ptr = new_root->split_child(0);
        }
        else {
            // �� ��尡 �θ��� �� ��° �ڽ����� Ȯ��
            auto it = find(node->parent->children.begin(), node->parent->children.end(), node);
            int split_index = distance(node->parent->children.begin(), it);
            // �θ��� split_child �Լ��� ���� ��带 ������ ��,
            // �� �θ� ���� ��� ȣ��
            fix_excess(node->parent->split_child(split_index));
        }
    }

    // �������� ������ �ּ� ����(ceil(M/2) 1) �̸��� �� Ʈ�� ���� ����
    void fix_shortage(BTreeNode<T>* node) {
        if (node->dataset.size() >= ceil(M / 2.0f) - 1)
            return;

        // ��Ʈ ��尡 ����ִٸ� �����ϰ�, �ڽ��� ���ο� ��Ʈ�� ����
        if (node == root_ptr && root_ptr->dataset.size() == 0) {
            BTreeNode<T>* temp = root_ptr;
            root_ptr = root_ptr->children[0];
            root_ptr->parent = nullptr;
            delete temp;
            return;
        }

        BTreeNode<T>* parent = node->parent;
        // �� ��尡 �θ��� �� ��° �ڽ����� Ȯ��
        auto it = find(parent->children.begin(), parent->children.end(), node);
        int index = distance(parent->children.begin(), it);

        // ���� ���� ����� �������� ������ ceil(M/2)�� �̻��� ���
        if (index > 0 && parent->children[index - 1]->dataset.size() >= ceil(M / 2.0f)) {
            BTreeNode<T>* left_sibling = parent->children[index - 1];
            // �θ� ����� (index-1)��° �����͸� ������
            node->loose_insert(parent->dataset[index - 1]);
            // ���� ���� ��忡�� ���� ū �����͸� �θ� ���� �ű�
            parent->dataset[index - 1] = left_sibling->dataset.back();
            left_sibling->dataset.pop_back();
            if (!node->is_leaf()) { // ���� ���� ����� ���� ū �ڽ��� ������
                node->children.insert(node->children.begin(), left_sibling->children.back());
                node->children[0]->parent = node;
                left_sibling->children.pop_back();
            }
        }

        // ������ ���� ����� �������� ������ ceil(M/2)�� �̻��� ���
        else if (index < parent->dataset.size() && parent->children[index + 1]->dataset.size() >= ceil(M / 2.0f)) {
            BTreeNode<T>* right_sibling = parent->children[index + 1];
            // �θ� ����� index��° �����͸� ������
            node->loose_insert(parent->dataset[index]);
            // ������ ���� ��忡�� ���� ���� �����͸� �θ� ���� �ű�
            parent->dataset[index] = right_sibling->dataset[0];
            right_sibling->dataset.erase(right_sibling->dataset.begin());
            if (!node->is_leaf()) { // ������ ���� ����� ���� ���� �ڽ��� ������
                node->children.push_back(right_sibling->children[0]);
                node->children.back()->parent = node;
                right_sibling->children.erase(right_sibling->children.begin());
            }
        }

        // ���� ���� ���� �����ϴ� ���
        else if (index > 0) {

            // �θ� ����� (index-1)��° �����͸� ������
            node->loose_insert(parent->dataset[index - 1]);
            parent->dataset.erase(parent->dataset.begin() + index - 1);
            parent->merge_children(index - 1);
        }

        // ������ ���� ���� �����ϴ� ���
        else {
            // �θ� ����� index��° �����͸� ������
            node->loose_insert(parent->dataset[index]);
            parent->dataset.erase(parent->dataset.begin() + index);
            parent->merge_children(index); // ������ ������ ����
        }

        fix_shortage(parent);
    }



public:

    BTree(int maximum = 3, BTreeNode<T>* init_root = nullptr) : M(maximum) {
        root_ptr = init_root;
    }
    // ������ Ž��
    bool count(T data) {
        return search_recursion(root_ptr, data) != nullptr;
    }

    // ������ ����
    void insert(T data) {
        if (root_ptr == nullptr) {
            root_ptr = new BTreeNode<T>();
            root_ptr->loose_insert(data);
        }
        else {
            BTreeNode<T>* insert_point
                =
                find_insert_point(root_ptr, data);
            insert_point->loose_insert(data);
            fix_excess(insert_point);
        }
    }

    void print_tree() {
        queue<BTreeNode<T>*> node_queue = queue<BTreeNode<T>*>();
        int current_level = 0; // ���� ������ ��� �� ����
        int next_level = 0; // ���� ������ ��� �� ����
        int count = 0; // ���� �������� ���ݱ��� ����� ��� ����

        node_queue.push(root_ptr);
        root_ptr->print_data();
        cout << endl;

        while (!node_queue.empty()) {
            BTreeNode<T>* node = node_queue.front();
            node_queue.pop();
            if (!node->is_leaf()) {
                cout << "[";
                for (int i = 0; i < node->children.size(); i++) {
                    node_queue.push(node->children[i]);
                    node->children[i]->print_data();
                    next_level++;
                }
                cout << "]";
            }

            count++;
            // ���� ������ ��� ��带 ����ϸ� ���� �ٲٰ� ���� ���� ���
            if (count >= current_level) {
                cout << endl;
                current_level = next_level;
                next_level = 0;
                count = 0;
            }
        }
    }


    void remove(T data) {
        BTreeNode<T>* target_node = search_recursion(root_ptr, data);
        if (target_node == nullptr) return;

        if (!target_node->is_leaf()) {
            // �����Ϸ��� �����Ͱ� �� ��° �ε����� �ִ��� Ȯ��
            auto it = find(target_node->dataset.begin(), target_node->dataset.end(), data);
            int index = distance(target_node->dataset.begin(), it);

            // �����Ϸ��� �����Ϳ� index��° ���� Ʈ���� �ִ��� ��ȯ
            BTreeNode<T>* sub_max = target_node->children[index];
            while (!sub_max->is_leaf())
                sub_max = sub_max->children.back();
            swap(target_node->dataset[index], sub_max->dataset.back());

            // ���� ��忡�� ������ ����
            target_node = sub_max;
            data = sub_max->dataset.back();
        }

        target_node->loose_erase(data);
        fix_shortage(target_node);
    }
};