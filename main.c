#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define COL 18
#define FIL 6

//prototipos de las funciones
void inicio();
void jugar();
int tablero(int[9], int[9], int);
void presentacion(char[25], char[25], char, char, int, int, int);
int validarGanador(char[FIL][COL]);

void posicionACoordenadas(int*, int*, int);
int coordenadaAPosicion(int,int);
int posValidas(int, int[9], int[9], int, int);
int validarVolverJugar();

void vaciar(int[9]);

void limP();
void pausa();
void clean_stdin(void);

int main(){

	//variables locales
	int salir=0;
	int opc=0;
	
	//ciclo principal del juego
	do{
		limP(); //limpiar pantalla
		printf("          Juego Del Gato\n\n");
		inicio();
		printf("\n\n\n*Ingrese 1 para jugar\n\n");
		printf("*Ingrese 0 para salir\n->");
		scanf("%i", &opc);
		
		clean_stdin();
		limP(); //limpiar pantalla
		
		switch(opc){//estudio de casos para ejecutar la solicitud del usuario sobre jugar o salir
			
			case 0:{
				printf("Saliendo...\n");	
				salir=1;
				break;
			}
				
			case 1:{
				jugar();
				break;
			}
					
			default:{
					printf("Parametro ingresado al menu no valido!\n");
					pausa();
					break;
			}	
			
		}
	}while(salir!=1);
	
	return 0;
}
	
//funcion que pinta un tablero para el inicio
void inicio(){
	
	int i=0,j=0;
	
	//cargar matriz
	char matriz[FIL][COL]={ 
		"     [0] [1] [2] ",
		"[0] |   |   |   |",
		"    .............",
		"[1] |   |   |   |",
		"    .............",
		"[2] |   |   |   |",
	};
	
	for(i=0;i<FIL;i++){
		for(j=0;j<COL;j++){	
			printf("%c",matriz[i][j]);
		}
		printf("\n");
	}
	
}

