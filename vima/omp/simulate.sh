#!/bin/bash
HOME="/home/sairo/Experiment"
SIM_HOME=$HOME"/OrCS"
CODE_HOME=$HOME"/orcs_validation/vima/omp"
TRACE_HOME=$CODE_HOME"/traces"
THREADS=(2 4 8 16 32)
VECTOR_SIZE=(256B 8K)
CACHE_SIZE=(32 64 128 256)
DATE_TIME=$(date '+%d%m%Y_%H%M%S');

cd $CODE_HOME

if [ ! -d "resultados" ]; then
	mkdir -p "resultados"
fi

for t in "${THREADS[@]}"
do
    echo "THREADS: ${t}"
    cd $TRACE_HOME
    for i in *.${t}t.tid0.stat.out.gz
    do 
        cd $SIM_HOME
        TRACE=${i%.tid0.stat.out.gz}
        COUNTER=0
        COMMAND="./orcs"
        while [ $COUNTER -lt $THREADS ]; do
            COMMAND=${COMMAND}' -t '${TRACE_HOME}/${TRACE}
            let COUNTER=COUNTER+1
        done
        for k in "${CACHE_SIZE[@]}"
        do
                if [[ ${TRACE} == matmul* || ${TRACE} == *_256* ]]; then 
                    CONFIG_FILE="configuration_files/vima_variations/sandy_vima_256B_${k}_${t}cores.cfg"
                else
                    CONFIG_FILE="configuration_files/vima_variations/sandy_vima_8K_${k}_${t}cores.cfg"
                fi
                echo "nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${k}_${DATE_TIME}.txt &"
                #nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${j}_${k}_${DATE_TIME}.txt &
                #echo "${COMMAND} -c ${CONFIG_FILE}"
        done
    done
done
