library(ggplot2)
library(dplyr)
theme_set(theme_bw())

tbl <- read.csv("data/results_clever.csv") %>%
    bind_rows(read.csv("data/results_brute_force.csv")) %>%
    left_join(read.csv("data/graph_params.csv"), by = "graph")

ggplot(tbl, aes(x = n, y = time, color = algo)) +
    stat_summary(geom = "line") +
    stat_summary()
