#!/bin/bash
HOME="/home/srsantos/Experiment"
SIM_HOME=$HOME"/OrCS"
CODE_HOME=$HOME"/orcs_validation/x86/avx512_omp"
TRACE_HOME=$HOME"/orcs_validation/x86/avx512_omp/traces"
THREADS=2
CONFIG_FILE="configuration_files/sandy_bridge_${THREADS}cores.cfg"
DATE_TIME=$(date '+%d%m%Y_%H%M%S');

cd $CODE_HOME

if [ ! -d "resultados" ]; then
	mkdir -p "resultados"
fi

cd $TRACE_HOME

for i in *.${THREADS}t.tid0.stat.out.gz
do 
    cd $SIM_HOME
    TRACE=${i%.tid0.stat.out.gz}
    COUNTER=0
    COMMAND="./orcs"
    while [ $COUNTER -lt $THREADS ]; do
        COMMAND=${COMMAND}' -t '${TRACE_HOME}/${TRACE}
        let COUNTER=COUNTER+1
    done

    if [[ ${TRACE} != matmul* ]]; then 
        echo "nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt"
        nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt &
    fi
done
