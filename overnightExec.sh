#!/bin/bash

from=2600
to=2801   # not included
NAGENTS=$from

echo "[$(date)] Starting analysis with N_AGENTS = $NAGENTS" >> log.txt

while (( NAGENTS < to )); do
    echo "N_AGENTS = $NAGENTS" >> log.txt
    echo "Running analysis with N_AGENTS = $NAGENTS"

    ./build/analysis "$NAGENTS" >> log.txt

    if [ $? -ne 0 ]; then
        echo "Error occurred during execution. Exiting."
        echo "Error at N_AGENTS = $NAGENTS" >> log.txt
        exit 1
    fi

    echo "Execution completed successfully." 
    sleep 120

    # check temperature
    TEMP=$(sensors | sed -n '/Tctl/s/.*Tctl:\s*\([+][0-9]*\.[0-9]*\).*/\1/p')
    TEMP=${TEMP#+}

    if [[ -z "$TEMP" ]]; then
        echo "Could not read temperature. Out of safety, exiting."
        exit 1
    fi

    while [ "$(echo "$TEMP > 70.0" | bc -l)" -eq 1 ]; do
        echo "Temperature too high: $TEMP°C. Sleeping a bit more"
        sleep 60
        TEMP=$(sensors | sed -n '/Tctl/s/.*Tctl:\s*\([+][0-9]*\.[0-9]*\).*/\1/p')
        TEMP=${TEMP#+} 
    done

    NAGENTS=$((NAGENTS + 100))
done

echo "[$(date)] Analysis completed successfully." >> log.txt
