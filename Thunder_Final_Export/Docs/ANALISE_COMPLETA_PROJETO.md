# Auditoria completa do projeto Thunder

Data da análise: 2026-08-06. Escopo: todos os arquivos do repositório, com revisão semântica linha a linha dos arquivos autorais e análise por metadados dos binários, imagens, caches e arquivos gerados.

## 1. Resultado executivo

O projeto compila nos diretórios de build existentes, mas atualmente é uma prova de conceito Linux/Qt, não uma camada "bare metal", Ring-0, de 100 aceleradores comprovados. Grande parte dos nomes TH representa: (a) wrappers pequenos de APIs Linux; (b) flags/variáveis de ambiente; (c) mensagens de status; ou (d) placeholders. Os relatórios de ganho não derivam seus percentuais de medições: o benchmark fabrica os ganhos numa tabela determinística.

Classificação geral:

- **Crítico:** benchmarks inválidos e alegações de telemetria inventada; risco de leitura fora de limites em rotinas SIMD; inicialização pode criar múltiplas threads destacadas que acessam objeto destruído; memória W+X no JIT; scripts root alteram parâmetros globais perigosos.
- **Alto:** chamadas privilegiadas ignoram erros e imprimem sucesso incondicional; API de parsing recebe ponteiros sem tamanho; `parallelBitmaskFilter` lê além do array; `findPathStart` lê antes do início; configuração fixa para apenas CPUs 0/1 e GPU `card0`; serviço systemd possui caminhos incorretos/inconsistentes.
- **Médio:** falta de testes automatizados, portabilidade limitada a x86-64/Linux, headers não autocontidos, vazamentos/contratos de desalocação ausentes, árvores duplicadas divergentes, exportação não reproduzível.
- **Baixo:** documentação aponta arquivos/diretórios inexistentes, logo ausente, nomenclatura e versões inconsistentes, arquivos de IDE/build/binários versionados.

## 2. Evidência de build

- `cmake --build ThunderSDK/build -j2`: sucesso.
- `cmake --build "Thunder Linux/build" -j2`: sucesso em todos os alvos.
- Aviso repetido: retorno de `system()` ignorado em `thunder_gpu_warmer.h:34`.
- O sucesso depende do host atual e dos builds já configurados; `-march=native -mavx2 -mfma -maes` produz binários não portáveis e pode causar `SIGILL` em outra CPU.
- Não há `CTest`, testes unitários, sanitizers, CI ou casos de erro. Os executáveis chamados de testes são demos/benchmarks.

## 3. Achados comprovados por linha

### 3.1 Segurança, memória e comportamento indefinido

1. `ThunderSDK/src/network/thunder_net_optimizer.h:129-138`: o laço avança de quatro em quatro, mas não trata `count % 4`; a última carga AVX2 pode ler até três `uint64_t` além do array.
2. `ThunderSDK/src/network/thunder_net_optimizer.h:103-105`: desreferencia quatro bytes sem aceitar tamanho, sem validar `nullptr` e por ponteiro possivelmente desalinhado; isso é comportamento indefinido em C++ e pode ultrapassar o buffer.
3. `ThunderSDK/src/network/thunder_url_warp.h:21-24`: lê oito bytes mesmo para strings menores e sem checar `nullptr`.
4. `ThunderSDK/src/network/thunder_url_warp.h:27-32`: na primeira iteração, se o primeiro caractere for `/`, `*(p-1)` lê antes do buffer; `*(p+1)` também exige contrato não documentado.
5. `ThunderSDK/src/network/thunder_advanced_network.h:46-48`: `decodeQPACK` lê 16 bytes sem tamanho ou validação e não implementa QPACK; apenas retorna os primeiros oito bytes.
6. `ThunderSDK/src/kernel/thunder_hex_utils.h:19-22`: a expressão ternária está logicamente quebrada; a condição contém `& 0x0` e é sempre falsa, então `10..15` viram `':'..'?'`, não `A..F`.
7. `ThunderSDK/src/kernel/thunder_huge_tlb.h:40-72`: `size == 0` produz mapeamento de tamanho zero; overflow no arredondamento não é verificado; a API não fornece `free` nem o tamanho arredondado necessário para `munmap`.
8. `Thunder Linux/src/examples/thunder_ultimate_benchmark.cpp:43-44`: não testa `MAP_FAILED` antes de `memset`, podendo falhar imediatamente.
9. `Thunder Linux/src/examples/ultimate_stress_test.cpp:44`: desaloca `test_size`, embora o alocador trabalhe com tamanho arredondado; neste caso 128 MiB já está alinhado, mas o exemplo ensina um contrato inseguro.
10. `ThunderSDK/src/kernel/thunder_jit_accelerator.h:35-67`: aloca memória simultaneamente gravável e executável (`RWX`), copia bytes arbitrários e não oferece desalocação; viola W^X e amplia exploração de corrupção de memória.
11. `ThunderSDK/src/kernel/thunder_cache_pool.h:34-48`: cada chamada inicia outra thread; a thread é destacada, nunca recebe `false`, não há destrutor de sincronização e ela acessa `this` durante/apos destruição estática.
12. `ThunderSDK/src/kernel/thunder_lockless_matrix.h:22-43`: funciona apenas sob premissas SPSC não documentadas; múltiplos produtores/consumidores causam corridas lógicas.
13. `ThunderSDK/src/kernel/thunder_bios_sync.h:45-52`: usar TSC como fallback não fornece entropia criptográfica, contrariando o comentário.
14. `ThunderSDK/src/kernel/thunder_aes_accelerator.h:23-40`: não valida `keys`/`rounds`; a rotina só processa bloco com schedule pronto e não é uma API de criptografia segura/completa.

