# Plik: inst/tinytest/test_impute_knn.R


df_logic <- data.frame(
  A = c(0, 0, 0, 10, 10),
  B = c(0, 2, NA, 10, 12)
)
wynik_logic <- impute_knn(df_logic, k = 2, n_threads = 1)
expect_equal(wynik_logic$B[3], 1.0)

set.seed(42)
duza_macierz <- matrix(rnorm(1000 * 5), ncol = 5)
duza_macierz[sample(1:5000, 100)]

wynik_duzy <- impute_knn(duza_macierz, k = 5, n_threads = 2)
expect_equal(sum(is.na(wynik_duzy)), 0)


df_bad <- data.frame(A = c("jeden", "dwa"), B = c(1, 2))
expect_error(impute_knn(df_bad, k = 2), "Every column must be numeric")


