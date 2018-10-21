/*
  Send
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

int prefixBits = 4;

int buttonBits = 3;
int buttonPin[3] = {2, 3, 6};
int buttonShift = prefixBits;
int buttonAdress = 0;
unsigned long buttonMask[3];
unsigned long buttonVal[3];
unsigned long sendButtonData;

int joystick1Xbits = 10;
int joystick1Ybits = 10;
int joystick1XPin = A0;
int joystick1YPin = A1;
int joystick1XShift = prefixBits;
int joystick1YShift = joystick1XShift + joystick1Xbits;
int joystick1Adress = 1;
unsigned long joystick1XMask;
unsigned long joystick1YMask;
unsigned long joystick1Xval = 0UL;
unsigned long joystick1Yval = 0UL;
unsigned long sendJoystick1Data = 0UL;



int joystick2Xbits = 10;
int joystick2Ybits = 10;
int joystick2XPin = A3;
int joystick2YPin = A4;
int joystick2XShift = prefixBits;
int joystick2YShift = joystick2XShift + joystick2Xbits;
int joystick2Adress = 2;
unsigned long joystick2XMask;
unsigned long joystick2YMask;
unsigned long joystick2Xval;
unsigned long joystick2Yval;
unsigned long sendJoystick2Data;



void setup() {
  Serial.begin(250000);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  for (int i=0; i < buttonBits; i++)
  {
    pinMode(buttonPin[i], INPUT);    // declare pushbutton as input
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

// the loop function runs over and over again forever
void loop() 
{
  sendButtonData = 0UL;
  sendJoystick1Data = 0UL;
  sendJoystick2Data = 0UL;
  for (int i=0; i < buttonBits; i++)
  {
    if (digitalRead(buttonPin[i]) == HIGH)
    {
      buttonVal[i] = buttonMask[i];
    }
    else
    {
      buttonVal[i] = 0UL;
    }
    sendButtonData = sendButtonData | buttonVal[i];
  }
  joystick1Xval = ((unsigned long)analogRead(joystick1XPin) << joystick1XShift);
  joystick1Yval = ((unsigned long)analogRead(joystick1YPin) << joystick1YShift);
  unsigned long test1 = 1UL << 31UL;  
  sendButtonData = sendButtonData | buttonAdress | test1;
  sendJoystick1Data = sendJoystick1Data | joystick1Xval | joystick1Yval | joystick1Adress | test1;
  sendJoystick2Data = sendJoystick2Data | joystick2Xval | joystick2Yval | joystick2Adress | test1;
  radio.write(&sendButtonData, sizeof(sendButtonData));
  radio.write(&sendJoystick1Data, sizeof(sendJoystick1Data));
  radio.write(&sendJoystick2Data, sizeof(sendJoystick2Data));
  Serial.println(sendButtonData, BIN);
  Serial.println(sendJoystick1Data, BIN);
  Serial.println(sendJoystick2Data, BIN);
}