### 3.2 Kernel, privilégios e confiabilidade

1. `ThunderSDK/src/kernel/kernel_bridge.cpp:38-99`: afinidade, `mlockall`, timer slack, prioridade, governor e segundo scheduler são aplicados quase sempre sem conferir retorno. A linha 98 anuncia êxito mesmo quando todas as operações falham.
2. `kernel_bridge.cpp:41-45`: presume CPUs 0 e 1 disponíveis; falha em container/cpuset, máquinas de uma CPU ou topologias em que não sejam núcleos de desempenho.
3. `kernel_bridge.cpp:55-59` e `94-96`: `SCHED_FIFO` 99 pode impedir progresso de tarefas críticas; a segunda chamada usa constante literal combinada ao policy e também ignora falha.
4. `kernel_bridge.cpp:73-76`: `RLIMIT_AS = infinity` não aumenta `vm.max_map_count`, apesar da documentação sugerir isso, e normalmente não é uma otimização.
5. `kernel_bridge.cpp:80-88`: escreve governors somente de `cpu0` e `cpu1`, ignora escrita curta/erro e altera política energética do sistema sem restauração.
6. `kernel_bridge.cpp:116-127`: o "handshake" não verifica hardware algum; apenas imprime cinco estados fixos.
7. `kernel_bridge.cpp:130-134`: números mágicos do `prctl` e argumentos sem validação; a alegação de desativar Spectre/Meltdown não é comprovada pelo retorno nem pelo estado consultado.
8. `thunder_gpu_warmer.h:23-36`: não compila shaders; define ambiente e executa shell para sysfs. O caminho é AMD-específico, fixo em `card0`, requer privilégio e o resultado é ignorado.
9. `thunder_io_matrix.h:30-57`: usa syscall 425 e constantes literais dependentes de ABI; não mapeia SQ/CQ nem submete IO. A chamada `setsockopt` de kTLS não constitui offload funcional.
10. `thunder_dma_sync.h:18-23`, `thunder_rendering_warp.h:18-34` e `thunder_security_warp.h:33-44`: funções apenas imprimem mensagens ou são placeholders, sem realizar a ação descrita.
11. `thunder_vdso_warp.h:31-37`: apenas localiza/imprime o endereço do vDSO; nenhuma função é resolvida ou usada.
12. `thunder_vram_cache.h:18-33`: variáveis específicas de drivers não criam cache L5/VRAM e podem forçar drivers incompatíveis.
13. `thunder_protection_matrix.h:19-52`: anti-debug é parcial e o fingerprint é explicitamente simulado; não sustenta "proteção de IP".
14. `thunder_irq_shield.sh:14-19`: escreve `f`, que representa CPUs 0-3, apesar de afirmar remover CPUs 0/1; em muitas máquinas faz o oposto da intenção. Variáveis não estão entre aspas e erros são ocultados.
15. `thunder_unleash_kernel.sh:9-35`: não verifica root/erros, desabilita globalmente throttling RT, muda rede global, monta filesystem e concede capacidades amplas. Não há rollback; caminhos absolutos tornam o script específico de uma máquina.

### 3.3 Benchmark e alegações científicas

