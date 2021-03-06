// LEDolas
#include <TimerOne.h>
#include "ledlib.h"
#define latchPin  10                          //Latch (Pin 12)
#define clockPin  11                          //Clock serie (Pin 11)
#define dataPin_anodo  8                      // DS (Pin 14) para anodos
#define INTERVALO 2000                          //microsegundos entre interrupciones
#define INTERVALO2 1000
//Variables Interrupcion
unsigned long previous_millis = 0;
//Variables Registros
byte byte_anodo;              // Almaceno los datos de los anodos
byte byte_catodo;             // Almaceno capa a encender
int nbytes;                   // Cuantos bytes se necesitan para enviar los anodos y los catodos
// Variables CUBO :
int capa = 0;
boolean cubo[512] ;           //Array que alamacena el valor de los anodos
byte *BytestoSend;            // Puntero
// Puntero Sinonima de la anterior --> byte BytetoSend[];
unsigned animetimer = 0;
bool alterna = true;
int i = 0;
int j = 0;
int k = 0;
//Cabeceras de las funciones necesarias para el funcionamiento del cubo
int bytesNecesarios( ) ;
byte BooltoByte( boolean boolArray[ ], int celda);
void IntroducirCatodo(byte BytesArray[], int capa);
void MostrarByteArray(byte BytesArray[] );
void BuildByteArray(byte BytestoSend[], boolean boolArray[ ], int capa);
void mostrarCubo(boolean cubo[ ] );

void planosRandom(bool boolArray[]) {
  bool alterna = true;
  if (alterna) {
    fullCube(boolArray);
    alterna = false;
  }
  
 
  planoZ(int(random(0, 8)), bool(random(0,1)), boolArray);
  planoY(int(random(0, 8)), bool(random(0,1)), boolArray);
  planoX(int(random(0, 8)), bool(random(0,1)), boolArray);
 
}
void voxelLluvia(bool boolArray[ ])
  {
  voxelWrite(int(random(0, 8)), int(random(0, 8)), int(random(0, 8)), true, cubo);
  voxelWrite(int(random(0, 8)), int(random(0, 8)), int(random(0, 8)), true, cubo);
  voxelWrite(int(random(0, 8)), int(random(0, 8)), int(random(0, 8)), true, cubo);
  voxelWrite(int(random(0, 8)), int(random(0, 8)), int(random(0, 8)), true, cubo);
  voxelWrite(int(random(0, 8)), int(random(0, 8)), int(random(0, 8)), true, cubo);
  voxelWrite(int(random(0, 8)), int(random(0, 8)), int(random(0, 8)), true, cubo);
  voxelWrite(int(random(0, 8)), int(random(0, 8)), int(random(0, 8)), true, cubo);
  voxelWrite(int(random(0, 8)), int(random(0, 8)), int(random(0, 8)), true, cubo);
  voxelWrite(int(random(0, 8)), int(random(0, 8)), int(random(0, 8)), true, cubo);
  voxelWrite(int(random(0, 8)), int(random(0, 8)), int(random(0, 8)), true, cubo);
  }

void setup() {
  n = 8;                      //Tamaño del cubo
  n_capas = n;
  n_ledcapa = n * n;
  n_anodos = n * n * n;
  clearCube(cubo);
  nbytes = bytesNecesarios( );    // Calculo cuantos bytes necesito segun el tamaño del cubo
  Serial.begin(9600);
  //Serial.print("bytes necesarios: "); Serial.println(nbytes);
  BytestoSend = new byte[ nbytes];// Determino el tamaño que tiene el array despues de calcularlo
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin_anodo, OUTPUT);
  //Temporizacoin animaciones
  previous_millis = millis();
  //Config Timer
  Timer1.initialize(INTERVALO);
  Timer1.attachInterrupt(Lanzacapas);  // attaches callback() as a timer overflow interrupt/
 
}
// Estructura Temporal para las visuales
int timerV1,timerV2,timerV3,timerV4;

// Puertas para visuales
bool loading = true;
bool v1= true;
bool v2= true;
bool v3= true;
bool v4= true;
bool v5= true;
bool v6= true;
bool v7= true;
bool v8= true;
bool estadoper = true;

//////
void visual1(bool boolArray[]){
    timerV1 = 0;
    while(timerV1 <= 100) {
      //perCuboPares(boolArray);
        timerV1++; 
  }
  v1 = false;
  v2 =true;
}
void visual2(bool boolArray[]){
    timerV2 = 0;
    while(timerV2 <= 100 ) {
      //diagonalLine(boolArray);
      fullCube(boolArray);
        timerV2++; 
  }
  v2 = false;
  v1 = true;
}

