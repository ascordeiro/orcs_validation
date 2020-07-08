#!/bin/bash
HOME="/home/sairo/Experiment"
SIM_HOME=$HOME"/OrCS"
PIN_HOME=$SIM_HOME"/trace_generator/pin"
SINUCA_TRACER_HOME=$SIM_HOME"/trace_generator/extras/pinplay/bin/intel64/sinuca_tracer.so"
CODE_HOME=$HOME"/orcs_validation/vima"
COMP_FLAGS="-O2 -DNOINLINE -fopenmp"
SIZES=(1 2 4 8 16 32 64)
SIZES_MATMUL=(1 2 4 8)

if [ ! -d "$CODE_HOME/exec" ]; then
	mkdir -p "$CODE_HOME/exec"
fi

if [ ! -d "$CODE_HOME/traces" ]; then
	mkdir -p "$CODE_HOME/traces"
fi

cd $CODE_HOME

for i in *.c
do 
    rm exec/${i%.c}.out
    g++ $i $COMP_FLAGS -o exec/${i%.c}.out
    export OMP_NUM_THREADS=8
    if [[ ${i%.c} != matmul* ]]; then
	for j in "${SIZES[@]}";
	do
    		echo "$PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -threads 8 -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &"
	    	#nohup $PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -threads 8 -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &
	done
    else
    	for j in "${SIZES_MATMUL[@]}";
	do
    		echo "$PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -threads 8 -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &"
	    	#nohup $PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -threads 8 -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &
	done
    fi
done