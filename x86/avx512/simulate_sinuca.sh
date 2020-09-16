#!/bin/bash
HOME="/home/srsantos/Experiment"
SIM_HOME=$HOME"/sinuca"
CODE_HOME=$HOME"/orcs_validation/x86/avx512"
TRACE_HOME=$HOME"/orcs_validation/x86/avx512/traces"
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
    	echo "nohup ./sinuca -t ${TRACE_HOME}/${TRACE} -c config_examples/sandy_8cores_hmc/sandy_8cores_hmc.cfg &> ${CODE_HOME}/resultados/${TRACE}_sinuca_${DATE_TIME}.txt"
    	nohup ./sinuca -t ${TRACE_HOME}/${TRACE} -c config_examples/sandy_8cores_hmc/sandy_8cores_hmc.cfg &> ${CODE_HOME}/resultados/${TRACE}_sinuca_${DATE_TIME}.txt &
    fi
done
