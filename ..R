install.packages("devtools")
install.packages('Rcpp')
"C:/Users/Medion/Desktop/fastCleanR/R"

install.packages("microbenchmark")
library(microbenchmark)
library(dbscan) # Gotowy pakiet z CRAN
library(mojpakiet) # Twój pakiet

# Generujemy 5000 punktów
duze_dane <- matrix(rnorm(10000), ncol=2)

wyniki <- microbenchmark(
  "Moj_LOF_1_Watek" = detect_lof(duze_dane, k = 10, n_threads = 1),
  "Moj_LOF_4_Watki" = detect_lof(duze_dane, k = 10, n_threads = 4),
  "dbscan_CRAN"     = dbscan::lof(duze_dane, minPts = 10),
  times = 10 # Odpal każdy kod 10 razy i wyciągnij średni czas
)

print(wyniki)
install.packages("dbscan")
