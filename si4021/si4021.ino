#include <SPI.h>
#include <RCSwitch.h>

#define CS 10
#define FSK 8

// Memo SPI Uno : 11-MOSI, 12-MISO, 13-SCK

RCSwitch mySwitch = RCSwitch();

void setup() {
  // configuration ports en sortie
  pinMode(CS, OUTPUT);
  pinMode(FSK, OUTPUT);

  // CS à l'état haut
  // le module n'est PAS "sélectionné" et
  // ne traite donc PAS les éventuelles données SPI
  digitalWrite(CS, HIGH);
  
  // Configuration SPI
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);

  // On utilise la sortie FSK (broche 8)
  mySwitch.enableTransmit(FSK);

  // Option : durée de l'impulsion
  //  mySwitch.setPulseLength(320);
  // Option : protocole (1 fonctionne la plupart du temps)
  mySwitch.setProtocol(1);
  // Option : nombre de répétition du message
  mySwitch.setRepeatTransmit(10);
  
  delay(100);

  // Configuration :
  // bande de fréquence sur 433 Mhz
  // sortie horloge à 10 MHz
  digitalWrite(CS, LOW);
  // 100 b1 b0 d2 d1 d0
  // b1b0 = 01 = 433 Mhz band
  // d2d1d0 = 111 = 10 Mhz clock out freq
  SPI.transfer(B10001111); // 0x8F
  // x3 x2 x1 x0 ms m2 m1 m0
  // x3x2x1x0 = 0000 = crystal load capa 8,5 pf
  // ms = 1 et M = 000  sortie fréquence +30Khz
  SPI.transfer(B00001000); // 0x08
  digitalWrite(CS, HIGH);

  // configuration de la fréquence sur 433.92 MHz
  digitalWrite(CS, LOW);
  // 1010 f11...f0 -> F = 96 to 3903 
  // freq = 10 Mhz * C1 * (C2 + F/4000)
  // pour 433 Mhz, C1=1 C2=43
  // 11000100000 = 1568
  // 10*1*(43+1568/4000) = 433.92 Mhz
  SPI.transfer(B10100110); // 0xA6
  SPI.transfer(B00100000); // 0x20
  // 10*1*(43+1594/4000) = 433.985 Mhz
  // 11000111010 = 1594
//  SPI.transfer(B10100110); // 1594 = 433.985 Mhz
//  SPI.transfer(B00111010); //
  digitalWrite(CS, HIGH);

/*
  //data rate command
  //data rate set to 4800bps
  digitalWrite(CS, LOW);
  // 11001000
  SPI.transfer(B11001000); // 0xC8
  // r7..r0
  // rate = 10 Mhz / 29 / (R+1)
  // R = 1000111 = 71
  // 10/29/(71+1) = 0.004789 Mhz = 4789 Hz
  SPI.transfer(B01000111); // 0x47
  digitalWrite(CS, HIGH);

  //low battery detector command
  //note we set the ebs bit to enable TX synchronization
  digitalWrite(CS, LOW);
  // 11000010
  SPI.transfer(B11000010); // 0xC2
  // dwc 0 ebs t4..t0
  // ebs = 1 = Enables the tx Bit Synchronization
  SPI.transfer(B00100000); // 0x20
  digitalWrite(CS, HIGH);

  //power management command
  //set to default: 0xC000
  digitalWrite(CS, LOW);
  // 11000000
  SPI.transfer(B11000000); // 0xC0
  // a1 a0 ex es ea eb et dc
  SPI.transfer(B00000000);
  digitalWrite(CS, HIGH);
*/

  // Configuration de l'emission
  digitalWrite(CS, LOW);
  // 1011 ook p2 p1 p0
  // ook on -> FSK pin -> transmit
  // (il fallait virer la resistance 0 ohm sur le circuit)
  SPI.transfer(B10111000); // OOK + power 
  digitalWrite(CS, HIGH);

  // Configuration de l'alimentation
  digitalWrite(CS, LOW);
  // 11000000
  SPI.transfer(B11000000); // 0xC0;
  // a1 a0 ex es ea eb et dc
  // ex = 1 = crystal oscillator enabled
  // es = 1 = synthesizer enabled
  // ea = 1 = power amplifier enabled
  // dc = 0 = enable clock output buffer
  SPI.transfer(B00111000); // 0x38
  digitalWrite(CS, HIGH);

}

