#!/bin/bash
HOME="/home/srsantos/Experiment"
SIM_HOME=$HOME"/sinuca"
PIN_HOME=$SIM_HOME"/trace_generator/pin"
SINUCA_TRACER_HOME=$SIM_HOME"/trace_generator/extras/pinplay/bin/intel64/sinuca_tracer.so"
CODE_HOME=$HOME"/orcs_validation/x86/avx512_omp"
COMP_FLAGS="-O2 -static -mavx2 -march=native -fopenmp"
SIZES=(1 2 4 8 16 32 64)
SIZES_MATMUL=(1 2 4 8)
THREADS_N=(2 4 8 16 32)

cd $CODE_HOME

if [ ! -d "exec" ]; then
	mkdir -p "exec"
fi

if [ ! -d "traces" ]; then
	mkdir -p "traces"
fi

for THREADS in "${THREADS_N[@]}";
do
	for i in *.c
	do 
    		rm exec/${i%.c}.out
    		g++ $i $COMP_FLAGS -o exec/${i%.c}.out
    		export OMP_NUM_THREADS=${THREADS}
		export OMP_WAIT_POLICY=passive
    		if [[ ${i%.c} != matmul* ]]; then
			for j in "${SIZES[@]}";
			do
				echo "$PIN_HOME -t $SINUCA_TRACER_HOME -trace x86 -output $CODE_HOME/traces/${i%.c}.${j}MB.${THREADS}t -threads ${THREADS} -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out"
				#nohup $PIN_HOME -t $SINUCA_TRACER_HOME -trace x86 -output $CODE_HOME/traces/${i%.c}.${j}MB.${THREADS}t -threads ${THREADS} -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out
			done
    		else
    			for j in "${SIZES_MATMUL[@]}";
			do
				echo "$PIN_HOME -t $SINUCA_TRACER_HOME -trace x86 -output $CODE_HOME/traces/${i%.c}.${j}MB.${THREADS}t -threads ${THREADS} -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &"
				nohup $PIN_HOME -t $SINUCA_TRACER_HOME -trace x86 -output $CODE_HOME/traces/${i%.c}.${j}MB.${THREADS}t -threads ${THREADS} -- $CODE_HOME/exec/${i%.c}.out ${j} &> nohup.out &
			done
    		fi
	done
done
