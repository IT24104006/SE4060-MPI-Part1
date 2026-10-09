#!/bin/bash
export OMPI_MCA_rmaps_base_oversubscribe=1
echo "procs,run,time" > pi_any_results.csv
for p in 1 2 4 8; do
  for r in 1 2 3 4 5 6 7 8 9 10; do
    t=$(mpirun --bind-to none -n $p ./pi_any_source | tail -n 1 | awk '{print $(NF-1)}')
    echo "$p,$r,$t" >> pi_any_results.csv
  done
done
