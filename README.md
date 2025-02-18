<samp>
<h1 align="center"> Conversor A/D (ADC) :desktop_computer: </h1>
  
<h2 align="center"> Sobre :pencil: </h2>

<p align="justify"> &emsp;&emsp;&emsp;&emsp; Repositório com o objetivo de consolidar os conceitos sobre o uso de conversores A/D no RP2040 e explorar as funcionalidades da placa de desenvolvimento BitDogLab, como parte de uma tarefa desenvolvida na Unidade 4 do Capítulo 8 da EmbarcaTech, residência de Sistemas Embarcados. Essa tarefa foi desenvolvida pela residente Isabela Madureira Argolo.

<h3> Descrição da tarefa </h3>

<p align="justify"> &emsp;&emsp;&emsp;&emsp; A tarefa consiste em utilizar os seguintes componentes:

  1. LED RGB, com os pinos conectados às GPIOs 11, 12 e 13;
  2. Botão do Joystick conectado à GPIO 22;
  3. Joystick conectado aos GPIOs 26 e 27;
  4. Botão A conectado à GPIO 5;
  5. Display SSD1306 conectado via I2C (GPIO 14 e GPIO15).

 &emsp;&emsp;&emsp;&emsp; O programa deve possuir as seguintes funcionalidades:

<p align="justify"> * O joystick fornecerá valores analógicos correspondentes aos eixos X e Y, que serão utilizados para:

<p align="justify">  1. <b> Controlar a intensidade luminosa dos LEDs RGB: </b> 
<p align="justify"> &emsp;&emsp;&emsp;&emsp; • O LED Azul terá seu brilho ajustado conforme o valor do eixo Y. Quando o joystick estiver solto (posição central - valor 2048), o LED permanecerá apagado. À medida que o joystick for movido para cima (valores menores) ou para baixo (valores maiores), o LED aumentará seu brilho gradualmente, atingindo a intensidade máxima nos extremos (0 e 4095); </p>
<p align="justify"> &emsp;&emsp;&emsp;&emsp; • O LED Vermelho seguirá o mesmo princípio, mas de acordo com o eixo X. Quando o joystick estiver solto (posição central - valor 2048), o LED estará apagado. Movendo o joystick para a esquerda
(valores menores) ou para a direita (valores maiores), o LED aumentará de brilho, sendo mais intenso nos extremos (0 e 4095); </p>
<p align="justify"> &emsp;&emsp;&emsp;&emsp; • Os LEDs serão controlados via PWM para permitir variação suave da intensidade luminosa. </p>

<p align="justify">  2. Exibir no display SSD1306 um quadrado de 8x8 pixels, inicialmente centralizado, que se moverá proporcionalmente aos valores capturados pelo joystick; </p>
<p align="justify">  3. <b> Botão do Joystick: </b> alternar o estado do LED verde a cada acionamento e modificar a borda do display para indicar quando foi pressionado, alternando entre diferentes estilos de borda a cada novo acionamento; </p>
<p align="justify">  4. <b> Botão A: </b> Ativar ou desativar os LED PWM a cada acionamento. </p>

<h3> Estruturação dos Arquivos </h3>

<p align="justify"> &emsp;&emsp;&emsp;&emsp; O projeto é composto pelos arquivos comuns a um projeto Pico W, além de uma pasta chamada 'include', que possui 4 arquivos na extensão .c, que são: button.c (que organiza as funções relacionadas aos botões), joystick.c (que organiza as funções relacionadas ao joystick), led-rgb.c (que organiza as funções relacionadas ao LED RGB) e sdd1306.c (que organiza as funções relacionadas ao display), 4 arquivos na extensão .h, que são: button.h, joystick.h, led-rgb.h e ssd1306.h (em que ambos possuem as importações e o cabeçalho das funções), e os arquivos conversorAD-embarcatech.c (que gerencia todo o projeto) e READ.ME (descrição do projeto).

<h3></h3>
<h2 align="center"> Vídeo :video_camera: </h2>
<p align="justify"> &emsp;&emsp;&emsp;&emsp; Aqui está o link para o vídeo com uma prática de simulação associada a esta tarefa: </p>
<p align="center"> https://youtube.com/shorts/p0UosE7wxV4?feature=share </p>

<h2></h2>
<h2 align="center"> Como utilizar :computer_mouse: </h2>

<p align="justify"> &emsp;&emsp;&emsp;&emsp; Para utilizar o programa, o usuário deve:

- Clonar o repositório em uma pasta separada para recebê-lo;
- Abrir o projeto no VS Code, importando-o pela extensão Raspberry Pi Pico;
- Conectar a placa BitDogLab em modo BOOTSEL;
- Compilar e executar o projeto;
- Testar as funcionalidades descritas acima.

</samp>
