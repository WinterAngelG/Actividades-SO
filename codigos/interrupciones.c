#include <stdio.h>
#include <string.h>
#include <unistd.h>  

char buffer[100];

typedef struct Dispositivo
{
    int id;
    char nombreDispositivo[50];
    int interrupcion;
    char memoria[100];
} Dispositivo;

typedef struct Controladora
{
    int ocupado;
    int operacion;
    char datos[100];
    Dispositivo Dispositivo;
} Controladora;

typedef struct TablaEstados
{
    Dispositivo dispositivo;
    int estado;
    int operacion;
} TablaEstados;

typedef struct Driver
{
    Controladora controladora;
    int operacion;
} Driver;

Driver driverUSB;
Controladora controladoraUSB;
Dispositivo USB;
TablaEstados tablaEstados;

void inicializacion()
{
    strcpy(USB.nombreDispositivo, "USB de ANGEL");
    strcpy(USB.memoria, "SOY LA INFORMACION DE LECTURA");
    USB.id = 1;
    USB.interrupcion = 0;

    driverUSB.controladora = controladoraUSB;
    driverUSB.operacion = -1;

    driverUSB.controladora.ocupado = 1;    
    driverUSB.controladora.operacion = -1;
    driverUSB.controladora.Dispositivo = USB;

    tablaEstados.dispositivo = USB;
    tablaEstados.estado = 0;
    tablaEstados.operacion = -1;
}

void interrupcion()
{
    printf("\nInterrupción generada, La operación del dispositivo %s ha finalizado.\n", USB.nombreDispositivo);

    if (tablaEstados.estado == 1)
    {
        printf("Manejando interrupción para el dispositivo %s...\n", tablaEstados.dispositivo.nombreDispositivo);
        sleep(1);
        if (tablaEstados.operacion == 0)
        {
            for (int i = 0; i < 3; i++)
            {
                printf("Atendiendo interrupcion de lectura...\n");
                sleep(1);
            }
        }
        else if (tablaEstados.operacion == 1)
        {
            for (int i = 0; i < 3; i++)
            {
                printf("Atendiendo interrupcion de escritura...\n");
                sleep(1);
            }
        }

        tablaEstados.estado = 0;
        printf("EL dispositivo %s termino de ejecutar las IRS.\n", tablaEstados.dispositivo.nombreDispositivo);
    }
}

void lectura()
{
    if (driverUSB.controladora.ocupado != 0)
    {
        printf("El dispositivo ya se encuentra en ocupado(en uso por un proceso). Espera a que se desocupe\n");
        for(int i = 0; i <3; i++){
            printf("\nEsperando...");
            sleep(1);
        }
        driverUSB.controladora.ocupado = 0;

        printf("\nEl dispositivo esta libre para recibir ordenes. Iniciando Dispositivo: %s", USB.nombreDispositivo);
    }

    driverUSB.operacion = 0;
    driverUSB.controladora.operacion = 0;
    tablaEstados.operacion = 0;
    driverUSB.controladora.ocupado = 1;

    tablaEstados.estado = 1;

    for (int i = 0; i < 3; i++)
    {
        printf("Leyendo datos de la usb.....\n");
        sleep(1);
    }

    strcpy(buffer, USB.memoria);
    printf("Datos leidos y almacenados en el buffer: %s\n", buffer);

    interrupcion();

    driverUSB.controladora.ocupado = 0;
}

void escritura()
{
    if (driverUSB.controladora.ocupado != 0)
    {
        printf("El dispositivo ya se encuentra en ocupado(en uso por un proceso)\n");
        printf("Esperando a que se desocupe...\n");
        sleep(1);
    }

    driverUSB.operacion = 1;
    driverUSB.controladora.operacion = 1;
    tablaEstados.operacion = 1;
    driverUSB.controladora.ocupado = 1;

    for (int i = 0; i < 5; i++)
    {
        printf("Escribiendo datos hacia la usb.....\n");

        strcpy(buffer, "SOY INFORMACION DE ESCITURA ");
        strcpy(driverUSB.controladora.datos, buffer);
        strcpy(USB.memoria, driverUSB.controladora.datos);
        sleep(1);
    }

    driverUSB.controladora.ocupado = 0;
}

void mostrarINFOUSB()
{
    printf("El proceso terminó de usar la USB. \nLa información final que tiene es: %s\n", USB.memoria);
    sleep(2);
    printf("Terminando simulación....\n");
    sleep(5);

    printf("Simulación terminada.");
}

int main()
{
    inicializacion();

    printf("Iniciando simulación.....\n");
    sleep(1);
    printf("\nProceso solicita lectura de la información del dispositivo %s\n", USB.nombreDispositivo);
    sleep(1);
    printf("\nDriver recibe solicitud de lectura.\n");
    sleep(1);

    lectura();

    printf("\nLa información que tiene el buffer en este momento es: %s\n", buffer);
    sleep(2);

    printf("\nProceso solicita escribir información en el dispositivo %s\n", USB.nombreDispositivo);
    sleep(1);
    printf("\nDriver recibe solicitud de escritura.\n");
    sleep(1);

    escritura();

    mostrarINFOUSB();
}