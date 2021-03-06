#!/bin/bash
HOME="/home/srsantos/Experiment"
SIM_HOME=$HOME"/OrCS"
CODE_HOME=$HOME"/orcs_validation/vima/omp"
TRACE_HOME=$HOME"/benchmarks/traces/traces_aline_mlp_knn/desespero_e_odio"
THREADS=(1)
#(2 4 8 16 32)
VECTOR_SIZE=(8K 256B)
CACHE_SIZE=(64)
#(32 64 128 256)
DATE_TIME=$(date '+%d%m%Y_%H%M%S');

cd $CODE_HOME

if [ ! -d "resultados" ]; then
	mkdir -p "resultados"
fi

for t in "${THREADS[@]}"
do
    echo "THREADS: ${t}"
    cd $TRACE_HOME
    for i in *.tid0.stat.out.gz
    do 
        cd $SIM_HOME
        TRACE=${i%.tid0.stat.out.gz}
        COUNTER=0
        COMMAND="./orcs"
        while [ $COUNTER -lt $t ]; do
            COMMAND=${COMMAND}' -t '${TRACE_HOME}/${TRACE}
            let COUNTER=COUNTER+1
        done
        for k in "${CACHE_SIZE[@]}"
        do
	    #if [[ ${TRACE} == memset* ]]; then
		if [[ ${TRACE} == *_avx_* ]]; then
	 	    CONFIG_FILE="configuration_files/sandy_bridge.cfg"
		    nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${k}_${DATE_TIME}.txt &
                    echo "nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${k}_${DATE_TIME}.txt &"
		fi
                if [[ ${TRACE} == *_256v_* ]]; then 
                    CONFIG_FILE="configuration_files/vima_variations/sandy_vima_256B_${k}_${t}cores.cfg"
		    nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${k}_${DATE_TIME}.txt &
		    echo "nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${k}_${DATE_TIME}.txt &"
		fi
                if [[ ${TRACE} == *_8192v_* ]]; then
                    CONFIG_FILE="configuration_files/vima_variations/sandy_vima_8K_${k}_${t}cores.cfg"
		    nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${j}_${k}_${DATE_TIME}.txt &
		    echo "nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${j}_${k}_${DATE_TIME}.txt &"
                fi
                #echo "nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${k}_${DATE_TIME}.txt &"
                #nohup ${COMMAND} -c ${CONFIG_FILE} &> ${CODE_HOME}/resultados/${TRACE}_${k}_${DATE_TIME}.txt &
                #echo "${COMMAND} -c ${CONFIG_FILE}"
	    #fi
        done
    done
done
