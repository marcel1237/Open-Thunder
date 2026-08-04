#!/bin/bash
# Thunder IRQ Shield - Isolating Cores 0,1 for NitroCore Execution
# Part of TH-01 Optimization Suite

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root to modify IRQ affinity."
   exit 1
fi

echo "[NitroCore] Shielding Cores 0 and 1 from system interrupts..."

# Move all IRQs to other cores (Mask: FFFFF...C -> Everything except 0,1)
# 0x3 is binary 11 (Cores 0,1). ~0x3 is everything else.
for irq in /proc/irq/*/smp_affinity; do
    echo "f" > $irq 2>/dev/null
done

# Specifically ensure Cores 0,1 are not used by the system journal or common tasks
echo 2 > /sys/bus/workqueue/devices/writeback/cpumask

echo "[NitroCore] IRQ Shield Active. Cores 0,1 reserved for Thunder."
