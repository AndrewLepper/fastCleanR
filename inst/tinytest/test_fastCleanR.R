library(tinytest)
library(fastCleanR)

message("Running test: NaN Isolation...")
nan_dataset <- data.frame(
  X = c(1.0, 1.1, 1.2, 99.0, 100.0, NA, NA),
  Y = c(1.0, 1.1, NA,  99.0, 100.0, NA, 5.0)
)

imputed <- impute_knn(nan_dataset, k = 2, n_threads = 2)


expect_true(imputed[3, "Y"] < 2.0) 
expect_false(any(is.na(imputed))) 


message("Running test: Identical points stability...")

identical_dataset <- data.frame(
  X = c(5.0, 5.0, 5.0, 5.0, 5.0, 50.0),
  Y = c(5.0, 5.0, 5.0, 5.0, 5.0, 50.0)
)


lof_identical <- detect_lof(identical_dataset, k = 3, n_threads = 2)
expect_true(lof_identical[6] > 5.0)
expect_true(all(is.finite(lof_identical)))


message("Running test: Geometric circle anomaly...")

theta <- seq(0, 2*pi, length.out = 20)
circle_x <- cos(theta)
circle_y <- sin(theta)

geo_dataset <- data.frame(
  X = c(circle_x, 0.0),
  Y = c(circle_y, 0.0)
)
lof_circle <- detect_lof(geo_dataset, k = 5, n_threads = 2)

expect_true(lof_circle[21] > mean(lof_circle[1:20]))