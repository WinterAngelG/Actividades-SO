#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define tamanoMemoria 50
#define procesosMaximos 500
#define numeroMarcos 5

int contador = 0;

typedef struct Proceso
{
    int id;
    int tamMemoria;
} Proceso;

typedef struct Marco
{
    int numMarco;
    int libre;
    int procesoID;
} Marco;

Marco memoriaPrincipal[numeroMarcos];
Marco memoriaSecundaria[numeroMarcos];
Proceso procesos[procesosMaximos];

void inicializar()
{
    for (int i = 0; i < numeroMarcos; i++)
    {
        memoriaPrincipal[i].numMarco = i;
        memoriaPrincipal[i].libre = 1;
        memoriaSecundaria[i].numMarco = i;
        memoriaSecundaria[i].libre = 1;
    }
}

void intercambioFIFO()
{
    int procesoASwapear = memoriaPrincipal[0].procesoID;

    if (procesoASwapear == 0)
    {
        printf("\nNo hay procesos para intercambiar.\n");
        return;
    }

    for (int i = 0; i < numeroMarcos; i++)
    {
        if (memoriaSecundaria[i].libre)
        {
            memoriaSecundaria[i].procesoID = procesoASwapear;
            memoriaSecundaria[i].libre = 0;
            printf("\nProceso %d intercambiado a memoria secundaria (Marco %d).\n", procesoASwapear, memoriaSecundaria[i].numMarco);
            break;
        }
    }

    for (int i = 0; i < numeroMarcos - 1; i++)
    {
        memoriaPrincipal[i] = memoriaPrincipal[i + 1];
        memoriaPrincipal[i].numMarco = i;
    }

    memoriaPrincipal[numeroMarcos - 1].libre = 1;
    memoriaPrincipal[numeroMarcos - 1].procesoID = 0;
    memoriaPrincipal[numeroMarcos - 1].numMarco = numeroMarcos - 1;

    printf("\nMemoria principal ajustada tras el intercambio.\n");
}

void asignarProceso()
{
    if (contador >= procesosMaximos)
    {
        printf("\nError: No se pueden asignar más procesos a la memoria. Iniciando intercambio (Swapping) de procesos por FIFO...\n");
    }
    else
    {
        for (int i = 0; i < numeroMarcos; i++)
        {
            if (memoriaPrincipal[i].libre == 1)
            {
                memoriaPrincipal[i].procesoID = procesos[contador].id;
                memoriaPrincipal[i].libre = 0;
                printf("\nSe cargó el proceso %d en el marco %d.\n", memoriaPrincipal[i].procesoID, memoriaPrincipal[i].numMarco);
                return;
            }
        }
        printf("\nMemoria RAM llena. Iniciando proceso de swapping...\n");
        intercambioFIFO();
        asignarProceso();
    }
}

void creaProceso()
{
    if (contador >= procesosMaximos)
    {
        printf("\nNo se pueden crear más procesos. Límite alcanzado.\n");
        return;
    }

    Proceso nuevo;
    nuevo.id = contador + 1;
    nuevo.tamMemoria = rand() % 101 + 10;

    procesos[contador] = nuevo;
    printf("\nProceso creado: ID %d, Tamaño %d.\n", nuevo.id, nuevo.tamMemoria);
    asignarProceso();
    contador++;
}

void mostrarMemoria(int control)
{
    if (control == 1)
    {
        printf("\nMemoria Principal:\n");
        for (int i = 0; i < numeroMarcos; i++)
        {
            if (memoriaPrincipal[i].libre)
            {
                printf("Marco %d: Libre\n", memoriaPrincipal[i].numMarco);
            }
            else
            {
                printf("Marco %d: Proceso %d\n", memoriaPrincipal[i].numMarco, memoriaPrincipal[i].procesoID);
            }
        }
    }
    else
    {
        printf("\nMemoria Secundaria:\n");
        for (int i = 0; i < numeroMarcos; i++)
        {
            if (memoriaSecundaria[i].libre)
            {
                printf("Marco %d: Libre\n", memoriaSecundaria[i].numMarco);
            }
            else
            {
                printf("Marco %d: Proceso %d\n", memoriaSecundaria[i].numMarco, memoriaSecundaria[i].procesoID);
            }
        }
    }
}

int main()
{
    srand(time(NULL));
    inicializar();

    int eleccion;

    do
    {
        printf("\n\n    || MENU ||\n");
        printf("1.- Generar proceso\n");
        printf("2.- Mostrar estado de memoria principal\n");
        printf("3.- Mostrar estado de memoria secundaria\n");
        printf("4.- Salir\n");
        printf("Elección: ");
        scanf("%d", &eleccion);

        switch (eleccion)
        {
        case 1:
            creaProceso();
            break;
        case 2:
            mostrarMemoria(1);
            break;
        case 3:
            mostrarMemoria(0);
            break;
        case 4:
            printf("Saliendo...\n");
            break;
        default:
            printf("No existe esa opción.\n");
        }

    } while (eleccion != 4);

    return 0;
}