1. `thunder_ultimate_benchmark.cpp:21-24`: baseline recebe atributo `O0`, enquanto Thunder é compilado com `O3`, invalidando comparação algorítmica.
2. `:31-49`: buffers diferentes e custos de inicialização/alocação/prefault são excluídos do tempo Thunder; o retorno de ambas as buscas é descartado, permitindo otimização; não há repetição, warm-up estatístico, barreira ou verificação do trabalho.
3. `:54-68`: os ganhos dos 100 pilares são fórmulas hard-coded, não telemetria. TH-100 recebe 99% embora seja apenas impressão.
4. `:97-98`: divide por tempo possivelmente zero e sempre declara "Absolute Dominance", independentemente do resultado.
5. `no_thunder_report.cpp:22,29` e `with_thunder_report.cpp:23,32`: o núcleo exibido é uma constante presumida, não telemetria (`sched_getcpu`).
6. `with_thunder_report.cpp:40-44`: estados "NitroCore", "OmniLock" e "OPTIMIZED" são impressos sem consultar retornos.
7. `ultimate_stress_test.cpp:31-41`: anuncia 512 bits, mas o build força AVX2 (256 bits); não usa resultado da busca e calcula throughput assumindo que todos os bytes foram efetivamente consumidos.
8. `THUNDER_ARCH_PERFORMANCE_REPORT.md:13-26`, `THUNDER_PERFORMANCE_MANIFESTO.md:33-36` e `THUNDER_TECHNICAL_MANUAL.md:66-69`: percentuais, 20x, >20 GB/s e <1 ns não são sustentados pela metodologia presente.

## 4. Revisão arquivo por arquivo

As linhas de copyright, include guards e fechamento de namespace são boilerplate válido salvo observação. Abaixo, todos os arquivos autorais são cobertos por intervalos.

### Raiz e documentação

- `README.md` — 1-20: visão e alegações exageradas (XDP, Vulkan, bare metal não implementados); 22-27: cita `Thunder Windows`, `Thunder Mac` e `Dark Volt/`, inexistentes com esses nomes; 29-32: licença; 34-43: dependências/handshake, mas Vulkan não é ligado e handshake não diagnostica; 45-48: metadados.
- `LICENSE.md` — 1-39: licença proprietária legível, porém termos como "marcas registradas" não são comprovados pelo repositório e a cláusula automática de propriedade de contribuições não substitui necessariamente um CLA aceito; 43-51: identificador. Exige revisão jurídica profissional, não apenas técnica.
- `CONTRIBUTING.md` — 1-34: política interna sem fluxo de build/test/review reproduzível; recomenda hex indiscriminadamente e validação por texto fixo de handshake.
- `THUNDER_ARCH_PERFORMANCE_REPORT.md` — 1-81: descreve ganhos como reais, mas eles são inseridos manualmente no benchmark; várias ações listadas são parciais/placeholders.
- `THUNDER_CODE_IMPLEMENTATION_DETAILS.md` — 1-162: reproduz trechos, não é realmente explicação de cada linha/100 pilares; confunde wrappers user-space com controle de hardware e não documenta falhas/retornos.
- `THUNDER_FULL_100_PILLARS_REPORT.md` — 1-147: catálogo de marketing; não há uma implementação/teste independente para cada pilar.
- `THUNDER_PERFORMANCE_MANIFESTO.md` — 1-40: diretrizes; "nunca regredir" sem baseline estável/estatística/CI e recomendações de RT/energia têm risco operacional.
- `THUNDER_TECHNICAL_MANUAL.md` — 1-72: fluxo e exemplo; não testa falha de alocação nem libera memória; métricas finais sem evidência.
- `THUNDER_BRAZILIAN_SOFTWARE_RIGHTS.md` — 1-34 e `THUNDER_INTELLECTUAL_PROPERTY_PATENT.md` — 1-42: declarações jurídicas, não prova de registro, patente concedida, marca registrada ou assinatura digital; precisam de advogado/agente de PI.
- `Thunder_Tech_Architecture.csv` — 1-103: inventário TH; status e ganhos não têm coluna de evidência/teste/commit.
- `Thunder_Tech_Dashboard.html` — 1-263: dashboard estático que apresenta o CSV/claims, não coleta telemetria.
- `resumo-05-08-2026-09-01.md` — 1-28: resumo declarativo; sua cópia exportada é idêntica.

### Build

