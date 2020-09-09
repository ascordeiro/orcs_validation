#!/bin/bash
HOME="/home/srsantos/Experiment"
SIM_HOME=$HOME"/OrCS"
CODE_HOME=$HOME"/orcs_validation/hive"
TRACE_HOME=$HOME"/benchmarks/traces/hive"
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
    echo "./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/sandy_hive.cfg &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt &"
    nohup ./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/sandy_hive.cfg &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt &
done
