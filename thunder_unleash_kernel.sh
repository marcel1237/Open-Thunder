#!/usr/bin/env bash
set -euo pipefail

settings=(
    "kernel.sched_rt_runtime_us=-1"
    "vm.max_map_count=1000000"
    "net.core.busy_poll=50"
    "net.core.busy_read=50"
    "net.core.netdev_max_backlog=10000"
)

if [[ ${THUNDER_APPLY:-0} != 1 ]]; then
    printf 'Dry-run; nenhuma configuração foi alterada. Propostas:\n'
    printf '  %s\n' "${settings[@]}"
    echo "Para aplicar conscientemente: sudo env THUNDER_APPLY=1 $0"
    exit 0
fi

if [[ ${EUID} -ne 0 ]]; then
    echo "Execute como root para aplicar." >&2
    exit 1
fi

rollback_file=${THUNDER_ROLLBACK_FILE:-/tmp/thunder-kernel-rollback.sh}
umask 077
{
    echo '#!/usr/bin/env bash'
    echo 'set -euo pipefail'
    for setting in "${settings[@]}"; do
        key=${setting%%=*}
        old=$(sysctl -n "${key}")
        printf 'sysctl -w %q=%q\n' "${key}" "${old}"
    done
} > "${rollback_file}"
chmod 700 "${rollback_file}"

for setting in "${settings[@]}"; do sysctl -w "${setting}"; done
echo "Configuração aplicada. Rollback: ${rollback_file}"
echo "Capacidades de binário e CAP_SYS_RAWIO não são concedidas automaticamente."
