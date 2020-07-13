#!/bin/bash
HOME="/home/sairo/Experiment"
SIM_HOME=$HOME"/OrCS"
CODE_HOME=$HOME"/orcs_validation/vima"
TRACE_HOME=$HOME"/orcs_validation/vima/traces"
DATE_TIME=$(date '+%d%m%Y_%H%M%S');

cd $CODE_HOME

if [ ! -d "resultados" ]; then
	mkdir -p "resultados"
fi

cd $TRACE_HOME

for i in *.tid0.stat.out.gz
do 
    cd $SIM_HOME
    if [[ ${i%.c} != matmul* ]]; then
        TRACE=${i%.tid0.stat.out.gz}
        echo "./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/sandy_vima_8192.cfg &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt &"
        #nohup ./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/sandy_vima_8192.cfg &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt &
    else
        TRACE=${i%.tid0.stat.out.gz}
        echo "./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/sandy_vima_256.cfg &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt &"
        #nohup ./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/sandy_vima_256.cfg &> ${CODE_HOME}/resultados/${TRACE}_${DATE_TIME}.txt &
    fi
done