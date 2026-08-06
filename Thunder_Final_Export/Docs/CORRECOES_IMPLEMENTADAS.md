# Relatório de correções implementadas

Data: 2026-08-06.

## Segurança e correção

- Corrigida conversão nibble/hex e adicionada validação de caracteres e overflow.
- APIs de imagem, URL e prefixo de protocolo agora recebem tamanho e validam ponteiros.
- Eliminadas leituras além da cauda no filtro AVX2; caminho escalar cobre o restante.
- Scanner valida entrada e delega a `memchr`, que faz dispatch SIMD otimizado para a CPU em uso; o AVX2 manual perdeu para libc no benchmark observado.
- Adicionada desalocação simétrica e validação de overflow/tamanho para huge pages.
- Cache warmer passou a ser idempotente, joinable e encerrado no destrutor.
- JIT passou de RWX para W^X: escreve primeiro e sela com `mprotect` antes de executar.
- IPC usa `memfd_create`, fecha o descritor após `mmap` e valida parâmetros.
- DMA sync retorna sucesso real do `ioctl`; placeholders de log foram removidos desse caminho.
- io_uring usa `SYS_io_uring_setup`, inicialização idempotente e fecha o fd.
- RDRAND não usa mais TSC como suposta entropia criptográfica.

## Medição e observabilidade

- `OptimizationReport` registra afinidade, lock de memória, timer slack, scheduler, DMA latency, governors e erros.
- Handshake fixo foi substituído por relatório do estado observável.
- Benchmark de 100 percentuais hard-coded foi removido.
- Novo benchmark compara o mesmo buffer/trabalho entre `std::memchr` e o dispatch Thunder, aquece ambos, usa 31 pares em ordem alternada, mediana, sink observável e valida o resultado.
- Medição final no host de auditoria (31 pares alternados, 64 MiB): `std::memchr` 4,884 ms / 13,740 GB/s; Thunder 4,913 ms / 13,661 GB/s; razão 0,994x. O resultado é equivalência dentro do ruído, não aceleração alegada.
- Stress test passou a declarar AVX2 corretamente e impede eliminação do trabalho.

## Build e testes

- `Thunder Linux` agora inclui `ThunderSDK` como target CMake, sem link para `.so` pré-compilada.
- Flags são associadas a opções; build SDK portátil é o padrão e tuning nativo é explícito.
- Adicionados warnings e opção ASan/UBSan.
- Criado `ThunderCoreTests` para hex, limites SIMD, cauda bitmask, WebP e URLs.
- Builds de auditoria são feitos fora da árvore para não alterar artefatos versionados.

## Operação privilegiada

- Scripts agora usam modo dry-run por padrão, validação de root/entrada e `set -euo pipefail`.
- Ajustes sysctl geram script de rollback antes da aplicação.
- Removida concessão automática de `CAP_SYS_RAWIO` e outras capabilities.
- Máscara IRQ deixou de ser presumida; o operador deve fornecer uma máscara válida para sua topologia.
- Serviço Dark Volt usa caminhos instaláveis, usuário dedicado, hardening e restart somente em falha.

## Continuação: pendências resolvidas

- A árvore duplicada `Thunder Linux/src/lib` foi removida; `ThunderSDK/src` é a única fonte de verdade.
- O adblock foi reduzido a uma API funcional e testada, com carregamento local explícito, comentários, exceções, domínio, âncoras e curingas. Não há download silencioso de listas.
- O interceptor agora bloqueia requests correspondentes e retorna imediatamente.
- Flags Chromium/Mesa/driver hard-coded foram removidas. Flags experimentais só entram por `THUNDER_EXPERIMENTAL_GPU_FLAGS` antes do `QApplication`.
- A instalação passou a seguir GNUInstallDirs e exporta o pacote CMake `Thunder::ThunderSDK` com versão e dependências.

## Pendências deliberadas restantes

- Documentos históricos de “100 pilares” permanecem como registros, mas não devem ser tratados como evidência de benchmark. Este relatório e o README definem o estado verificável atual.

## Validação executada

- Build limpo e portátil do SDK (`THUNDER_NATIVE_OPTIMIZATIONS=OFF`): aprovado.
- Build limpo integrado de todos os alvos Linux: aprovado.
- `ThunderCoreTests` no SDK e no projeto Linux: 100% aprovado.
- ASan + UBSan: testes aprovados executando com `ASAN_OPTIONS=detect_leaks=0`. O LeakSanitizer isolado não opera neste ambiente por estar sob `ptrace`; isso é limitação do executor, não um resultado de ausência de leaks.
- `bash -n` e dry-run dos dois scripts privilegiados: aprovados, sem alteração do sistema.
- `git diff --check`: aprovado.
