# E209 - Sistemas Microcontrolados e Microprocessados

[![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)](https://www.learn-c.org/)
[![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B&logoColor=white)](https://cplusplus.com/)
[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=flat&logo=Arduino&logoColor=white)](https://www.arduino.cc/)
[![AVR](https://img.shields.io/badge/AVR-Microcontroller-red)](https://www.microchip.com/en-us/products/microcontrollers-and-microprocessors/8-bit-mcus/avr-mcus)

Repositório acadêmico dedicado ao desenvolvimento de projetos e exercícios práticos em sistemas embarcados, microcontroladores e microprocessadores. Este repositório documenta a evolução do aprendizado desde conceitos básicos até implementações avançadas com microcontroladores AVR (ATmega328P).

## 🎯 Objetivo da Disciplina

Desenvolver competências em programação de sistemas embarcados, compreendendo desde algoritmos fundamentais até implementações práticas com microcontroladores, incluindo manipulação de registradores, controle de periféricos e desenvolvimento de projetos integrados.

## 📚 Estrutura do Repositório

### 📖 Aulas Teóricas e Práticas

#### Aula01 - Revisão de Algoritmos
**Base conceitual para sistemas embarcados**
- Revisão de estruturas de controle
- Algoritmos fundamentais
- Diagramas de fluxo para sistemas embarcados

**Arquivos:**
- `aula_micro.drawio` - Diagramas conceituais

---

#### Aula02 - Máquinas de Estado
**Implementação de sistemas baseados em estados**
- Modelagem de máquinas de estado finito
- Aplicações em sistemas embarcados
- Documentação em formato texto e diagrama

**Arquivos:**
- `maquina_de_estado.drawio` - Diagramas de estado
- `maquina_de_estado.txt` - Especificações textuais

---

#### Aula03 - Controle de GPIO Básico
**Introdução ao controle de portas GPIO**
- Manipulação básica de pinos digitais
- Controle de LEDs e sensores
- Programação sem uso de máscaras de bits

**Arquivos:**
- `gpio_sem_mascara.ino` - Código Arduino básico
- `relatorio03_maquina.drawio` - Diagramas explicativos

---

#### Aula04 - GPIO Avançado com Máscaras
**Controle otimizado de GPIO usando manipulação de registradores**
- Uso de máscaras de bits para controle preciso
- Análise do datasheet do ATmega328P
- Programação em baixo nível

**Arquivos:**
- `gpio_com_mascara.ino` - Implementação com máscaras
- `Datasheet_ATmega328P.pdf` - Documentação técnica
- `Diagrama.drawio` - Esquemas de conexão
- Laboratórios práticos (`E209_lab04`, `E209_aula05`)

---

#### Aula05 - Integração de Sistemas
**Desenvolvimento de sistemas mais complexos**
- Integração de múltiplos periféricos
- Otimização de código
- Relatórios técnicos

**Arquivos:**
- `Relatorio05_codigo.ino` - Código documentado

---

#### Aula06 - Controle e Monitoramento
**Sistemas de controle em tempo real**
- Implementação de controladores
- Monitoramento de sensores
- Comunicação com módulos externos

**Arquivos:**
- `Exercicio01_CM.ino` - Exercício de controle e monitoramento

---

#### Aula07 - Projetos Integrados
**Desenvolvimento de soluções completas**
- Integração de conceitos aprendidos
- Resolução de problemas práticos
- Documentação técnica avançada

**Arquivos:**
- `diagramas.drawio` - Diagramas de sistema
- `questao01.ino`, `questao02.ino` - Soluções práticas

---

### 🚀 Projeto Final
**Sistema Embarcado Completo**

Desenvolvimento de um projeto integrado demonstrando a aplicação prática de todos os conceitos estudados.

**Componentes:**
- `code_test.ino` - Código de teste e validação
- `teste_finalizado.c` - Implementação final em C
- `prototipo.jpeg` - Foto do protótipo físico
- `tinkercad.jpeg` - Simulação no Tinkercad

**Características do Projeto:**
- Sistema completo com múltiplos sensores
- Interface de usuário
- Controle em tempo real
- Documentação técnica completa

### 📊 Relatórios e Exercícios Especializados

#### Exemplos Práticos
- **Exemplo_UART** - Comunicação serial
- **Exercicio_PWM** - Modulação por largura de pulso
- **Maquina_de_Estado** - Implementações avançadas de FSM
- **ProjetoLab** - Projetos de laboratório

#### Projetos Temáticos
- **Projeto_bomba** - Sistema de controle de bomba
- **Projeto_cronometro** - Cronômetro digital com display
- **Relatorio01** - Fundamentos teóricos
- **Relatorio02** - Análises práticas

## 🛠️ Tecnologias e Ferramentas

### Hardware
- **ATmega328P** - Microcontrolador principal
- **Arduino Uno** - Plataforma de desenvolvimento
- **Sensores diversos** - Temperatura, umidade, movimento
- **Atuadores** - LEDs, motores, displays
- **Componentes passivos** - Resistores, capacitores

### Software
- **Arduino IDE** - Ambiente de desenvolvimento principal
- **C/C++** - Linguagens de programação
- **Tinkercad** - Simulação de circuitos
- **Draw.io** - Criação de diagramas
- **AVR-GCC** - Compilador para microcontroladores AVR

### Ferramentas de Documentação
- **Drawio** - Diagramas técnicos
- **PDF** - Documentação e datasheets
- **Simuladores** - Tinkercad, Proteus

## 🔧 Configuração do Ambiente

### Pré-requisitos
```bash
# Arduino IDE (versão 1.8.x ou superior)
# Drivers para Arduino Uno
# Bibliotecas necessárias (especificadas em cada projeto)
```

### Instalação
1. **Arduino IDE**
   ```
   Download: https://www.arduino.cc/en/software
   Instalar drivers USB para Arduino
   ```

2. **Configuração de Placas**
   ```
   Tools > Board > Arduino Uno
   Tools > Port > (selecionar porta correta)
   ```

3. **Bibliotecas Adicionais**
   ```
   Sketch > Include Library > Manage Libraries
   Instalar bibliotecas conforme necessidade do projeto
   ```

## 🔍 Projetos Destacados

### Sistema de Controle de Bomba
- Monitoramento de nível de água
- Controle automático de acionamento
- Interface com LEDs indicadores
- Proteção contra funcionamento a seco

### Cronômetro Digital
- Display de 7 segmentos
- Controle por botões
- Precisão de milissegundos
- Funções de start/stop/reset

### Sistema UART
- Comunicação bidirecional
- Protocolo personalizado
- Controle remoto via serial
- Log de eventos
