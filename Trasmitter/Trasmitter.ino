/*
  Send
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

/* Number of bits for the parameter adress */
int prefixBits = 4;

/* Button setup */
int buttonBits = 4;
int buttonPin[4] = {2, 3, 4, 5};
int buttonShift = prefixBits;
int buttonAdress = 0;
int buttonVal[4];
unsigned long buttonMask[4];
unsigned long sendButtonData;

/* Joystick1 setup */
int joystick1Xval;
int joystick1Yval;
int joystick1Xbits = 10;
int joystick1Ybits = 10;
int joystick1XPin = A0;
int joystick1YPin = A1;
int joystick1XShift = prefixBits;
int joystick1YShift = joystick1XShift + joystick1Xbits;
int joystick1Adress = 1;
int joystick1ButtonPin = buttonPin[0];
unsigned long joystick1XMask;
unsigned long joystick1YMask;
unsigned long sendJoystick1Data;

/* Joystick2 setup */
int joystick2Xval;
int joystick2Yval;
int joystick2Xbits = 10;
int joystick2Ybits = 10;
int joystick2XPin = A2;
int joystick2YPin = A3;
int joystick2XShift = prefixBits;
int joystick2YShift = joystick2XShift + joystick2Xbits;
int joystick2Adress = 2;
int joystick2ButtonPin = buttonPin[1];
unsigned long joystick2XMask;
unsigned long joystick2YMask;
unsigned long sendJoystick2Data;

void setup() {
  Serial.begin(500000); 
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  for (int i=0; i < buttonBits; i++)
  {
    pinMode(buttonPin[i], INPUT);
    buttonMask[i] = 1UL << (i + buttonShift);
  }
  joystick1XMask = 0UL;
  for (int i=0; i < joystick1Xbits; i++)
  {
    joystick1XMask = (joystick1XMask | (1UL << (i + joystick1XShift)));
  }
  joystick1YMask = 0UL;
  for (int i=0; i < joystick1Ybits; i++)
  {
    joystick1YMask = (joystick1YMask | (1UL << (i + joystick1YShift)));
  }
  joystick2XMask = 0UL;
  for (int i=0; i < joystick2Xbits; i++)
  {
    joystick2XMask = (joystick2XMask | (1UL << (i + joystick2XShift)));
  }
  joystick2YMask = 0UL;
  for (int i=0; i < joystick2Ybits; i++)
  {
    joystick2YMask = (joystick2YMask | (1UL << (i + joystick2YShift)));
  }
}

/* Main function */
void loop() 
{
  sendButtonData = 0UL;
  sendJoystick1Data = 0UL;
  sendJoystick2Data = 0UL;
  for (int i=0; i < buttonBits; i++)
  {
    
    if (digitalRead(buttonPin[i]) == HIGH)
    {
      buttonVal[i] = 1;
    }
    else
    {
      buttonVal[i] = 0;
    }
    if  ((buttonPin[i] == joystick1ButtonPin) ||
         (buttonPin[i] == joystick2ButtonPin))
    {
      buttonVal[i] ^= 1; //inverts button logic for joystick
    }
    sendButtonData |= (buttonMask[i]*buttonVal[i]);
  }
  joystick1Xval = analogRead(joystick1XPin);
  joystick1Yval = analogRead(joystick1YPin);
  joystick2Xval = analogRead(joystick2XPin);
  joystick2Yval = analogRead(joystick2YPin);
  
  unsigned long test1 = 1UL << 31;
  sendButtonData = sendButtonData | buttonAdress | test1;
  sendJoystick1Data = (((unsigned long)joystick1Xval << joystick1XShift) | 
                       ((unsigned long)joystick1Yval << joystick1YShift) | 
                        joystick1Adress | test1);
  sendJoystick2Data = (((unsigned long)joystick2Xval << joystick2XShift) | 
                       ((unsigned long)joystick2Yval << joystick2YShift) | 
                        joystick2Adress | test1);
  radio.write(&sendButtonData, sizeof(sendButtonData));
  radio.write(&sendJoystick1Data, sizeof(sendJoystick1Data));
  radio.write(&sendJoystick2Data, sizeof(sendJoystick2Data));
  Serial.println(sendButtonData, BIN);
  Serial.println(sendJoystick1Data, BIN);
  Serial.println(sendJoystick2Data, BIN);
}
