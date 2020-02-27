/*  --------------------------------------------------------------------------------
 *  Projeto:      Teste RTC DS3231
 *  Autor:        André Aquino
 *  Inicio:       24/05/2019
 *  Atualização:  
  ----------------------------------------------------------------------------------*/

#include "ds3231.h"

#define LED 7

char* charAux200 = (char *) malloc(sizeof(char) * 200);

// Veriáveis RTC 
dadosRtc relogio;
ds3231Class rtc;

/*******************************************************************
 * SETUP
 ******************************************************************/
void setup() {

  // Inicia Serial ------------------------------------------------
  Serial.begin(115200);
  Serial.println("TESTE SERIAL!");

  // RTC ----------------------------------------------------------
  rtc.definePino(9, 13);

  // Configura LED
  pinMode(LED, OUTPUT);

  rtc.habilitaClock();
  relogio = rtc.le();
  sprintf(charAux200, "%u:%u:%u - %u/%u/%u - %u", relogio.hora, relogio.minuto, relogio.segundo, relogio.dia, relogio.mes, relogio.ano, relogio.diaSemana);
  Serial.println(charAux200);
  Serial.print("TEMPERATURA: ");
  Serial.println(rtc.leTemperatura());
  Serial.println();

  relogio.hora = 8;
  relogio.minuto = 30;
  relogio.segundo = 0;
  relogio.dia = 26;
  relogio.mes = 2;
  relogio.ano = 20;
  relogio.diaSemana = 4;
  rtc.atualiza(relogio);
  
}

void loop() {
  delay(400);
  digitalWrite(LED, HIGH);
  delay(400);
  digitalWrite(LED, LOW);
  relogio = rtc.le();
  sprintf(charAux200, "%u:%u:%u - %u/%u/%u - %u", relogio.hora, relogio.minuto, relogio.segundo, relogio.dia, relogio.mes, relogio.ano, relogio.diaSemana);
  Serial.println(charAux200);

}
