/**
 *  QRCode
 *
 * Um exemplo rápido de geração de um código QR.
 *
 * Isso imprime o código QR no monitor serial como blocos sólidos. Cada módulo
 * tem dois caracteres de largura, uma vez que a fonte monoespaçada usada no monitor serial
 * tem aproximadamente o dobro da altura.
 * 
 *  Link: https://wokwi.com/arduino/projects/306273169020289601
 */

#include "qrcode.h"

void ticket (){


}

void setup() {
    Serial.begin(115200);

    //Hora de inicio
    uint32_t dt = millis();
  
    // Criando o QRcode
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(3)];
    qrcode_initText(&qrcode, qrcodeData, 3, 0, "Teste01\nMercedes\nTrab Circuitos");
  
    // Tempo delta
    dt = millis() - dt;
    Serial.print("QR Code Generation Time: ");
    Serial.print(dt);
    Serial.print("\n");

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

void loop() {

}
