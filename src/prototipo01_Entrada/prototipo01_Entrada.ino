/* PROTOTIPO (ENTRADA) 
Link: https://wokwi.com/arduino/projects/308268012530565698 */

#include <Adafruit_SSD1306.h>
#include <qrcode.h>
#include <Servo.h>
#include <stdlib.h>
#include <string.h>
#include "RTClib.h"  

const int pinoPIR = 13;    // Pino do sensor PIR

const int pinoServo = 3;   // Pino do motor Servo
Servo cancela;             // Objeto da classe Servo
int posicao_cancela = 0;   // Posição Servo (posiçao "cancela abaixada" )
 
#define SCREEN_WIDTH 128   // largura do display OLED, em pixels
#define SCREEN_HEIGHT 64   // altura do display OLED, em pixels
// Declaração do display SSD1306 
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS1307 rtc;          // Objeto da classe RTC_DS1307
//const char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};
int dia, mes, ano, hora, minuto;
char dia01[3], mes01[3], ano01[3], hora01[3], minuto01[3]; 

const char* MESSAGE_DA_ENTRADA[4] = { ">Ticket<", "Tire com", "o App", "aqui:" };
const char* INF_TICKET_BASE = {"Ticket\nStatus: Pendente\nEntrada: "};
char* INF_TICKET;
QRCode qrcode; // A estrutura para gerenciar o código QR


void setup() {
  Serial.begin(9600);
  pinMode(pinoPIR, INPUT); // Pino de Entrada
  cancela.attach(pinoServo);
  cancela.write(posicao_cancela);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("Falha na alocação de SSD1306"));
    for(;;); // Don't proceed, loop forever
  }

  if (! rtc.begin()) {
    Serial.println("Não foi possível encontrar RTC");
    Serial.flush();
    abort();
  }

  display.clearDisplay();
  display.display();
}

void loop() {
   
 if(digitalRead(pinoPIR) == HIGH){ // SE A LEITURA DO pinoPIR FOR IGUAL A HIGH -> UM CARRO FOI DETECTADO! 
    posicao_cancela = 90; // posiçao "cancela aberta/levantada "
    cancela.write(posicao_cancela);
    
    // Obtendo a Data e Hora Atual:
    DateTime dhAtual = rtc.now();
    dia = dhAtual.day();
    mes = dhAtual.month();
    ano = dhAtual.year();
    hora = dhAtual.hour();
    minuto = dhAtual.minute();

    // char * itoa (valor int, char * str, base int); converte int p/ char
    itoa(dia, dia01, 10); 
    itoa(mes, mes01, 10); 
    itoa(ano, ano01, 10); 
    itoa(hora, hora01, 10); 
    itoa(minuto, minuto01, 10);

    // Serial.print(diasDaSemana[dhAtual.dayOfTheWeek()]);

    // Concatenando e formando a mensagem que será convertida em Qrcode:
    char* INF_TICKET = concat(INF_TICKET_BASE, dia01);
    INF_TICKET = concat(INF_TICKET, "/");
    INF_TICKET = concat(INF_TICKET, mes01);
    INF_TICKET = concat(INF_TICKET, "/");
    INF_TICKET = concat(INF_TICKET, ano01);
    INF_TICKET = concat(INF_TICKET, " as ");
    INF_TICKET = concat(INF_TICKET, hora01);
    INF_TICKET = concat(INF_TICKET, "h");
    INF_TICKET = concat(INF_TICKET, minuto01);




    // o qr code com os dados do Ticket deve ser exibido no display
    display.clearDisplay();
    mostrarQrCode(INF_TICKET, MESSAGE_DA_ENTRADA);
    delay(6000);  // aumente para ter mais tempo para ler o Qrcode no display
    // "Ticket\nEntrada: /08/21 as 09:25\nStatus: Pendente"

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

// Função para concatenar :
char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
