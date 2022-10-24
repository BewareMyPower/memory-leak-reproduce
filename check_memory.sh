#!/bin/bash
set -e
LOOP_CNT=0
for i in {1..1000}; do
    PROC_ID=$(ps aux | grep "a.out" | grep -v grep | awk '{print $2}')
    if [[ $PROC_ID ]]; then
        echo "LOOP_CNT: $LOOP_CNT"
        set -x
        cat /proc/$PROC_ID/status | grep VmData
        set +x
        LOOP_CNT=$((LOOP_CNT+1))
        sleep 1
    else
        echo "No process running a.out is found"
    fi
    sleep 1
done
