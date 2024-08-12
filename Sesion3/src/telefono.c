#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <mqueue.h>
#include <definitions.h>

void telefono();
void controlador (int senhal);

// Modulo principal
int main(int argc,char *argv[]){
    telefono();

    return EXIT_SUCCESS;
}

void telefono(){
    // Define variables locales
    int pid = getpid(), rc = 0;
    mqd_t qHandlerLlamadas;
    mqd_t qHandlerLinea;
    char buzonLinea[TAMANO_MENSAJES];
    char buffer[TAMANO_MENSAJES+1];

    srand(pid);

    // Retrollamada de finalización.
    if (signal(SIGINT, controlador) == SIG_ERR) {
        fprintf(stderr, "Abrupt termination.\n"); 
        exit(EXIT_FAILURE);
    }

    //Recuperar buzones
    qHandlerLlamadas = mq_open(BUZON_LLAMADAS, O_RDWR);

    if (qHandlerLlamadas == -1){
		fprintf(stderr, "\tError abriendo HandlerLlamadas: %s\n", strerror(errno));
    }
    
    // Se pone en estado de libre incrementando el número de teléfonos libres
    while(1){

        // Mensaje de Espera
        printf("Teléfono [%d] en espera...\n", pid);

        //Flujo de mensajes entrada a telefono
        rc = mq_receive(qHandlerLlamadas, (char *) &buzonLinea, sizeof(buzonLinea), NULL);
        sprintf(buffer,"%s",buzonLinea);

        if (rc == -1){
			fprintf(stderr, "\tError recibiendo mensaje en HandlerLlamadas: %s\n", strerror(errno));
        }
        
        // Mensaje en conversacion
        printf("Teléfono [%d] en conversacion de llamada desde Linea: %s\n", pid, buffer);

        // Espera en conversación
        sleep(rand() % 10 + 10);

        //Fin de conversación
        printf("Teléfono [%d] ha colgado la llamada: %s\n", pid, buffer);

        //Flujo de mensajes salida a linea
        qHandlerLinea = mq_open(buzonLinea, O_RDWR);

        if (qHandlerLinea == -1){
			fprintf(stderr, "\tError abriendo HandlerLinea: %s\n", strerror(errno));
        }

        sprintf(buzonLinea, "Telefono [%d] acaba de colgar", pid);

        mq_send(qHandlerLinea, buzonLinea, sizeof(buzonLinea), 0);
        close(qHandlerLinea);
    }
}

void controlador (int senhal) {
    printf("[Telefono %d] Finalizado (SIGINT)\n", getpid());
    exit(EXIT_SUCCESS);
}