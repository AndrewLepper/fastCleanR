#include <Rcpp.h>
#ifdef _OPENMP
  #include <omp.h>
#endif
#include <cmath>
#include <algorithm>
#include "KDTree.h"

using namespace Rcpp;

inline double distanceEuclidean(const std::vector<double>& a, const std::vector<double>& b) {
    double sum = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (!std::isnan(a[i]) && !std::isnan(b[i])) {
            sum += (a[i] - b[i]) * (a[i] - b[i]);
        }
    }
    return std::sqrt(sum);
}

// [[Rcpp::plugins(openmp)]]
// [[Rcpp::export]]
NumericVector cpp_lof_omp(NumericMatrix mat, int k, int n_threads) {
    int nrow = mat.nrow();
    int ncol = mat.ncol();
    #ifdef _OPENMP
        omp_set_num_threads(n_threads);
    #endif
    std::vector<std::vector<double>> points(nrow, std::vector<double>(ncol));
    for (int i = 0; i < nrow; i++) {
        for (int j = 0; j < ncol; j++) {
            points[i][j] = mat(i, j);
        }
    }
    KDTree tree;
    tree.build(points);
    std::vector<std::vector<int>> all_neighbors(nrow);
    std::vector<double> k_distance(nrow, 0.0);
    std::vector<double> lrd(nrow, 0.0);
    NumericVector lof_scores(nrow);

    #pragma omp parallel for
    for (int i = 0; i < nrow; i++)
    {
        std::vector<int> neighbors = tree.findKNearest(points[i], k + 1);
        std::vector<int> true_neighbors;
        for (int n_idx : neighbors)
        {
            if (n_idx != i && true_neighbors.size() < (size_t)k) {
                true_neighbors.push_back(n_idx);
            }
        }
        all_neighbors[i] = true_neighbors;

        if (!true_neighbors.empty())
        {
            int last_neighbor = true_neighbors.back();
            k_distance[i] = distanceEuclidean(points[i], points[last_neighbor]);
        }
    }
    #pragma omp parallel for
    for (int i = 0; i < nrow; i++)
    {
        double sum_reach_dist = 0.0;
        for (int n_idx : all_neighbors[i])
        {
            double actual_dist = distanceEuclidean(points[i], points[n_idx]);
            double reach_dist = std::max(k_distance[n_idx], actual_dist);
            sum_reach_dist += reach_dist;
        }

        if (sum_reach_dist > 0)
        {
            lrd[i] = (double)all_neighbors[i].size() / sum_reach_dist;
        } else {
            lrd[i] = 1.0; 
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < nrow; i++)
    {
        double sum_lrd_ratio = 0.0;
        for (int n_idx : all_neighbors[i])
        {
            sum_lrd_ratio += lrd[n_idx] / lrd[i];
        }

        if (!all_neighbors[i].empty())
        {
            lof_scores[i] = sum_lrd_ratio / all_neighbors[i].size();
        }
        else
        {
            lof_scores[i] = 1.0;
        }
    }
    return lof_scores;
}