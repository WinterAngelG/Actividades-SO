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
    int tamaño;
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
    scanf("%d", &nuevo->tamaño);

    nuevo->paginas = (nuevo->tamaño + tamMarco - 1) / tamMarco;
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
    printf("Ingresa la página a acceder(de 0 a n): ");
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
                return;
            }

            for (int j = 0; j < numMarcos; j++)
            {
                if (memoria[j].libre)
                {
                    memoria[j].libre = 0;
                    memoria[j].procesoID = id;
                    memoria[j].paginaID = pagina;

                    procesos[i].tablaPaginas[pagina].numMarco = j;
                    procesos[i].tablaPaginas[pagina].presente = 1;

                    printf("Página %d del proceso %d cargada en el marco %d.\n", pagina, id, j);
                    return;
                }
            }

            printf("Fallo de página: no hay marcos disponibles. Se usuará el algoritmo de LRU para reemplazar la pagina\n");


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
    printf("Ingresa la página a eliminar(de 0 a n): ");
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
            printf("Marco %d: Proceso %d, Página %d\n", i, memoria[i].procesoID, memoria[i].paginaID);
        }
    }
}

int main()
{
    Proceso procesos[MAX_PROCESOS];
    int contador = 0;

    int tamMemoria, tamMarco;
    printf("Ingresa el tamaño de la memoria (KB): ");
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
