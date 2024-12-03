#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define tamanoMemoria 50
#define procesosMaximos 5
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
    int libre; // 1 = true 0=  false
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

void asignarProceso()
{
    if (contador > procesosMaximos)
    {
        printf("\nError: NO se puede asignar más procesos a la memoria. Iniciando intercambio(Swapping) de procesos por FIFO..... \n");
    }
    else
    {
        for (int i = 0; i < numeroMarcos; i++)
        {
            if (memoriaPrincipal[i].libre == 1)
            {
                memoriaPrincipal[i].procesoID = procesos[contador-1].id;
                //memoriaPrincipal[i].paginaID = procesos[contador-1].Pagina.numPagina;
                memoriaPrincipal[i].libre = 0; // ya no esta libre la memoria

                printf("\nSe cargo el proceso proceso %d en en el marco %d", memoriaPrincipal[i].procesoID, memoriaPrincipal[i].numMarco);
                return;
            }
        }
    }
}

void creaProceso()
{

    Proceso nuevo;
    nuevo.id = contador + 1;
    nuevo.tamMemoria = rand() % 101 + 10;
    nuevo.prioridad = rand() % 5 + 1;

    procesos[contador] = nuevo;
    printf("Proceso creado.");
    contador++;
    asignarProceso();
    
    
    
}

void mostrarMemoria(int control)
{
    if (control == 1)
    {
        for (int i = 0; i < numeroMarcos;i++)
        {
            if(memoriaPrincipal[i].libre){
                printf("\nMarco %d: Libre", memoriaPrincipal[i].numMarco);
            }else{
                printf("\nMarco %d: Proceso %d", memoriaPrincipal[i].numMarco, memoriaPrincipal[i].procesoID, memoriaPrincipal[i].paginaID);
            }
            
        }
    }else{

        for (int i = 0; i < numeroMarcos;i++)
        {

            if(memoriaSecundaria[i].libre){
                printf("\nMarco %d: Libre", memoriaPrincipal[i].numMarco);
            }else{
                printf("\nMarco %d: Proceso %d", memoriaSecundaria[i].numMarco, memoriaSecundaria[i].procesoID, memoriaSecundaria->paginaID);
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
        printf("4.- Traer Proceso a memoria principal (swap-IN)\n");
        printf("5.- Salir\n");
        printf("Eleccion: ");
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
            mostrarMemoria(0);
            break;
        case 5:
            printf("Saliendo...\n");
            break;
        default:
            printf("No existe esa opción.\n");
        }

    } while (eleccion != 4);

    return 0;
}
