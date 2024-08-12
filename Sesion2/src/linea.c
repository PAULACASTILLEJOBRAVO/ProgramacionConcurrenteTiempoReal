
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <definitions.h>
#include <memoriaI.h>
#include <semaforoI.h>

// Modulo principal
int main(int argc,char *argv[]){

    // Define variables locales
    pid_t pid = getpid();
    int valorEspera = 0, num_llamadas_espera;
    sem_t *lineas, *telefonos, *mutex;
	
    // Coge semáforos y memoria compartida
    srand(pid);
    lineas = get_sem(argv[2]);
    telefonos = get_sem(argv[3]);
    mutex = get_sem(argv[1]);
    num_llamadas_espera = obtener_var(argv[4]);
    
    // Realiza una espera entre 1..60 segundos
    printf("Linea [%d] esperando llamada...\n", pid);
    sleep(rand() % 30 + 1);

    //Aumenta las llamadas en espera
    wait_sem(mutex);
    consultar_var(num_llamadas_espera, &valorEspera);
    valorEspera++;
    modificar_var(num_llamadas_espera, valorEspera);
    signal_sem(mutex);

    // Espera telefono libre
    printf("Linea [%d] esperando telefono libre...Nº Llamadas en espera: %d\n", pid, valorEspera);
    wait_sem(telefonos);

    // Lanza la llamada
    printf("Linea [%d] desviando llamada a un telefono...\n", pid);
    signal_sem(lineas);

    return EXIT_SUCCESS;
}