/*
Arbol de Navidad animado  ISC 1A
*/

/*librerias*/
#include <stdio.h>  //libreria para printf y scanf
#include <stdlib.h> //libreria para rand
#include <time.h>   //libreria para rand y para la intermitencia del texto
#include <unistd.h> //libreria para sleep()
#include <ctype.h>  //para la validacion de entrada
#include <string.h> //para la validacion de entrada

/*funciones distintas en linux y windows limpiar pantalla (cls || clear) y kbhit*/
#if defined(_WIN32) //si detecta el sistema operativo de windwos se importara la libreria conio.h para la funcion kbhit usada para regresar al menu
#include <conio.h>//(solo funciona en windows) incluye la funcion kbhit
void clearScreen(){system("cls");system("mode 800");} //limpia la pantalla y la pone en pantalla completa
void textoSalirAnimacion(){puts("\x1b[34m" "(presione espacio para salir de la animacion)" "\x1b[0m");};
//gotoxy para windows recuperado de: https://stackoverflow.com/questions/55635791/how-to-use-function-gotoxyint-x-int-y
#include <windows.h>  
void gotoxy(int x, int y){ 
    COORD coordinate;
    coordinate.X = x;
    coordinate.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}
#else //sino se asume que es linux o mac y limpiar pantalla es clear
void clearScreen(){system("clear");}
void textoSalirAnimacion(){puts("\x1b[34m" "(presione enter para salir de la animacion)" "\x1b[0m");};
//gotoxy para linux recuperado de: https://stackoverflow.com/questions/7581347/gotoxy-function-with-c-linux-unix
void gotoxy(int x,int y){printf("%c[%d;%df",0x1B,y,x);} 
// este kbhit usa la implementacion de Alan Cox incluyendo la libreria de sys/select.h
/*nos autoprizo para usar esta implementacion de kbhit para linux;
Alan Cox (1995) https://www.linuxjournal.com/article/1138, codigo recuperado de https://www.linuxjournal.com/files/linuxjournal.com/linuxjournal/articles/011/1138/1138l1.html */
#include <sys/select.h>
int kbhit(void){
  struct timeval tv;fd_set read_fd;tv.tv_sec=0;tv.tv_usec=0;FD_ZERO(&read_fd);FD_SET(0,&read_fd);
  if(select(1, &read_fd,NULL,NULL,&tv)== -1)return 0;  
  if(FD_ISSET(0,&read_fd))return 1;return 0;
}
#endif
/*constantes*/
#define REN 10 //numero de renglones
const int COL = 2*REN-1; //numero de columnas
#define MAX 50 //numero maximo para cadenas
/*constantes ansi*/
#define COLOR_RESET   "\x1b[0m"
#define ROJO      "\x1b[31m"
#define VERDE     "\x1b[32m"
#define AMARILLO  "\x1b[33m"
#define AZUL      "\x1b[34m"
#define MAGENTA   "\x1b[35m"
#define CYAN      "\x1b[36m"


/*prototipos*/

/*Funciones que NO retornan datos y SI reciben parametros */

