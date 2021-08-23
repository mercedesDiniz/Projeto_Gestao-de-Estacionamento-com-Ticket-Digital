/* Essa não é a versão final!!!
Tudo aqui pode ser modificado para melhores resultados.
 ~ Mercedes Diniz
*/

#include <Adafruit_SSD1306.h>
#include <qrcode.h>
#include <Servo.h>

const int pinoPIR = 13;    // Pino do sensor PIR
const int pinoServo = 3;   // Pino do motor Servo
Servo cancela;             // Objeto da classe Servo
int posicao_cancela = 0;   // Posição Servo (posiçao "cancela abaixada" )

#define SCREEN_WIDTH 128   // largura do display OLED, em pixels
#define SCREEN_HEIGHT 64   // altura do display OLED, em pixels

// Declaração do display SSD1306 
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* MESSAGE_DA_ENTRADA[4] = { ">Ticket<", "Tire com", "o App", "aqui:" };
QRCode qrcode; // A estrutura para gerenciar o código QR

void setup() {
  Serial.begin(9600);
  pinMode(pinoPIR, INPUT); // Pino de Entrada
  cancela.attach(pinoServo);
  cancela.write(posicao_cancela);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display();
}

void loop() {
 if(digitalRead(pinoPIR) == HIGH){ // SE A LEITURA DO pinoPIR FOR IGUAL A HIGH -> UM CARRO FOI DETECTADO! 
    posicao_cancela = 90; // posiçao "cancela aberta/levantada "
    cancela.write(posicao_cancela);

    // o qr code com os dados do Ticket deve ser exibido no display
    display.clearDisplay();
    mostrarQrCode("Ticket\nEntrada: 01/08/21 as 09:25\nStatus: Pendente", MESSAGE_DA_ENTRADA);
    delay(6000);  // aumente para ter mais tempo para ler o Qrcode no display

 }else{  // SE A LEITURA DO pinoPIR FOR IGUAL A LOW -> NENHUM CARRO FOI DETECTADO! 
    posicao_cancela = 0; // posiçao "cancela abaixada"
    cancela.write(posicao_cancela);
    display.clearDisplay(); // display apagado (ou similar) 
    display.display(); 
 }


}

// FUNÇAO (Ps.: descobrir como essa merda funciona - matriz )
void mostrarQrCode(const char* qrStr, const char* lines[]) {
	uint8_t qrcodeData[qrcode_getBufferSize(3)];
	qrcode_initText(&qrcode, qrcodeData, 3, ECC_LOW, qrStr);
 
  // Ponto de partida do texto
  int cursor_start_y = 10;
  int cursor_start_x = 4;
  int font_height = 12;

  // Ponto de partida do código QR
  int offset_x = 62;
  int offset_y = 3;

  for (int y = 0; y < qrcode.size; y++) {
      for (int x = 0; x < qrcode.size; x++) {
          int newX = offset_x + (x * 2);
          int newY = offset_y + (y * 2);

          if (qrcode_getModule(&qrcode, x, y)) {
							display.fillRect( newX, newY, 2, 2, 0);
          }
          else {
							display.fillRect( newX, newY, 2, 2, 1);
          }
      }
  }
  display.setTextColor(1,0);
  for (int i = 0; i < 4; i++) {
    display.setCursor(cursor_start_x,cursor_start_y+font_height*i);
    display.println(lines[i]);
  }
  display.display();
}