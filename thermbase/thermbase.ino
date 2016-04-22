#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"

// définition des broches
#define TX_PIN 6 // jaune
#define RX_PIN 5 // vert

// port série logiciel
SoftwareSerial mySerial(RX_PIN, TX_PIN);
// imprimante
Adafruit_Thermal printer(&mySerial);

void setup() {
  // Initialisation port série
  mySerial.begin(9600);
  // Initialisation imprimante
  printer.begin();
  // configuration par défaut
  printer.setDefault();

  // défilement une ligne
  printer.feed(1);

  printer.setBarcodeHeight(200);
  printer.printBarcode("5014908002665", EAN13);
  printer.feed(4);
}

void loop() {
  // vide, on ne fait rien en boucle
}
