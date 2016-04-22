#include <Adafruit_Thermal.h>
#include <SoftwareSerial.h>
#include "qrencode.h"

#define TX_PIN 6 // Arduino envoi sur RX de l'imprimante (jaune)
#define RX_PIN 5 // Arduino réception sur TX de l'imprimante (vert)

// port série logiciel
SoftwareSerial mySerial(RX_PIN, TX_PIN);
// imprimante sur ce port
Adafruit_Thermal printer(&mySerial);

// fonction d'impression du QRcode avec une taille x4
void imprime() {
  // On imprime ligne par ligne 53 pixels fois 4
  // divisé par 8 pour obtenir des octets
  // 53*4/8 = 26,5 = 27 octets
  uint8_t ligne[27];

  // variable pour la position en octets
  int pix;

  // on boucle ligne par ligne
  for(int y=0; y<53; y++) {
    // réinitialisation compteur de pixels par ligne
    pix=0;
    // réinitialisation du tableau d'octets (image)
    memset(ligne,0,sizeof(ligne));

    // on boucle sur les pixels par saut de deux
    for(int x=0; x<53; x=x+2) {
      //Un octet de l'image est composé de 2 pixels originaux, répétés 4 fois
      // pixel x
      ligne[pix]= QRBIT(x,y) << 7;
      ligne[pix]|= QRBIT(x,y) << 6;
      ligne[pix]|= QRBIT(x,y) << 5;
      ligne[pix]|= QRBIT(x,y) << 4;
      // pixel suivant
      ligne[pix]|= QRBIT(x+1,y) << 3;
      ligne[pix]|= QRBIT(x+1,y) << 2;
      ligne[pix]|= QRBIT(x+1,y) << 1;
      ligne[pix]|= QRBIT(x+1,y);
      // octet suivant
      pix++;
    }
    // on imprime la ligne/image 4 fois
    printer.printBitmap(WD*4, 1, ligne, false);
    printer.printBitmap(WD*4, 1, ligne, false);
    printer.printBitmap(WD*4, 1, ligne, false);
    printer.printBitmap(WD*4, 1, ligne, false);
  }
}

void setup() {
  // moniteur série
  Serial.begin(115200);
  // softserial imprimante
  mySerial.begin(9600);
  // initialisation avec temps de chauffe à 200
  printer.begin(200);

  // petit message
  Serial.println(F("GO GO GO"));

  // réinitialisation imprimante
  printer.setDefault();
  // impression message
  printer.println(F("GO GO GO"));
  // défilement papier
  printer.feed(2);
}

void loop() {
  // objet pour la chaîne de caractère reçue
  String saisie;

  Serial.println("Texte svp ?");

  // On attend l'arrivée de la ligne
  while (Serial.available()==0);

  // lecture du texte
  saisie=Serial.readString();
  // copie dans le tampon pour le QRcode
  saisie.toCharArray((char *)strinbuf, 371);
  // encodage
  qrencode();

  // on imprime le texte
  printer.println(saisie);
  // un espace
  printer.feed(1);
  // impression du QRcode
  imprime();
  // un grand espace
  printer.feed(3);
}
