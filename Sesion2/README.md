# Sesión 1

## Enuciado

Es nuestra intención crear un sistema básico de control de llamadas telefónicas denominado Obelix. En este sistema, nos encontraremos con un proceso Manager que será el encargado del control de Obelix. Tendremos, por un lado, líneas que recibirán llamadas telefónicas, y por otro lado, tendremos teléfonos que serán los encargados de atender estas llamadas. Además, nos piden que en todo momento podamos informar de las llamadas que, habiendo entrado a las líneas, se encuentren en espera de poder ser atendidas por los teléfonos. 

Construya un sistema compuesto por tres ejecutables que simule el funcionamiento que se detalla a continuación, y que es, la especificación básica de funcionamiento del sistema Obelix.

- Manager: Deberá cumplir las siguientes especificaciones:

  - Recibirá dos argumentos: <nº teléfonos> <nº líneas>.
  
  - Inicializará los semáforos y la memoria compartida para almacenar el número de llamadas en espera.

  - Lanzará el número de procesos teléfono y procesos línea especificado en los argumentos.

  - Realizará el apagado controlado de Obelix. Para ello, esperará la finalización automática de todos los procesos línea y forzará la finalización de los procesos teléfono una vez hayan finalizados todos los procesos línea.

  - Controlará el Apagado de emergencia de Obelix, ante la pulsación de Ctrl-C. Esto es:
    
    - Forzará la finalización de todos los procesos línea actualmente en funcionamiento.
    
    - Forzará la finalización de todos los procesos teléfono existentes.

  - Por último, pero no menos importante, liberará todos los recursos utilizados. (semáforos, memoria compartida, etc.)

- Teléfono: Realizará las siguientes tareas dentro de su función:

    - Se inician en espera de una llamada, informando de ello: “Teléfono[UID] en espera…”.

    - Cuando reciben una llamada, tienen una conversación variable entre 10..20 segundos e informan de ello: “Teléfono[UID] en conversación…”, además en esa misma línea informarán del número de llamadas en espera en ese momento:"Nº de llamadas en espera: [shm]” y decrementarán el número de llamadas en espera.

    - Cuando finalicen su llamada, volverán al estado de espera inicial.
    
    - En ningún caso finalizarán su ejecución salvo cuando el proceso Manager lo considere oportuno.

- Línea: Realizará las siguientes tareas dentro de su función:
    - Se inician y esperan una llamada (Simulado con un rand() entre 1..30 segundos), informando de ello: “Linea[UID] esperando llamada”.
    
    - Cuando reciben la llamada (Finalización de rand()):
      
      - Incrementarán el contador de llamadas en espera.
      
      - Esperarán a que exista un teléfono libre, informando de ello: “Linea[UID] esperando teléfono libre…”, además en esa misma línea informarán del número de llamadas en espera en ese momento:"Nº de llamadas en espera: [shm]”.
      
      - Cuando exista un teléfono libre, desviarán la llamada hacia el teléfono, informando de ello: “Linea[UID] desviando llamada a un teléfono…” y finalizarán su trabajo.

## Entrga

[Memoria.pdf](doc/Memoria.pdf).

[Makefile](Makefile).

[linea.c](src/linea.c).

[manager.c](src/manager.c).

[memoriaI.c](src/memoriaI.c).

[semaforoI.c](src/semaforoI.c).

[telefono.c](src/telefono.c).
