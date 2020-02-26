/*  --------------------------------------------------------------------------------
 *  Projeto:      Teste RTC DS3231
 *  Autor:        André Aquino
 *  Inicio:       24/05/2019
 *  Atualização:  
  ----------------------------------------------------------------------------------*/

#include <user_interface.h>
#include "ds3231.h"

char* charAux200 = (char *) malloc(sizeof(char) * 200);

// Veriáveis RTC 
dadosRtc relogio;
ds3231Class rtc;
os_timer_t tmr0;

/*******************************************************************
 * SETUP
 ******************************************************************/
void setup() {

  // Inicia Serial ------------------------------------------------
  Serial.begin(115200);
  Serial.println("TESTE SERIAL!");

  // RTC ----------------------------------------------------------
  rtc.definePino(9, 13);
  rtc.habilitaClock();
  relogio = rtc.le();
  sprintf(charAux200, "%u:%u:%u - %u/%u/%u - %u", relogio.hora, relogio.minuto, relogio.segundo, relogio.dia, relogio.mes, relogio.ano, relogio.diaSemana);
  Serial.println(charAux200);
  Serial.print("TEMPERATURA: ");
  Serial.println(rtc.leTemperatura());
  Serial.println();

  // TESTE ATUALIZAÇÃO ---------------------------------------------
  relogio.hora = 8;
  relogio.minuto = 30;
  relogio.segundo = 0;
  relogio.dia = 26;
  relogio.mes = 2;
  relogio.ano = 20;
  relogio.diaSemana = 4;
  rtc.atualiza(relogio);
  
  // INICIA TIMER --------------------------------------------------
  os_timer_setfn(&tmr0, timer0, NULL); //Indica ao Timer qual sera sua Sub rotina.
  os_timer_arm(&tmr0, 1000, true);
}

/*******************************************************************
 * TIMER
 ******************************************************************/
void timer0(void*z){
 
  // Atualiza relogio interno
  relogio.segundo++;
  if(relogio.segundo >= 60){
    relogio.segundo = 0;
    relogio.minuto++;
    if(relogio.minuto >= 60){
      relogio.minuto = 0;
      relogio.hora++;
      if(relogio.hora >= 24){
        relogio.hora = 0;
        sprintf(charAux200, "%u:%u:%u - %u/%u/%u - %u", relogio.hora, relogio.minuto, relogio.segundo, relogio.dia, relogio.mes, relogio.ano, relogio.diaSemana);
        Serial.println(charAux200);
        relogio = rtc.le();
      } // end if hora
    } // end if minuto
  } // end if segundo
  
} // end timer0()

void loop() {
  // Imprimi dados relogio
  sprintf(charAux200, "%u:%u:%u - %u/%u/%u - %u", relogio.hora, relogio.minuto, relogio.segundo, relogio.dia, relogio.mes, relogio.ano, relogio.diaSemana);
  Serial.println(charAux200);
  delay(1000);
}
