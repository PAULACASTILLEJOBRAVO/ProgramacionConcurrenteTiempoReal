#include <stdio.h>
#include <stdlib.h>
#include <definitions.h>
#include <string.h>

// Crea una lista con un nodo.
void crear(TLista *pLista, char *valor)
{
  pLista->pPrimero =  malloc(sizeof(TNodo));
  char *nodoValor = malloc(strlen(valor) * sizeof(char));
  strcpy(nodoValor, valor);
  pLista->pPrimero->valor = nodoValor;
  pLista->pPrimero->pSiguiente = NULL;
}

void destruir(TLista *pLista)
{
  TNodo *pAux1;
  struct Nodo *pAux2;

  for (pAux1 = pLista->pPrimero; pAux1 != NULL;)
  {
    pAux2 = pAux1->pSiguiente;
    free(pAux1);
    pAux1 = pAux2;
  }

  free(pLista);
}

void insertarFinal(TLista *pLista, char *valor)
{
  //Crear espacio en memoria nueva
  TNodo *nodoNuevo = malloc(sizeof(TNodo));
  nodoNuevo->pSiguiente = NULL;
  nodoNuevo->valor = malloc(strlen(valor) * sizeof(char));
  strcpy(nodoNuevo->valor, valor);
  //Buscar el último nodo de la lista
  TNodo *PunteroAuxiliar = pLista->pPrimero;
  while (PunteroAuxiliar->pSiguiente != NULL)
  {
    PunteroAuxiliar = PunteroAuxiliar->pSiguiente;
  }
  //Actualizar la posición del nuevo nodo al final de la lista
  PunteroAuxiliar->pSiguiente=nodoNuevo;
}

char *getElementoN(TLista *pLista, int index)
{
  //Recorrer la lista de principio al elemento deseado
  TNodo *PunteroAuxiliar = pLista->pPrimero;
  int contador = 0;
  while ((PunteroAuxiliar->pSiguiente) && (contador<index))
  {
    PunteroAuxiliar = PunteroAuxiliar->pSiguiente;
    contador++;
  }
  return PunteroAuxiliar->valor;
}

int longitud(TLista *pLista)
{
  //Recorrer la lista de principio a fin
  TNodo *PunteroAuxiliar = pLista->pPrimero;
  int contador = 0;
  while (PunteroAuxiliar != NULL)
  {
    PunteroAuxiliar = PunteroAuxiliar->pSiguiente;
    contador++;
  }
  //Regresar el contador después de haberse contado cuantos nodos tiene la lista, 
  //(por cuantos se tiene que pasar para llegar al último conectado a NULL)
  return contador;
}
