#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue> // B-트리 출력을 위해 큐 사용

using namespace std;

template <typename T>
class BTreeNode {
public: // 구현의 단순화를 위해 캡슐화는 생략
    BTreeNode* parent; // 부모 노드
    vector<BTreeNode*> children; // 자식 노드 집합
    vector<T> dataset; // 데이터 집합

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
    // 자식 노드 분할 함수
    BTreeNode* split_child(int index) {
        // 자식 노드의 데이터의 중앙값을 가져옴
        int mid = children[index]->dataset.size() / 2;
        loose_insert(children[index]->dataset[mid]);

        // 자식 노드 분할
        BTreeNode* left = new BTreeNode(children[index]->parent);
        BTreeNode* right = new BTreeNode(children[index]->parent);

        // 데이터 분할
        for (int i = 0; i < children[index]->dataset.size(); i++) {
            if (i < mid) {
                left->dataset.push_back(children[index]->dataset[i]);
            }
            else if (i > mid) {
                right->dataset.push_back(children[index]->dataset[i]);
            }
        }

        // 후손 노드 분할
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

    // index번째 자식 노드와 (index + 1)번째 자식 노드를 결합
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

    // 데이터 탐색을 위한 재귀 함수
    BTreeNode<T>* search_recursion(BTreeNode<T>* node, T data) {
        if (node->is_leaf()) {
            // <algorithm>에 있는 std::find 함수를 이용
            auto it = find(node->dataset.begin(), node->dataset.end(), data);
            // it == dataset.end()라면 없는것
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


    // 데이터를 새로 삽입하기에 적절한 노드를 탐색
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
    // 데이터의 개수가 최대 개수(M-1)를 초과할 때 트리 교정 진행
    void fix_excess(BTreeNode<T>* node) {
        if (node->dataset.size() < M)
            return;
        if (node == root_ptr) {
            // 루트 노드의 데이터의 개수가 초과된 경우
            // 현재 루트를 자식으로 가지는 새로운 노드를 생성
            BTreeNode<T>* new_root = new BTreeNode<T>(nullptr, vector<BTreeNode<T>*>(1, node));
            node->parent = new_root;
            // 기존의 루트는 분할하고, 새로 만든 노드를 새로운 루트로 설정
            root_ptr = new_root->split_child(0);
        }
        else {
            // 이 노드가 부모의 몇 번째 자식인지 확인
            auto it = find(node->parent->children.begin(), node->parent->children.end(), node);
            int split_index = distance(node->parent->children.begin(), it);
            // 부모의 split_child 함수를 통해 노드를 분할한 후,
            // 그 부모에 대해 재귀 호출
            fix_excess(node->parent->split_child(split_index));
        }
    }

    // 데이터의 개수가 최소 개수(ceil(M/2) 1) 미만일 때 트리 교정 진행
    void fix_shortage(BTreeNode<T>* node) {
        if (node->dataset.size() >= ceil(M / 2.0f) - 1)
            return;

        // 루트 노드가 비어있다면 삭제하고, 자식을 새로운 루트로 설정
        if (node == root_ptr && root_ptr->dataset.size() == 0) {
            BTreeNode<T>* temp = root_ptr;
            root_ptr = root_ptr->children[0];
            root_ptr->parent = nullptr;
            delete temp;
            return;
        }

        BTreeNode<T>* parent = node->parent;
        // 이 노드가 부모의 몇 번째 자식인지 확인
        auto it = find(parent->children.begin(), parent->children.end(), node);
        int index = distance(parent->children.begin(), it);

        // 왼쪽 형제 노드의 데이터의 개수가 ceil(M/2)개 이상인 경우
        if (index > 0 && parent->children[index - 1]->dataset.size() >= ceil(M / 2.0f)) {
            BTreeNode<T>* left_sibling = parent->children[index - 1];
            // 부모 노드의 (index-1)번째 데이터를 가져옴
            node->loose_insert(parent->dataset[index - 1]);
            // 왼쪽 형제 노드에서 가장 큰 데이터를 부모 노드로 옮김
            parent->dataset[index - 1] = left_sibling->dataset.back();
            left_sibling->dataset.pop_back();
            if (!node->is_leaf()) { // 왼쪽 형제 노드의 가장 큰 자식을 가져옴
                node->children.insert(node->children.begin(), left_sibling->children.back());
                node->children[0]->parent = node;
                left_sibling->children.pop_back();
            }
        }

        // 오른쪽 형제 노드의 데이터의 개수가 ceil(M/2)개 이상인 경우
        else if (index < parent->dataset.size() && parent->children[index + 1]->dataset.size() >= ceil(M / 2.0f)) {
            BTreeNode<T>* right_sibling = parent->children[index + 1];
            // 부모 노드의 index번째 데이터를 가져옴
            node->loose_insert(parent->dataset[index]);
            // 오른쪽 형제 노드에서 가장 작은 데이터를 부모 노드로 옮김
            parent->dataset[index] = right_sibling->dataset[0];
            right_sibling->dataset.erase(right_sibling->dataset.begin());
            if (!node->is_leaf()) { // 오른쪽 형제 노드의 가장 작은 자식을 가져옴
                node->children.push_back(right_sibling->children[0]);
                node->children.back()->parent = node;
                right_sibling->children.erase(right_sibling->children.begin());
            }
        }

        // 왼쪽 형제 노드와 결합하는 경우
        else if (index > 0) {

            // 부모 노드의 (index-1)번째 데이터를 가져옴
            node->loose_insert(parent->dataset[index - 1]);
            parent->dataset.erase(parent->dataset.begin() + index - 1);
            parent->merge_children(index - 1);
        }

        // 오른쪽 형제 노드와 결합하는 경우
        else {
            // 부모 노드의 index번째 데이터를 가져옴
            node->loose_insert(parent->dataset[index]);
            parent->dataset.erase(parent->dataset.begin() + index);
            parent->merge_children(index); // 오른쪽 형제와 결합
        }

        fix_shortage(parent);
    }



public:

    BTree(int maximum = 3, BTreeNode<T>* init_root = nullptr) : M(maximum) {
        root_ptr = init_root;
    }
    // 데이터 탐색
    bool count(T data) {
        return search_recursion(root_ptr, data) != nullptr;
    }

    // 데이터 삽입
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
        int current_level = 0; // 현재 레벨의 노드 총 개수
        int next_level = 0; // 다음 레벨의 노드 총 개수
        int count = 0; // 현재 레벨에서 지금까지 출력한 노드 개수

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
            // 현재 레벨의 모든 노드를 출력하면 줄을 바꾸고 다음 레벨 출력
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
            // 삭제하려는 데이터가 몇 번째 인덱스에 있는지 확인
            auto it = find(target_node->dataset.begin(), target_node->dataset.end(), data);
            int index = distance(target_node->dataset.begin(), it);

            // 삭제하려는 데이터와 index번째 서브 트리의 최댓값을 교환
            BTreeNode<T>* sub_max = target_node->children[index];
            while (!sub_max->is_leaf())
                sub_max = sub_max->children.back();
            swap(target_node->dataset[index], sub_max->dataset.back());

            // 리프 노드에서 삭제를 진행
            target_node = sub_max;
            data = sub_max->dataset.back();
        }

        target_node->loose_erase(data);
        fix_shortage(target_node);
    }
};