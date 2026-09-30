#!/bin/bash
export OMPI_MCA_rmaps_base_oversubscribe=1
echo "procs,run,time" > sum_results.csv
for p in 1 2 4 8; do
  for r in 1 2 3 4 5 6 7 8 9 10; do
    t=$(mpirun --bind-to none -n $p ./sum_mpi | awk '{print $(NF-1)}')
    echo "$p,$r,$t" >> sum_results.csv
  done
done
