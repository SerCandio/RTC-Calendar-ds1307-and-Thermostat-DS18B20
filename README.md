# RTC-Calendar-ds1307-and-Thermostat-DS18B20
Mini- Proyecto de Reloj de Tiempo Real (usando ds1307) con alarma de Buzzer y Termostato de Temperatura(con el sensor ds18b20) con alarmas de limites

(imagen real board)

<h2 dir="auto" tabindex="-1">DESCRIPCION Y EXPLICACION:</h2>
Este sencillo proyecto es capaz de mostrar la informacion de tiempo real conformado por la fecha y la hora asi como la informacion de temperatura ambiente a traves de un display LCD16X2 y por puerto de consola EUSART ; asimismo el usuario sera capaz de pre-establecer una fecha y hora acorde la actual y establecer alarmas sea de tiempo o de limite alto o bajo de temperatura.

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/39d2f73f-b6e3-4b77-acdd-0fcc4452da11)

Todo lo anterior se realiza mediante un paradijma <B>Multitarea</B> o <B>Multi-Tasking</B> ,temprizadas por un Timer (Timer 1 en este caso, disparado cada 1ms a partir de una base de tiempo de 32.768KHz) y organizadas segun prioridad mediante un Task - Shceduler. Como se nota en la imagen, cada tarea cuenta con un "poll rate Ms", que es cada cuanto tiempo en milisegundos (ms) se le llama.

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/519454f2-e9d0-4cce-900b-ae194c107b68)

Nuestra <B><A HREF="https://github.com/SerCandio/Microcontrolador-PIC18F/blob/main/LIBRERIAS%20PIC18/RTOS.h">libreria RTOS de PIC18F</A></B> con base la libreria millis()  (funcion similar a la de Arduino) facilita el armado y la temporizacion de las tareas donde basicamnete :

<B>1.</B> Se leen las entradas o sensores y se guarda la infomacion en variables locales o glboales

<B>2.</B> Con la informacion en variables de los sensores, se hace un procesamiento

<B>3.</B> Se actualiza las salidas acorde

Entonces estructuramos 8 tareas a fin de leer el sensor de temperatura ds18b20 y la informacion de tiempo del DS1307 , evaluar si se ejecuta o no una alarma (3 primers tareas) y posibilutar modificado del tiempo actual, alarmas de manera asincronica para finalmente mostrar toda la data actual por UART:

