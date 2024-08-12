#include <stdlib.h>
#include <stdio.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void procesar(char *nombre_fichero, char *patron);

int main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(stderr, "Error. Número de argumentos de entrada para CONTADOR incorretos. \n");
    return -1;
  }

  procesar(argv[1], argv[2]); 
  return EXIT_SUCCESS;
}

void procesar(char *nombre_fichero, char *patron){
  FILE *fp;
  char texto[PATH_MAX];

  //Abrimos el fichero con el texto en el cuál queremos sustraer la repetición de ciertos patrones
  if ((fp = fopen(nombre_fichero, "r")) == NULL)
  {
    fprintf(stderr, "Error al abrir el fichero %s\n", nombre_fichero);
    exit(EXIT_FAILURE);
  }

  char *token;
  int n_lineas = 1;
  char *comprobacion;

  //Se recorre el archivo hasta el final
  while (fgets(texto, sizeof(texto), fp) != NULL)
  {
    //El comando strtok divides los tramos del texto en líneas al fraccionar la cadena por saltos de línea (\n)
    token = strtok(texto, "\n");
    //Se recorre cada linea almacenada en el token
    
    while (token != NULL)
    {
      //Se corrobora si el patron esta almacenado dentro de una línea, aumentando el contador de lineas en caso afirmativo
      comprobacion = strstr(token, patron);
    
      while(comprobacion != NULL){
        printf("[PROCESADOR %d] Patrón '%s' encontrado en linea %d\n", getpid(), patron, n_lineas++);
        comprobacion = strstr(comprobacion+1, patron);
      }
    
     token = strtok(NULL, "\n");
    }
  }
  
  fclose(fp);
}