//funcion que ejecutara las rutinas del juego al iniciarse
void jugar(){
	srand(time(0)); // seteando la semilla
		
	//cadenas para almacenar el nombre
	char jugador1[25]; 
	char jugador2[25];
	
	char tecla=' ';//almacenara la tecla
	
	//variable control del loop
	int Jugar=1;
	
	//caracteres de los jugadores
	char caracter1=' ';
	char caracter2=' ';
	
	int nJugadores=0; //canitdad de jugadores
	
	int fila=0, columna=0; //variables que almacenan la selccion de coordenadas del jugador
	
	int posicion=0; //variable que almacena la posicion ingresada por el jugador en forma de coordenada
	
	int posOcupadasX[9]; //alamacena las posiciones ocupadas para validar la X
	int posX=0; //variable iterador del vector
	
	int posOcupadasO[9]; //alamacena las posiciones ocupadas para validar la O
	int posO=0; //variable iterador del vector
	
	int ganador=2;//variable de control que alerta si hay un ganador
	
	int posAux=0; //varaible auxiliar que serivira para saber si la posiciones ya fue ocupada
	
	int jugarPC=0; //variable de control para saber si va a jugar la pc (1 para si, cero para no)
	
	int papel=0; //variable de control, si se desea jugar en papel
	
	int volverJugar=0; //variable de control que indica volver a jugar
	
	//conteo de victorias y empates
	int victoriasJugador1=0;
	int victoriasJugador2=0;
	int empates=0;

	do{
		//preguntar cuantos jugadores
		printf("*Ingrese la cantidad de jugadores 1/2\n->");
		scanf("%i", &nJugadores);
		
		if(nJugadores<1 || nJugadores>2)
			printf("La cantidad de jugadores solo puede ser 1 o 2\n");
		
	} while(nJugadores<1 || nJugadores>2); //ciclo que asegura que se ingrese una cantidad correcta de jugadores
	
	clean_stdin();
	
	if(nJugadores==2){
		//solicitud de los nombres de los jugadores
		printf("\n*Ingrese el nombre del primer jugador\n->");
		scanf("%s", &jugador1);
		
		clean_stdin();
		
		//pedir caracter para el jugador 1
		do{
			printf("\n*Jugador 1 Ingrese el caracter a jugar entre x/o\n->");
			scanf("%c", &caracter1);
		
			if(caracter1!='x' && caracter1!='o')
				printf("Solo puede elegir entra x/o para jugar...\n");
		
		} while(caracter1!='x' && caracter1!='o');
		
		clean_stdin();
		
		//se le da el caracter del jugador 2 dependiendo de la eleccion del jugador 1
		if(caracter1=='x')
			caracter2='o';
		else
			caracter2='x';
		
		printf("\n*Ingrese el nombre del segundo jugador\n->");
		scanf("%s", &jugador2);
		
		clean_stdin();
		
		printf("\nJugador 2, su caracter es: %c\n", caracter2);
		
		jugarPC=0;
		
	}
	else
		{ //si es un jugador vs la maquina...
	
		//pedir nombre del jugador
		printf("\n*Ingrese el nombre del primer jugador\n->");
		scanf("%s", &jugador1);
		
		clean_stdin();
		
		//pedir caracter para el jugador
		do{
			printf("\n*Ingrese el caracter a jugar entre x/o\n->");
			scanf("%c", &caracter1);
			
			if(caracter1!='x' && caracter1!='o')
				printf("Solo puede elegir entra x/o para jugar...\n");
			
		} while(caracter1!='x' && caracter1!='o');
		
		clean_stdin();
		
		strcpy(jugador2, "Computadora"); //el jugador sera la comutadora por defecto
		
		//se le da el caracter a la computadora dependiendo de la eleccion del jugador 1
		if(caracter1=='x')
			caracter2='o';
		else
			caracter2='x';
			
		jugarPC=1;
	}
	
	//preguntar si desea jugar en pc o en papel
	
	do{
		printf("\n*Desea jugar en PC o en Papel? 1=Si, 0=No\n->");
		scanf("%i", &papel);
	}while(papel<0 || papel>1);
	
	limP(); //borrar pantalla
	
	clean_stdin(); //se limpia el buffer
	
	vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
	vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
	
	//***************************** Rutina del loop del Juego* ******************************************/
	while(Jugar==1 || volverJugar==1){
		
		volverJugar=0;
			//----------Jugador 1 ------------
		if(papel==1){
			ganador=tablero(posOcupadasX, posOcupadasO, papel); //invocacion de la funcion que muestra el tablero
			limP();
			presentacion(jugador1, jugador2,caracter1, caracter2, victoriasJugador1, victoriasJugador2, empates); //invocacion de la funcion que muestra la presentacion
			printf("\n*Revisar el txt\n");
		}else{
			presentacion(jugador1, jugador2,caracter1, caracter2, victoriasJugador1, victoriasJugador2, empates); //invocacion de la funcion que muestra la presentacion
			ganador=tablero(posOcupadasX, posOcupadasO, papel); //invocacion de la funcion que muestra el tablero
		}
		
		//validacion del valor de ganador para saber si alguien gano
		if(ganador==0 && caracter1=='x'){
			printf("\n*Felicidades: %s has ganado!!\n\n", jugador1);
			volverJugar=validarVolverJugar();
			if(volverJugar==1){
				vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
				vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
				posO=0; posX=0;
				victoriasJugador1++;
			}else
				break;		
		}else if(ganador==0 && caracter1=='o'){
			if(jugarPC==1){
				printf("\n*Lo siento, la comutadora ha ganado!!\n\n", jugador2);
				volverJugar=validarVolverJugar();
				if(volverJugar==1){
					vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
					vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
					posO=0; posX=0;
					victoriasJugador2++;
				}else
					break;
			}else{
				printf("\n*Felicidades: %s has ganado!!\n\n", jugador2);
				volverJugar=validarVolverJugar();
				if(volverJugar==1){
					vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
					vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
					posO=0; posX=0;
					victoriasJugador2++;
				}else
					break;
			}
		}
		if(ganador==1 && caracter1=='o'){
			printf("\n*Felicidades: %s has ganado!!\n\n", jugador1);
			volverJugar=validarVolverJugar();
			if(volverJugar==1){
				vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
				vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
				posO=0; posX=0;
				victoriasJugador1++;
			}else
				break;
		}else if(ganador==1 && caracter1=='x'){
			if(jugarPC==1){
				printf("\n*Lo siento, la comutadora ha ganado!!\n\n", jugador2);
				volverJugar=validarVolverJugar();
				if(volverJugar==1){
					vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
					vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
					posO=0; posX=0;
					victoriasJugador2++;
				}else
					break;
			}else{
				printf("\n*Felicidades: %s has ganado!!\n\n", jugador2);
				volverJugar=validarVolverJugar();
				if(volverJugar==1){
					vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
					vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
					posO=0; posX=0;
					victoriasJugador2++;
				}else
					break;
			}
		}
		
		//validacion de empate
		if(posX+posO==9){ 
			printf("\n*Ha sido un empate! No hay Ganador!\n\n");
			volverJugar=validarVolverJugar();
			pausa();
			limP();
			if(volverJugar==1){
				vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
				vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
				posO=0; posX=0;
				empates++;
			}else
				break;
		}
		
		if(volverJugar==1){ //si se vuelve a jugar, se rePinta el tablero para empezar en cero para el segundo jugador
			limP();
			if(papel==1){
				ganador=tablero(posOcupadasX, posOcupadasO, papel); //invocacion de la funcion que muestra el tablero
				limP();
				presentacion(jugador1, jugador2,caracter1, caracter2, victoriasJugador1, victoriasJugador2, empates); //invocacion de la funcion que muestra la presentacion
				printf("\n*Revisar el txt\n");
			}else{
				presentacion(jugador1, jugador2,caracter1, caracter2, victoriasJugador1, victoriasJugador2, empates); //invocacion de la funcion que muestra la presentacion
				ganador=tablero(posOcupadasX, posOcupadasO, papel); //invocacion de la funcion que muestra el tablero
			}
		}
		
		//solicitud de las coordenadas del jugador 1
		do{
			printf("\nTurno para: %s\n",jugador1);
			printf("\n*Ingrese la Fila de la posicion a alegir\n->");
			scanf("%i", &fila);
			
			printf("*Ingrese la Columna de la posicion a alegir\n->");
			scanf("%i", &columna);
			
			posicion=coordenadaAPosicion(fila, columna);
			
		}while(fila<0 || columna<0 || fila>2 || columna>2 || posValidas(posicion, posOcupadasX, posOcupadasO, posX, posO)!=1); //valiadacion de las coordenadas
		
		if(caracter1=='x'){ 
			posOcupadasX[posX]=posicion;
			posX++;
		}else{
			posOcupadasO[posO]=posicion;
			posO++;
		}
		
		pausa();
		limP();
		
		//----------Jugador 2 ------------

		if(papel==1){
			ganador=tablero(posOcupadasX, posOcupadasO, papel); //invocacion de la funcion que muestra el tablero
			limP();
			presentacion(jugador1, jugador2,caracter1, caracter2, victoriasJugador1, victoriasJugador2, empates); //invocacion de la funcion que muestra la presentacion
			printf("\n*Revisar el txt\n");
		}else{
			presentacion(jugador1, jugador2,caracter1, caracter2, victoriasJugador1, victoriasJugador2, empates); //invocacion de la funcion que muestra la presentacion
			ganador=tablero(posOcupadasX, posOcupadasO, papel); //invocacion de la funcion que muestra el tablero
		}
		
		//validacion del valor de ganador para saber si alguien gano
		if(ganador==0 && caracter1=='x'){
			printf("\n*Felicidades: %s has ganado!!\n\n", jugador1);
			volverJugar=validarVolverJugar();
			if(volverJugar==1){
				vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
				vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
				posO=0; posX=0;
				victoriasJugador1++;
			}else
				break;		
		}else if(ganador==0 && caracter1=='o'){
			if(jugarPC==1){
				printf("\n*Lo siento, la comutadora ha ganado!!\n\n", jugador2);
				volverJugar=validarVolverJugar();
				if(volverJugar==1){
					vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
					vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
					posO=0; posX=0;
					victoriasJugador2++;
				}else
					break;
			}else{
				printf("\n*Felicidades: %s has ganado!!\n\n", jugador2);
				volverJugar=validarVolverJugar();
				if(volverJugar==1){
					vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
					vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
					posO=0; posX=0;
					victoriasJugador2++;
				}else
					break;
			}
		}
		if(ganador==1 && caracter1=='o'){
			printf("\n*Felicidades: %s has ganado!!\n\n", jugador1);
			volverJugar=validarVolverJugar();
			if(volverJugar==1){
				vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
				vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
				posO=0; posX=0;
				victoriasJugador1++;
			}else
				break;
		}else if(ganador==1 && caracter1=='x'){
			if(jugarPC==1){
				printf("\n*Lo siento, la comutadora ha ganado!!\n\n", jugador2);
				volverJugar=validarVolverJugar();
				if(volverJugar==1){
					vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
					vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
					posO=0; posX=0;
					victoriasJugador2++;
				}else
					break;
			}else{
				printf("\n*Felicidades: %s has ganado!!\n\n", jugador2);
				volverJugar=validarVolverJugar();
				if(volverJugar==1){
					vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
					vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
					posO=0; posX=0;
					victoriasJugador2++;
				}else
					break;
			}
		}
		
		//validacion de empate
		if(posX+posO==9){ 
			printf("\n*Ha sido un empate! No hay Ganador!\n\n");
			volverJugar=validarVolverJugar();
			pausa();
			limP();
			if(volverJugar==1){
				vaciar(posOcupadasX); //se borra o inicializa en cero todas las posciones del vector
				vaciar(posOcupadasO); //se borra o inicializa en cero todas las posciones del vector
				posO=0; posX=0;
				empates++;
			}else
				break;
		}
		
		if(volverJugar==1){ //si se vuelve a jugar, se rePinta el tablero para empezar en cero para el segundo jugador
			limP();
			if(papel==1){
				ganador=tablero(posOcupadasX, posOcupadasO, papel); //invocacion de la funcion que muestra el tablero
				limP();
				presentacion(jugador1, jugador2,caracter1, caracter2, victoriasJugador1, victoriasJugador2, empates); //invocacion de la funcion que muestra la presentacion
				printf("\n*Revisar el txt\n");
			}else{
				presentacion(jugador1, jugador2,caracter1, caracter2, victoriasJugador1, victoriasJugador2, empates); //invocacion de la funcion que muestra la presentacion
				ganador=tablero(posOcupadasX, posOcupadasO, papel); //invocacion de la funcion que muestra el tablero
			}
		}
		
		//solicitud de las coordenadas del jugador 2
		if(jugarPC==1){ //si se elgicio jugar vs la pc
		
			do{
				fila =rand() % 3; //numeros aleatorios entre 0 y 2
				columna =rand() % 3; //numeros aleatorios entre 0 y 2
				
				posicion=coordenadaAPosicion(fila, columna);

			} while (posValidas(posicion, posOcupadasX, posOcupadasO, posX, posO)!=1); //ciclo que asegura que elija una posicion no ocupada
			
		}else{ //sino..
		
			do{
				printf("\nTurno para: %s\n",jugador2);
				printf("\n*Ingrese la Fila de la posicion a alegir\n->");
				scanf("%i", &fila);
				
				printf("*Ingrese la Columna de la posicion a alegir\n->");
				scanf("%i", &columna);
				
				posicion=coordenadaAPosicion(fila, columna);
				
			}while(fila<0 || columna<0 || fila>2 || columna>2 || posValidas(posicion, posOcupadasX, posOcupadasO, posX, posO)!=1); //valiadacion de las coordenadas
		}
		
		if(caracter2=='x'){
			posOcupadasX[posX]=posicion;
			posX++;
		}else{
			posOcupadasO[posO]=posicion;
			posO++;
		}

		pausa();
		limP();
		
	}
	//***************************** Fin del loop del Juego *******************************************/
	
	printf("\n\n");
	pausa();
}