- `ThunderSDK/CMakeLists.txt` — 1-7: projeto C++17; 8-19: flags globais duplicam LTO e quebram distribuição/CPUs sem extensões; 21-28: Qt/macro global; 30-45: includes globais e fontes; 47-59: shared library e liburing opcional, embora fonte use syscall diretamente; 61-69: instala toda a árvore de headers, inclusive detalhes Linux não portáveis.
- `Thunder Linux/CMakeLists.txt` — 1-69: mistura biblioteca local e `../ThunderSDK`, criando duas fontes de verdade; flags e Qt são globais; demos não são registrados como testes; caminhos relativos dependem do layout do checkout.

### Aplicação Qt e adblock (`ThunderSDK/src`)

- `app/thundercommon.h` — 1-54: macros de exportação/atributos; dependência Qt e macros GNU reduzem portabilidade. A cópia Linux diverge.
- `app/thundercommon.cpp` — 1-23: unidade mínima de símbolo/versão; sem problema funcional relevante.
- `app/thunder_ui_constants.h` — 1-30: constantes UI; cópia idêntica.
- `app/mainapplication.h` — 1-33; `mainapplication.cpp` — 1-45: wrapper QApplication e criação de janela; estado de fechamento simples, sem tratamento de falha de inicialização.
- `app/browserwindow.h` — 1-47; `browserwindow.cpp` — 1-210: configura QWebEngine/Chromium e UI; várias flags experimentais/driver-specific não são verificadas, interceptor praticamente não bloqueia nada, e a cópia Linux diverge.
- `adblock/adblockmanager.h` — 1-107: singleton/cache de regras; implementação relevante ausente no `.cpp`, portanto várias declarações não formam um gerenciador completo utilizável.
- `adblock/adblockrule.h` — 1-166: interface extensa com ponteiro cru `m_regExp` e dependência de tipos forward-declared; `adblockrule.cpp` — 1-53 implementa apenas construtor/destrutor e getters básicos. Métodos de parsing/matching declarados ficam sem implementação se chamados.
- `network/thunder_url_interceptor.h` — 1-52: calcula método/scan e descarta tudo; nunca bloqueia/redireciona uma request. Variável `method` não usada. `thunder_url_interceptor.cpp` — 1-18: unidade vazia; comentário sobre construtor/destrutor não corresponde a definição out-of-line.

### Kernel/CPU (`ThunderSDK/src/kernel`)

- `kernel_bridge.h` — 1-28: quatro APIs globais; `isPageAligned` fixa 4 KiB e aceita ponteiro mutável desnecessariamente. `.cpp` — 1-138: coberto em 3.2.
- `thunder_advanced_cpu.h` — 1-63: singleton; alinhamento é no-op; prefetch não garante saturação; AVX-512 apenas zera registrador e descarta resultado.
- `thunder_aes_accelerator.h` — 1-56: primitivas de rodada AES; CPUID inline assembly é frágil sob PIC/arquiteturas e redundante porque o binário já exige AES por CMake.
- `thunder_bios_sync.h` — 1-58: timer slack/TSC/RDRAND; não sincroniza BIOS/UEFI e fallback não é criptográfico.
- `thunder_branch_optimizer.h` — 1-15: somente header de compatibilidade, sem lógica.
- `thunder_cache_optimizer.h` — 1-37: alinhamento macro útil; `prioritizeCacheLocality` é no-op e comentário sobre `mlockall` não equivale a prioridade L3.
- `thunder_cache_pool.h` — 1-54: não mantém pool nem toca dados; aloca novo mmap e inicia thread que apenas dorme; ciclo de vida crítico descrito acima.
- `thunder_dma_optimizer.h` — 1-54: faltam include guard e includes autocontidos para `_IOW`, `uint64_t`, `ioctl`; ignora retorno.
- `thunder_dma_sync.h` — 1-29: parâmetro `fd` não usado; somente log.
- `thunder_gpu_warmer.h` — 1-43: includes não usados; ambiente/shell, sem warming real.
- `thunder_hex_utils.h` — 1-43: conversor nibble quebrado; parser aceita caracteres inválidos e overflow silenciosamente.
- `thunder_huge_tlb.h` — 1-78: alocador Linux com fallback razoável como protótipo, mas contrato/erros/desalocação incompletos.
- `thunder_io_matrix.h` — 1-61: criação incompleta de io_uring; fd pode vazar; não configura ring nem usa liburing.
- `thunder_ipc_warp.h` — 1-65: `memfd_create`/`mmap` básicos; não verifica `ftruncate`, não fecha fd/desmapeia e nome/endereço são logados.
- `thunder_jit_accelerator.h` — 1-83: alocador RWX e patch arbitrário; `initNativeJit` não demonstra compilação JIT.
- `thunder_lockless_matrix.h` — 1-54: ring SPSC fixo de 1024; nome promete matriz genérica, sem parametrização/capacidade e sem restrição explícita.
- `thunder_prefetch.h` — 1-44: wrapper de `_mm_prefetch`; parâmetro `locality` do template não participa do intrinsic e as especializações reais são limitadas.
- `thunder_protection_matrix.h` — 1-65: detecção parcial/simulada, potencial falso positivo e ausência de política clara.
- `thunder_rendering_warp.h` — 1-40: dois logs, nenhuma aceleração de layout/fonte.
- `thunder_security_warp.h` — 1-50: checagem de macro/CPUID e log; não "enforce CFI".
- `thunder_simd_accelerator.h` — 1-96: scan AVX2 é a implementação de performance mais substantiva; alinhamento inicial e cargas alinhadas são coerentes, mas prefetch pode apontar muito além do objeto (mesmo sem desreferência C++, há risco arquitetural), exige AVX2 e matrix multiply é apenas oito multiplicações elemento a elemento, não multiplicação matricial.
- `thunder_vdso_warp.h` — 1-47: introspecção apenas.
- `thunder_vram_cache.h` — 1-39: hints de ambiente, sem cache.