/*funciones generales*/
void llenarMatriz(char matrizArbol[][COL]);//llena la matriz del arbol
void textoArbol(int SegundoActual);//imprime de forma intermitente el texto al final del arbol
void menu(char matrizArbol[][COL],int arrayFilas[],int cantidadEsferas[],int arrayCol[],int cantidadEsferasCol[],int gotoy); //menu de opciones
/*opcion 1*/
void ponerEsferas(char matrizArbol[][COL],int cantidadEsferas[]);//cambia (*) por (O) de manera al azar pero siguiendo las reglas de cantidad de esfereas por renglon
void pintarMatriz(char matrizArbol[][COL]);//imprime los caracteres dados de la matriz
/*opcion 2*/
void seleccionarFil(int arrayFilas[],int gotoy);//el usuario selecciona la cantidad y las filas para animar mientras se valida la entrada
void ponerEsferasFil(char matrizArbol[][COL],int arrayFilas[],int cantidadEsferas[]);//se asigna la cantidad de esferas por fila a la matriz Arbol de manera aleatoria 
void pintarMatrizFil(char matrizArbol[][COL],int arrayFilas[]);//imprime los caracteres de la matriz, pero si es de la fila seleccionada se pintan los colores al azar
void ponerEsferasSeleccionadasFil(char matrizArbol[][COL],int arrayFilas[],int cantidadEsferas[]);//solo se asigna la cantidad de esferas por fila seleccionada a la matriz Arbol de manera aleatoria 
void borrarmatrizFil(char matrizArbol[][COL],int arrayFilas[]);//se cambian de (O) a (*) las filas seleccionadas
/*opcion 3*/
void seleccionarCol(int arrayCol[],int gotoy);//el usuario selecciona la cantidad y las columnas para animar mientras se valida la entrada
void ponerEsferasCol(char matrizArbol[][COL],int arrayCol[],int cantidadEsferasCol[]);//se asigna la cantidad de esferas por columna a la matriz Arbol
void pintarMatrizCol(char matrizArbol[][COL],int arrayCol[]);//se imprime al azar solo las columnas seleccionadas sino 
void ponerEsferasSeleccionadasCol(char matrizArbol[][COL],int arrayCol[],int cantidadEsferasCol[]);//solo se asigna la cantidad de esferas por columna seleccionada a la matriz Arbol de manera aleatoria 
void borrarmatrizCol(char matrizArbol[][COL],int arrayCol[]);//se cambian de (O) a (*) las filas seleccionadas
int validacion(int gotoy);
/*Funciones que NO retornan datos y NO reciben parametros */
void tronco();//llena e imprime la matrizTronco

/*Funciones que SI retornan datos y NO reciben parametros*/

int main(){
  srand(time(NULL));  //se inicializa el srand
    char matrizArbol [REN][COL]; //se crea la matriz fundamental que es el arbol (piramide)
    int gotoy;
    int arrayFilas[REN]; //es el array que selecciona las filas (renglones) a animar
    int arrayCol[COL]; //es el array que selecciona las columnas a animar
    int cantidadEsferas[REN] = {1,2,3,5,5,5,7,7,7,7};//cuantas esferas debe haber en la posicion del array para la primera posicion [0] habra 1 esfera, para la segunda [1] habra 2 esferas...  
    int cantidadEsferasCol[COL]={1,1,1,2,3,3,4,5,6,6,6,5,4,3,2,2,1,1,1};  //la cantidad de esferas de las columnas 
  menu(matrizArbol,arrayFilas,cantidadEsferas,arrayCol,cantidadEsferasCol,gotoy); //se llama al menu con las variables
return 0;
}


