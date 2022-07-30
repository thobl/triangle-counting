#!/usr/bin/env python3
import os
import glob
import run
import multiprocessing
import re

run.use_cores(multiprocessing.cpu_count() - 4)

######################################################################
######################################################################
######################################################################
run.section("[1/2] Generate random graphs")

######################################################################
# generate random graphs
run.group("gen")

run.add(
    "girg",
    "code/release/girg -n [[n]] --deg [[deg]] --dim [[dim]] --ple [[ple]] -T [[T]] --seed [[seed]] --no-header [[file]]",
    {
        "n": [
            100,
            200,
            400,
            800,
            1600,
            3200,
            6400,
            12800,
            25600,
            51200,
            102400,
            204800,
            409600,
            # 819200,
        ],
        "deg": 10,
        "dim": 2,
        "ple": 2.2,
        "T": 0,
        "seed": [64, 321, 356, 489, 836, 1234, 3212, 4531, 4565, 6721],
        "name": "girg_n=[[n]]_deg=[[deg]]_dim=[[dim]]_ple=[[ple]]_T=[[T]]_seed=[[seed]]",
        "file": "data/graphs/[[name]]",
    },
    stdout_res="[[name]],[[stdout]]",
    stdout_file="data/graph_params.csv",
    header_command="code/release/girg --only-header",
    header_mod=lambda head: f"graph,{head}",
)

run.run()

######################################################################
######################################################################
######################################################################
run.section("[2/2] Count the triangles")

graphs = [os.path.basename(f) for f in glob.glob("data/graphs/*")]
graphs_small = [graph for graph in graphs if int(re.split("=|_", graph)[2]) < 4000]

######################################################################
# generate random graphs
run.group("count")

run.add(
    "clever",
    "code/release/count_triangles --algo [[algo]] --no-header data/graphs/[[graph]]",
    {
        "algo": ["clever", "clever_skip", "clever_sort_skip"],
        "graph": graphs,
    },
    stdout_file="data/results_clever.csv",
    header_command="code/release/count_triangles --only-header",
)

run.add(
    "brute_force",
    "code/release/count_triangles --algo [[algo]] --no-header data/graphs/[[graph]]",
    {
        "algo": ["brute_force"],
        "graph": graphs_small,
    },
    stdout_file="data/results_brute_force.csv",
    header_command="code/release/count_triangles --only-header",
)


run.run()
