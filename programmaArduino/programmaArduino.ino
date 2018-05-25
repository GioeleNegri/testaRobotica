#include <Servo.h>
/**
 * @author: Negri Gioele, Wolf Riccardo
 * Descrizione: Testa robot che segue il movimento dell'oggetto che si trova davanti
 */
 
//PORTE
const int portaMotore = 11;       //porta del motore
const int echoPortRight = 10;     //echo occhio destro
const int triggerPortRight = 9;   //trigger occhio destro
const int echoPortLeft = 8;       //echo occhio sinistro
const int triggerPortLeft = 7;    //trigger occhio sinistro
const int echoPortC = 6;          //echo occhio centrale
const int triggerPortC = 5;       //trigger occhio centrale
const int portarossa = 4;         //pin rosso led RGB
const int portaverde = 3;         //pin verde led RGB
const int portablu = 2;           //pin blu led RGB

//VARIABILI
Servo motore;             //dichiarazione del motore
int posizioneMotore = 90; //variabile per tenere in memoria la posizione del motore
int numErrore = 0;
long durata = 0;          //variabile per memorizzare la durata dell'impulso inviato dal sensore di distanza
long distanza = 0;        //variabile per memorizzare la distanza ottenuta dal sensore

//SETTTAGGI
const long distanzaMax = 30;  //distanza oltre il quale i sensori non leggono
const int tempoCiclo = 500;  //tempo di ciclo del programma in ms
const int giraDestra = -5;   //angolo incremento movimento motore
const int giraSinistra = +5; //angolo incremento movimento motore

//SETUP DEI SENSORI
void setup() {
  //sensore distanza destra
  pinMode(triggerPortRight, OUTPUT);
  pinMode(echoPortRight, INPUT);
  
  //sensore distanza sinistra
  pinMode(triggerPortLeft, OUTPUT);
  pinMode(echoPortLeft, INPUT);
  
  //sensore distanza centrale
  pinMode(triggerPortC, OUTPUT);
  pinMode(echoPortC, INPUT);
  
  //led rgb
  pinMode(portarossa, OUTPUT);
  pinMode(portaverde, OUTPUT);
  pinMode(portablu, OUTPUT);
  
  //monitor seriale
  Serial.begin(9600);
  
  //motore
  motore.attach(portaMotore);
  motore.write(posizioneMotore); //posizionamento del motore a 90 gradi
}

//PROGRAMMA
void loop() {
  String s = String(posizioneMotore);
  s = s + ";";
  Serial.print(s);
    long oDX = occhioDestro(); 
    long oSX = occhioSinistro();
    long oC = occhioCentro();
    //Serial.println("|-------------------------|");
    if(oC < distanzaMax && oDX > distanzaMax && oSX > distanzaMax){
      //Serial.println("|Testa ferma");
      ledRGB('v');
    }else{
      if(oDX < distanzaMax){
          movimentoVersoDx();
        }else if(oSX < distanzaMax){
            movimentoVersoSx();
          }else{
              //Serial.println("|Errore");
              ledRGB('r');
            }
    }
    //Serial.println("|------------------------|");  
    delay(tempoCiclo);
}

//FUNZIONI
/**
  * funzione per leggere la distanza rilevata dall'occhio centro
 */
long occhioCentro(){
  digitalWrite( triggerPortC, LOW );
  digitalWrite( triggerPortC, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( triggerPortC, LOW );
  durata = pulseIn( echoPortC, HIGH );
  distanza = 0.034 * durata / 2;
  //Serial.print("|Distanza Centro: ");
  if( durata > 38000 ){
    //Serial.println("|Fuori portata Centro   ");
    return -1;
  }
  else{ 
    //Serial.print(distanza); 
    //Serial.println(" cm     ");
    return distanza;
  }
}
/**
  * funzione per leggere la distanza rilevata dall'occhio destro
 */
long occhioDestro(){
  digitalWrite( triggerPortRight, LOW );
  digitalWrite( triggerPortRight, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( triggerPortRight, LOW );
  durata = pulseIn( echoPortRight, HIGH );
  distanza = 0.034 * durata / 2;
  //Serial.print("|Distanza Right: ");
  
  if( durata > 38000 ){
    //Serial.println("|Fuori portata Right   ");
    return -1;
  }
  else{ 
    //Serial.print(distanza); 
    //Serial.println(" cm     ");
    return distanza;
  }
}
/**
  * funzione per leggere la distanza rilevata dall'occhio sinistro
 */
long occhioSinistro(){
  digitalWrite( triggerPortLeft, LOW );
  digitalWrite( triggerPortLeft, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( triggerPortLeft, LOW );
  durata = pulseIn( echoPortLeft, HIGH );
  distanza = 0.034 * durata / 2;
  //Serial.print("|Distanza Left: ");
  
  if( durata > 38000 ){
    //Serial.println("|Fuori portata Left   ");
    return -1;
  }
  else{ 
    //Serial.print(distanza); 
    //Serial.println(" cm     ");
    return distanza;
  }
}
/**
  * funzione per muovere la testa verso destra
 */
void movimentoVersoDx(){
  posizionamentoMotore(giraDestra);//rotazione del motore
  motore.write(posizioneMotore);
  ledRGB('b');
}
/**
  * funzione per muovere la testa verso sinistra
 */
void movimentoVersoSx(){
  posizionamentoMotore(giraSinistra);//rotazione del motore
  motore.write(posizioneMotore);
  ledRGB('b');
}
/**
 * funzione per colorare il led
 * v = verde
 * r = rosso
 * b = blu
 */
void ledRGB(char col){
  switch(col){
    case 'v':
      colore(244,0 , 161);
      break;
    case 'b':
      colore(255, 0,0);
      break;
    case 'r':
      colore(0,0,255);
      break;
    }
}
/**
 * funzione per scrivere colore
 */
void colore (unsigned char rosso, unsigned char verde, unsigned char blu){
 analogWrite(portarossa, rosso); //attiva il led rosso con l’intensita’ definita nella variabile rosso
 analogWrite(portablu, blu); //attiva il led blu con l’intensita’ definita nella variabile blu
 analogWrite(portaverde, verde); //attiva il led verde con l’intensita’ definita nella variabile verde
}
/**
 * metodo per cambiare la variabile del posizionamento del motore
 */
boolean posizionamentoMotore(int spostamento){
    if(posizioneMotore >= 0 && posizioneMotore <= 180){
          posizioneMotore = posizioneMotore + spostamento;
          //Serial.print("|Posizione motore:");         
          //Serial.println(posizioneMotore);  
    }else{
      //Serial.println("|ERRORE: non posso girare più di cosi");  
      if(posizioneMotore < 0){
        posizioneMotore = 0;
        String s = "0";
        s = s + ";";
        Serial.print(s);  
      }
      if(posizioneMotore > 180){
        posizioneMotore = 180;  
        String s = "180";
        s = s + ";";
        Serial.print(s);
      }
    }
}
