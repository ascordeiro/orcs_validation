#!/bin/bash
HOME="/home/sairo/Experiment"
SIM_HOME=$HOME"/OrCS"
PIN_HOME=$SIM_HOME"/trace_generator/pin"
SINUCA_TRACER_HOME=$SIM_HOME"/trace_generator/extras/pinplay/bin/intel64/sinuca_tracer.so"
CODE_HOME=$HOME"/orcs_validation/vima/omp"
COMP_FLAGS="-O2 -DNOINLINE -static -fopenmp"
SIZES=(1 2 4 8 16 32 64)
SIZES_MATMUL=(1 2 4 8)
THREADS=2

cd $CODE_HOME

if [ ! -d "exec" ]; then
	mkdir -p "exec"
fi

if [ ! -d "traces" ]; then
	mkdir -p "traces"
fi

for i in *.c
do 
    rm exec/${i%.c}.out
    g++ $i $COMP_FLAGS -o exec/${i%.c}.out
    export OMP_NUM_THREADS=${THREADS}
    if [[ ${i%.c} != matmul* ]]; then
	for j in "${SIZES[@]}";
	do
    		echo "$PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.${THREADS}t -threads ${THREADS} -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out"
	    	nohup $PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.${THREADS}t -threads ${THREADS} -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out
	done
    else
    	for j in "${SIZES_MATMUL[@]}";
	do
    		echo "$PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.${THREADS}t -threads ${THREADS} -- $CODE_HOME/exec/${i%.c}.out 64 ${j} &> nohup.out"
	    	#nohup $PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.${THREADS}t -threads ${THREADS} -- $CODE_HOME/exec/${i%.c}.out 64 ${j} &> nohup.out
	done
    fi
done