//validar 

//funcion que muestra la cabecera con los nombres
void presentacion(char jugador1[25], char jugador2[25], char c1, char c2, int victoriasJugador1, int victoriasJugador2, int empates){
	//presentacion
	printf("           Jugador 1 %s con: %c  |VS|  Jugador 2 %s con: %c\n\n", jugador1,c1, jugador2,c2);
	printf("Victorias Jugador1: %i. |||  Victorias Jugador2: %i. ||| Empates: %i\n\n\n", victoriasJugador1,victoriasJugador2, empates);
}

//funcion del tablero 
int tablero(int posOcupadasX[9], int posOcupadasO[9], int papel){
	FILE* archivo=fopen("gato.txt", "wt"); //se abre o se crea el archivo en modo Sobreescritura
	
	//var locales
	int i=0,j=0, k=0;
	
	int control = 0;
	
	int pos1X=0, pos2X=0, pos3X=0, pos4X=0, pos5X=0, pos6X=0, pos7X=0, pos8X=0, pos9X=0;
	int pos1O=0, pos2O=0, pos3O=0, pos4O=0, pos5O=0, pos6O=0, pos7O=0, pos8O=0, pos9O=0;
	
	int ganador=0;
	
	//cargar matriz
	char matriz[FIL][COL]={ 
		"     [0] [1] [2] ",
		"[0] |   |   |   |",
		"    .............",
		"[1] |   |   |   |",
		"    .............",
		"[2] |   |   |   |",
	};
	
	//validaciones de las banderas de las posiciones ocupadas X
	for(k=0;k<9;k++){
		if(posOcupadasX[k]==1){
			pos1X=1;
		}if(posOcupadasX[k]==2){
			pos2X=1;
		}if(posOcupadasX[k]==3){
			pos3X=1;
		}if(posOcupadasX[k]==4){
			pos4X=1;
		}if(posOcupadasX[k]==5){
			pos5X=1;
		}if(posOcupadasX[k]==6){
			pos6X=1;
		}if(posOcupadasX[k]==7){
			pos7X=1;
		}if(posOcupadasX[k]==8){
			pos8X=1;
		}if(posOcupadasX[k]==9){
			pos9X=1;
		}
	}
	
	//validaciones de las banderas de las posiciones ocupadas O
	for(k=0;k<9;k++){
		if(posOcupadasO[k]==1){
			pos1O=1;
		}if(posOcupadasO[k]==2){
			pos2O=1;
		}if(posOcupadasO[k]==3){
			pos3O=1;
		}if(posOcupadasO[k]==4){
			pos4O=1;
		}if(posOcupadasO[k]==5){
			pos5O=1;
		}if(posOcupadasO[k]==6){
			pos6O=1;
		}if(posOcupadasO[k]==7){
			pos7O=1;
		}if(posOcupadasO[k]==8){
			pos8O=1;
		}if(posOcupadasO[k]==9){
			pos9O=1;
		}
	}
	
	int bandera=0;
	
	//mostrar matriz
	for(i=0;i<FIL;i++){
		for(j=0;j<COL;j++){	
			
			//validaciones de acuerdo a la posicion para ocupar un caracter de x o o en la matriz del tablero
			//para las x
			if(pos1X==1 && i==1 && j==6){
				matriz[i][j]='X';
				printf("X");
				
				if(papel==1){
					fputs("X",archivo);
				}
				
				bandera=1;
			}
			
			if(pos2X==1 && i==1 && j==10){
				matriz[i][j]='X';
				printf("X");
				if(papel==1){
					fputs("X",archivo);
				}
				
				bandera=1;
			}
			
			if(pos3X==1 && i==1 && j==14){
				matriz[i][j]='X';
				printf("X");
				
				if(papel==1){
					fputs("X",archivo);
				}
				
				bandera=1;
			}
			
			if(pos4X==1 && i==3 && j==6){
				matriz[i][j]='X';
				printf("X");
				
				if(papel==1){
					fputs("X",archivo);
				}
				
				bandera=1;
			}
			
			if(pos5X==1 && i==3 && j==10){
				matriz[i][j]='X';
				printf("X");
				
				if(papel==1){
					fputs("X",archivo);
				}
				
				bandera=1;
			}
			
			if(pos6X==1 && i==3 && j==14){
				matriz[i][j]='X';
				printf("X");
				
				if(papel==1){
					fputs("X",archivo);
				}
				
				bandera=1;
			}
			
			if(pos7X==1 && i==5 && j==6){
				matriz[i][j]='X';
				printf("X");
				
				if(papel==1){
					fputs("X",archivo);
				}
				
				bandera=1;
			}
			
			if(pos8X==1 && i==5 && j==10){
				matriz[i][j]='X';
				printf("X");
				
				if(papel==1){
					fputs("X",archivo);
				}
				
				bandera=1;
			}
			
			if(pos9X==1 && i==5 && j==14){
				matriz[i][j]='X';
				printf("X");
				
				if(papel==1){
					fputs("X",archivo);
				}
				
				bandera=1;
			}
			
			//para las o
			if(pos1O==1 && i==1 && j==6){
				matriz[i][j]='O';
				printf("O");
				
				if(papel==1){
					fputs("O",archivo);
				}
				
				bandera=1;
			}
			
			if(pos2O==1 && i==1 && j==10){
				matriz[i][j]='O';
				printf("O");
				
				if(papel==1){
					fputs("O",archivo);
				}
				
				bandera=1;
			}
			
			if(pos3O==1 && i==1 && j==14){
				matriz[i][j]='O';
				printf("O");
				
				if(papel==1){
					fputs("O",archivo);
				}
				
				bandera=1;
			}
			
			if(pos4O==1 && i==3 && j==6){
				matriz[i][j]='O';
				printf("O");
				
				if(papel==1){
					fputs("O",archivo);
				}
				
				bandera=1;
			}
			
			if(pos5O==1 && i==3 && j==10){
				matriz[i][j]='O';
				printf("O");
				
				if(papel==1){
					fputs("O",archivo);
				}
				
				bandera=1;
			}
			
			if(pos6O==1 && i==3 && j==14){
				matriz[i][j]='O';
				printf("O");
				
				if(papel==1){
					fputs("O",archivo);
				}
				
				bandera=1;
			}
			
			if(pos7O==1 && i==5 && j==6){
				matriz[i][j]='O';
				printf("O");
				
				if(papel==1){
					fputs("O",archivo);
				}
				
				bandera=1;
			}
			
			if(pos8O==1 && i==5 && j==10){
				matriz[i][j]='O';
				printf("O");
				
				if(papel==1){
					fputs("O",archivo);
				}
				
				bandera=1;
			}
			
			if(pos9O==1 && i==5 && j==14){
				matriz[i][j]='O';
				printf("O");
				
				if(papel==1){
					fputs("O",archivo);
				}
				
				bandera=1;
			}
		
			//si no se imprimio un caracter de x o o se imprime el caracter original en la matriz definida previamente
			if(bandera!=1){ 
				printf("%c",matriz[i][j]);
				
				if(papel==1){
					fputc(matriz[i][j],archivo);
				}
			}
			bandera=0;
		}
		printf("\n");
		
		if(papel==1){
			fputs("\n",archivo);
		}
	}
	
	ganador=validarGanador(matriz);
	
	fclose(archivo);
	return ganador;
}

