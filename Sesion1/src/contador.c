#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void contar(char *line, int line_number);

int main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(stderr, "Error. Número de argumentos de entrada para CONTADOR incorretos. \n");
    return -1;
  }
  contar(argv[1], atoi(argv[2]));
  return EXIT_SUCCESS;
}

void contar(char *linea, int numero_linea)
{
  int n_palabras = 0, dentro_palabra = 0;
  const char *it = linea;

  do
  {
    switch (*it)
    {
    case '\0':
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      if (dentro_palabra)
      {
        dentro_palabra = 0;
        n_palabras++;
      }
      break;
    default:
      dentro_palabra = 1;
    }
  } while (*it++);

  printf("[CONTADOR %d] La linea '%d' tiene %d palabras\n", getpid(), numero_linea, n_palabras);
}