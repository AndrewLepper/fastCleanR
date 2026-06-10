#' Multidimensional anomaly detection
#'
#' The function calculates the Local Outlier Factor score for each row 
#' in the dataset, using parallel C++ processing and a KD-Tree.
#' Values close to 1 indicate "normal" observations. Values significantly greater than 1 
#' (e.g., > 1.5 or > 2.0) suggest distinct density-based anomalies.
#'
#' @param df A data frame or a numeric matrix.
#' @param k The number of nearest neighbors to consider, default is 20.
#' @param n_threads Number of threads to use. Defaults to all available cores.
#' 
#' @return A numeric vector with LOF scores. Higher scores indicate greater anomaly potential.
#' @export
#' @useDynLib mojpakiet, .registration = TRUE
detect_lof <- function(df, k = 20, n_threads = parallel::detectCores()) {
  if (!is.data.frame(df) && !is.matrix(df)) stop("expected data frame or matrix")
  mat <- as.matrix(df)
  if (!is.numeric(mat)) stop("Every column must be numeric")
  if (nrow(mat) <= k) stop("Amount of rows must be greater than k")
  wyniki_lof <- cpp_lof_omp(mat, as.integer(k), as.integer(n_threads))
  
  return(wyniki_lof)
}