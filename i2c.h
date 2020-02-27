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
 
class i2cClass{

  private:
  
  #define DELAY_I2C   1
  
  byte SCL = 9;
  byte SDA = 13;

  public:

  void definePin(byte pinoSCL, byte pinoSDA){
    SCL = pinoSCL;
    SDA = pinoSDA;
  }
  
  void start(void){
  
      pinMode(SCL, OUTPUT);
      pinMode(SDA, OUTPUT);
  
      // IDLE codition
      digitalWrite(SCL, HIGH);
      digitalWrite(SDA, HIGH);
      delay(DELAY_I2C);
      
      // START
      digitalWrite(SDA, LOW);
     
  }
  
  void stop(void){
  
      pinMode(SDA, OUTPUT);
  
      digitalWrite(SCL, HIGH);
      delay(DELAY_I2C);
      digitalWrite(SDA, LOW);
      delay(DELAY_I2C);
      digitalWrite(SDA, HIGH);
      
  }
  
  void sendAck(void){
  
      pinMode(SDA, OUTPUT);
      
      delay(DELAY_I2C);
      digitalWrite(SDA, LOW);
      delay(DELAY_I2C);
      digitalWrite(SCL, HIGH);
      delay(DELAY_I2C);
      digitalWrite(SCL, LOW);
  }
  
  void sendNack(void){
  
      pinMode(SDA, OUTPUT);    
  
      delay(DELAY_I2C);
      digitalWrite(SDA, HIGH);
      delay(DELAY_I2C);
      digitalWrite(SCL, HIGH);
      delay(DELAY_I2C);
      digitalWrite(SCL, LOW);
  }

  unsigned char write(unsigned char dado){
  
      unsigned char i, ack;
      
      pinMode(SDA, OUTPUT);
      
      // Envia dados
      for(i=0 ; i<8 ; i++){
          digitalWrite(SCL, LOW);
          delay(DELAY_I2C);
          digitalWrite(SDA, ( (char) (dado >> 7-i) & 0x01));
          delay(DELAY_I2C);
          digitalWrite(SCL, HIGH);
          delay(DELAY_I2C);
      }
  
      // Receve ACK/NACK
      digitalWrite(SCL, LOW);
      pinMode(SDA, INPUT);
      delay(DELAY_I2C);
      digitalWrite(SCL, HIGH);
      delay(DELAY_I2C);
      ack = digitalRead(SDA);
      digitalWrite(SCL, LOW);
      
      return ack;
  
  }
  
  unsigned char read(void){
  
      unsigned char dado;
      int i;
      
      pinMode(SDA, INPUT);
      
      // Envia dados
      dado=0;
      for(i=0 ; i<8 ; i++){
          digitalWrite(SCL, LOW);
          delay(DELAY_I2C);
          digitalWrite(SCL, HIGH);
          delay(DELAY_I2C);
          dado = dado | digitalRead(SDA);
          if(i<7)
              dado = dado << 1;
      }
      digitalWrite(SCL, LOW);
      
      return dado;
  }
};