### Rede (`ThunderSDK/src/network`)

- `thunder_net_optimizer.h` — 1-50: opções socket sem retorno/contexto cliente-servidor; 52-89: parser little-endian, DELETE nunca tratado e tamanho 8 tem shift evitado corretamente; 91-112: detector inseguro e WEBP não implementado; 114-122: struct não faz parse de byte order; 124-145: SIMD com overread final.
- `thunder_advanced_network.h` — 1-55: opção TCP_REPAIR inadequada como filtro e pseudo-QPACK inseguro.
- `thunder_network_latency.h` — 1-44: três `setsockopt` ignorados; busy poll consome CPU e requer avaliação por workload.
- `thunder_url_warp.h` — 1-40: leituras fora de limites descritas em 3.1.
- `thunder_zero_copy.h` — 1-38: `vmsplice` com `SPLICE_F_GIFT` transfere contrato delicado de páginas; chamador precisa garantir alinhamento/vida útil, não documentados.

### Executável e exemplos (`Thunder Linux/src`)

- `main/main.cpp` — 1-60: suprime todas as mensagens Qt, inclusive erros críticos; executa otimizações duas vezes conceitualmente em relação ao inicializador disponível; força xcb/wayland e mascara diagnósticos.
- `examples/generic_app.cpp` — 1-45 e `no_thunder_app.cpp` — 1-27: cargas equivalentes, mas CMake/estado do processo tornam comparação não controlada; argumentos não usados.
- `examples/no_thunder_report.cpp` — 1-45 e `with_thunder_report.cpp` — 1-48: relatórios com telemetria presumida, não medida.
- `examples/thunder_ultimate_benchmark.cpp` — 1-102: benchmark inválido e ganhos fabricados, detalhado em 3.3.
- `examples/ultimate_stress_test.cpp` — 1-46: teste de scan sem validação do resultado, nomenclatura 512-bit incorreta e inicialização perigosa.
- `src/lib/**`: 25 arquivos são um snapshot antigo/parcial do SDK. Arquivos idênticos podem ser deduplicados; `browserwindow.cpp`, `thundercommon.h`, `kernel_bridge.cpp`, `thunder_hex_utils.h`, `thunder_huge_tlb.h`, `thunder_simd_accelerator.h` e `thundersdk.h` divergem. Muitos headers novos só existem no SDK. Manter ambas as árvores gera bugs de versão.

### Dark Volt e scripts

- `Dark Volt Kernel Technology/dark-volt-env.sh` — 1-21: path da linha 11 aponta para `/Dark Volt/`, mas o diretório real é `Dark Volt Kernel Technology`; usar `export` em `EnvironmentFile` systemd também não segue o formato esperado de simples `KEY=VALUE`.
- `kms-config.json` — 1-6: JSON válido, mas fixa `/dev/dri/card0` e opção `pb_size` precisa validação contra Qt alvo.
- `thunder-dark-volt.service` — 1-19: paths das linhas 9-10 não correspondem aos arquivos/binários existentes; ordenação `Before=sysinit.target` com dependência de local-fs/udev é suscetível a ciclo; FIFO 99/restart infinito podem prejudicar boot.
- `dark_volt_core.txt` — 1-24: documento conceitual; ganho de 20 s para 2 s não possui teste.
- `thunder_irq_shield.sh` — 1-21 e `thunder_unleash_kernel.sh` — 1-37: riscos globais e erros descritos em 3.2.

