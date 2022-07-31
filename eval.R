library(ggplot2)
library(dplyr)
library(forcats)

theme_set(theme_bw())
colors <- c("#df9b1b", "#009682", "#4664aa", "#a22223", "#a3107c", "#23a1e0", "#8cb63c")

algos <- c(
    "brute_force_matrix",
    "brute_force_matrix_skip",
    "brute_force_hash_skip",
    "brute_force_clever_skip",
    "clever",
    "clever_skip",
    "clever_sort_skip"
)

tbl <- read.csv("data/results_clever.csv") %>%
    bind_rows(read.csv("data/results_clever_skip.csv")) %>%
    bind_rows(read.csv("data/results_clever_sort_skip.csv")) %>%
    bind_rows(read.csv("data/results_brute_force_matrix.csv")) %>%
    bind_rows(read.csv("data/results_brute_force_matrix_skip.csv")) %>%
    bind_rows(read.csv("data/results_brute_force_hash_skip.csv")) %>%
    bind_rows(read.csv("data/results_brute_force_clever_skip.csv")) %>%
    left_join(read.csv("data/graph_params.csv"), by = "graph") %>%
    mutate(algo = factor(algo, levels = algos))


unit_x <- function(unit) {
    scale_x_continuous(
        labels = scales::label_number(scale_cut = scales::cut_si(unit))
    )
}
unit_y <- function(unit) {
    scale_y_continuous(
        labels = scales::label_number(scale_cut = scales::cut_si(unit))
    )
}

memory_plot <- function(tbl) {
    p <- ggplot(tbl, aes(x = n, y = memory * 1e3, color = algo, shape = algo)) +
        stat_summary(geom = "line", fun.data = mean_se) +
        stat_summary(fun.data = mean_se) +
        scale_colour_manual(values = colors) +
        unit_x("") +
        unit_y("B") +
        ylab("memory")
    print(p)
}

time_plot <- function(tbl, max_y = NA) {
    p <- ggplot(tbl, aes(x = n, y = time / 1000, color = algo, shape = algo)) +
        stat_summary(geom = "line", fun.data = mean_se) +
        stat_summary(fun.data = mean_se) +
        scale_colour_manual(values = colors) +
        unit_x("") +
        unit_y("s") +
        coord_cartesian(ylim = c(NA, max_y)) +
        ylab("time")
    print(p)
}

pdf("plots.pdf", width = 8, height = 5, onefile = TRUE)
time_plot(tbl %>%
    filter(algo %in% (algos %>% head(2))))
memory_plot(tbl %>%
    filter(algo %in% (algos %>% head(2))))
memory_plot(tbl %>%
    filter(algo %in% (algos %>% head(3))))
time_plot(tbl %>%
    filter(algo %in% (algos %>% head(3))))
time_plot(tbl %>%
    filter(algo %in% (algos %>% head(4))))
time_plot(tbl %>%
    filter(algo %in% (algos %>% head(4))), max_y = 70)
memory_plot(tbl %>%
    filter(algo %in% (algos %>% head(4))))
time_plot(tbl %>%
    filter(algo %in% (algos %>% head(4))), max_y = 70)
time_plot(tbl %>%
    filter(algo %in% (algos %>% head(5))), max_y = 70)
time_plot(tbl %>%
    filter(algo %in% (algos %>% head(6))), max_y = 70)
time_plot(tbl %>%
    filter(algo %in% (algos %>% head(6))), max_y = 13)
time_plot(tbl, max_y = 13)
dev.off()
