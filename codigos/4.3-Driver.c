//Esta es una variacion del codigo del 4.2 con estructuras y manejo de varios dispositivos

#include <stdio.h>
#include <string.h>
#include <unistd.h>  

#define cantidadDispositivos 3

char buffer[100];

typedef struct Dispositivo {
    int id;
    char nombreDispositivo[50];
    int interrupcion;
    char memoria[100];
} Dispositivo;

typedef struct Controladora {
    int ocupado;
    int operacion;
    char datos[100];
    Dispositivo dispositivo;
} Controladora;

typedef struct TablaEstados {
    Dispositivo dispositivo;
    int estado;
    int operacion;
} TablaEstados;

typedef struct Driver {
    Controladora controladora;
    int operacion;
} Driver;

Dispositivo dispositivos[cantidadDispositivos];
Driver driverUSB[cantidadDispositivos];
Controladora controladoras[cantidadDispositivos];
TablaEstados tablaEstados[cantidadDispositivos];

void inicializacion() {
    for (int i = 0; i < cantidadDispositivos; i++) {
        dispositivos[i].id = i + 1;
        sprintf(dispositivos[i].nombreDispositivo, "DISCO DURO:  %d", i + 1);
        strcpy(dispositivos[i].memoria, "SOY LA INFORMACION DE LECTURA");
        dispositivos[i].interrupcion = 0;

        driverUSB[i].controladora = controladoras[i];
        driverUSB[i].operacion = -1;
        driverUSB[i].controladora.ocupado = 0;    
        driverUSB[i].controladora.operacion = -1;
        driverUSB[i].controladora.dispositivo = dispositivos[i];

        tablaEstados[i].dispositivo = dispositivos[i];
        tablaEstados[i].estado = 0;
        tablaEstados[i].operacion = -1;
    }
}

void interrupcion(int i) {
    printf("\nInterrupción generada, La operación del dispositivo %s ha finalizado.\n", dispositivos[i].nombreDispositivo);

    if (tablaEstados[i].estado == 1) {
        printf("Manejando interrupción para el dispositivo %s...\n", tablaEstados[i].dispositivo.nombreDispositivo);
        sleep(1);
        if (tablaEstados[i].operacion == 0) {
            for (int j = 0; j < 3; j++) {
                printf("Atendiendo interrupción de lectura...\n");
                sleep(1);
            }
        }
        else if (tablaEstados[i].operacion == 1) {
            for (int j = 0; j < 3; j++) {
                printf("Atendiendo interrupción de escritura...\n");
                sleep(1);
            }
        }

        tablaEstados[i].estado = 0;
        printf("EL dispositivo %s terminó de ejecutar las IRS.\n", tablaEstados[i].dispositivo.nombreDispositivo);
    }
}

void lectura(int i) {
    if (driverUSB[i].controladora.ocupado != 0) {
        printf("El dispositivo %s ya se encuentra ocupado. Espera a que se desocupe...\n", dispositivos[i].nombreDispositivo);
        for(int j = 0; j < 3; j++){
            printf("\nEsperando...");
            sleep(1);
        }
        driverUSB[i].controladora.ocupado = 0;
        printf("\nEl dispositivo %s está libre para recibir órdenes. Iniciando Dispositivo...\n", dispositivos[i].nombreDispositivo);
    }

    driverUSB[i].operacion = 0;
    driverUSB[i].controladora.operacion = 0;
    tablaEstados[i].operacion = 0;
    driverUSB[i].controladora.ocupado = 1;

    tablaEstados[i].estado = 1;

    for (int j = 0; j < 3; j++) {
        printf("Leyendo datos de la %s.....\n", dispositivos[i].nombreDispositivo);
        sleep(1);
    }

    strcpy(buffer, dispositivos[i].memoria);
    printf("Datos leídos y almacenados en el buffer: %s\n", buffer);

    interrupcion(i);

    driverUSB[i].controladora.ocupado = 0;
}

void escritura(int i) {
    if (driverUSB[i].controladora.ocupado != 0) {
        printf("El dispositivo %s ya se encuentra ocupado.\n", dispositivos[i].nombreDispositivo);
        printf("Esperando a que se desocupe...\n");
        sleep(1);
    }

    driverUSB[i].operacion = 1;
    driverUSB[i].controladora.operacion = 1;
    tablaEstados[i].operacion = 1;
    driverUSB[i].controladora.ocupado = 1;

    for (int j = 0; j < 5; j++) {
        printf("Escribiendo datos hacia la %s.....\n", dispositivos[i].nombreDispositivo);

        strcpy(buffer, "SOY INFORMACION DE ESCRITURA ");
        strcpy(driverUSB[i].controladora.datos, buffer);
        strcpy(dispositivos[i].memoria, driverUSB[i].controladora.datos);
        sleep(1);
    }

    driverUSB[i].controladora.ocupado = 0;
}

void mostrarINFOUSB(int i) {
    printf("El proceso terminó de usar el dispositivo %s. \nLa información final que tiene es: %s\n", dispositivos[i].nombreDispositivo, dispositivos[i].memoria);
    sleep(1);
}

int main() {
    inicializacion();

    printf("Iniciando simulación.....\n");
    sleep(1);

    for (int i = 0; i < cantidadDispositivos; i++) {
        printf("\nProceso solicita lectura de la información del dispositivo %s\n", dispositivos[i].nombreDispositivo);
        sleep(1);
        printf("\nDriver recibe solicitud de lectura.\n");
        sleep(1);

        lectura(i);

        printf("\nLa información que tiene el buffer en este momento es: %s\n", buffer);
        sleep(1);

        printf("\nProceso solicita escribir información en el dispositivo %s\n", dispositivos[i].nombreDispositivo);
        sleep(1);
        printf("\nDriver recibe solicitud de escritura.\n");
        sleep(1);

        escritura(i);

        mostrarINFOUSB(i);
    }

    printf("Terminando simulación....\n");
    sleep(1);

    printf("Simulación terminada.\n");

    return 0;
}
