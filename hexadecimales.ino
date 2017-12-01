/**
   Este programa son experimentos para convertir un numero decimal a una representacion hexadecimal y de ahi nuevamente a decimal.
   Su razon de ser es para ser usado en el llamado a ciertas funciones por el puerto serial al modulo DFPlayer Mini.
   Estas funciones tienen que transformarse en secuencias de numeros que representan valores en hexadecimal.
   En el pariticular caso de la funcion que sirve para ejecutar un archivo dentro de la carpeta /mp3/ que segun la hoja de datos soporta hasta 2999 archivos
   Dicha funcion recibe dos parametros y en el caso de que se quiera por ejemplo ejecutar el archivo 2999
   es necesario convertir a hexadecimal el 2999 y separar en dos 2 bytes el numero resultante y se corresponderan con el byte de orden mayor y el byte de orden menor
   que se pasaran a la funcion en ese orden.

   decimal 1000 = 0x03E8 => DH = 0x03; DL = 0xE8

*/

#define PRUEBA_FINAL 1

#define bufferSize 4                          //se esperan solo 4 posiciones hexadecimales pero la funcion itoa o sprintf devuelve al final un cr o lf

int index = 0;                                //es el numero decimal que se transformara a hexadecimal
int sigma = 0;                                //utilizada para sumar los valores decimales y comprobar que el obtenido es el mismo que index

char * indexAsHex;
  
/**
   Devuelve el valor decimal de la representacion hexadecimal que se le pasa como parametro
*/
int hexChar2int(char value) {
  switch (value) {
    case 'a': return 10;
    case 'b': return 11;
    case 'c': return 12;
    case 'd': return 13;
    case 'e': return 14;
    case 'f': return 15;
    case NULL: return 0;
    default: return value - '0';
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  indexAsHex = (char *)malloc( sizeof(char) * bufferSize);
}

void loop() {

#ifndef PRUEBA_FINAL
      
  memset(&indexAsHex[0], NULL, sizeof(char) * bufferSize);  //Es necesario limpiar el array para tener uno completamente limpio a la hora de empezar cada iteracion
  //itoa (index, indexAsHex, 16);                     //itoa se puede utilizar pero sprintf si pertenece a la especificacion de C
  sprintf(indexAsHex, "%x", index);
  Serial.print(index);
  Serial.print(" = ");
  Serial.print(indexAsHex); // para imprimir el equivalente en hexadecimal

  Serial.print("   (");
  for (byte k = 0; k < bufferSize; k++) {
    Serial.print(indexAsHex[k]);
    if (k < bufferSize - 1) Serial.print(",");
  }
  Serial.print(")");

  int i = 0;
  int j = 0;
  while (indexAsHex[bufferSize - 1] == NULL) {              //Se recorren los caracteres a la derecha para tener un posicionamiento correcto
    i = bufferSize - 1;                                     // aunque este bloque podria no ser necesario y simplemente analizarse de derecha a
    j = bufferSize - 2;                                     //izquierda el array hasta qeu se encuentre un valor no nulo

    while (i > 0 ) {
      indexAsHex[i] = indexAsHex[j];
      indexAsHex[j] = 0;
      i--;
      j--;
    }
  }

  Serial.print("   (");
  for (byte k = 0; k < bufferSize; k++) {
    Serial.print(indexAsHex[k]);
    if (k < bufferSize - 1) Serial.print(",");
  }
  Serial.print(")");

  int val0 = hexChar2int(indexAsHex[0]);      
  int val1 = hexChar2int(indexAsHex[1]);      
  int val2 = hexChar2int(indexAsHex[2]);      
  int val3 = hexChar2int(indexAsHex[3]);      

  val0 = val0 << 12;                  // se hace el corrimiento de bits para obtener la magnitud para cada digito
  val1 = val1 << 8;
  val2 = val2 << 4;

  Serial.print("     ");
  Serial.print(val0);
  Serial.print(" + ");
  Serial.print(val1);
  Serial.print(" + ");
  Serial.print(val2);
  Serial.print(" + ");
  Serial.print(val3);
  Serial.print(" = ");

  int totalVal = val0 + val1 + val2 + val3;       // se vuelve a sumar el resultado obtenido y debe coincidir con el valor de index
  Serial.println(totalVal);

  if (index >= 2999) {
    delay(600000);
  }
  

#else      
      Serial.println();
      Serial.print("dandole play al archivo ");
      Serial.println(index);
      int * index2Bytes;
      index2Bytes = (int *)malloc(2);
      index2Bytes = getHighLowOrderBytes(index);
      delay(5000);
      
#endif

  index++;
} // loop

/**
 * Utilizando la funcionalidad probada arriba se genera una funcion que devuelva un array con dos decimales que 
 * que representan los bytes de orden alto y bajo segun el decimal pasado como parametro 
 */

int * getHighLowOrderBytes(int param){      
  static int highLowBytes[2];
  char * paramAsHex;

  if(param>2999) return NULL;                 //para nuestra aplicacion solo manejaremos valores hasta 2999 que es lo que puede manejar el DFPlayer
  
  paramAsHex = (char *)malloc( sizeof(char) * bufferSize);
  memset(&paramAsHex[0], NULL, sizeof(char) * bufferSize);
  sprintf(paramAsHex, "%x", param);  
  Serial.print("valor paramAsHex ");
  Serial.println(paramAsHex);

  Serial.print("   (");
  for (byte k = 0; k < bufferSize; k++) {
    Serial.print(paramAsHex[k]);
    if (k < bufferSize - 1) Serial.print(",");
  }
  Serial.println(")");

  int i = 0;
  int j = 0;
  while (paramAsHex[bufferSize - 1] == NULL) {              //Se recorren los caracteres a la derecha para tener un posicionamiento correcto
    i = bufferSize - 1;                                     // aunque este bloque podria no ser necesario y simplemente analizarse de derecha a
    j = bufferSize - 2;                                     //izquierda el array hasta qeu se encuentre un valor no nulo

    while (i > 0 ) {
      paramAsHex[i] = paramAsHex[j];
      paramAsHex[j] = 0;
      i--;
      j--;
    }
  }
  
  int val0 = hexChar2int(paramAsHex[0]);      
  int val1 = hexChar2int(paramAsHex[1]);      
  int val2 = hexChar2int(paramAsHex[2]);      
  int val3 = hexChar2int(paramAsHex[3]);      

  val0 = val0 << 12;                  // se hace el corrimiento de bits para obtener la magnitud para cada digito
  val1 = val1 << 8;
  val2 = val2 << 4;

  Serial.print("     ");
  Serial.print(val0);
  Serial.print(" + ");
  Serial.print(val1);
  Serial.print(" = ");
  int totalVal = val0 + val1;
  Serial.print(totalVal);
  
  Serial.print("    ");
  Serial.print(val2);
  Serial.print(" + ");
  Serial.print(val3);
  Serial.print(" = ");
  totalVal = val2 + val3;
  Serial.print(totalVal);

  highLowBytes[0] = val0 + val1 ;
  highLowBytes[1] = val2 + val3 ; 
 
  free(paramAsHex);
  return highLowBytes;
}