void loop() {
  if (animetimer >= INTERVALO2 && animetimer <= INTERVALO2*3  ) {
    //Init FullCube Fade Out
    if(v1){
        clearCube(cubo);
        fullCube(cubo);
        v1 =false;
      }
    //Animaicion
    voxelWrite(int (random(0,8)),int (random(0,8)),int (random(0,8)),false, cubo );
   }
   if (animetimer >= INTERVALO2*3 && animetimer <= INTERVALO2*5  ) {
    //Init FullCube Fade IN
    if(v2){
        clearCube(cubo);
        v2 =false;
      }
    //Animaicion
    voxelWrite(int (random(0,8)),int (random(0,8)),int (random(0,8)),true, cubo );
   }
   if (animetimer >= INTERVALO2*6  && animetimer <= INTERVALO2*7 ){
      //Init CARITA
      if(v3){
        clearCube(cubo);
        v3 =false;
        caritaCube(cubo);
      } 
      //mover plano
    }
    if (animetimer >= INTERVALO2*7  && animetimer <= INTERVALO2*10 ){
      //Init Rain
      if(v4){
        i=0;
        j=0;
        clearCube(cubo);
        v4 =false;
        voxelLluvia(cubo);
      } 
      i++;
      if(i>8000){
          moverPlanoX(cubo ,0);
          i=0;
          j++;
        }
        if(j>=n){
          voxelLluvia(cubo);
          
          j=0;
        }
      //mover plano
    }
    if (animetimer >= INTERVALO2*10  && animetimer <= INTERVALO2*12 ){
      //Init diagonal
      
      if(v5){
        clearCube(cubo);
        i=0;
        j=0;
        v5 =false;
        diagonalLine(cubo) ;     
        } 
        i++;
      if(i>10000){
          moverPlanoZ(cubo ,0);
          i=0;
          j++;
        }
        if(j>=n){
          diagonalLine(cubo) ;               
          j=0;
        }
      //mover plano
    }
    if (animetimer >= INTERVALO2*12  && animetimer <= INTERVALO2*17 ){
      //Init perimetral cubo
      if(v6){
        clearCube(cubo);
        i=0;
        j=0;
        v6 =false;
        estadoper = true;
             
      } 
      
      perimetralCube( estadoper,cubo,j,0); 
        i++;
      if(i>1000){
          //moverPlanoX(cubo ,0);
          i=0;
          j++;
        }
        if(j>n){
           estadoper = false; 
           //perimetralCube( true,cubo,8,0);  
           //perCuboPares(cubo);  
          j=0;
        }
      //mover plano
    }
    
    if(animetimer >= INTERVALO2*17){
      clearCube(cubo);
      animetimer =0;
      v1=true;
      v2=true;
      v3=true; 
      v4= true;
      v5=true;
      v6=true; 
      }
    
}


void Lanzacapas() {
  //Serial.print("Capa: "); Serial.println(capa);
  BuildByteArray(BytestoSend, cubo + capa * n_ledcapa, capa);
  IntroducirCatodo(BytestoSend, capa);
  MostrarByteArray(BytestoSend);
  capa++;
  if (capa == n) {
    capa = 0;
  }
  animetimer++;
}
void mostrarCubo(boolean cubo[ ] ) {
  for (int i = 0; i < n; i++) {
    Serial.println(" ");
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        Serial.print("  "); Serial.print(cubo[i + k * n + j * n * n]);
      }
    }
  }
}
void BuildByteArray(byte BytestoSend[], boolean boolArray[ ], int capa) {
  for (int i = 0; i < nbytes; i++) {
    BytestoSend[i] = BooltoByte(boolArray, i * 8 );
  }
}

void MostrarByteArray(byte BytesArray[] ) {
  //Serial.print("nbytes : "); Serial.println(nbytes);
  //LOW  MIENTRAS TRANSMITO
  digitalWrite(latchPin, LOW);
  for (int i = 0; i <= nbytes - 1 ; i++) {
    shiftOut(dataPin_anodo, clockPin, LSBFIRST, BytesArray[i]);
    //Serial.print("Byte : "); Serial.println(BytesArray[i], BIN);
  }
  //HIGH  CUANDO PARO LA TRANSMISION

  digitalWrite(latchPin, HIGH);
}
void IntroducirCatodo(byte BytesArray[], int capa) {
  int numbyte = n_ledcapa / 8;
  int numbit = (n_ledcapa % 8) + capa;
  while (numbit >= 8) {
    numbyte++;
    numbit -= 8;
  }
  bitWrite(BytesArray[numbyte], 7 - numbit, 1);
}
byte BooltoByte( boolean boolArray[ ], int celda) {
  // Recibe u array booleano y un indice celda,
  // genera un byte con los 8 siguientes valores
  // Si no hay mas valores que introducir de la capa pone ceros.
  byte result = 0;
  for (int i = 0; i < 8; i++) {
    if (i < n_ledcapa  && ((i + celda ) <  n_ledcapa) ) {
      bitWrite(result, abs(i - 7)  , boolArray[i + celda ] );
    }
  }
  return  result;
}
int bytesNecesarios( ) {
  // Calcula cuantos bits tengo que generar para un cubo de tamaño n
  int bits = 0;
  if (n_ledcapa + n_capas  < 8) {
    bits = 8;
  } else {
    bits = 0;
    while (bits < n_ledcapa + n_capas) {
      //incremento cada 8 hasta que supere el valor necesario
      bits += 8;
    }
  }
  //Serial.print("bytes necesarios: "); Serial.println(nbytes);
  return (bits / 8);
}
