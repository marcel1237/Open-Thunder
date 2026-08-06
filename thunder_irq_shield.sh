#!/usr/bin/env bash
set -euo pipefail

if [[ ${THUNDER_APPLY:-0} != 1 ]]; then
    echo "Dry-run: defina THUNDER_APPLY=1 e THUNDER_IRQ_MASK com uma máscara válida."
    echo "Nenhuma afinidade foi alterada."
    exit 0
fi

if [[ ${EUID} -ne 0 ]]; then
    echo "Execute como root para aplicar." >&2
    exit 1
fi

: "${THUNDER_IRQ_MASK:?Defina a máscara das CPUs que devem receber IRQs (não use máscara presumida).}"
[[ ${THUNDER_IRQ_MASK} =~ ^[0-9a-fA-F,]+$ ]] || { echo "Máscara inválida" >&2; exit 1; }

changed=0
for affinity in /proc/irq/*/smp_affinity; do
    [[ -w ${affinity} ]] || continue
    if printf '%s\n' "${THUNDER_IRQ_MASK}" > "${affinity}"; then
        ((changed += 1))
    fi
done
echo "Afinidade atualizada em ${changed} IRQs; valide /proc/interrupts antes de executar carga crítica."
