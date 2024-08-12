
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <definitions.h>
#include <memoriaI.h>
#include <semaforoI.h>

void telefono(sem_t *telefonos, sem_t *lineas, sem_t *mutex, int num_llamadas_espera, int valorEspera);

// Modulo principal
int main(int argc,char *argv[]){

    // Define variables locales
    int valorEspera = 0, num_llamadas_espera;
    sem_t *telefonos, *lineas, *mutex;

    // Coge semáforos y memoria compartida
    srand((int)getpid());
    lineas = get_sem(argv[2]);
    telefonos = get_sem(argv[3]);
    mutex = get_sem(argv[1]);
    num_llamadas_espera = obtener_var(argv[4]);
    
    telefono(telefonos, lineas, mutex, num_llamadas_espera, valorEspera);

    return EXIT_SUCCESS;
}

void telefono(sem_t *telefonos, sem_t *lineas, sem_t *mutex, int num_llamadas_espera, int valorEspera){
        pid_t pid = getpid();
    
    // Se pone en estado de libre incrementando el número de teléfonos libres
    while(1){

        // Mensaje de Espera
        printf("Teléfono [%d] en espera...\n", pid);

		//TODO: Aquí hay que realizar procesos
        signal_sem(telefonos);
        wait_sem(lineas);

        wait_sem(mutex);
        consultar_var(num_llamadas_espera, &valorEspera);
        valorEspera--;
        modificar_var(num_llamadas_espera, valorEspera);
        signal_sem(mutex);
        
        // Mensaje de en conversacion
        printf("Teléfono [%d] en conversacion... Nº Llamadas en espera: %d\n", pid, valorEspera);

        // Espera en conversación
        sleep(rand() % 10 + 10);
    }
}