/*menu de opciones. ciclado y con una opcion para salir.*/
void menu(char matrizArbol[][COL],int arrayFilas[],int cantidadEsferas[],int arrayCol[],int cantidadEsferasCol[],int gotoy){
int opc=0;//la variable de opciones a hacer en el switch case del menu
      do{
        clearScreen();//limpa la pantalla de la terminal mandando un commando de terminal, en windows y linux 
    gotoxy(50,9); 
    printf(VERDE "------------------------------ Arbol de navidad-------------------------------");gotoxy(50,10);
    printf(VERDE "Reproducir la animacion de forma aleatoria en todo el arbol          ....... 1");gotoxy(50,11);
    printf(VERDE "Reproducir la animacion solo en las lineas del arbol seleccionadas   ....... 2");gotoxy(50,12);
    printf(VERDE "Reproducir la animacion solo en las columnas del arbol seleccionadas ....... 3");gotoxy(50,13);
    printf(VERDE "Salir                                                                ....... 4");gotoxy(50,14);
    printf(VERDE "------------------------------------------------------------------------------" COLOR_RESET);
        do{//validador de entrada de 1 a 4
         gotoxy(50,15);
         printf(AZUL "Selecciona una opcion: " COLOR_RESET);
         gotoxy(50,16);
         gotoy = 16;
         opc=validacion(gotoy);//se valida la entrada de datos del usuario
          if(opc<1 || opc>4){
           gotoxy(50,gotoy+1);
           printf(AMARILLO "'%i' no esta entre 1 y 4, intente de nuevo\n" COLOR_RESET,opc);
          }
        }while(opc<1 || opc>4);//while de la validacion de cantidad de filas
         
          switch (opc) {
          /*Reproducir la animacion de forma aleatoria en todo el arbol*/
          case 1:
            while(kbhit()!=1){//mientras no se precione una tecla se hara el while (que es la animacion)
            clearScreen();//limpia la terminal para la animacion
            time_t SegundoActual = time(0);//esta variable sirve para la intermitencia ya que obtiene el tiempo actual
            llenarMatriz(matrizArbol);//se llena la matriz Arbol con (*)
            ponerEsferas(matrizArbol,cantidadEsferas);//se asignan las esferas de manera aleatoria
            pintarMatriz(matrizArbol);//se imprimen las esferas de manera aleatoria con colores aleatorios
            tronco();//se imprime el tronco 
            textoArbol(SegundoActual);//se imprime el texto bajo el tronco de manera intermitente 
            //printf("\a");//se hace sonar un sonido de beep, pero no funciona bien en computadoras modernas, asi que mejor no fue implementado
            sleep(1);//se espera por un segundo para que la animacion se vea bonita
            };//while
            break;

          /*Reproducir la animacion solo en las lineas del arbol seleccionadas*/
          case 2:
            seleccionarFil(arrayFilas,gotoy);//se seleccionan las filas
            llenarMatriz(matrizArbol);//se llena la matriz Arbol con (*)
            ponerEsferasFil(matrizArbol,arrayFilas,cantidadEsferas);//se ponen las esferas aletoriamente en toda la matriz Arbol
            while(kbhit()!=1){//mientras no se precione una tecla se hara el while (que es la animacion)
            clearScreen();//limpia la terminal para la animacion
            time_t SegundoActual = time(0);//esta variable sirve para la intermitencia
            ponerEsferasSeleccionadasFil(matrizArbol,arrayFilas,cantidadEsferas);//se ponen las esferas aletoriamente en las filas seleccionadas en la matriz Arbol   
            pintarMatrizFil(matrizArbol,arrayFilas);//se imprime las esferas de las filas seleccionadas de manera aleatoria y las otras en orden
            borrarmatrizFil(matrizArbol,arrayFilas);//se cambian por (*) las (O) de las filas seleccionadas par a poder volver a asignar esferas
            tronco();//se imprime el tronco 
            textoArbol(SegundoActual);//se imprime el texto bajo el tronco de manera intermitente 
            //printf("\a");//se hace sonar un sonido de beep, pero no funciona bien en computadoras modernas, asi que mejor no fue implementado
            sleep(1);//se espera por un segundo para que la animacion se vea bonita
            };
            break;

          /*Reproducir la animacion solo en las columnas del arbol seleccionadas*/
          case 3:
            seleccionarCol(arrayCol,gotoy);//se seleccionan las columnas
            llenarMatriz(matrizArbol);//se llena la matriz Arbol con (*)
            ponerEsferasCol(matrizArbol,arrayCol,cantidadEsferasCol);
            while(kbhit()!=1){//mientras no se precione una tecla se hara el while (que es la animacion)
            clearScreen();//limpia la terminal para la animacion
            time_t SegundoActual = time(0);//esta variable sirve para la intermitencia
            ponerEsferasSeleccionadasCol(matrizArbol,arrayCol,cantidadEsferasCol);
            pintarMatrizCol(matrizArbol,arrayCol);//se imprime las esferas de columnas seleccionadas de manera aleatoria y las otras en orden
            borrarmatrizCol(matrizArbol,arrayCol);//se cambian por (*) las (O) de las filas seleccionadas par a poder volver a asignar esferas
            tronco();//se imprime el tronco 
            textoArbol(SegundoActual);//se imprime el texto bajo el tronco de manera intermitente 
            //printf("\a");//se hace sonar un sonido de beep, pero no funciona bien en computadoras modernas, asi que mejor no fue implementado
            sleep(1);//se espera por un segundo para que la animacion se vea bonita
            };
            break;
        }//switch de las animaciones
    }while(opc != 4);
}//void menu


