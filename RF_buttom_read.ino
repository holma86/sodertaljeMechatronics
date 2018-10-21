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
int indexMask =15;

int joystick1Xbits = 10;
int joystick1Ybits = 10;
int joystick1XPin = A0;
int joystick1YPin = A1;
int joystick1XShift = prefixBits;
int joystick1YShift = joystick1XShift + joystick1Xbits;
int joystick1Adress = 1;
unsigned long joystick1XMask;
unsigned long joystick1YMask;
int joystick1Xval = 0UL;
int joystick1Yval = 0UL;
int joystick2Xval = 0UL;
int joystick2Yval = 0UL;
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
//unsigned long joystick2Xval;
//unsigned long joystick2Yval;
unsigned long sendJoystick2Data;
    unsigned long int buttoms = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

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
void loop() {
  if (radio.available()) {
  buttoms = 0;
    radio.read(&buttoms, sizeof(buttoms));
 //   Serial.println(buttoms, BIN);
   // unmask(buttoms);
     int index= indexMask & buttoms;
  //       Serial.println(index, BIN);
  if (index==0){
  Serial.println("Knappar");
  }
  else if(index==1){
 //   Serial.println("jeystick1");
     unsigned long temp = joystick1XMask & buttoms;
      joystick1Xval = (temp >> joystick1XShift);
               temp = joystick1YMask & buttoms;
      joystick1Yval = (temp >> joystick1YShift);
  Serial.print("1 x = ");
  Serial.print(joystick1Xval);
  Serial.print("\t 1 y = ");
  Serial.println(joystick1Yval);
  }
    else if(index==2){
 //   Serial.println("jeystick2");
      unsigned long int temp = joystick2XMask & buttoms;
      joystick2Xval = (temp >> joystick2XShift);
                        temp = joystick2YMask & buttoms;
      joystick2Yval = (temp >> joystick2YShift);
        Serial.print("2 x = ");
  Serial.print(joystick2Xval);
  Serial.print("\t 2 y = ");
  Serial.println(joystick2Yval);
  }
  }
}

