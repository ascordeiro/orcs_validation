#!/bin/bash
HOME="/home/sairo/Experiment"
SIM_HOME=$HOME"/OrCS"
CODE_HOME=$HOME"/orcs_validation/x86/avx256"
TRACE_HOME=$HOME"/orcs_validation/x86/avx256/traces"

cd $CODE_HOME

if [ ! -d "../resultados" ]; then
	mkdir -p "../resultados"
fi

cd $TRACE_HOME

for i in *.tid0.stat.out.gz
do 
    cd $SIM_HOME
    TRACE=${i%.tid0.stat.out.gz}
    echo "nohup ./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/sandy_bridge.cfg &> ${CODE_HOME}/resultados/${TRACE}.txt &"
    nohup ./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/sandy_bridge.cfg &> ${CODE_HOME}/resultados/${TRACE}.txt &
done