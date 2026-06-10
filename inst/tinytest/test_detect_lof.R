# Plik: inst/tinytest/test_detect_lof.R


set.seed(42)
klaster <- data.frame(
  X = rnorm(50, mean = 0, sd = 0.1),
  Y = rnorm(50, mean = 0, sd = 0.1)
)
anomalia <- data.frame(X = 10, Y = 10)
dane_testowe <- rbind(klaster, anomalia)

wyniki <- detect_lof(dane_testowe, k = 5, n_threads = 1)


expect_true(wyniki[1] < 2.5)
expect_true(wyniki[51] > 10.0)
male_dane <- data.frame(X = c(1,2), Y = c(1,2))
expect_error(detect_lof(male_dane, k = 5), "ammount of rows must be greater than k")

złe_dane <- data.frame(X = c("jeden", "dwa"), Y = c(1, 2))
expect_error(detect_lof(złe_dane, k = 1), "every column must be numeric")