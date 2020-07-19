#!/bin/bash
HOME="/home/sairo/Experiment"
SIM_HOME=$HOME"/OrCS"
CODE_HOME=$HOME"/orcs_validation/vima/omp"
TRACE_HOME=$HOME"/traces"
THREADS=16
VECTOR_SIZE=(256B)
CACHE_SIZE=(32)
DATE_TIME=$(date '+%d%m%Y_%H%M%S');

cd $CODE_HOME

if [ ! -d "resultados" ]; then
	mkdir -p "resultados"
fi

cd $TRACE_HOME

for i in *_256.1MB.${THREADS}t.tid0.stat.out.gz
do 
    cd $SIM_HOME
    TRACE=${i%.tid0.stat.out.gz}
    COUNTER=0
    COMMAND="./orcs"
    while [ $COUNTER -lt $THREADS ]; do
        COMMAND=${COMMAND}' -t '${TRACE_HOME}/${TRACE}
        let COUNTER=COUNTER+1
    done

    for j in "${VECTOR_SIZE[@]}";
        do
        for k in "${CACHE_SIZE[@]}";
            do
                CONFIG_FILE="configuration_files/vima_variations/sandy_vima_${j}_${k}_${THREADS}cores.cfg"
                #if [[ ${TRACE} == matmul* ]]; then 
                    echo "nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${j}_${k}_${DATE_TIME}.txt &"
                    nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${j}_${k}_${DATE_TIME}.txt &
		    #echo "${COMMAND} -c ${CONFIG_FILE}"
                #fi
            done
        done
done
