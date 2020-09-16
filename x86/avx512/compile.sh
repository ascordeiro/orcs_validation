#!/bin/bash
HOME="/home/srsantos/Experiment"
SIM_HOME=$HOME"/sinuca"
PIN_HOME=$SIM_HOME"/trace_generator/pin"
SINUCA_TRACER_HOME=$SIM_HOME"/trace_generator/extras/pinplay/bin/intel64/sinuca_tracer.so"
CODE_HOME=$HOME"/orcs_validation/x86/avx512"
COMP_FLAGS="-O2 -static -mavx2 -march=native"
SIZES=(1 2 4 8 16 32 64)
SIZES_MATMUL=(1 2 4 8)

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
    if [[ ${i%.c} != matmul* ]]; then
	for j in "${SIZES[@]}";
	do
    		echo "$PIN_HOME -t $SINUCA_TRACER_HOME -trace x86 -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &"
<<<<<<< HEAD
	    	nohup $PIN_HOME -t $SINUCA_TRACER_HOME -trace x86 -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out
=======
	    	#nohup $PIN_HOME -t $SINUCA_TRACER_HOME -trace x86 -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &
>>>>>>> 5980ae7357a1318fc3d1d81b59e3c608f43f3147
	done
    else
    	for j in "${SIZES_MATMUL[@]}";
	do
    		echo "$PIN_HOME -t $SINUCA_TRACER_HOME -trace x86 -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &"
	    	nohup $PIN_HOME -t $SINUCA_TRACER_HOME -trace x86 -output $CODE_HOME/traces/${i%.c}.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &
	done
    fi
done
