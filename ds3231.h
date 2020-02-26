/*
 Copyright (c) 2020 André Aquino

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 Disponível em: https://github.com/andraq/ds3231
 
 */
 
#include "i2c.h"

struct dadosRtc{
    unsigned char segundo, minuto, hora, dia, mes, ano, diaSemana;
};

class ds3231Class{

private:

  i2cClass i2c;

  unsigned char converteHexDec(unsigned char dado){
      
      unsigned char dezena, unidade;
      
      dezena = (dado & 0xF0) >> 4;    // dezena
      unidade = (dado & 0x0F);        // unidade
      dado = (dezena*10) + unidade;   // valor convertido
      
      return dado;
  }

  unsigned char converteDecHex(unsigned char dado){
      
      unsigned char dezena, unidade;
      
      dezena = dado/10;                   // dezena
      unidade = dado - (dezena*10);       // unidade
      dado = ((unsigned char) (dezena << 4)) | unidade;     // valor convertido
      
      return dado;
  }

  public:

  void definePino(byte pinoSCL, byte pinoSDA){
    i2c.definePin(pinoSCL, pinoSDA);
  }

  struct dadosRtc le(void){
    
    struct dadosRtc rtc;

    i2c.start();                                 // Start
    i2c.write(0xD0);                             // Slave address + write
    i2c.write(0x00);                             // Posição Inicial
    i2c.start();                                 // ReStart
    i2c.write(0xD1);                             // Slave address + read
    rtc.segundo = converteHexDec(i2c.read());    // Addr 0
    i2c.sendAck();                               // Ack
    rtc.minuto = converteHexDec(i2c.read());     // Addr 1
    i2c.sendAck();                               // Ack
    rtc.hora = converteHexDec(i2c.read());       // Addr 2
    i2c.sendAck();                               // Ack
    rtc.diaSemana = converteHexDec(i2c.read());  // Addr 3
    i2c.sendAck();                               // Ack
    rtc.dia = converteHexDec(i2c.read());        // Addr 4
    i2c.sendAck();                               // Ack
    rtc.mes = converteHexDec(i2c.read());        // Addr 5
    i2c.sendAck();                               // Ack
    rtc.ano = converteHexDec(i2c.read());        // Addr 6
    i2c.sendNack();
    i2c.stop();

    return rtc;
  }

  void atualiza(struct dadosRtc rtc){
    
    i2c.start();                               // Start
    i2c.write(0xD0);                           // Slave address + Write
    i2c.write(0x00);                           // Word address
    i2c.write(converteDecHex(rtc.segundo));    // Addr 0
    i2c.write(converteDecHex(rtc.minuto));     // Addr 1
    i2c.write(converteDecHex(rtc.hora));       // Addr 2
    i2c.write(converteDecHex(rtc.diaSemana));  // Addr 3
    i2c.write(converteDecHex(rtc.dia));        // Addr 4
    i2c.write(converteDecHex(rtc.mes));        // Addr 5
    i2c.write(converteDecHex(rtc.ano));        // Addr 6
    i2c.stop();                                // Stop
  }

  float leTemperatura(void){

    float temperatura;
    unsigned char parteDecimal;
    
    i2c.start();                                 // Start
    i2c.write(0xD0);                             // Slave address + write
    i2c.write(0x11);                             // Posição Inicial
    i2c.start();                                 // ReStart
    i2c.write(0xD1);                             // Slave address + read
    temperatura = i2c.read();                    // MSB of temp
    i2c.sendAck();                               // Ack
    parteDecimal = i2c.read();                   // LSB of temp
    i2c.sendNack();
    i2c.stop();

    parteDecimal = parteDecimal >> 6;
    temperatura = temperatura + (float) (parteDecimal * 0.25);

    return temperatura;
  }

  void habilitaClock(void){

    unsigned char valorEOSC;
    
    // Lê o valor atual
    i2c.start();                                 // Start
    i2c.write(0xD0);                             // Slave address + write
    i2c.write(0x11);                             // Posição Inicial
    i2c.start();                                 // ReStart
    i2c.write(0xD1);                             // Slave address + read
    valorEOSC = i2c.read();                      // Recebe valor
    i2c.sendNack();
    i2c.stop();

    // Modifica o bit de controle do clock mantendo os demais bits inalterados
    valorEOSC = valorEOSC & 0x01111111b;

    // escreve o novo valor
    i2c.start();                                 // Start
    i2c.write(0xD0);                             // Slave address + write
    i2c.write(0x0E);                             // Word Address
    i2c.write(valorEOSC);                             
    i2c.stop();

  }

};
