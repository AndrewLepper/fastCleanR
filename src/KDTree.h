#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

struct KDNode 
{
    std::vector<double> point;  
    int original_index;
    KDNode* right;
    KDNode* left;

    KDNode(std::vector<double> pt, int idx) : point(pt), original_index(idx), left(nullptr), right(nullptr) {}
};

struct Neighbor
{
    double distance;
    int index;
    bool operator<(const Neighbor& other) const
    {
        return distance < other.distance; 
    }
};

class KDTree
{
private:
    KDNode* root;
    int dimensions; 

    KDNode* buildRecursive(std::vector<std::pair<std::vector<double>, int>>& points, int depth, int left, int right);   
    void searchRecursive(KDNode* node, const std::vector<double>& target, int depth, int k, std::priority_queue<Neighbor>& pq);
    void freeTree(KDNode* node);
    double squaredDistance(const std::vector<double>& a, const std::vector<double>& b);

public:
    KDTree();   
    ~KDTree();
    void build(const std::vector<std::vector<double>>& points  );
    std::vector<int> findKNearest(const std::vector<double>& target, int k );
};  
#endif