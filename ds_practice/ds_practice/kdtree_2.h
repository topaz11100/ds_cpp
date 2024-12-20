#pragma once

#include "base.h"

template <typename T, size_t Dim>
class KDTree {
public:
    struct Point {
        T coords[Dim];
        Point(initializer_list<T> values) {
            copy(values.begin(), values.end(), coords);
        }
        T operator[](size_t index) const { return coords[index]; }
        T& operator[](size_t index) { return coords[index]; }
    };

    struct Node {
        Point point;
        Node* left;
        Node* right;
        Node(const Point& pt) : point(pt), left(nullptr), right(nullptr) {}
    };

    KDTree() : root(nullptr) {}

    void insert(const Point& point) {
        root = insertRec(root, point, 0);
    }

    void rangeSearch(const Point& lower, const Point& upper, vector<Point>& result) const {
        rangeSearchRec(root, lower, upper, 0, result);
    }

    Point nearestNeighbor(const Point& target) const {
        Point bestPoint = root->point;
        T bestDist = numeric_limits<T>::max();
        nearestNeighborRec(root, target, 0, bestPoint, bestDist);
        return bestPoint;
    }

private:
    Node* root;

    Node* insertRec(Node* node, const Point& point, size_t depth) {
        if (!node) return new Node(point);

        size_t axis = depth % Dim;
        if (point[axis] < node->point[axis])
            node->left = insertRec(node->left, point, depth + 1);
        else
            node->right = insertRec(node->right, point, depth + 1);

        return node;
    }

    void rangeSearchRec(Node* node, const Point& lower, const Point& upper, size_t depth, vector<Point>& result) const {
        if (!node) return;

        bool inside = true;
        for (size_t i = 0; i < Dim; ++i) {
            if (node->point[i] < lower[i] || node->point[i] > upper[i]) {
                inside = false;
                break;
            }
        }
        if (inside) result.push_back(node->point);

        size_t axis = depth % Dim;
        if (node->left && lower[axis] <= node->point[axis])
            rangeSearchRec(node->left, lower, upper, depth + 1, result);
        if (node->right && upper[axis] >= node->point[axis])
            rangeSearchRec(node->right, lower, upper, depth + 1, result);
    }

    void nearestNeighborRec(Node* node, const Point& target, size_t depth, Point& bestPoint, T& bestDist) const {
        if (!node) return;

        T dist = squaredDistance(node->point, target);
        if (dist < bestDist) {
            bestDist = dist;
            bestPoint = node->point;
        }

        size_t axis = depth % Dim;
        Node* first = target[axis] < node->point[axis] ? node->left : node->right;
        Node* second = target[axis] < node->point[axis] ? node->right : node->left;

        nearestNeighborRec(first, target, depth + 1, bestPoint, bestDist);
        if (abs(node->point[axis] - target[axis]) < sqrt(bestDist))
            nearestNeighborRec(second, target, depth + 1, bestPoint, bestDist);
    }

    T squaredDistance(const Point& a, const Point& b) const {
        T dist = 0;
        for (size_t i = 0; i < Dim; ++i)
            dist += (a[i] - b[i]) * (a[i] - b[i]);
        return dist;
    }
};

/*
int main() {
    KDTree<double, 2> tree;

    tree.insert({ 2.0, 3.0 });
    tree.insert({ 5.0, 4.0 });
    tree.insert({ 9.0, 6.0 });
    tree.insert({ 4.0, 7.0 });
    tree.insert({ 8.0, 1.0 });
    tree.insert({ 7.0, 2.0 });

    // Range search
    vector<KDTree<double, 2>::Point> result;
    tree.rangeSearch({ 3.0, 2.0 }, { 8.0, 5.0 }, result);
    cout << "Range Search Results:\n";
    for (const auto& pt : result)
        cout << "(" << pt[0] << ", " << pt[1] << ")\n";

    // Nearest neighbor
    auto nn = tree.nearestNeighbor({ 6.0, 3.0 });
    cout << "Nearest Neighbor: (" << nn[0] << ", " << nn[1] << ")\n";

    return 0;
}
*/

