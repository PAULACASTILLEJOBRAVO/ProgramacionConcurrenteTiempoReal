#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <mqueue.h>
#include <definitions.h>

void instalar_manejador_senhal();
void manejador_senal(int sign);
void terminar_procesos();
void terminar_procesos_especificos(struct TProcess_t *process_table, int process_num);
void esperar_procesos();
void iniciar_tabla_procesos(int n_procesos_telefono, int n_procesos_linea);
void liberar_recursos();
void crear_procesos(int numTelefonos, int numLineas);
void lanzar_proceso_telefono(const int indice_tabla);
void lanzar_proceso_linea(const int indice_tabla);
void crear_buzones();


int g_telefonos_Processes = 0;
int g_lineas_Processes = 0;
struct TProcess_t *g_process_telefonos_table;
struct TProcess_t *g_process_lineas_table;
mqd_t qHandlerLlamadas;
mqd_t qHandlerLineas[NUMLINEAS];


int main(int argc, char *argv[])
{
    // Creamos los buzones
    crear_buzones();

    // Manejador de Ctrl-C
    instalar_manejador_senhal();
 
    // Crea Tabla para almacenar los pids de los procesos
    iniciar_tabla_procesos(NUMTELEFONOS,NUMLINEAS);

    // Tenemos todo
    // Lanzamos los procesos
    crear_procesos(NUMTELEFONOS,NUMLINEAS);
  
    // Esperamos a que finalicen las lineas
    esperar_procesos();

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

void esperar_procesos()
{
    int i;

    for (i = 0; i < NUMLINEAS; i++)
    {
        waitpid(g_process_lineas_table[i].pid, 0, 0);
        
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

void liberar_recursos()
{
    int i; char caux[30];

    free(g_process_telefonos_table);
    free(g_process_lineas_table);

    mq_close(qHandlerLlamadas);
    mq_unlink(BUZON_LLAMADAS);

    for (i = 0; i < NUMLINEAS; i++) {
        sprintf(caux, "%s%d", BUZON_LINEAS, i);
        mq_close(qHandlerLineas[i]);
        mq_unlink(caux);
    }
}

void crear_procesos(int numTelefonos, int numLineas)
{
    int i;

    for (i = 0; i < numLineas; i++)
    {
        lanzar_proceso_linea(i);
    }
    printf("\t[MANAGER] %d lineas creados\n", numLineas);

    for (i = 0; i < numTelefonos; i++)
    {
        lanzar_proceso_telefono(i);
    }
    printf("\t[MANAGER] %d telefonos creados\n", numTelefonos);
    sleep(1);
}

void lanzar_proceso_linea(const int indice_tabla)
{
    pid_t pid;
    char caux[30];

    sprintf(caux, "%s%d", BUZON_LINEAS, indice_tabla);

    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, "\t[MANAGER] Error al lanzar proceso lineas: %s.\n", strerror(errno));
        terminar_procesos();
        liberar_recursos();
        exit(EXIT_FAILURE);
    case 0:
        if (execl(RUTA_LINEA, CLASE_LINEA, caux, NULL) == -1)
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
        if (execl(RUTA_TELEFONO, CLASE_TELEFONO, NULL) == -1)
        {
            fprintf(stderr, "\t[MANAGER] Error usando execl() en el poceso %s: %s.\n", CLASE_TELEFONO, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    g_process_telefonos_table[indice_tabla].pid = pid;
    g_process_telefonos_table[indice_tabla].clase = CLASE_TELEFONO;
}

void crear_buzones(){
    struct mq_attr mqAttr;
    char caux[30];

    mqAttr.mq_maxmsg = NUMLINEAS; 
    mqAttr.mq_msgsize = TAMANO_MENSAJES;

    qHandlerLlamadas = mq_open(BUZON_LLAMADAS, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR, &mqAttr);
    
    if (qHandlerLlamadas == -1){
		fprintf(stderr, "\tError creando HandlerLlamadas: %s\n", strerror(errno));
    }

    int i;
    
    mqAttr.mq_maxmsg = 1;

    for (i = 0; i < NUMLINEAS; i++) {
        sprintf(caux, "%s%d", BUZON_LINEAS, i);
        qHandlerLineas[i] = mq_open(caux, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR, &mqAttr);
        
        if (qHandlerLineas[i] == -1){
		    fprintf(stderr, "\tError creando HandlerLinea %s: %s\n", caux, strerror(errno));
        }
    }
}