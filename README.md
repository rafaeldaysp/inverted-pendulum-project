# Manual do Projeto - Pêndulo Invertido

## Descrição do Projeto
Este projeto consiste na modelagem, construção e controle de um pêndulo invertido utilizando um ESP32, um motor DC e encoders para medição de posição e ângulo. O sistema é controlado por um algoritmo embarcado no ESP32, que processa os sinais dos sensores e ajusta o motor para estabilizar o pêndulo.

## Hardware Utilizado
- **ESP32** - Microcontrolador responsável pelo processamento e controle.
- **Motor DC 775** - Fornece força para movimentar o carrinho.
- **Driver BTS7960** - Controla o motor DC via PWM.
- **Encoders rotacionais** - Capturam posição do carrinho e ângulo do pêndulo.
- **Fonte de Alimentação 12V** - Fornece energia para o motor e circuitos.
- **Guia Linear** - Permite movimento do carrinho.
- **Estrutura em MDF** - Base para montagem do sistema.

## Design Geral do Sistema
![Esquema elétrico](./images/esquema-eletrico.jpg)

O diagrama acima mostra a conexão dos componentes eletrônicos, incluindo alimentação, sensores e atuadores.

## Explicação Eletrônica do Sistema

O sistema eletrônico do pêndulo invertido é baseado no microcontrolador **ESP32**, que gerencia os sensores e atuadores do sistema. A seguir, são descritas as conexões principais do hardware.  

### **1. Conexão dos Encoders**
Os encoders incrementais são responsáveis pela medição da posição do carrinho e do ângulo do pêndulo. Eles são conectados ao ESP32 da seguinte maneira:

| **Componente** | **Sinal** | **Porta do ESP32** |
|--------------|----------|----------------|
| **Encoder do Pêndulo** | CLK (A) | GPIO 26 |
|  | DT (B) | GPIO 25 |
| **Encoder do Carrinho** | CLK (A) | GPIO 14 |
|  | DT (B) | GPIO 27 |

- O ESP32 lê os pulsos gerados pelos encoders para calcular deslocamento e ângulo.  
- As portas **GPIO 34 e 35** são entradas analógicas apenas, enquanto as **GPIO 32 e 33** suportam interrupções para contagem precisa dos pulsos.  

### **2. Controle do Motor DC (Driver BTS7960)**
O motor DC é controlado pelo **driver BTS7960**, que recebe sinais do ESP32 para definir direção e intensidade da força. As conexões são:

| **Sinal** | **Porta do ESP32** |
|----------|----------------|
| PWM Motor (esquerda) | GPIO 32 |
| PWM Motor (direita) | GPIO 12 |
| Habilitação PWM | GPIO 33 |

- O ESP32 gera sinais **PWM** para controlar a velocidade e direção do motor.  
- O **driver BTS7960** usa esses sinais para fornecer potência ao motor DC **775**.  
- A alimentação do motor vem de uma **fonte de 12V**, conectada ao driver.  

## Algoritmo de Controle
![Fluxograma do Algoritmo](./images/logica-fluxograma.jpg)

O algoritmo embarcado segue a lógica acima:
1. **Obter dados dos encoders** - Mede a posição do carrinho e o ângulo do pêndulo.
2. **Verificar se está na região de controle** - Decide se o controle deve ser aplicado.
3. **Calcular o sinal de controle** - Computa a ação necessária para estabilizar o pêndulo.
4. **Enviar o sinal ao motor** - Controla o motor via PWM.
5. **Repetir continuamente** - Atualiza as medições e atua no motor.

## Inicialização do Pêndulo
Antes de iniciar o controle, siga os seguintes passos:
1. Posicione o carrinho no centro da trilha.
2. Certifique-se de que o pêndulo está totalmente para baixo e em repouso.
3. Conecte o ESP32 ao computador via USB.
4. Monitore as variáveis no terminal da porta serial.
5. Pressione o botão **RST** para resetar as variáveis antes de iniciar o controle.

## Monitoramento em Tempo Real
O código implementado no ESP32 contém a função `display`, que permite visualizar as variáveis do sistema no terminal serial. Para isso:
1. Abra um monitor serial no baud rate adequado.
2. Verifique os valores de posição e ângulo do pêndulo em tempo real.
3. Utilize os dados para ajustes e validações do controle.

## Estrutura dos Arquivos do Repositório
O repositório contém os seguintes arquivos principais:
- **`main.ino`** - Código embarcado do ESP32, contendo toda a lógica de controle.
- **`simulation.slx`** - Arquivo do Simulink usado para simulação do sistema.
- **`lqr_design.m`** - Script MATLAB para projeto do controlador LQR.

---
Este manual foi criado para facilitar a utilização e entendimento do projeto. Boa sorte e bons experimentos! 🚀

