#!/bin/bash
HOME="/home/srsantos/Experiment"
SIM_HOME=$HOME"/OrCS"
PIN_HOME=$SIM_HOME"/trace_generator/pin"
SINUCA_TRACER_HOME=$SIM_HOME"/trace_generator/extras/pinplay/bin/intel64/sinuca_tracer.so"
CODE_HOME=$HOME"/orcs_validation/vima"
COMP_FLAGS="-O2 -DNOINLINE -static"
VECTOR=(2048)
SIZES=(1)
SIZES_MATMUL=(1)

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
		for k in "${VECTOR[@]}";
		do
			for j in "${SIZES[@]}";
			do
					echo "$PIN_HOME -t $SINUCA_TRACER_HOME -orcs_tracing 1 -trace iVIM -output $CODE_HOME/traces/${i%.c}_${k}B.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} ${k} &> ${i%.c}.${j}MB_${k}B.1t.out &"
					nohup $PIN_HOME -t $SINUCA_TRACER_HOME -orcs_tracing 1 -trace iVIM -output $CODE_HOME/traces/${i%.c}_${k}B.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} ${k} &> ${i%.c}_${k}B.${j}MB.1t.out &
			done
		done
    else
    	for j in "${SIZES_MATMUL[@]}";
    	do
    		echo "$PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}_64B.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} &> ${i%.c}.${j}MB_${k}B.1t.out &"
    	    	nohup $PIN_HOME -t $SINUCA_TRACER_HOME -trace iVIM -output $CODE_HOME/traces/${i%.c}_64B.${j}MB.1t -- $CODE_HOME/exec/${i%.c}.out ${j} 64 &> ${i%.c}.${j}MB_${k}B.1t.out &
    	done
    fi
done
