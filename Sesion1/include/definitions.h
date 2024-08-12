#define CLASE_PROCESADOR "PROCESADOR"
#define RUTA_PROCESADOR "./exec/procesador"
#define CLASE_CONTADOR "CONTADOR"
#define RUTA_CONTADOR "./exec/contador"
#include <sys/types.h>

struct TProcess_t
{
  pid_t pid;
  char *clase;
};

typedef struct Nodo
{
  char *valor;
  struct Nodo *pSiguiente;
} TNodo;

typedef struct
{
  TNodo *pPrimero;
} TLista;

void crear(TLista *pLista, char *valor);
void destruir(TLista *pLista);

// 'insertar' inserta al ppio de la lista.
void insertarFinal(TLista *pLista, char *valor);

char *getElementoN(TLista *pLista, int index);

int longitud(TLista *pLista);