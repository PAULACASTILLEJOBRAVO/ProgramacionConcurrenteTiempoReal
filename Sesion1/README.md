# Sesión 1

## Enuciado

Construya un sistema compuesto por tres ejecutables que simule el funcionamiento que se detalla a continuación. El sistema contará con tres tipos de procesos:

- Manager: este proceso será responsable de crear un número determinado de procesos de tipo procesador y de tipo contador, gestionando de manera adecuada su finalización y liberando los recursos previamente reservados. Este proceso recibirá por la línea de órdenes la ruta de dos ficheros.

  - <archivo_texto>: será abierto por el proceso manager.
    
    - Leerá su contenido línea a línea.
    
    - Por cada línea creará un proceso contador.

  - <archivo_patrones>: este será procesado por el proceso manager.
    
    - Leerá su contenido palabra a palabra.

    - Por cada palabra insertará un nodo en una lista empleando la estructura de datos creada en la práctica P1.1.1.

  - Procesando dicha lista, creará un proceso procesador por cada nodo de la misma.

- Procesador: estos procesos recibirán en el momento de su creación la ruta del fichero

  - <archivo_texto> y un patrón. Su función consistirá en comprobar si este patrón se corresponde con alguna de las palabras que conforman el texto contenido en el fichero <archivo_texto>.

- Contador: estos procesos recibirán en el momento de su creación un número de línea y una
línea. Su función consistirá en contar el número de palabras que conforman la línea recibida.

Las rutas de los archivos <archivo_texto> y <archivo_patrones> serán indicados por el usuario a través de la línea de órdenes al ejecutar el único proceso de tipo manager:

./exec/manager <archivo_texto> <archivo_patrones>

A continuación, se muestra un ejemplo de <archivo_texto> y de <archivo_patrones>.

Pablito clavó un clavito en la calva de un calvito
Un clavito clavó Pablito en la calva de un calvito
¿Qué clavito clavó Pablito?
Pablito clavito calvito

Figura 1: Ejemplo <archivo_texto>

Pablito clavito calvito

Figura 2: Ejemplo <archivo_patrones>

La finalización de la simulación tendrá lugar si se cumple una de las dos condiciones siguientes:

- Todos los procesos de tipo procesador y contador finalizan su ejecución. El proceso manager, tras detectar esta situación, liberará recursos.

- El usuario pulsa la combinación de teclas Ctrl + C. El proceso manager, tras detectar este evento, enviará una señal de finalización a todos los procesos de tipo procesador y contador que estén en ejecución y liberará recursos.

## Entrega

[Memoria.pdf](doc/Memoria.pdf).

[Makefile](Makefile).

[contador.c](src/contador.c).

[lista.c](src/lista.c).

[manager.c](src/manager.c).

[procesador.c](src/procesador.c).
