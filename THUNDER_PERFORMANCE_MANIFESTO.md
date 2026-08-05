# ⚡ THUNDER PERFORMANCE MANIFESTO (TPM-01)

Este documento define o padrão obrigatório de desenvolvimento para o ecossistema Thunder (SDK e Browser). A performance não é um objetivo; é a lei fundamental do projeto.

---

## 🏛️ 1. A Regra de Ouro (Zero-Regression Policy)
**A performance nunca pode cair.** Qualquer novo commit, pilar (TH) ou modificação de código que resulte em uma regressão de milissegundos em relação ao benchmark anterior deve ser sumariamente rejeitado e re-arquitetado.

## 🚀 2. Princípios de Desenvolvimento Enforced-Hardware
Para garantir o domínio constante do hardware, todo código deve seguir estes quatro dogmas:

### A. Substituição de Lógica por Silício
Sempre que possível, algoritmos de software (loops, condicionais, buscas) devem ser substituídos por instruções diretas de hardware:
*   **Strings/Parsing:** Devem usar Hex-Bitmask e SIMD (VectorShield).
*   **Branching:** Evitar `if/else` complexos; usar lógica branchless ou dicas de hardware (`TD_LIKELY`).

### B. Soberania sobre o Kernel (NitroCore)
O software não deve pedir permissão ao sistema operacional; ele deve impor seu estado.
*   Prioridades devem ser `SCHED_FIFO` ou `SCHED_RR`.
*   Afinidade de CPU deve ser fixa para evitar cache-misses por migração de núcleo.

### C. Alocação Determinística (OmniLock)
O uso de memória dinâmica padrão (`malloc/std::vector`) deve ser minimizado em caminhos críticos.
*   Memória de alta performance deve ser alocada via **HugeTLB (2MB/1GB)**.
*   Páginas devem ser travadas no silício (`mlockall`) para eliminar latência de swap.

### D. Zero-Latência Térmica (ApexPower)
A CPU e a GPU nunca devem entrar em estados de repouso (C-States) enquanto o software estiver ativo. A latência de DMA deve ser mantida em **0ns**.

---

## 📊 3. Validação de Performance
Antes de finalizar qualquer fase, os seguintes testes são obrigatórios:
1.  **Thunder_Hardware_Stress:** Validar se a vazão de vetores permanece acima de 20GB/s.
2.  **Thunder_Ultimate_Benchmark:** Confirmar que o veredito permanece no patamar de **20x mais rápido** que o sistema baseline.

---
*Assinado: Thunder Core Architecture Team*
*Status: PERFORMANCE IS HARDWARE-ENFORCED*