//valida las posiciones desocupadas 
int posValidas(int pos, int posOcupadasX[9], int posOcupadasO[9], int posX, int posO){
	int i=0;
	//validar posiciones ocupadas por X
	for (i = 0; i < posX; i++){

		if (pos == posOcupadasX[i]){
			return 0;
		}
	
	}
	
	//validar posiciones ocupadas por O
	for (i = 0; i < posO; i++){

		if (pos == posOcupadasO[i]){
			return 0;
		}
	
	}

	return 1;
}

/**********************************inicio funcion ganador**************************************/
int validarGanador(char matriz[FIL][COL]){
	
	//validar desde la posicion 1 las 3 posiciones de su final y columna
	if(matriz[1][6] == 'X' || matriz[1][6] == 'O'){
		if(matriz[1][6] == matriz[1][10] && matriz[1][10] == matriz[1][14]){
			if(matriz[1][6] == 'X'){
				return 0;
			}
			else{
				return 1; 
			}
		}
		if(matriz[1][6] == matriz[3][6] && matriz[3][6] == matriz[5][6]){
			if(matriz[1][6] == 'O'){
				return 1; 
			}
			else{
				return 0; 
			}
		}
	}
	
	//validar desde el centro las diagolaes su fila y su columna
	if(matriz[3][10] == 'X' || matriz[3][10] == 'O'){
		//validar  la diagonal principal
		if(matriz[3][10] == matriz[1][6] && matriz[1][6] == matriz[5][14]){
			if(matriz[3][10] == 'X'){
				return 0;
			}
			else{
				return 1;
			
			}
		}
		//validar fila central
		if(matriz[3][10] == matriz[3][6] && matriz[3][6] == matriz[3][14]){
			if(matriz[3][10] == 'X'){
				return 0; 
			}
			else{
				return 1; 
			
			}
		}
		//validar diagonal secundaria
		if(matriz[3][10] == matriz[5][6] && matriz[5][6] == matriz[1][14]){
			if(matriz[3][10] == 'X'){
				return 0; 
			}
			else{
				return 1; 
			
			}
		}
		//validar columna central
		if(matriz[3][10] == matriz[1][10] && matriz[1][10] == matriz[5][10]){
			if(matriz[3][10] == 'X'){
				return 0;
			}
			else{
				return 1;
			
			}
		}
	}
	
	//validar desde la posicion 9 las 3 posiciones de su final y columna
	if(matriz[5][14] == 'X' || matriz[5][14] == 'O'){
		if(matriz[5][14] == matriz[5][6] && matriz[5][6] == matriz[5][10]){
			if(matriz[5][14] == 'X'){
				return 0; 
			}
			else{
				return 1; 
			
			}
		}
		if(matriz[5][14] == matriz[1][14] && matriz[1][14] == matriz[3][14]){
			if(matriz[5][14] == 'X'){
				return 0; 
			}
			else{
				return 1;
			
			}
		}
		
	} 
	return 2;
}
/**********************************fin funcion ganador**************************************/