/*funciones generales*/
//
//
void llenarMatriz(char matrizArbol[][COL]){//crea una matriz de piramide con asteriscos en funcion de las constantes REN y COL 
    int count = 0;
    for(int i=0; i<REN; i++){
        for(int j=0;j<COL; j++){
          if(j>=COL/2-count && j<=COL/2+count){ //si estan en el rango de la piramide se pintara un asterisco
            matrizArbol[i][j] = '*';           
            }else{ //sino estara fuera de la piramide y sera un espacio
            matrizArbol[i][j] = ' ';
            } 
        }//j
    count++;
    }//i
}//void llenar matriz

void tronco(){
  char arrayTronco[COL];
    for(int i=0;i<COL; i++){
          if(i==COL/2-1){//posicion a la izquierda de enmedio
            arrayTronco[i] = 'm';           
          }
          if(i==COL/2){//posicion enmedio
          arrayTronco[i] = 'W';           
          }
          if(i==COL/2+1){//posicion a la derecha de enmedio
            arrayTronco[i] = 'm';           
          }
          if(i!=COL/2-1 && i!=COL/2+1 && i!=COL/2){//las demas posiciones seran espacios
            arrayTronco[i] = ' ';
          }
    }//i
  int k=0; //entero para sumar el gotoxy en y
  /*pintar arrayTronco*/
  for(int i=0;i<2;i++){//i<2 porque la altura es de 2
    gotoxy(69,25+k); //gotoxy para la posicion en el centro
    for(int j=0;j<COL;j++){
    printf(AMARILLO "%c " COLOR_RESET,arrayTronco[j]);//se va por cada renglon y columna imprimiendo los valores de caracter dados
    }//j
  printf("\n");
  k+=1;//se va sumando
  }//i
printf("\n");
}//void tronco

void textoArbol(int SegundoActual){
if (SegundoActual%2==0){//si el tiempo actual es par se imprime de color magenta y sin la palabra "code" 
gotoxy(69,27);
printf(MAGENTA "\
\t       Merry christmas\n");
gotoxy(69,28);
printf("\t   and lots of      in 2023 \n" COLOR_RESET);
}else{//sino, se imprimira "code" con colores aleatorios
 gotoxy(69,27);
  printf(CYAN "\
\t       Merry christmas\n");
gotoxy(69,28);
printf("\t   and lots of " COLOR_RESET);

            int rColor=rand()%5;//son 5 pq es del 0 al 4 y si es 4 es default
          switch (rColor) {
          case 0:printf(ROJO "code " COLOR_RESET);
            break;
          case 1:printf(AMARILLO "code " COLOR_RESET);
            break;
          case 2:printf(AZUL "code " COLOR_RESET);
            break;
            break;
          case 3:printf(MAGENTA "code " COLOR_RESET);
            break;
          default:printf("code "); //color blanco
            break;
          }

  printf(CYAN "in 2023 \n" COLOR_RESET);
  }
gotoxy(67,29);
textoSalirAnimacion();
gotoxy(67,30);
}//void texto

int validacion(int gotoy){
  char entrada[MAX];
  int TamCadena,validado,entradaValidada;
  do{
    scanf (" %s", entrada);
    TamCadena = strlen (entrada); //da la longitud de la cadena para revisar cada digito o caracter
      for (int i=0;i<TamCadena; i++){
        if (!isdigit(entrada[i])){ //entrada no validada
            gotoxy(50,gotoy);
            printf (ROJO "'%s' no es un numero valido," AZUL " intente de nuevo: " COLOR_RESET,entrada);
            gotoy = gotoy +1;
            validado=0;//no se detecta un numero y se sigue el ciclo
            break;
        }else{
            gotoy = gotoy+1;
            entradaValidada=strtol(entrada,NULL,0); //se convierte de string a int
            validado=1;//se detecta un numero y se rompe el ciclo
            break;
        }//entrada validada
      }//i
    }while(validado==0);
return entradaValidada;
}//int validacion


/*opcion 1*/
//
//

void ponerEsferas(char matrizArbol[][COL],int cantidadEsferas[]){
  for(int i=0;i<REN;i++){//mientras que i sea menor que renglones se imprime la cantidad de esferas
   int cont = 0; 
   while(cont!=cantidadEsferas[i]){//imprime cantidad de esferas hasta la cantidad de esferas del array
    int rCOL = rand() % COL;// numero aleatoreo de 0 a cantidad de columna
    if(matrizArbol[i][rCOL] =='*'){//si la matriz en esa posicion es *
    matrizArbol[i][rCOL] = 'O'; //sera una esfera
    cont++;//se aumenta el contador para recorrer el array cantidadEsferas
    }//if
   }//while
  }//for
}//void poner esferas

