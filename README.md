# Projeto: Matriz de LEDs com Raspberry Pi Pico

## Descrição
Este projeto utiliza um Raspberry Pi Pico para controlar uma matriz 5x5 de LEDs WS2812, um LED RGB e dois botões. O sistema exibe números na matriz de LEDs, controla o LED RGB piscante e utiliza interrupções para capturar eventos dos botões.

## Video Projeto Funcionando

## Funcionalidades
- Exibição de números de 0 a 9 na matriz de LEDs
- Controle de um LED RGB
- Pisca um LED vermelho a 5Hz
- Utiliza interrupções para capturar eventos dos botões
- Implementa debounce para evitar leituras errôneas dos botões

## Componentes Utilizados
- Raspberry Pi Pico
- Matriz 5x5 de LEDs WS2812
- LED RGB (pinos individuais para R, G e B)
- Dois botões de entrada

## Conexões
| Componente | Pino no Raspberry Pi Pico |
|------------|-------------------------|
| Matriz WS2812 | GPIO 7 |
| LED Vermelho | GPIO 13 |
| LED Verde | GPIO 11 |
| LED Azul | GPIO 12 |
| Botão 1 | GPIO 5 |
| Botão 2 | GPIO 6 |

## Instalação e Execução
1. Clone este repositório:
   ```sh
   git clone https://github.com/SEU_USUARIO/NOME_DO_REPOSITORIO.git
   ```
2. Entre na pasta do projeto:
   ```sh
   cd interrupcao
   ```
3. Compile e envie o código para o Raspberry Pi Pico (usando o SDK do Pico).

## Estrutura do Código
- `main.c` - Arquivo principal do projeto, onde a lógica de controle da matriz e dos LEDs está implementada.
- `ws2812.pio` - Programa para o PIO que controla os LEDs WS2812.

## Como Usar
1. Pressione o **Botão 1** para incrementar o número exibido na matriz de LEDs.
2. Pressione o **Botão 2** para decrementar o número exibido na matriz de LEDs.
3. O LED RGB pisca a uma determinada frequência para indicar funcionamento.
4. O LED vermelho pisca 5 vezes por segundo.
