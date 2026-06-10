#include <Rcpp.h>
#include <omp.h>
#include "KDTree.h"
using namespace Rcpp;

// [[Rcpp::plugins(openmp)]]
// [[Rcpp::export]]
NumericMatrix cpp_impute_knn_omp(NumericMatrix mat, int k, int n_threads) {
    int nrow = mat.nrow();
    int ncol = mat.ncol();  
    omp_set_num_threads(n_threads); 
    NumericMatrix res = clone(mat);

    std::vector<std::vector<double>> points(nrow, std::vector<double>(ncol));
    for (int i=0; i<nrow; i++)
    {
        for (int j=0; j<ncol; j++)
        {
            points[i][j] = mat(i, j);
        }
    }
    KDTree tree;    
    tree.build(points);
    #pragma omp parallel for
    for (int i = 0; i<nrow; i++)
    {
        
        bool has_na = false;    
        for (int j = 0; j<ncol; j++)
        {
            if (NumericVector::is_na(points[i][j]))
            {
                has_na = true;
                break;
            }
        }

        if (has_na) {
            std::vector<int> neighbors = tree.findKNearest(points[i], k + 1);
            for (int j = 0; j < ncol; j++) 
            {
                if (NumericVector::is_na(points[i][j]))
                {
                    double sum = 0.0; 
                    int count = 0; 

                    for (int n_idx : neighbors)
                    {
                        if (n_idx != i && !NumericVector::is_na(points[n_idx][j]))
                        { 
                            sum += points[n_idx][j]; 
                            count++;    
                        }
                    }
                    if (count > 0)
                    {    
                        res(i, j) = sum / count;    
                    }
                }
            }
        }
    }
    return res;
}