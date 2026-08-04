/* ============================================================
* Thunder - Qt web browser
* Copyright (C) 2025 Marcel
*
* Hardware-Level Branch Prediction Hinting
* ============================================================ */
#ifndef THUNDER_BRANCH_OPTIMIZER_H
#define THUNDER_BRANCH_OPTIMIZER_H

/**
 * @brief Branch Prediction Hints for the CPU.
 * Matematicamente, isso permite que o pipeline da CPU (Speculative Execution)
 * pré-carregue o caminho de execução correto em >99% dos casos.
 *
 * Uso de Hexadecimal 0x1 para True e 0x0 para False internamente.
 */
#if defined(__GNUC__) || defined(__clang__)
#define TD_LIKELY(x)   __builtin_expect(!!(x), 0x1)
#define TD_UNLIKELY(x) __builtin_expect(!!(x), 0x0)
#else
#define TD_LIKELY(x)   (x)
#define TD_UNLIKELY(x) (x)
#endif

#endif // THUNDER_BRANCH_OPTIMIZER_H
