#!/bin/bash

BUILDMODE="full"
MAX_CORES="4"

if [ $1 ]
then
    if [ $1 == '--fast' ]
    then
        BUILDMODE="fast"
    fi

    if [ $1 == '--test-only' ]
    then
        BUILDMODE="test-only"
    fi

    if [ $1 == '--scan-only' ]
    then
        BUILDMODE="scan-only"
    fi

    if [ $1 == '--all-esp32' ]
    then
        BUILDMODE="all-esp32"
    fi
fi

if [ $BUILDMODE == "fast" ]
then
    echo "doing the fast test with 2 boards and 2 examples"
    boards=("esp32dev" "ATmega1280")
    examples=("7-Segment-counting" "Led-matrix-rocket-hwSPI")
else
    if [ $BUILDMODE == "full" ]
    then
        echo "doing the full test with 5 boards and 6 examles"
        boards=("esp32dev" "d1_mini" "uno" "ATmega1280" "leonardo" "due" "teensy41" "bluefruitmicro")
    else
        echo "doing the full test with 1 board and 6 examles"
        boards=("esp32dev")
    fi
    examples=("Led-matrix-rocket" "Led-matrix-rocket-multi" "Led-matrix-counting" "Led-matrix-rocket-hwSPI" "Led-matrix-message" "7-Segment-counting")
fi

function build(){
    local ex=$1
    local board=$2
    PLATFORMIO_CI_SRC="examples/english/$ex/$ex.ino" python3 -m platformio ci --lib="." -b $board > >(tee logs/$ex-$board.log) 2> >(tee  logs/$ex-$board.error.log >&2)
    if [ $? -eq 1 ]
    then
        echo "error while building $ex"
        touch error.tmp
    fi
}

if [ $BUILDMODE != "test-only" ] && [ $BUILDMODE != "scan-only" ]
then
    rm -rf logs
    mkdir logs
    for ex in "${examples[@]}"; do 
        for board in "${boards[@]}"; do 
            ((i=i%MAX_CORES)); ((i++==0)) && wait
            build "$ex" "$board" &
        done
    done
    wait
    
    if [ -f "error.tmp" ]
    then
        echo "error while building"
        rm error.tmp
        exit 1
    fi
fi

if [ $BUILDMODE != "scan-only" ]
then
    echo "running the native unit tests"
    python3 -m platformio test -e native > >(tee logs/tests.log) 2> >(tee  logs/tests.error.log >&2)
fi

echo "The following warnings/error are about the LedController:"
grep LedController logs/*error.log

exit 0
