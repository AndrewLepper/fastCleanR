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