void loop() {
    mySwitch.send("000101010101000101010101"); //  chan 1 bouton 3 ON
    delay(1000);
    mySwitch.send("000101010101000101010100"); //  chan 1 bouton 3 OFF
    delay(2000);

    /*
    mySwitch.send("000101010001010101010101"); //  chan 1 bouton 1 ON
    mySwitch.send("000101010001010101010100"); //  chan 1 bouton 1 OFF
    mySwitch.send("000101010100010101010101"); //  chan 1 bouton 2 ON
    mySwitch.send("000101010100010101010100"); //  chan 1 bouton 2 OFF
    mySwitch.send("000101010101000101010101"); //  chan 1 bouton 3 ON
    mySwitch.send("000101010101000101010100"); //  chan 1 bouton 3 OFF
    mySwitch.send("000101010101010001010101"); //  chan 1 bouton 4 ON
    mySwitch.send("000101010101010001010100"); //  chan 1 bouton 4 ON

    mySwitch.send("010001010001010101010101"); //  chan 2 bouton 1 ON
    mySwitch.send("010001010001010101010100"); //  chan 2 bouton 1 OFF
    mySwitch.send("010001010100010101010101"); //  chan 2 bouton 2 ON
    mySwitch.send("010001010100010101010100"); //  chan 2 bouton 2 OFF
    mySwitch.send("010001010101000101010101"); //  chan 2 bouton 3 ON
    mySwitch.send("010001010101000101010100"); //  chan 2 bouton 3 OFF
    mySwitch.send("010001010101010001010101"); //  chan 2 bouton 4 ON
    mySwitch.send("010001010101010001010100"); //  chan 2 bouton 4 OFF
    
    mySwitch.send("010100010001010101010101"); //  chan 3 bouton 1 ON
    mySwitch.send("010100010001010101010100"); //  chan 3 bouton 1 OFF
    mySwitch.send("010100010100010101010101"); //  chan 3 bouton 2 ON
    mySwitch.send("010100010100010101010100"); //  chan 3 bouton 2 OFF
    mySwitch.send("010100010101000101010101"); //  chan 3 bouton 3 ON
    mySwitch.send("010100010101000101010100"); //  chan 3 bouton 3 OFF
    mySwitch.send("010100010101010001010101"); //  chan 3 bouton 4 ON
    mySwitch.send("010100010101010001010100"); //  chan 3 bouton 4 OFF

    mySwitch.send("010101000001010101010101"); //  chan 4 bouton 1 ON
    mySwitch.send("010101000001010101010100"); //  chan 4 bouton 1 OFF
    mySwitch.send("010101000100010101010101"); //  chan 4 bouton 2 ON
    mySwitch.send("010101000100010101010100"); //  chan 4 bouton 2 OFF
    mySwitch.send("010101000101000101010101"); //  chan 4 bouton 3 ON
    mySwitch.send("010101000101000101010100"); //  chan 4 bouton 3 OFF
    mySwitch.send("010101000101010001010101"); //  chan 4 bouton 4 ON
    mySwitch.send("010101000101010001010100"); //  chan 4 bouton 4 OFF
    */
    
    /*
    digitalWrite(FSK, HIGH);
    delay(100);    
    digitalWrite(FSK, LOW);
    delay(1000);
    */

/*
    mySerial.println("Hello World Hello World Hello World Hello World ");
    digitalWrite(FSK, LOW);
    delay(1000);
*/

    /*
    digitalWrite(CS, LOW);
    SPI.transfer(B11000000); // 0xC0;
    SPI.transfer(B00111000); // 0x38
    digitalWrite(CS, HIGH);
    
//    mySerial.println("Hello World");
    delay(1000);

    digitalWrite(CS, LOW);
    SPI.transfer(B11000000); // 0xC0
    SPI.transfer(B00000000);
    digitalWrite(CS, HIGH);
    delay(1000);
    */
}