### Exportação, imagens, binários e gerados

- `Thunder_Final_Export/Docs/*`: cópias byte a byte dos documentos correspondentes na raiz, incluindo CSV, HTML e resumo. Não devem ser mantidas manualmente; gere o pacote em release.
- `Thunder_Final_Export/Binaries/ThunderBrowser`, `Thunder_Hardware_Stress`, `Thunder_Ultimate_Benchmark`, `libThunderSDK.so`: ELF compilados; não têm linhas-fonte auditáveis. Reprodutibilidade, símbolos, RPATH, hardening e assinatura devem ser verificados no pipeline. Versioná-los aumenta tamanho e risco de binário obsoleto.
- `Thunder_Final_Export/README_EXPORT.md` — 1-21: índice de pacote; nomes/claims herdam os problemas acima.
- `Image/*` (19 arquivos): raster JPEG/PNG/WebP, sem linhas semânticas. Há duplicação exata entre `Pink_Lightning.jpg` e `Pink_Lightning (1).jpg`; licenças/proveniência não estão documentadas. Nenhum `logo.png` existe na raiz apesar do README.
- `.idea/*`: configuração local do IntelliJ; `workspace.xml` e `caches/deviceStreaming.xml` são estado/caches de usuário e não devem ser versionados. O cache tem 2240 linhas geradas, sem lógica do produto.
- `ThunderSDK/build/**` e `Thunder Linux/build/**`: centenas de arquivos CMake, MOC, `.o`, `.d`, Makefiles e executáveis gerados. Foram analisados como artefatos e usados no build, não linha a linha como autoria. Devem ser ignorados pelo Git e recriados a partir do CMake.

## 5. Arquitetura real

O fluxo efetivo é: aplicação Qt chama inicializadores user-space; estes tentam syscalls/configuração Linux e definem ambiente; QWebEngine executa o navegador. Não há módulo kernel, driver, código BPF/XDP, Vulkan, shader, implementação de Skia, integração BIOS/UEFI, Ring-0/SMM ou controle direto de PCIe. AVX2, mmap/huge pages, afinidade, scheduler, `mlockall`, memfd e algumas opções de socket são recursos reais, mas sua disponibilidade e êxito precisam ser medidos e reportados honestamente.

## 6. Ordem recomendada de correção

1. Remover percentuais hard-coded e criar benchmark verificável (Google Benchmark/Celero), mesma otimização, mesmo buffer, repetição, pinning controlado, resultados consumidos e estatística.
2. Corrigir todos os overreads/UB e adicionar ASan/UBSan, testes de limites e fuzzing dos parsers.
3. Transformar inicialização em API que retorna relatório estruturado por capability, com erro/`errno`, estado anterior e rollback; nunca imprimir sucesso incondicional.
4. Remover RWX, shell `system()`, syscalls numéricas e mudanças globais; aplicar menor privilégio e configuração opt-in.
5. Unificar `ThunderSDK/src` como única fonte; fazer `Thunder Linux` apenas consumir o target CMake.
6. Separar capabilities reais de experimentos/placeholders; renomear alegações para o que o código efetivamente faz.
7. Criar CI limpa com GCC/Clang, Debug/Release, sanitizers e CPUs sem AVX2; usar dispatch runtime por CPUID.
8. Remover do Git builds, IDE caches e binários; gerar exportação reproduzível com hashes/SBOM.
9. Corrigir serviço/paths e fornecer instalação parametrizada, usuário dedicado, limites, watchdog e rollback.
10. Revisar documentação técnica e jurídica, anexando evidência reproduzível a cada claim.

## 7. Conclusão

Há componentes aproveitáveis — aplicação Qt funcional, scan AVX2, wrappers de memória/scheduler e estrutura CMake — mas o estado atual mistura protótipo, marketing e telemetria fictícia. Antes de distribuição ou execução privilegiada, os itens críticos devem ser corrigidos. A prioridade não é adicionar mais pilares: é tornar os poucos mecanismos reais seguros, testáveis, portáveis e mensuráveis.
