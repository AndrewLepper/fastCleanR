library("mojpakiet")
stopifnot(c(1, 2, 3) %+% c(10, 11, 12) == c(11, 13, 15))
stopifnot(all(is.na(c(NA_real_) %+% c(10, 11, 12))))
stopifnot(length(numeric(0) %+% c(10, 11, 12)) == 0)
