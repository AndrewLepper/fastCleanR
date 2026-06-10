#' Fast missing value imputation using K-Nearest Neighbors
#'
#' This function utilizes a multithreaded C++ implementation
#' and a custom KD-Tree data structure to rapidly calculate the mean 
#' of the K-nearest neighbors for every missing value.
#'
#' @param df A data frame or a numeric matrix.
#' @param k An integer. The number of nearest neighbors to consider. Default is 5.
#' @param n_threads Number of threads to be used by OpenMP. Defaults to all available cores.
#' 
#' @return A data frame or matrix of the same size, with NA values imputed.
#' @export
#' @useDynLib mojpakiet, .registration = TRUE
#' @examples
#' dirty_data <- data.frame(
#'   X = c(1.0, 2.0, 1.5, 100.0, 105.0, 102.0),
#'   Y = c(2.0, 3.0, NA,  200.0, 205.0, NA)
#' )
#' clean_data <- impute_knn(dirty_data, k = 2, n_threads = 2)
#' print(clean_data)
impute_knn <- function(df, k = 5, n_threads = parallel::detectCores())
  {
  if (!is.data.frame(df) && !is.matrix(df)) {
    stop("Input must be data frame or numeric matrix")
  }
  mat <- as.matrix(df)
  if (!is.numeric(mat))
  {
    stop("Every column must be numeric")
  }
  res_mat <- cpp_impute_knn_omp(mat, as.integer(k), as.integer(n_threads))
  
  if (is.data.frame(df))
    {
    res_df <- as.data.frame(res_mat)
    colnames(res_df) <- colnames(df)
    rownames(res_df) <- rownames(df)
    return(res_df)
  } else
    {
    colnames(res_mat) <- colnames(df)
    rownames(res_mat) <- rownames(df)
    return(res_mat)
  }
}