// KD-Tree 노드 정의
template <typename T, size_t K>
struct KDNode {
    array<T, K> point;  // k차원 포인트
    unique_ptr<KDNode> left;  // 왼쪽 서브트리
    unique_ptr<KDNode> right; // 오른쪽 서브트리

    KDNode(const array<T, K>& point) : point(point), left(nullptr), right(nullptr) {}
};

// KD-Tree 클래스 정의
template <typename T, size_t K>
class KDTree {
private:
    using Node = KDNode<T, K>;
    unique_ptr<Node> root;

    // 재귀적으로 KD-Tree 생성
    unique_ptr<Node> build(vector<array<T, K>>& points, size_t depth) {
        if (points.empty()) return nullptr;

        size_t axis = depth % K; // 현재 분할 축
        size_t median = points.size() / 2;

        // 축(axis)을 기준으로 정렬 후 중간값 선택
        std::nth_element(points.begin(), points.begin() + median, points.end(),
            [axis](const array<T, K>& a, const array<T, K>& b) {
                return a[axis] < b[axis];
            });

        // 현재 노드 생성
        auto node = make_unique<Node>(points[median]);

        // 왼쪽과 오른쪽 서브트리 재귀적으로 생성
        vector<array<T, K>> left_points(points.begin(), points.begin() + median);
        vector<array<T, K>> right_points(points.begin() + median + 1, points.end());

        node->left = build(left_points, depth + 1);
        node->right = build(right_points, depth + 1);

        return node;
    }

    // 최근접 이웃 검색
    void nearestNeighbor(const Node* current, const array<T, K>& target, size_t depth,
        const Node*& best, T& best_dist) const {
        if (!current) return;

        size_t axis = depth % K; // 현재 분할 축
        T d = distance(current->point, target);

        // 현재 노드가 더 가까우면 업데이트
        if (d < best_dist) {
            best = current;
            best_dist = d;
        }

        // 분할 축을 기준으로 검색 방향 결정
        const Node* next = target[axis] < current->point[axis] ? current->left.get() : current->right.get();
        const Node* other = target[axis] < current->point[axis] ? current->right.get() : current->left.get();

        nearestNeighbor(next, target, depth + 1, best, best_dist);

        // 다른 서브트리도 탐색할 필요가 있는지 확인
        if (abs(target[axis] - current->point[axis]) < best_dist) {
            nearestNeighbor(other, target, depth + 1, best, best_dist);
        }
    }

    // 두 점 사이의 거리 계산
    T distance(const array<T, K>& a, const array<T, K>& b) const {
        T dist = 0;
        for (size_t i = 0; i < K; ++i) {
            dist += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return sqrt(dist);
    }

public:
    // KD-Tree 생성자
    KDTree(vector<array<T, K>> points) {
        root = build(points, 0);
    }

    // 최근접 이웃 검색 함수
    array<T, K> nearestNeighbor(const array<T, K>& target) const {
        const Node* best = nullptr;
        T best_dist = numeric_limits<T>::max();
        nearestNeighbor(root.get(), target, 0, best, best_dist);
        return best ? best->point : array<T, K>();
    }
};

/*

int main() {
    // 3차원 KD-Tree 예제
    vector<array<double, 3>> points = {
        {2.0, 3.0, 4.0}, {5.0, 4.0, 2.0}, {9.0, 6.0, 7.0},
        {4.0, 7.0, 9.0}, {8.0, 1.0, 5.0}, {7.0, 2.0, 6.0}
    };

    KDTree<double, 3> tree(points);

    array<double, 3> target = { 6.0, 3.0, 5.0 };
    auto nearest = tree.nearestNeighbor(target);

    cout << "Nearest point: (";
    for (size_t i = 0; i < nearest.size(); ++i) {
        cout << nearest[i] << (i + 1 < nearest.size() ? ", " : ")\n");
    }

    return 0;
}

*/