//pasar de posicion a coordenadas
void posicionACoordenadas(int *f, int *c, int pos){

	switch (pos){
		
		case 1:
			*f = 0;
			*c = 0;
			break;
		case 2:
			*f = 0;
			*c = 1;
			break;
		case 3:
			*f = 0;
			*c = 2;
			break;
		case 4:
			*f = 1;
			*c = 0;
			break;
		case 5:
			*f = 1;
			*c = 1;
			break;
		case 6:
			*f = 1;
			*c = 2;
			break;
		case 7:
			*f = 2;
			*c = 0;
			break;
		case 8:
			*f = 2;
			*c = 1;
			break;
		case 9:
			*f = 2;
			*c = 2;
			break;
		default:
			break;
	}
}

//funcion que pasa de coordenada a posicion
int coordenadaAPosicion(int f,int c){
	int pos=0;
	
	if(f==0 && c==0)
		pos=1;
	else if(f==0 && c==1)
		pos=2;
	else if(f==0 && c==2)
		pos=3;
	else if(f==1 && c==0)
		pos=4;
	else if(f==1 && c==1)
		pos=5;
	else if(f==1 && c==2)
		pos=6;
	else if(f==2 && c==0)
		pos=7;
	else if(f==2 && c==1)
		pos=8;
	else if(f==2 && c==2)
		pos=9;
	
	return pos;
}

//funcion que valida si se desea volver a jugar con los mismo alias
int validarVolverJugar(){
	int volverJugar=0;
	
	clean_stdin();
	
	do{
		printf("\n*Ingrese 1 si desea continuar jugando, 0 para salir\n->");
		scanf("%i", &volverJugar);
		
	}while(volverJugar<0 || volverJugar>1);
	
	clean_stdin();
	
	return volverJugar;
}

// vaciar array
void vaciar(int posOcupadas[9])
{
	int i=0;
	for (i = 0; i < 9; i++)
	{
		posOcupadas[i] = 0;
	}
}

//limpia la pantalla
void limP()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

//pausa al sistema
void pausa()
{
	printf("\n\nPresione una tecla para continuar...\n");
	getchar();
}

//metodo que limpia el buffer
void clean_stdin(void)
{
	int c;
	do
	{
		c = getchar();
	} while (c != '\n' && c != EOF);
}
