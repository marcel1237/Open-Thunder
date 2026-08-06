# ⚡ THUNDER PERFORMANCE MANIFESTO (TPM-01)

Este documento define o padrão obrigatório de desenvolvimento para o ecossistema Thunder (SDK e Browser). A performance não é um objetivo; é a lei fundamental do projeto.

---

## 🏛️ 1. A Regra de Ouro (Zero-Regression Policy)
**A performance medida nunca deve regredir sem decisão documentada.** Comparações devem usar o mesmo compilador, flags, dados, trabalho observável, repetições e estatística. Segurança, correção e portabilidade são restrições obrigatórias; um ganho que introduz comportamento indefinido não é aceito.

## 🚀 2. Princípios de Desenvolvimento Enforced-Hardware
Para garantir o domínio constante do hardware, todo código deve seguir estes quatro dogmas:

### A. Substituição de Lógica por Silício
Sempre que possível, algoritmos de software (loops, condicionais, buscas) devem ser substituídos por instruções diretas de hardware:
*   **Strings/Parsing:** Devem usar Hex-Bitmask e SIMD (VectorShield).
*   **Branching:** Evitar `if/else` complexos; usar lógica branchless ou dicas de hardware (`TD_LIKELY`).

### B. Soberania sobre o Kernel (NitroCore)
O software solicita capacidades ao sistema operacional, registra cada retorno e continua de forma segura quando não autorizado.
*   Tempo real e afinidade são opt-in, medidos e limitados ao cpuset disponível.

### C. Alocação Determinística (OmniLock)
O uso de memória dinâmica padrão (`malloc/std::vector`) deve ser minimizado em caminhos críticos.
*   Memória de alta performance deve ser alocada via **HugeTLB (2MB/1GB)**.
*   Páginas devem ser travadas no silício (`mlockall`) para eliminar latência de swap.

### D. Zero-Latência Térmica (ApexPower)
Políticas de energia são opt-in e devem ser restauráveis. O estado solicitado e o estado confirmado devem ser reportados separadamente.

---

## 📊 3. Validação de Performance
Antes de finalizar qualquer fase, os seguintes testes são obrigatórios:
1. **ThunderCoreTests:** correção, limites e regressões sob sanitizers.
2. **Thunder_Hardware_Stress:** estabilidade e vazão observada, sem limiar universal inventado.
3. **Thunder_Ultimate_Benchmark:** mediana de caminhos equivalentes; reporta o resultado mesmo quando a otimização perde.

---
*Assinado: Thunder Core Architecture Team*
*Status: PERFORMANCE IS HARDWARE-ENFORCED*
