# fastCleanR: High-Performance Data Cleaning and Anomaly Detection

[![R build status](https://github.com/AndrewLepper/fastCleanR/workflows/R-CMD-check/badge.svg)](https://github.com/AndrewLepper/fastCleanR)

`mojpakiet` is a high-performance R package designed for rapid data imputation and multidimensional anomaly detection. It leverages custom **C++ data structures (KD-Tree)** and **OpenMP multithreading** via the `Rcpp` framework to overcome the performance bottlenecks of native R loops.

## Features
* **K-Nearest Neighbors Imputation (`impute_knn`):** Fills `NA` values using a highly optimized KD-Tree search. Handles NaN poisoning efficiently.
* **Local Outlier Factor (`detect_lof`):** A density-based anomaly detection algorithm capable of finding local outliers in complex multidimensional spaces.
* **Multithreading:** Both core functions natively support OpenMP, automatically scaling across available CPU cores.

## Installation

You can install the development version of `fastCleanR` from GitHub using `devtools`:

```r
# install.packages("devtools")
devtools::install_github("AndrewLepper/fastCleanR")

## Example Usage

library(mojpakiet)

dirty_data <- data.frame(
  X = c(1.0, 2.0, 1.5, 100.0, 105.0, 102.0),
  Y = c(2.0, 3.0, NA,  200.0, 205.0, NA)
)

# Impute using 2 nearest neighbors on 4 CPU threads
clean_data <- impute_knn(dirty_data, k = 2, n_threads = 4)
print(clean_data)


# Generate a dense cluster and one massive anomaly
cluster <- data.frame(X = rnorm(100, 0, 0.5), Y = rnorm(100, 0, 0.5))
anomaly <- data.frame(X = 10, Y = 10)
dataset <- rbind(cluster, anomaly)

# Calculate LOF scores
lof_scores <- detect_lof(dataset, k = 10, n_threads = 4)

# The normal points will score ~1.0, while the anomaly will score > 20.0
print(lof_scores[101])