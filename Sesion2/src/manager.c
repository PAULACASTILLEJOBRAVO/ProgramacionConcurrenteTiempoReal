#include <errno.h>
#include <linux/limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <definitions.h>
#include <memoriaI.h>
#include <semaforoI.h>

void instalar_manejador_senhal();
void manejador_senal(int sign);
void terminar_procesos();
void terminar_procesos_especificos(struct TProcess_t *process_table, int process_num);
void liberar_recursos();
void esperar_procesos();
void iniciar_tabla_procesos(int n_procesos_telefono, int n_procesos_linea);
void lanzar_proceso_linea(const int indice_tabla);
void lanzar_proceso_telefono(const int indice_tabla);
void crear_procesos(int numTelefonos, int numLineas);
void procesar_argumentos(int argc, char *argv[], int *numTelefonos, int *numLineas);

int g_telefonos_Processes = 0;
int g_lineas_Processes = 0;
struct TProcess_t *g_process_telefonos_table;
struct TProcess_t *g_process_lineas_table;

int main(int argc, char *argv[])
{
    // Define variables locales
    int numTelefonos;
    int numLineas;

    // Procesa los argumentos y los guarda en las dos variables
    procesar_argumentos(argc, argv, &numTelefonos, &numLineas);

    // Creamos semáforos y memoria compartida
    crear_sem(MUTEXESPERA, 1);
    crear_sem(TELEFONOS, 0);
    crear_sem(LINEAS, 0);
    crear_var(LLAMADASESPERA, 0); // No hay llamadas en espera

    // Manejador de Ctrl-C
    instalar_manejador_senhal();

    // Crea Tabla para almacenar los pids de los procesos
    iniciar_tabla_procesos(numTelefonos, numLineas);

    // Tenemos todo
    // Lanzamos los procesos
    crear_procesos(numTelefonos, numLineas);

    // Esperamos a que finalicen las lineas
    esperar_procesos();

    // Matamos los telefonos y cualquier otro proceso restante
    terminar_procesos();

    // Finalizamos Manager
    printf("\n[MANAGER] Terminacion del programa (todos los procesos terminados).\n");
    liberar_recursos();

    return EXIT_SUCCESS;
}

void instalar_manejador_senhal()
{
    if (signal(SIGINT, manejador_senal) == SIG_ERR)
    {
        fprintf(stderr, "\t[MANAGER] Error al instalar el manejador se senhal: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void manejador_senal(int sign)
{
    printf("\n[MANAGER] Terminacion del programa (Ctrl + C).\n");
    terminar_procesos();
    liberar_recursos();
    exit(EXIT_SUCCESS);
}

void terminar_procesos()
{
    terminar_procesos_especificos(g_process_lineas_table, g_lineas_Processes);
    terminar_procesos_especificos(g_process_telefonos_table, g_telefonos_Processes);
}

void terminar_procesos_especificos(struct TProcess_t *process_table, int process_num)
{
    int i;
    printf("\n----- [MANAGER] Terminar con los procesos hijos ejecutándose ----- \n");

    for (i = 0; i < process_num; i++)
    {
        if (process_table[i].pid != 0)
        {
            printf("[MANAGER] Terminando proceso %s [%d]...\n", process_table[i].clase, process_table[i].pid);
            if (kill(process_table[i].pid, SIGINT) == -1)
            {
                fprintf(stderr, "[MANAGER] Error al usar kill() en proceso %d: %s.\n", process_table[i].pid, strerror(errno));
            }
        }
    }
}

void liberar_recursos()
{
    destruir_var(LLAMADASESPERA);
    destruir_sem(MUTEXESPERA);
    destruir_sem(LINEAS);
    destruir_sem(TELEFONOS);
    free(g_process_telefonos_table);
    free(g_process_lineas_table);
}

void esperar_procesos()
{
    int i, n_num_processes = g_lineas_Processes;
    pid_t pid;

    while (n_num_processes > 0)
    {
        pid = wait(NULL);
        for (i = 0; i < g_lineas_Processes; i++)
        {
            if (pid == g_process_lineas_table[i].pid)
            {
                printf("\t[MANAGER] Proceso %s terminado [%d]...\n", g_process_lineas_table[i].clase, g_process_lineas_table[i].pid);
                g_process_lineas_table[i].pid = 0;
                n_num_processes--;
                break;
            }
        }
    }
}

void iniciar_tabla_procesos(int n_processes_telefono, int n_processes_linea)
{
    int i;
    g_lineas_Processes = n_processes_linea, g_telefonos_Processes = n_processes_telefono;

    g_process_lineas_table = malloc(g_lineas_Processes * sizeof(struct TProcess_t));
    g_process_telefonos_table = malloc(g_telefonos_Processes * sizeof(struct TProcess_t));

    for (i = 0; i < g_lineas_Processes; i++)
    {
        g_process_lineas_table[i].pid = 0;
    }

    for (i = 0; i < g_telefonos_Processes; i++)
    {
        g_process_telefonos_table[i].pid = 0;
    }
}

void lanzar_proceso_linea(const int indice_tabla)
{
    pid_t pid;

    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, "\t[MANAGER] Error al lanzar proceso lineas: %s.\n", strerror(errno));
        terminar_procesos();
        liberar_recursos();
        exit(EXIT_FAILURE);
    case 0:
        if (execl(RUTA_LINEA, CLASE_LINEA, MUTEXESPERA, LINEAS, TELEFONOS, LLAMADASESPERA, NULL) == -1)
        {
            fprintf(stderr, "\t[MANAGER] Error usando execl() en el proceso %s: %s.\n", CLASE_LINEA, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    g_process_lineas_table[indice_tabla].pid = pid;
    g_process_lineas_table[indice_tabla].clase = CLASE_LINEA;
}

void lanzar_proceso_telefono(const int indice_tabla)
{
    pid_t pid;

    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, "\t[MANAGER] Error al lanzar proceso telefono: %s.\n", strerror(errno));
        terminar_procesos();
        liberar_recursos();
        exit(EXIT_FAILURE);
    case 0:
        if (execl(RUTA_TELEFONO, CLASE_TELEFONO, MUTEXESPERA, LINEAS, TELEFONOS, LLAMADASESPERA, NULL) == -1)
        {
            fprintf(stderr, "\t[MANAGER] Error usando execl() en el poceso %s: %s.\n", CLASE_TELEFONO, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    g_process_telefonos_table[indice_tabla].pid = pid;
    g_process_telefonos_table[indice_tabla].clase = CLASE_TELEFONO;
}

void crear_procesos(int numTelefonos, int numLineas)
{
    int i;

    for (i = 0; i < numTelefonos; i++)
    {
        lanzar_proceso_telefono(i);
    }
    printf("\t[MANAGER] %d telefonos creados\n", numTelefonos);

    for (i = 0; i < numLineas; i++)
    {
        lanzar_proceso_linea(i);
    }
    printf("\t[MANAGER] %d lineas creados\n", numLineas);
    sleep(1);
}

void procesar_argumentos(int argc, char *argv[], int *numTelefonos, int *numLineas)
{
    if (argc != 3)
    {
        fprintf(stderr, "Error. Usa: ./exec/manager <telefonos> <líneas>.\n");
        exit(EXIT_FAILURE);
    }

    *numTelefonos = atoi(argv[1]);
    *numLineas = atoi(argv[2]);
}