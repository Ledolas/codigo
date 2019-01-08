//Este codigo lee un array bidimensional de tamaño [n][n*n]
//con la informacion de los anodos a encender en [n*n]
//y lo reparte en bytes para ir rellenando los registros asociados a los anodos.
//Los registros asociados a los catodos se rellenan segun la capa a encender del cubo,
//este valor lo determina [n]
// Asignacion de los Pines
#define latchPin  10        //Latch (Pin 12)
#define clockPin  11        //Clock serie (Pin 11)
#define dataPin_anodo  8      // DS (Pin 14) para anodos
#define dataPin_catodo  9        //DS (Pin 14) para catodos

//Variables Interrupcion
unsigned long previous_millis = 0;
unsigned long intervalo = 200; // intervalo en ms para la interrupcion
//Variables Registros
byte byte_anodo; // Almaceno los datos de los anodos
byte byte_catodo; // Almaceno capa a encender
int nbytes;// Cuantos bytes se necesitan para enviar los anodos
// Variables CUBO :
int n = 2; // cambiar dependiendo del Array a usar
int n_capas = n;
int n_ledcapa = n * n;
int n_anodos = n * n * n;
int capa = 0;
// Arrays --> Cambiar n dependiendo que array usemos
boolean cubo[2][4] = {
  {1, 0, 1, 0 } ,    //Capa 0
  {0, 1, 0, 1 }    //Capa 1
};

/* boolean cubo[3][9] = {
  {1, 0, 1, 1, 0, 0, 0, 0, 1 },    //Capa 0
  {0, 0, 0, 1, 0, 0, 0, 1, 0 },    //Capa 1
  {0, 0, 0, 0, 0, 0, 0, 0, 1 }           //Capa 2
  };
  /*boolean cubo[4][16] = {
  // 0  1  2  3  4  5  6  7 |8  9 10 11 12 13 14 15|  Indices
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, //Capa 0
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, //Capa 1
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, //Capa 2
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }     //Capa 3
  };*/

void setup() {
  nbytes = bytesNecesarios( );// Calculo cuantos bytes necesito segun el cubo
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin_anodo, OUTPUT);
  pinMode(dataPin_catodo, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentmillis = 0;
  currentmillis = millis(); //Guardo el valor actual
  // - - Compruebo si han pasado 200 ms
  if ((currentmillis  - previous_millis) >= intervalo) {
      for (int i = 0 ; i < nbytes ; i++ ) {
        byte_anodo = BooltoByte(cubo[capa], i * 8 );
        bitWrite(byte_anodo, capa, 1);
        Serial.print("Byte anodo: "); Serial.println(byte_anodo, BIN);
        //LOW  MIENTRAS TRANSMITO
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin_anodo, clockPin, LSBFIRST, byte_anodo);
        //HIGH  CUANDO PARO LA TRANSMISION
        digitalWrite(latchPin, HIGH);
      }
      capa++;
      if(capa==n){
        capa=0;
        }
      previous_millis = millis();
    }
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
    while (bits < n_ledcapa) {
      //incremento cada 8 hasta que supere el valor necesario
      bits += 8;
    }
  }
  return (bits / 8) ;
}