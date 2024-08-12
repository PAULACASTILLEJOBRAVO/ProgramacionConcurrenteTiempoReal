# Sesión 3

## Enunciado

Estamos muy emocionados, y seguro que vosotros también, porque hemos descubierto la implementación de paso de mensajes entre procesos. Esto nos permite seguir trabajando sobre nuestro maravilloso sistema Obelix. En este sistema, nos encontraremos con un proceso Manager que será el encargado del control de Obelix. Tendremos, por un lado, líneas que recibirán llamadas telefónicas, y, por otro lado, tendremos teléfonos que serán los encargados de atender estas llamadas.

Construya un sistema compuesto por tres ejecutables que simule el funcionamiento que se detalla a continuación, y que es, la especificación básica de funcionamiento del sistema Obelix.

- Manager: Deberá cumplir las siguientes especificaciones:
    
    - Inicializar las colas de mensajes necesarias para establecer la comunicación entre líneas y teléfonos, así como, la comunicación entre los teléfonos y cada una de las líneas.
    
    - Lanzará el número de procesos teléfono y procesos línea definidos en este caso como constantes del sistema.
    
    - El sistema, gracias al paso de mensajes, no tendrá finalización, por lo que el Apagado Controlado de Obelix se realizará mediante la pulsación de Ctrl-C.
    
      - Forzará la finalización de todos los procesos línea actualmente en funcionamiento.
    
      - Forzará la finalización de todos los procesos teléfono existentes.
    
    - Por último, pero no menos importante, liberará todos los recursos utilizados. (tablas de procesos, colas de mensajes, etc.).

- Teléfono: Realizará las siguientes tareas dentro de su función:
    
    - Se inician en espera de una llamada, informando de ello: “Teléfono[UID] en espera…”.
    
    - Cuando reciben una llamada, tienen una conversación variable entre 10..20 segundos e informan de ello: “Teléfono[UID] en conversación de llamada desde la línea: [buzon_linea_n]…”.
    
    - Cuando finalicen su conversación, notificarán a la línea este suceso y mostrarán un mensaje: Teléfono [%d] ha colgado la llamada. [buzon_linea_n].
    
    - Volverán a su estado inicial de espera de llamada.
    
    - En ningún caso finalizarán su ejecución salvo cuando el proceso Manager lo considere oportuno.
    
      - Línea: Realizará las siguientes tareas dentro de su función:
    
    - Recibirá como parámetro su cola de mensajes para el proceso de notificación de teléfonos a líneas (buzon_linea_n)
    
    - Se inician y esperan una llamada (Simulado con un rand() entre 1..30 segundos), informando de ello: “Linea[UID] esperando llamada”.
    
    - Cuando reciben la llamada (Finalización de rand()):
      
      - Enviará un mensaje a la cola de llamadas para ser atendidas.
      
      - Esperarán la notificación desde un teléfono, de que la llamada ha finalizado.
      
      - Notificarán la recepción del teléfono y volverán al estado de espera de llamada.
    
    - En ningún caso finalizarán su ejecución salvo cuando el proceso Manager lo considere oportuno.

## Entrega

[Memoria.pdf](doc/Memoria.pdf).

[Makefile](Makefile).

[linea.c](src/linea.c).

[manager.c](src/manager.c).

[telefono.c](src/telefono.c).
