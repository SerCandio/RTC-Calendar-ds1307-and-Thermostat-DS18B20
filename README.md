# RTC-Calendar-ds1307-and-Thermostat-DS18B20
Mini- Proyecto de Reloj de Tiempo Real (usando ds1307) con alarma de Buzzer y Termostato de Temperatura(con el sensor ds18b20) con alarmas de limites administrado por un RTOS basado en Timer 1 y algoritmo tipo Task Scheduler de invocacion de tareas

![IMG_20240129_152216](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/cd4a2763-563a-4bb3-833b-6f79ae9c92cc)

<h2 dir="auto" tabindex="-1">DESCRIPCION Y EXPLICACION:</h2>
Este sencillo proyecto es capaz de mostrar la informacion de tiempo real conformado por la fecha y la hora asi como la informacion de temperatura ambiente a traves de un display LCD16X2 y por puerto de consola EUSART ; asimismo el usuario sera capaz de pre-establecer una fecha y hora acorde la actual y establecer alarmas sea de tiempo o de limite alto o bajo de temperatura.

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/39d2f73f-b6e3-4b77-acdd-0fcc4452da11)

Todo lo anterior se realiza mediante un paradijma <B>Multitarea</B> o <B>Multi-Tasking</B> ,temprizadas por un Timer (Timer 1 en este caso, disparado cada 1ms a partir de una base de tiempo de 32.768KHz) y organizadas segun prioridad mediante un Task - Shceduler. Como se nota en la imagen, cada tarea cuenta con un "poll rate Ms", que es cada cuanto tiempo en milisegundos (ms) se le llama.

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/519454f2-e9d0-4cce-900b-ae194c107b68)

Nuestra <B><A HREF="https://github.com/SerCandio/Microcontrolador-PIC18F/blob/main/LIBRERIAS%20PIC18/RTOS.h">libreria RTOS de PIC18F</A></B> con base la <B><A HREF="https://github.com/SerCandio/Microcontrolador-PIC18F/blob/main/LIBRERIAS%20PIC18/millis.h">libreria millis()</A></B> (funcion similar a la de Arduino) facilita el armado y la temporizacion de las tareas donde basicamnete :

<B>1.</B> Se leen las entradas o sensores y se guarda la infomacion en variables locales o glboales

<B>2.</B> Con la informacion en variables de los sensores, se hace un procesamiento

<B>3.</B> Se actualiza las salidas acorde

Entonces estructuramos 8 tareas a fin de leer el sensor de temperatura ds18b20 y la informacion de tiempo del DS1307 para luego evaluar si se ejecuta o no una alarma (4 primeras tareas) mostrando la data por LCD 16X2 y por UART :

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/3de4a245-e53e-40c5-a689-ffa261383cbf)

Las tareas o Task5, 6, 7 y 8 permiten modificar la fecha , hora y alarmas de tiempo y temperatura a otro valor que el usuario eliga(Task5) ; mientras que las tareas 6 y 7 escriben dicha informacion en los dispositivos externos a traves del protocolo I2C : 

<B>1.</B> El tiempo actual(Date, Month, Year,  Hour, Minute, Seconds) se escribe dirctamente en el <B><A HREF="https://www.sparkfun.com/datasheets/Components/DS1307.pdf">DS1307</A></B>

<B>2.</B> Las alarmas de tiempo(Hour, Minute, Seconds) y las de temperatura (TH, TL) se escribe dirctamente en la <B><A HREF="https://ww1.microchip.com/downloads/en/devicedoc/21061h.pdf">EEPROM 24C32</A></B> de acuerdo al siguiente mapa(ver <B><A HREF="https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/blob/main/CLOCK_CALENDAR.X/main.c">Task 7</A></B>):

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/e4c665ac-3202-43d4-aeac-bae377e8cb76)

<B>3.</B> Las alarmas son modificadas en la tarea 5 o <B>Task5</B> de manera asincrona mediante pulsadores(en configuracion pull down) conectado al puerto D : 

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/ea314a53-496c-41bf-8041-510ddb30d3f8)

<B>3.</B> Cada vez que se presione el boton <B>SHIFT_ALARM_Button_In()</B> se incrememta un contador de parametro <B>"buttonAlarmUserMode"</B> , dependiendo de su valor se pre-establece un parametro de alarma incrementandolo(se presiona <B>INC_Button_In()</B>) o decrementandolo(se presiona <B>DEC_Button_In()</B>) como se muestra :

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/1583a99b-4988-48ca-8d0e-0f56dddec409)

(mostrar resultados de "Alarm Parameters" de LCD)

<B>4.</B> Pero no ocurrira ninguna transferencia hacia la EEPROM hasta que todas las alarmas hayan sido actualizadas, por lo que <B>SHIFT_ALARM_Button_In()</B> debera presionarse unas 8 veces y posterioremente en la tarea 7 o <B>"Task 7"</B>se transfiera los 7 bytes empezando desde la direccion 0x0000 : 

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/e4e457a5-dfaf-4cd7-8970-75de729fa2a6)

Dando como resultado (B>"Task 7"</B>):

("All Alarms Update OK images, proteus y Board")

Tenga en cuenta que la <B><A HREF="https://ww1.microchip.com/downloads/en/devicedoc/21061h.pdf">EEPROM 24C32</A></B> tiene un modo de escritura por pagina, lo que ahorra tiempo de escritura y ahorra vida util de la memoria no volatil(ver datasheet pagina 6):

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/737a06e1-af3b-411b-b744-cc9d8b518282)

<h2 dir="auto" tabindex="-1">Nota 1</h2>
Recuerde que el ciclo de escritura se tarda alrededor de 15ms. El usuario recibe confirmacion por la LCD que el contenido de la memoeria EEPROM fue actualizada con la informacion de las alarmas:

<B>5.</B> Finalmente en la tarea 8 o <B>Task 8</B> se imprime el valor actual de las alarmas cada 5 segundos por consola Serial asi como el valor actual de las alarmas acorde:

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/a35478a9-51d6-4235-8a3e-b56c8c9334d0)

Podriamos entocnes resumir las 4 ultimas tareas como sigue:

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/49bbf923-c97a-4d60-87ea-6e0f723d7352)

(Resultado consola serial simulacion y placa)

<h2 dir="auto" tabindex="-1">Observacion</h2>
Recuerde que la EEPROM es una memoeria no volatil, cualquier conjunto de valores permancera incluso si se interrumpe la alimentcion (<I>power failure</I>) , acorde <B><A HREF="https://ww1.microchip.com/downloads/en/devicedoc/21061h.pdf">datasheet</A></B>:

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/0870c38f-e185-486a-87e7-81d6f8ddbced)

<h2 dir="auto" tabindex="-1">Nota 2</h2>
La EEPROM 24C32 y el RTC DS1307 ya vienen incorporados en un modulo <B><A HREF="https://www.indianhobbycenter.com/products/real-time-clock-ds1307-module-tiny-rtc-i2c-module">Tiny RTC I2C</A></B> como se muestra a continuacion:

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/a10fb5ba-1a73-4a51-b4bc-d868fa9282c4)

El RTC de Fecha, hora y Calendario es capaz de seguir actualziando sus registros de tiempo incluso no haya voltaje de operacion en Vcc como se indica su <B><A HREF="https://pdf1.alldatasheet.es/datasheet-pdf/view/254791/MAXIM/DS1307.html">datasheet</A></B> :

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/7d798725-3889-46ec-8ebf-933477e24d3b)

Sin embargo, para que dicha caracteristica de respaldo este disponible, se debe desoldar el diodo <B>D1 1N4148</B> , y en lugar la resistencia <B>R6</B> de 510k debe reemplazarse por un bypass a fin que la bateria alimente directamente al pin <B>VBAT</B>

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/92e09b5d-2602-4a61-9c75-aaadd2d67cb4)

<h2 dir="auto" tabindex="-1">Precaucion</h2> 
Si la pila CR2032 es de una <B>version no recargable, modificacion descrita en el apartado anterior es obligatoria</B>, caso contrario podria dañarse o explotar la bateria
