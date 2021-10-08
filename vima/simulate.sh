#!/bin/bash
HOME="/home/srsantos/Experiment"
SIM_HOME=$HOME"/OrCS"
CODE_HOME=$HOME"/orcs_validation/vima"
TRACE_HOME=$HOME"/orcs_validation/vima/traces"
DATE_TIME=$(date '+%d%m%Y_%H%M%S');
CACHE_SIZE=(64)

cd $CODE_HOME

if [ ! -d "resultados" ]; then
	mkdir -p "resultados"
fi

cd $TRACE_HOME

for i in *.1KB.1t.tid0.stat.out.gz
do 
    for j in "${CACHE_SIZE}"
    do
    cd $SIM_HOME
    if [[ ${i%.c} != matmul.* && ${i%.c} != *_256* ]]; then
        TRACE=${i%.tid0.stat.out.gz}
        echo "./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/skylakeServer.cfg &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt &"
        nohup ./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/skylakeServer.cfg &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt &
    else
        TRACE=${i%.tid0.stat.out.gz}
        echo "./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/sandy_vima_256.cfg &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt &"
        nohup ./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/sandy_vima_256.cfg &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt &
    fi
    done
done
