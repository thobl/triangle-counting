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


def size_filter(args):
    """Filter too large graphs for some algos."""
    n = int(re.split("=|_", args["graph"])[2])
    if args["algo"] in ["brute_force_matrix"] and n > 12800:
        return False
    if args["algo"] in ["brute_force_hash_skip"] and n > 51200:
        return False
    if (
        args["algo"]
        in [
            "brute_force_matrix_skip",
            "brute_force_hash_skip",
            "brute_force_clever_skip",
        ]
        and n > 102400
    ):
        return False
    return True


def mem_parser(stdout, res):
    """Parse the memory measurement and append it to  stdout."""
    return stdout + "," + res.stderr.strip()


######################################################################
# generate random graphs
run.group("count")

run.add(
    "[[algo]]",
    "/usr/bin/time -f '%M' code/release/count_triangles --algo [[algo]] --no-header data/graphs/[[graph]]",
    {
        "algo": [
            "clever",
            "clever_skip",
            "clever_sort_skip",
            "brute_force_matrix",
            "brute_force_matrix_skip",
            "brute_force_hash_skip",
            "brute_force_clever_skip",
        ],
        "graph": graphs,
    },
    combinations_filter=size_filter,
    stdout_file="data/results_[[algo]].csv",
    stdout_mod=mem_parser,
    header_command="code/release/count_triangles --only-header",
    header_mod=lambda head: f"{head},memory",
)


run.run()
