/* Uso do modulo RTC DS1307 para obter a data e hora atuais, e passando os dados como parametro para criar um QRcode e exibi-lo no Monitor Serial. 
Link: https://wokwi.com/arduino/projects/308481727134696003 */

#include <Adafruit_SSD1306.h>
#include <qrcode.h>
#include <Servo.h>
#include <Wire.h> 
#include <stdlib.h>
#include <string.h>
#include "RTClib.h"  


RTC_DS1307 rtc;            // Objeto da classe RTC_DS1307
//const char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};
int dia, mes, ano, hora, minuto;
char dia01[3], mes01[3], ano01[3], hora01[3], minuto01[3]; 

const char* INF_TICKET_BASE = {"Ticket\nStatus: Pendente\nEntrada: "};
QRCode qrcode; // A estrutura para gerenciar o código QR


void setup() {
  Serial.begin(9600);
  //Serial.begin(115200);

  if (! rtc.begin()) {
    Serial.println("Não foi possível encontrar RTC");
    Serial.flush();
    abort();
  }

}

void loop() {
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

  // Concatenando:
  char* INF_TICKET = concat(INF_TICKET_BASE, dia01);
  INF_TICKET = concat(INF_TICKET, "/");
  INF_TICKET = concat(INF_TICKET, mes01);
  INF_TICKET = concat(INF_TICKET, "/");
  INF_TICKET = concat(INF_TICKET, ano01);
  INF_TICKET = concat(INF_TICKET, " as ");
  INF_TICKET = concat(INF_TICKET, hora01);
  INF_TICKET = concat(INF_TICKET, "h");
  INF_TICKET = concat(INF_TICKET, minuto01);

  // Criar o Qrcode 
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0,INF_TICKET);
  
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
   delay(6000);  // aumente para ter mais tempo para ler o Qrcode no display

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


