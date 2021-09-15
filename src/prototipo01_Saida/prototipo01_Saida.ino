/* PROTOTIPO (SAIDA) 
Link: 

# REFERENCIAS:
- Bibliotecas:
    1. Para gerar Qrcode: qrcode (Autor: Richard Moore)
    2. Para o Display: Adafruit_SSD1306 (Autor: Adafruit)
    3. Para o Módulo RTC ds1307: RTClib (Autor: Adafruit) 

- Modelos Base:
    1. Dynamic QRcode on ssd1306-big-sram.ino (https://wokwi.com/arduino/projects/301360621680591373)
    2. ds1307-basic.ino (https://wokwi.com/arduino/projects/305979285237137984)
*/

#include <Adafruit_SSD1306.h>
#include <qrcode.h>
#include <Servo.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "RTClib.h"  

const int pinoPIR = 13;    // Pino do sensor PIR

const int pinoServo = 3;   // Pino do motor Servo
Servo cancela;             // Objeto da classe Servo
int posicao_cancela = 0;   // Posição Servo (posiçao "cancela abaixada" )

RTC_DS1307 rtc;          // Objeto da classe RTC_DS1307
int horaS, minutoS;
char hora02[3], minuto02[3];

const char MESSAGE_DA_SAIDA = "Apresente esse QrCode (Ticket) na Saida.";
const char* INF_TICKET_BASE = {"Valor pago: 15\nPermanecia: 25 min\n"};
char* INF_TICKET;
QRCode qrcode; // A estrutura para gerenciar o código QR


void setup() {
  Serial.begin(9600);
  pinMode(pinoPIR, INPUT); // Pino de Entrada
  cancela.attach(pinoServo);
  cancela.write(posicao_cancela);

  if (! rtc.begin()) {
    Serial.println("Não foi possível encontrar RTC");
    Serial.flush();
    abort();
  }

}

void loop() {
   
 if(digitalRead(pinoPIR) == HIGH){ // SE A LEITURA DO pinoPIR FOR IGUAL A HIGH -> UM CARRO FOI DETECTADO! 
    posicao_cancela = 0; // posiçao "cancela aberta/levantada "
    cancela.write(posicao_cancela);
    
    // Obtendo a Data e Hora Atual:
    DateTime dhAtual = rtc.now();
    horaS = dhAtual.hour();
    minutoS = dhAtual.minute();

    // char * itoa (valor int, char * str, base int); (converte int p/ char)

    itoa(horaS, hora02, 10); 
    itoa(minutoS, minuto02, 10);
    
    //Serial.print(diasDaSemana[dhAtual.dayOfTheWeek()]);
    //Serial.println(mes);
    //Serial.print(mes01);

    // Concatenando e formando a mensagem que será convertida em Qrcode:
    char* INF_TICKET = concat(INF_TICKET_BASE, hora02);
    INF_TICKET = concat(INF_TICKET, ":");
    INF_TICKET = concat(INF_TICKET, minuto02);

    // o Qrcode com os dados do Ticket deve ser exibido no Monitor serial (que seria como a tela do celular do usuario)
    Serial.println("QRcode apresentado pelo usuário na saída");
    mostrarQrCode(INF_TICKET);
    delay(1000);  // aumente para ter mais tempo para mostrar o Qrcode no Monitor
    

 }else{  // SE A LEITURA DO pinoPIR FOR IGUAL A LOW -> NENHUM CARRO FOI DETECTADO! 
    posicao_cancela = 90; // posiçao "cancela levantada"
    cancela.write(posicao_cancela);
    delay(2000);
    
 }

}

// Função para exibir o Qr code + mesagem na tela no Monitor Serial:
void mostrarQrCode(const char* Qrcode_mensagem) {
  // Criar o Qrcode 
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0,Qrcode_mensagem);
  
  // Exibir no Monitor Serial: der pause!!!
   // Zona calma superior 
    Serial.print("\n\n\n\n");

    for (uint8_t y = 0; y < qrcode.size; y++) {

        // Saiu da zona sossegada
        Serial.print("        ");

        // Cada módulo horizontal
        for (uint8_t x = 0; x < qrcode.size; x++) {

            // Imprima cada módulo (UTF-8 \ u2588 é um bloco sólido)
            Serial.print(qrcode_getModule(&qrcode, x, y) ? "\u2588\u2588": "  ");

        }

        Serial.print("\n");
    }

  // Zona tranquila inferior 
  Serial.print("\n\n\n\n");
}

// Função para concatenar :
char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 para o terminador nulo
    // em código real, você verificaria erros no malloc aqui
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

