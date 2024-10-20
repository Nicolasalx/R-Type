#!/bin/bash

INTERFACE="lo"

cleanup()
{
    sudo tc qdisc del dev "$INTERFACE" root 2>/dev/null
    exit 0
}

trap cleanup SIGINT SIGTERM EXIT

if [ $# -ne 3 ]; then
    echo -e "\e[93mUsage: $0 <latency_ms> <duplication_percentage> <loss_percentage>\e[0m"
    exit 1
fi

LATENCY_MS="$1"
DUPLICATION_PERCENT="$2"
LOSS_PERCENT="$3"

echo "network conditions applyed: ${LATENCY_MS}ms latency, ${DUPLICATION_PERCENT}% duplication, ${LOSS_PERCENT}% packet loss..."
sudo tc qdisc add dev "$INTERFACE" root netem delay "${LATENCY_MS}ms" loss "${LOSS_PERCENT}%" duplicate "${DUPLICATION_PERCENT}%"

while true; do
    sleep 1
done
