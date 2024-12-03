#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESOS 20

int numProcesos;

typedef struct Marco
{
    int numMarco;
    int libre;
    int procesoID;
    int paginaID;
    int tiempoUso;
} Marco;

typedef struct Pagina
{
    int numMarco;
    int referencia;
    int modificado;
    int presente;
    int pagina;
} Pagina;

typedef struct Proceso
{
    int id;
    int tamano;
    int paginas;
    Pagina *tablaPaginas;
} Proceso;

void crearProceso(Proceso procesos[], int tamMarco)
{
    if (numProcesos >= MAX_PROCESOS)
    {
        printf("Error: no se pueden crear más procesos.\n");
        return;
    }

    Proceso *nuevo = &procesos[numProcesos];
    numProcesos++;
    printf("Ingresa el ID del proceso: ");
    scanf("%d", &nuevo->id);
    printf("Ingresa el tamaño del proceso: ");
    scanf("%d", &nuevo->tamano);

    nuevo->paginas = (nuevo->tamano + tamMarco - 1) / tamMarco;
    nuevo->tablaPaginas = (Pagina*)calloc(nuevo->paginas, sizeof(Pagina));

    for (int i = 0; i < nuevo->paginas; i++)
    {
        nuevo->tablaPaginas[i].numMarco = -1;
        nuevo->tablaPaginas[i].presente = 0;
        nuevo->tablaPaginas[i].referencia = 0;
        nuevo->tablaPaginas[i].modificado = 0;
        nuevo->tablaPaginas[i].pagina = i;
    }

    printf("Proceso creado con ID %d y %d páginas.\n", nuevo->id, nuevo->paginas);
}

void asignarPagina(Proceso procesos[], Marco memoria[], int numMarcos)
{
    int id, pagina;
    printf("Ingresa el ID del proceso: ");
    scanf("%d", &id);
    printf("Ingresa la página a acceder (de 0 a n): ");
    scanf("%d", &pagina);

    for (int i = 0; i < numProcesos; i++)
    {
        if (procesos[i].id == id)
        {
            if (pagina >= procesos[i].paginas)
            {
                printf("Error: la página %d no existe.\n", pagina);
                return;
            }

            if (procesos[i].tablaPaginas[pagina].presente)
            {
                printf("La página %d ya está en memoria (marco %d).\n",
                       pagina, procesos[i].tablaPaginas[pagina].numMarco);

                for (int j = 0; j < numMarcos; j++)
                {
                    if (!memoria[j].libre)
                    {
                        memoria[j].tiempoUso++;
                    }
                }

                memoria[procesos[i].tablaPaginas[pagina].numMarco].tiempoUso = 0;
                return;
            }

            for (int j = 0; j < numMarcos; j++)
            {
                if (memoria[j].libre)
                {
                    memoria[j].libre = 0;
                    memoria[j].procesoID = id;
                    memoria[j].paginaID = pagina;
                    memoria[j].tiempoUso = 0;

                    procesos[i].tablaPaginas[pagina].numMarco = j;
                    procesos[i].tablaPaginas[pagina].presente = 1;

                    for (int k = 0; k < numMarcos; k++)
                    {
                        if (!memoria[k].libre)
                        {
                            memoria[k].tiempoUso++;
                        }
                    }

                    printf("Página %d del proceso %d cargada en el marco %d.\n", pagina, id, j);
                    return;
                }
            }

            printf("Fallo de página: no hay marcos disponibles. Se usará el algoritmo de LRU para reemplazar la página\n");

            int minUso = memoria[0].tiempoUso;
            int minIndex = 0;

            for (int j = 1; j < numMarcos; j++)
            {
                if (memoria[j].tiempoUso < minUso)
                {
                    minUso = memoria[j].tiempoUso;
                    minIndex = j;
                }
            }

            printf("Reemplazando la página %d del proceso %d en el marco %d.\n", memoria[minIndex].paginaID, memoria[minIndex].procesoID, minIndex);
            memoria[minIndex].procesoID = id;
            memoria[minIndex].paginaID = pagina;
            memoria[minIndex].tiempoUso = 0;

            procesos[i].tablaPaginas[pagina].numMarco = minIndex;
            procesos[i].tablaPaginas[pagina].presente = 1;
            return;
        }
    }

    printf("Error: proceso no encontrado.\n");
}

void eliminarPagina(Proceso procesos[], Marco memoria[], int numMarcos)
{
    int id, pagina;
    printf("Ingresa el ID del proceso: ");
    scanf("%d", &id);
    printf("Ingresa la página a eliminar (de 0 a n): ");
    scanf("%d", &pagina);

    for (int i = 0; i < numProcesos; i++)
    {
        if (procesos[i].id == id)
        {
            if (pagina >= procesos[i].paginas)
            {
                printf("Error: la página %d no existe.\n", pagina);
                return;
            }

            if (procesos[i].tablaPaginas[pagina].presente)
            {
                memoria[procesos[i].tablaPaginas[pagina].numMarco].libre = 1;
                procesos[i].tablaPaginas[pagina].numMarco = -1;
                procesos[i].tablaPaginas[i].presente = 0;
                procesos[i].tablaPaginas[i].referencia = 0;
                procesos[i].tablaPaginas[i].modificado = 0;
                printf("Se elimino la pagina exitosamente.");
                return;
            }
            else
            {
                printf("\nError la pagina %d no esta cargada en memoria. Debes cargarla primero para poder eliminarla",pagina);
                return;
            }
        }
    }

    printf("Error: proceso no encontrado.\n");
}

void mostrarMemoria(Marco memoria[], int numMarcos)
{
    printf("\nEstado de la memoria física:\n");
    for (int i = 0; i < numMarcos; i++)
    {
        if (memoria[i].libre)
        {
            printf("Marco %d: Libre\n", i);
        }
        else
        {
            printf("Marco %d: Proceso %d, Página %d, Tiempo de uso: %d\n", i, memoria[i].procesoID, memoria[i].paginaID, memoria[i].tiempoUso);
        }
    }
}

int main()
{
    Proceso procesos[MAX_PROCESOS];
    int contador = 0;
    printf("NOTA: El numero de marcos se calculará de forma automatica(tamaño de la memoria / tamaño de marco).\n");
    printf("NOTA: Cada que se asigna una pagina, el tiempo de uso de las otras paginas ya cargadas en la RAM tambien aumenta.\n");
    int tamMemoria, tamMarco;
    printf("\nIngresa el tamaño de la memoria (KB): ");
    scanf("%d", &tamMemoria);
    printf("Ingresa el tamaño del marco de página (KB): ");
    scanf("%d", &tamMarco);

    int numMarcos = tamMemoria / tamMarco;
    Marco memoria[numMarcos];

    for (int i = 0; i < numMarcos; i++)
    {
        memoria[i].numMarco = i;
        memoria[i].libre = 1;
    }

    int eleccion;
    do
    {
        printf("\n1. Crear proceso\n2. Asignar pagina a memoria\n3. Eliminar pagina de memoria\n4. Mostrar estado de memoria\n5.-Salir\nOpción: ");
        scanf("%d", &eleccion);

        switch (eleccion)
        {
        case 1:
            crearProceso(procesos, tamMarco);
            break;
        case 2:
            asignarPagina(procesos, memoria, numMarcos);
            break;
        case 3:
            eliminarPagina(procesos, memoria, numMarcos);
            break;
        case 4:
            mostrarMemoria(memoria, numMarcos);
            break;
        case 5:
            printf("Saliendo...\n");
            break;
        default:
            printf("No existe esa opcion.\n");
        }
    } while (eleccion != 5);

    return 0;
}
