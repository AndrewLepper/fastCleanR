#include "KDTree.h"

KDTree::KDTree() : root(nullptr), dimensions(0) {}

KDTree::~KDTree() {
    freeTree(root);
}

void KDTree::freeTree(KDNode* node)
{
    if (node)
    {
        freeTree(node->left);   
        freeTree(node->right);  
        delete node;
    }
}

double KDTree::squaredDistance(const std::vector<double>& a, const std::vector<double>& b)
{
    double sum = 0;
    for (size_t i = 0; i<dimensions; ++i)
    {
        if (!std::isnan(a[i]) && !std::isnan(b[i]))
        {
            sum += (a[i]-b[i])*(a[i]-b[i]);
        }   
    }
    return sum;
}

void KDTree::build(const std::vector<std::vector<double>>& points)
{
    if (points.empty()) return; 
    dimensions = points[0].size();
    std::vector<std::pair<std::vector<double>, int>> indexed_points;
    for (int i = 0; i < points.size(); ++i) 
    {
        indexed_points.push_back({points[i], i});
    }
    root = buildRecursive(indexed_points, 0, 0, indexed_points.size()-1);   
}

KDNode* KDTree::buildRecursive(std::vector<std::pair<std::vector<double>, int>>& points, int depth, int left, int right)
{
    if (left > right) return nullptr;

    int axis = depth % dimensions;  
    int mid = left + (right - left)/2;    

    std::nth_element(points.begin() + left, points.begin() + mid, points.begin() + right + 1,
                     [axis](const std::pair<std::vector<double>, int>& a, const std::pair<std::vector<double>, int>& b)
                     {
                         bool na_a = std::isnan(a.first[axis]);
                         bool na_b = std::isnan(b.first[axis]);
                         if (na_a && !na_b) return false;
                         if (!na_a && na_b) return true;
                         return a.first[axis] < b.first[axis];
                     });

    KDNode* node = new KDNode(points[mid].first, points[mid].second);
    node->left = buildRecursive(points, depth + 1, left, mid - 1);
    node->right = buildRecursive(points, depth + 1, mid + 1, right);

    return node;
}

void KDTree::searchRecursive(KDNode* node, const std::vector<double>& target, int depth, int k, std::priority_queue<Neighbor>& pq)
{
    if (!node) return;

    double dist = squaredDistance(node->point, target);
    int axis = depth % dimensions;

    if (pq.size() < k || dist < pq.top().distance)
    {
        pq.push({dist, node->original_index});
        if (pq.size() > k) {
            pq.pop();
        }
    }
    if (std::isnan(target[axis]) || std::isnan(node->point[axis]))
    {
        searchRecursive(node->left, target, depth + 1, k, pq);
        searchRecursive(node->right, target, depth + 1, k, pq);
        return;
    }
    double diff = target[axis] - node->point[axis];
    KDNode* first_path = (diff < 0) ? node->left : node->right;
    KDNode* second_path = (diff < 0) ? node->right : node->left;
    searchRecursive(first_path, target, depth + 1, k, pq);
    if (pq.size() < k || (diff * diff) < pq.top().distance)
    {
        searchRecursive(second_path, target, depth + 1, k, pq);
    }
}

std::vector<int> KDTree::findKNearest(const std::vector<double>& target, int k)
{
    std::priority_queue<Neighbor> pq;
    searchRecursive(root, target, 0, k, pq);
    std::vector<int> result;
    while (!pq.empty())
    {
        result.push_back(pq.top().index);
        pq.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}