void pintarMatriz(char matrizArbol[][COL]){ //imprime los caracteres dados
  int k=0;
  for(int i=0;i<REN;i++){
  gotoxy(69,5+k);
    for(int j=0;j<COL;j++){
        if(matrizArbol[i][j]=='*'){//si la matriz en esa posicion es asterisco
        printf(VERDE "%c " COLOR_RESET,matrizArbol[i][j]); //todos los asteriscos seran pintados color verde 
        }//asteriscos verdes
        if(matrizArbol[i][j]=='O'){ //si es esfera seran pintados de colores aleatorios 
          int rColor=rand()%6;//son 6 pq es del 0 al 5 y si es 5 es default
          switch (rColor) {
          case 0:printf(ROJO "%c " COLOR_RESET,matrizArbol[i][j]);
            break;
          case 1:printf(AMARILLO "%c " COLOR_RESET,matrizArbol[i][j]);
            break;
          case 2:printf(AZUL "%c " COLOR_RESET,matrizArbol[i][j]);
            break;
          case 3:printf(CYAN "%c " COLOR_RESET,matrizArbol[i][j]);
            break;
          case 4:printf(MAGENTA "%c " COLOR_RESET,matrizArbol[i][j]);
            break;
          default:printf("%c ",matrizArbol[i][j]); //color blanco
            break;
          }
        }//esferas y colores aleatorios
        if(matrizArbol[i][j]==' '){
        printf("%c ",matrizArbol[i][j]); //se pintan los esapcios
        }//espacios
      }//j
  k+=2;
  }//i
}//void pintar matriz


/*opcion 2*/
//
//Ya que el gotoy no lo cambia en la funcion validacion para luego utilizarlo en seleccionar filas/columnas
//tuvimos que ponerlo dentro de la misma funcion asi lo modifica y su posicion se mantiene correctamente
void seleccionarFil(int arrayFilas[],int gotoy){ 
int num,filas;
int k = 0;
  for(int i=0;i<REN;i++){//se inicializa el array en 0
  arrayFilas[i]=0;
  }//i
   
   do{//validador de entrada de 1 a 10
        gotoxy(50,gotoy);
        printf(CYAN "introduzca la cantidad de filas para animar (1 a 10 lineas)?: " COLOR_RESET);
        gotoy = gotoy +1;
         char entrada[MAX];
  int TamCadena,validado,entradaValidada;
  do{
    scanf (" %s", entrada);
    TamCadena = strlen (entrada); //da la longitud de la cadena para revisar cada digito o caracter
      for (int i=0;i<TamCadena; i++){
        if (!isdigit(entrada[i])){ //entrada no validada
            gotoxy(50,gotoy);
            printf (ROJO "'%s' no es un numero valido," AZUL " intente de nuevo: " COLOR_RESET,entrada);
            gotoy = gotoy +1;
            validado=0;//no se detecta un numero y se sigue el ciclo
            break;
        }else{
            entradaValidada=strtol(entrada,NULL,0); //se convierte de string a int
            validado=1;//se detecta un numero y se rompe el ciclo
            break;
        }//entrada validada
      }//i
    }while(validado==0);
    filas = entradaValidada;
         if(filas<1 || filas>10){//si las filas no estan de 1 a 10 manda un mensaje de error
            gotoxy(50,gotoy);
            printf(AMARILLO "'%i' no esta entre 1 y 10, intente de nuevo\n" COLOR_RESET,filas);
            gotoy +=1;
            }
    }while(filas<1 || filas>10);//while de la validacion de cantidad de filas
    
  for(int j=0;j<filas;j++){
  gotoxy(50,gotoy);
  printf(AZUL "introduzca el numero de la %i fila para animar: " COLOR_RESET,j+1);
  gotoy = gotoy +1;  
    char entrada[MAX];
  int TamCadena,validado,entradaValidada;
  do{
    scanf (" %s", entrada);
    TamCadena = strlen (entrada); //da la longitud de la cadena para revisar cada digito o caracter
      for (int i=0;i<TamCadena; i++){
        if (!isdigit(entrada[i])){ //entrada no validada
            gotoxy(50,gotoy);
            printf (ROJO "'%s' no es un numero valido," AZUL " intente de nuevo: " COLOR_RESET,entrada);
            gotoy = gotoy +1;
            validado=0;//no se detecta un numero y se sigue el ciclo
            break;
        }else{
            entradaValidada=strtol(entrada,NULL,0); //se convierte de string a int
            validado=1;//se detecta un numero y se rompe el ciclo
            break;
        }//entrada validada
      }//i
    }while(validado==0);
     num = entradaValidada;
    if(num<0 || num>10){//si el numero no esta entre 1 y 10 
      gotoxy(50,gotoy);
      printf(AMARILLO "'%i' no esta entre 1 y 10, intente de nuevo\n" COLOR_RESET,num);
      j--;//el contador regresa para volver a elegir ese numero
      gotoy+=1;
    };
  arrayFilas[num-1] = num;//se asigna el numero de la fila al array
  }//j
}//void seleccionar Filas

