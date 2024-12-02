#include <stdio.h>

typedef struct _nodo
{
    int id;
    int tamano;
} nodo;

void asignarProceso(int numProceso, int tamProceso, int length, nodo particiones[])
{
    int bandera = 0;
    printf("\nIngresa el id del proceso: ");
    scanf("%d", &numProceso);

    printf("\nIngresa el tamano del proceso: ");
    scanf("%d", &tamProceso);

    for (int i = 0; i < length; i++)
    {
        if (tamProceso <= particiones[i].tamano && particiones[i].id == -1)
        {
            particiones[i].id = numProceso;
            printf("Se asigno el proceso %d a la particion %d \n\n", particiones[i].id, i + 1);
            bandera = 1;
            break;
        }
    }

    if (!bandera)
    {
        printf("\nNo hay espacio suficiente para asignar el proceso\n\n");
    }
}

void eliminarProceso(int length, nodo particiones[])
{
    int numProceso;
    printf("\nIngresa el id del proceso a liberar: ");
    scanf("%d", &numProceso);

    for (int i = 0; i < length; i++)
    {
        if (particiones[i].id == numProceso)
        {
            particiones[i].id = -1;
            printf("Se libero el proceso con id %d de la particion %d\n", numProceso, i + 1);
            return;
        }
    }

    printf("No se encontro un proceso con el id %d\n", numProceso);
}

void mostrarMemoria(int length, nodo particiones[])
{
    for (int i = 0; i < length; i++)
    {
        if (particiones[i].id != -1)
        {
            printf("Particion %d: Proceso %d (tamano: %d KB)\n", i + 1, particiones[i].id, particiones[i].tamano);
        }
        else
        {
            printf("Particion %d: Libre (tamano: %d KB)\n", i + 1, particiones[i].tamano);
        }
    }
}

int main()
{
    int maxMemoria, numParticiones, tamParticion;
    printf("Ingresa el tamano total de la memoria (KB): ");
    scanf("%d", &maxMemoria);

    printf("Ingresa el numero de particiones: ");
    scanf("%d", &numParticiones);

    nodo particiones[numParticiones];
    printf("Ingresa el tamano de cada particion (KB): ");
    scanf("%d", &tamParticion);

    if (tamParticion * numParticiones > maxMemoria)
    {
        printf("Error: Las particiones exceden el tamano total de la memoria\n");
        return 1;
    }

    for (int i = 0; i < numParticiones; i++)
    {
        particiones[i].id = -1;
        particiones[i].tamano = tamParticion;
    }

    int opcion;
    do
    {
        printf("\n1. Asignar proceso\n2. Liberar proceso\n3. Mostrar memoria\n4. Salir\n");
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            asignarProceso(0, 0, numParticiones, particiones);
            break;
        case 2:
            eliminarProceso(numParticiones, particiones);
            break;
        case 3:
            mostrarMemoria(numParticiones, particiones);
            break;
        case 4:
            printf("Saliendo...\n");
            break;
        default:
            printf("Opcion invalida\n");
            break;
        }
    } while (opcion != 4);

    return 0;
}