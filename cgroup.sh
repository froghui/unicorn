#!/bin/bash

cgroups=("cpu" "cpuset" "cpuacct" "memory" "blkio")
for cgroup in ${cgroups[@]};do
    echo "setup  $cgroup in /tmp/cgroup/$cgroup"
    if [ -d /tmp/cgroup/$cgroup ];then
        echo "cgroup $cgroup has mounted. exit"
    else
        sudo mkdir -p /tmp/cgroup/$cgroup
        sudo mount -t cgroup $cgroup -o $cgroup /tmp/cgroup/$cgroup
    fi
done