void ponerEsferasFil(char matrizArbol[][COL],int arrayFilas[],int cantidadEsferas[]){
  for(int i=0;i<REN;i++){//mientras que i sea menor que renglones se imprime la cantidad de esferas
   int cont = 0; 
   while(cont!=cantidadEsferas[i]){//imprime cantidad de esferas hasta la cantidad de esferas del array
    int rCOL = rand() % COL;// numero aleatoreo de 0 a cantidad de columna
      if((i+1)==arrayFilas[i]){
      break;
      }else{
        if(matrizArbol[i][rCOL] =='*'){//si la matriz en esa posicion es *
        matrizArbol[i][rCOL] = 'O'; //sera una esfera
        cont++;//se aumenta el contador para recorrer el array cantidadEsferas
        }
      }//else
   }//while
  }//i
}//void poner esferas filas

void pintarMatrizFil(char matrizArbol[][COL],int arrayFilas[]){  
  int contColor=0;int k=0;
  for(int i=0;i<REN;i++){
    gotoxy(69,5+k);
    for(int j=0;j<COL;j++){
        //se imprimen los asteriscos verde 
        if(matrizArbol[i][j] == '*'){
        printf(VERDE "%c " COLOR_RESET,matrizArbol[i][j]);
        }
        //se imprimen los espacios
        if(matrizArbol[i][j] == ' '){
        printf("%c ",matrizArbol[i][j]);
        }
        
        if(matrizArbol[i][j] == 'O' && (i+1)!=arrayFilas[i]){//si es una esfera y no es de las filas seleccionadas se pintara en un solo orden
              if(contColor==6){contColor=0;}//se reinicia el contador de colores para volver a recorrer los colores

                switch (contColor) {
                  case 0:printf(ROJO "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 1:printf(AMARILLO "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 2:printf(AZUL "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 3:printf(CYAN "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 4:printf(MAGENTA "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  default:printf("%c ",matrizArbol[i][j]); //color blanco
                    break;
                  }//switch colores aleatorios
                  contColor++; 
                  }

            if(matrizArbol[i][j] == 'O' && (i+1)==arrayFilas[i]){//si es una esfera y es de las filas seleccionadas se pintaran aleatoriamente
              int rColor=rand()%6;//son 6 pq es del 0 al 5 y si es 5 es default
                switch (rColor) {
                  case 0:printf(ROJO "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 1:printf(AMARILLO "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 2:printf(AZUL "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 3:printf(CYAN "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 4:printf(MAGENTA "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  default:printf("%c ",matrizArbol[i][j]); //color blanco
                    break;
                }//switch colores aleatorios
              }
    }//j
    printf("\n\n");
    k+=2;
  }//i
}//void pintar Matriz Filas

void ponerEsferasSeleccionadasFil(char matrizArbol[][COL],int arrayFilas[],int cantidadEsferas[]){
  for(int i=0;i<REN;i++){
    int count = 0;
    while(count!=cantidadEsferas[i]){
      int rCOL = rand() % COL;
      if((i+1)==arrayFilas[i]){
        if(matrizArbol[i][rCOL] =='*'){
        matrizArbol[i][rCOL] = 'O';
        count += 1;
        }
      }else{
        count = cantidadEsferas[i];
        }
      }//while
    }//i
 }//void poner esferas filas

void borrarmatrizFil(char matrizArbol[][COL],int arrayFilas[]){
int count = 0;
    for(int i=0; i<REN; i++){
        for(int j=0;j<COL; j++){
          if((i+1)==arrayFilas[i]){
            if(j>=COL/2-count && j<=COL/2+count){
              matrizArbol[i][j] = '*';
            }else{
              matrizArbol[i][j] = ' ';
            }
          }//if
        }//j
    count++;
  }//i
}//void borrar matriz filas


/*opcion 3*/
//
//
void seleccionarCol(int arrayCol[],int gotoy){
int num,columnas;

  for(int i=0;i<COL;i++){//se inicializa la matriz en 0
  arrayCol[i]=0;
  }//i

   do{//validador de entrada de 1 a 19
        gotoxy(50,gotoy); // se establece la posicion en la pantalla
        printf(CYAN "introduzca la cantidad de columnas para animar (1 a 19 columnas)?: " COLOR_RESET);
        gotoy +=1;//el gotoy se recorre en 1
 char entrada[MAX];
  int TamCadena,validado,entradaValidada;
  do{//se valida la entra de datos del usuario para la variable columna
  scanf (" %s", entrada);
    TamCadena = strlen (entrada); //da la longitud de la cadena para revisar cada digito o caracter
      for (int i=0;i<TamCadena; i++){
        if (!isdigit(entrada[i])){ //entrada no validada
            gotoxy(50,gotoy);// se establece la posicion en la pantalla
            printf (ROJO "'%s' no es un numero valido," AZUL " intente de nuevo: " COLOR_RESET,entrada);
            gotoy = gotoy +1; // el gotoy se recorre en 1
            validado=0;//no se detecta un numero y se sigue el ciclo
            break;
        }else{
            entradaValidada=strtol(entrada,NULL,0); //se convierte de string a int
            validado=1;//se detecta un numero y se rompe el ciclo
            break;
        }//entrada validada
      }//i
    }while(validado==0);
     columnas = entradaValidada;
          if(columnas<1 || columnas>19){//si las columnas no estan de 1 a 10 manda un mensaje de error
          gotoxy(50,gotoy);// se establece la posicion en la pantalla
          printf(AMARILLO "'%i' no esta entre 1 y 19, intente de nuevo\n" COLOR_RESET,columnas);
          gotoy+=1; // el gotoy se recorre en 1
          }
    }while(columnas<1 || columnas>19);//while de la validacion de cantidad de filas
    
  for(int j=0;j<columnas;j++){
  gotoxy(50,gotoy); // se establece la posicion en la pantalla
  printf(AZUL "introduzca el numero de la %i columna para animar: " COLOR_RESET,j+1);
  gotoy +=1; //el gotoy se recorre en 1
  char entrada[MAX];
  int TamCadena,validado,entradaValidada;
  do{//se valida la entra de datos del usuario para la variable columna
  scanf (" %s", entrada);
    TamCadena = strlen (entrada); //da la longitud de la cadena para revisar cada digito o caracter
      for (int i=0;i<TamCadena; i++){
        if (!isdigit(entrada[i])){ //entrada no validada
            gotoxy(50,gotoy); // se establece la posicion en la pantalla
            printf (ROJO "'%s' no es un numero valido," AZUL " intente de nuevo: " COLOR_RESET,entrada);
            gotoy = gotoy +1;//el gotoy se recorre en 1
            validado=0;//no se detecta un numero y se sigue el ciclo
            break;
        }else{
            entradaValidada=strtol(entrada,NULL,0); //se convierte de string a int
            validado=1;//se detecta un numero y se rompe el ciclo
            break;
        }//entrada validada
      }//i
    }while(validado==0);
     num = entradaValidada;//se valida la entra de datos del usuario para la variable num (que es el num de columna)
    if(num<0 || num>19){//si el numero no esta entre 1 y 19 
      gotoxy(50,gotoy); // se establece la posicion en la pantalla
      printf(AMARILLO "'%i' no esta entre 1 y 19, intente de nuevo\n" COLOR_RESET,num);
      gotoy+=1;//el gotoy se recorre en 1
      j--;//el contador regresa para volver a elegir ese numero
    };
  arrayCol[num-1] = num;//se asigna el numero de la fila al array
  }//j
}//void seleccionar columnas

void ponerEsferasCol(char matrizArbol[][COL],int arrayCol[],int cantidadEsferasCol[]){ //pone las esferas con restricciones de columnas
  for(int i=0;i<COL;i++){//mientras que i sea menor que renglones se imprime la cantidad de esferas
  int cont = 0; 
    while(cont!=cantidadEsferasCol[i]){//imprime cantidad de esferas hasta la cantidad de esferas del array
    int rREN = rand() % REN;// numero aleatoreo de 0 a cantidad de renglon
      if((i+1)==arrayCol[i]){
        cont = cantidadEsferasCol[i];
      }else{
        if(matrizArbol[rREN][i] =='*'){//si la matriz en esa posicion es * 
        matrizArbol[rREN][i] = 'O'; //sera una esfera
        cont++;//se aumenta el contador para recorrer el array cantidadEsferasCol
        }
      }
    }//while
  }//i
}//void poner Esferas Columnas

void ponerEsferasSeleccionadasCol(char matrizArbol[][COL],int arrayCol[],int cantidadEsferasCol[]){
  for(int i=0;i<COL;i++){
    int count = 0;
    while(count!=cantidadEsferasCol[i]){
      int rREN = rand() % REN;
      if((i+1)==arrayCol[i]){
        if(matrizArbol[rREN][i] =='*'){
        matrizArbol[rREN][i] = 'O';
        count += 1;
        }
      }else{
      count = cantidadEsferasCol[i];
      }
    }//while
  }//i
}//void poner EsferasSeleccionadas Columnas

void pintarMatrizCol(char matrizArbol[][COL],int arrayCol[]){ //imprime los caracteres dados con restriccion de columnas 
  int contColor=0,k=0;
  for(int i=0;i<REN;i++){
    gotoxy(69,5+k);//se establece la posicion
    for(int j=0;j<COL;j++){
        //se imprimen los asteriscos verde 
        if(matrizArbol[i][j] == '*'){
        printf(VERDE "%c " COLOR_RESET,matrizArbol[i][j]);
        }
        //se imprimen los espacios
        if(matrizArbol[i][j] == ' '){
        printf("%c ",matrizArbol[i][j]);
        }

        if(matrizArbol[i][j] == 'O' && (j+1)!=arrayCol[j]){//si es una esfera y no es de las filas seleccionadas se pintara en un solo orden
              if(contColor==6){contColor=0;}//se reinicia el contador de colores para volver a recorrer los colores
                switch (contColor) {
                  case 0:printf(ROJO "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 1:printf(AMARILLO "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 2:printf(AZUL "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 3:printf(CYAN "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 4:printf(MAGENTA "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  default:printf("%c ",matrizArbol[i][j]); //color blanco
                    break;
                  }//switch colores aleatorios
                  contColor++; 
        }
            if(matrizArbol[i][j] == 'O' && (j+1)==arrayCol[j]){//si es una esfera y es de las filas seleccionadas se pintaran aleatoriamente
              int rColor=rand()%6;//son 6 pq es del 0 al 5 y si es 5 es default
                switch (rColor) {
                  case 0:printf(ROJO "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 1:printf(AMARILLO "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 2:printf(AZUL "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 3:printf(CYAN "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  case 4:printf(MAGENTA "%c " COLOR_RESET,matrizArbol[i][j]);
                    break;
                  default:printf("%c ",matrizArbol[i][j]); //color blanco
                    break;
                }//switch colores aleatorios
            }
    }//j
    k+=2; // se recorre la posicion
  }//i
}//void pintar Matriz 3

void borrarmatrizCol(char matrizArbol[][COL],int arrayCol[]){
  for(int i=0; i<REN; i++){
    for(int j=0;j<COL; j++){
      if((j+1)==arrayCol[j] && matrizArbol[i][j]!=' '){
        matrizArbol[i][j]='*';
      }
    }//j
  }//i
}//void borrar matriz Columnas