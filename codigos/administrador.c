// Escribe un programa en c o python que simule la administracion de memoria mediante particiones fijas

#include <stdio.h>

// estructura de un proceso con id y tamaño
typedef struct _nodo
{
    int id;
    int tamaño;

} nodo;

void asignarProceso(int numProceso, int tamProceso, int length, nodo particiones[])
{
    int bandera = 0; // Bandera para comprobar si hay alguna particion con el espacio suficiente para asignar un proceso.
    printf("\nIngresa el id del proceso: ");
    scanf("%d", &numProceso);

    printf("\nIngresa el tamaño del proceso: ");
    scanf("%d", &tamProceso);

    for (int i = 0; i < length; i++)
    {
        if (tamProceso <= particiones[i].tamaño && particiones[i].id == -1 && bandera == 0)
        {

            particiones[i].id = numProceso;
            printf("Se asigno el proceso %d a la particion %d \n\n", particiones[i].id, i + 1);
            bandera = 1;
        }
    }

    if (!bandera)
    {
        printf("\nNo hay espacio en memoria suficiente para asignar\n\n");
    }
}

void eliminarProceso(int length, nodo particiones[])
{
    int memoriaLibre = 1;
    for (int i = 0; i < length; i++)
    {
        if (particiones[i].id != -1)
        {
            memoriaLibre = 0;
            break;
        }
    }
    if (memoriaLibre)
    {
        printf("No hay procesos cargados en memoria.\n");
    }

    int numProceso;
    int bandera; // bandera para verificar si hay un proceso en memoria con ese id
    printf("\nIngresa el id del proceso:");
    bandera = 0;
    scanf("%d", &numProceso);
    for (int i = 0; i < length; i++)
    {
        if (particiones[i].id == numProceso)
        {

            int aux = particiones[i].id;
            particiones[i].id = -1;
            printf("Se libero el proceso con id %d de la partición %d", aux, i + 1);
            bandera = 1;
            break;
        }
    }

    if (!bandera)
    {
        printf("No hay proceso con el id ingresado. Intente de nuevo \n\n");
    }
}

void mostrarMemoria(int length, nodo particiones[])
{

    for (int i = 0; i < length; i++)
    {
        if (particiones[i].id != -1)
        {
            printf("\nLa partición %d esta ocupada por el proceso %d\n\n", i + 1, particiones[i].id);
        }
        else
        {
            printf("La particion %d esta desocupada\n\n", i + 1);
        }
    }
}

int main()
{
    int eleccion = 0, maxMemoria = 0, numParticiones = 0, numProceso = 0, tamProceso = 0, tamTotal = 0, tamParticion = 0;

    printf("Ingresa el tamaño de la memoria: (KB): ");
    scanf("%d", &maxMemoria);

    printf("Ingrese la cantidad de particiones:");
    scanf("%d", &numParticiones);

    nodo particiones[numParticiones];                          // arreglo de particiones fijas que simula la memoria
    int length = sizeof(particiones) / sizeof(particiones[0]); // length del arreglo

    int bandera = 0;
    do
    {
        tamTotal = 0;
        printf("\nIngresa el tamaño que tendran las particiones (KB): ");
        scanf("%d", &tamParticion);

        for (int i = 0; i < numParticiones; i++) // guarda en cada una de las particiones el mismo tamaño de particiones
        {
            particiones[i].id = -1;
            particiones[i].tamaño = tamParticion;
            tamTotal += particiones[i].tamaño;
        }

        if (tamTotal > maxMemoria)
        {
            printf("\nEl tamaño fijo de todas las particiones(%d KB) es mayor al tamaño maximo de memoria(%d KB). Por favor ingresa un tamaño fijo para las particiones menor.\n\n", tamTotal, maxMemoria);
        }
        else
        {
            bandera = 1;
        }
    } while (bandera == 0);

    printf("La memoria fisica tendrá un tamaño de %d KB divido en %d particiones de %d KB\n", maxMemoria, numParticiones, tamParticion);
    while (eleccion != 4)
    {
        printf("       ||MENU||      ");
        printf("\n1.- Asignar proceso");
        printf("\n2.- Liberar proceso");
        printf("\n3.- Mostrar estado de la memoria\n\n");
        printf("\n4.- Salir\n\n");
        printf("Opción:");
        scanf("%d", &eleccion);

        switch (eleccion)
        {

        case 1:
            asignarProceso(numProceso, tamProceso, length, particiones);
            break;
        case 2:
            eliminarProceso(length, particiones);
            break;
        case 3:
            mostrarMemoria(length, particiones);
            break;
        case 4:
            break;
        default:
            break;
        }
    }

    return 0;
}