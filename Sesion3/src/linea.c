
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <mqueue.h>
#include <definitions.h>

void lineas(int argc, char *num_linea);
void controlador (int senhal);

// Modulo principal
int main(int argc,char *argv[]){
    lineas(argc, argv[1]);

    return EXIT_SUCCESS;
}

void lineas(int argc, char *num_linea){
   // Define variables locales
    int pid = getpid(), rc = 0;
    mqd_t qHandlerLlamadas;
    mqd_t qHandlerLinea;
    char buzonLinea[TAMANO_MENSAJES];
    char buffer[TAMANO_MENSAJES+1];

    // Retrollamada de finalización.
    if (signal(SIGINT, controlador) == SIG_ERR) {
        fprintf(stderr, "Abrupt termination.\n"); 
        exit(EXIT_FAILURE);
    }

    // Verifica los parámetros
    if (argc != 2)
    {
        fprintf(stderr, "Error. Usa: ./exec/linea <cola_linea_llamante>.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(buzonLinea,"%s",num_linea);

    //Recuperar buzones
    qHandlerLlamadas = mq_open(BUZON_LLAMADAS, O_RDWR);
    qHandlerLinea = mq_open(buzonLinea, O_RDWR);

    if (qHandlerLlamadas == -1){
		fprintf(stderr, "\tError abriendo HandlerLlamadas: %s\n", strerror(errno));
    }

    if (qHandlerLinea == -1){
		fprintf(stderr, "\tError abriendo HandlerLinea: %s\n", strerror(errno));
    }

    // Inicia Random
    srand(pid);

    while(1){
        // Realiza una espera entre 1..60 segundos
        printf("Linea [%d] esperando llamada...\n", pid);
        sleep(rand() % 30 + 1);

        //Flujo de mensajes salida a telefono 
        mq_send(qHandlerLlamadas, buzonLinea, sizeof(buzonLinea), 0);

        // Espera recibir llamada
        printf("Linea [%d] recibida llamada (%s)\n", pid, buzonLinea);

        //Espera fin conversación
        printf("Linea [%d] esperando fin de conversación...\n", pid);

        //Flujo de mensajes entrada a linea
        rc = mq_receive(qHandlerLinea, (char *) &buffer, sizeof(buffer), NULL);

        if(rc == -1){
			fprintf(stderr, "\tError recibiendo mensaje en HandlerLinea: %s\n", strerror(errno));
        }

        printf("%s la linea: %s\n", buffer, buzonLinea);
        
        // Llamada finalizada
        printf("Linea [%d] conversación finalizada...\n", pid);
    }
}

void controlador (int senhal) {
    printf("[Linea %d] Finalizado (SIGINT)\n", getpid());
    exit(EXIT_SUCCESS);
}