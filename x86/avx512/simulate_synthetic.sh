#!/bin/bash
HOME="/home/srsantos/Experiment"
SIM_HOME=$HOME"/OrCS"
CODE_HOME=$HOME"/orcs_validation/x86/avx512"
TRACE_HOME=$HOME"/benchmarks/traces/avx512"
DATE_TIME=$(date '+%d%m%Y_%H%M%S');

cd $CODE_HOME

if [ ! -d "resultados" ]; then
	mkdir -p "resultados"
fi

cd $TRACE_HOME

for i in *.tid0.stat.out.gz
do 
    cd $SIM_HOME
    TRACE=${i%.tid0.stat.out.gz}
    if [[ ${TRACE} != matmul* ]]; then
    	echo "nohup ./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/sandy_bridge.cfg &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt"
    	nohup ./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/sandy_bridge.cfg &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt &
    fi
done
