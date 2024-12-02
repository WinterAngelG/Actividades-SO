#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char buffer[100]; 

typedef struct Dispositivo{
    int id;
    char nombreDispositivo[50];
    int interrupcion;
    char memoria[100]; //simula la meoria de la usb donde guarda la info
    

}Dispositivo;

typedef struct Controladora 
{   
    int ocupado;
    int operacion; // el driver le manda el tipo de operacion a la controladora
    char datos[100]; // la controladora guarda los datos del dispositivo real
    Dispositivo Dispositivo;
}Controladora;


typedef struct Driver{

    Controladora controladora;
    int operacion; //0.- lectura 2.- escritura, etc.
}Driver;

Driver driverUSB;
Controladora controladoraUSB;
Dispositivo USB;

void inicializacion(){

    strcpy(USB.nombreDispositivo, "USB de LALO");
    strcpy(USB.memoria, "Hola a todos putos.");
    USB.id = 1;
    USB.interrupcion = 0;

    driverUSB.controladora = controladoraUSB;
    driverUSB.operacion = -1; // No esta recibiendo ninguna operacion por parte del proceso
    
    controladoraUSB.ocupado = 0; // esta libre 
    controladoraUSB.operacion = -1; // no tiene niguna operacion 
    controladoraUSB.Dispositivo = USB;
}


void lectura(){
    if(driverUSB.controladora.ocupado != 0){
        printf("El dispositivo ya se encuentra en ocupado(en uso por un proceso)\n");
        sleep(5);
    }

    driverUSB.operacion = 0;
    driverUSB.controladora.operacion = 0;
    driverUSB.controladora.ocupado = 1;

    for(int i = 0; i < 5; i++){

        printf("Leyendo datos de la usb.....\n");
        strcpy(buffer,USB.memoria); //se guarda la info en el buffer del proceso para que sea usada.
        
        sleep(1);
    }

    driverUSB.controladora.ocupado = 0;

}

void escritura(){
     if(driverUSB.controladora.ocupado != 0){
        printf("El dispositivo ya se encuentra en ocupado(en uso por un proceso)\n");
        printf("Esperando a que se desocupe...\n");
        sleep(5);
    }

    driverUSB.operacion = 1; // la operacion es de escritura.
    driverUSB.controladora.operacion = 1; // el driver le dice pasa la operacion de escritura a la controladora para que lo ejecute sobre el dispositivo.
    driverUSB.controladora.ocupado = 1;

    for(int i = 0; i < 5; i++){

        printf("Escribiendo datos hacia la usb.....\n");

        strcpy(buffer, "INFORMACION DE ESCRITURA JIJI."); //El proceso guarda los datos que quiere guardar en el dispositivo fisico a traves del buffer
        strcpy(driverUSB.controladora.datos, buffer); // La informacion pasa del buffer al registro de datos de la controladora.
        strcpy(USB.memoria, driverUSB.controladora.datos); //Una vez que la info esta en la controladora la pasa a la memoria USB.
        sleep(2);
    }

    driverUSB.controladora.ocupado = 0;

}

void mostrarINFOUSB(){

    printf("El proceso termino de usar la USB. \nLa informacion final que tiene es: %s\n", USB.memoria);
    sleep(2);
    printf("Terminando simulacion....putos perros ya estoy hasta la verga.\n");
    sleep(5);

    printf("Simulacion  terminada.");
    
}


int main() {

    inicializacion();

    printf("Iniciando simulacion.....\n");
    sleep(1);
    printf("Proceso solicita lectura de la informacion del dispositivo %s\n", USB.nombreDispositivo);
    sleep(1);
    printf("Driver recibe solicitud de lectura.\n");
    sleep(1);
    
    lectura();

    printf("La informacion que tiene el buffer en este momento es: %s\n", buffer);
    sleep(2);

    printf("Proceso solicita escribir informacion en el dispositivo %s\n", USB.nombreDispositivo);
    sleep(1);
    printf("Driver recibe solicitud de escritura.\n");
    sleep(1);
    
    escritura();

    mostrarINFOUSB();
}
