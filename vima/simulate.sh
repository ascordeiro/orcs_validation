#!/bin/bash
HOME="/home/srsantos/Experiment"
SIM_HOME=$HOME"/OrCS"
CODE_HOME=$HOME"/orcs_validation/vima"
TRACE_HOME=$HOME"/orcs_validation/vima/traces_new_vima"
DATE_TIME=$(date '+%d%m%Y_%H%M%S');
CACHE_SIZE=(32)

cd $CODE_HOME

if [ ! -d "resultados" ]; then
	mkdir -p "resultados"
fi

cd $TRACE_HOME
for i in *_64B*.tid0.stat.out.gz
do 
    cd $SIM_HOME
    for j in "${CACHE_SIZE[@]}";
    do
        TRACE=${i%.tid0.stat.out.gz}
        echo "./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/vima_variations/sandy_vima_256B_${j}_1cores.cfg &> ${CODE_HOME}/resultados/${TRACE}_256B_${j}_1cores_${DATE_TIME}.txt &"
        nohup ./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/vima_variations/sandy_vima_256B_${j}_1cores.cfg &> ${CODE_HOME}/resultados/${TRACE}_256B_${j}_${DATE_TIME}.txt &
    done
done

cd $TRACE_HOME
for i in *_2048B*.tid0.stat.out.gz
do 
    cd $SIM_HOME
    for j in "${CACHE_SIZE[@]}";
    do
        TRACE=${i%.tid0.stat.out.gz}
        #echo "./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/vima_variations/sandy_vima_8K_${j}_1cores.cfg &> ${CODE_HOME}/resultados/${TRACE}_8K_${j}_1cores_${DATE_TIME}.txt &"
        #nohup ./orcs -t ${TRACE_HOME}/${TRACE} -c configuration_files/vima_variations/sandy_vima_8K_${j}_1cores.cfg &> ${CODE_HOME}/resultados/${TRACE}_8K_${j}_${DATE_TIME}.txt &
    done
done
