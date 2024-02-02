# RTC-Calendar-ds1307-and-Thermostat-DS18B20
Mini- Proyecto de Reloj de Tiempo Real (usando ds1307) con alarma de Buzzer y Termostato de Temperatura(con el sensor ds18b20) con alarmas de limites administrado por un RTOS basado en Timer 1 y algoritmo tipo Task Scheduler de invocacion de tareas

![IMG_20240129_152216](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/cd4a2763-563a-4bb3-833b-6f79ae9c92cc)

<I><B>Figura 1.- Prototipo terminado</B></I>

<h2 dir="auto" tabindex="-1">DESCRIPCION Y EXPLICACION:</h2>
Este sencillo proyecto es capaz de mostrar la informacion de tiempo real conformado por la fecha y la hora asi como la informacion de temperatura ambiente a traves de un <B><A HREF="https://www.ebay.com/itm/293186602264?chn=ps&norover=1&mkevt=1&mkrid=21565-165579-560236-2&mkcid=2&itemid=293186602264&targetid=293946777986&device=c&mktype=pla&googleloc=9060924&poi=&campaignid=19723034349&mkgroupid=141566926690&rlsatarget=pla-293946777986&abcId=&merchantid=102019429&gclid=Cj0KCQiAn-2tBhDVARIsAGmStVnM0kLHaYehLvTRdnJPwh6Jd6zg_dvRAQl_C5zd7E8ZXKiwgxUjkxsaApBCEALw_wcB">display LCD16X2 con interfaz I2C</A></B> y por puerto de consola <B>UART(9600, 8, No parity, 1 Stop bit, NFC)</B>; asimismo el usuario sera capaz de pre-establecer una fecha y hora acorde la actual y establecer alarmas sea de tiempo o de limite alto o bajo de temperatura.

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/39d2f73f-b6e3-4b77-acdd-0fcc4452da11)

<I><B>Figura 2.- Ejecucion de tareas en tiempo real</B></I>

Todo lo anterior se realiza mediante un paradijma <B>Multitarea</B> o <B>Multi-Tasking</B> ,temprizadas por un Timer (Timer 1 en este caso, disparado cada 1ms a partir de una base de tiempo de 32.768KHz) y organizadas segun prioridad mediante un Task - Shceduler. Como se nota en la imagen, cada tarea cuenta con un "poll rate Ms", que es cada cuanto tiempo en milisegundos (ms) se le llama.

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/519454f2-e9d0-4cce-900b-ae194c107b68)

<I><B>Figura 3.- Flujo de lectura de E/S y procesamiento</B></I>

Nuestra <B><A HREF="https://github.com/SerCandio/Microcontrolador-PIC18F/blob/main/LIBRERIAS%20PIC18/RTOS.h">libreria RTOS de PIC18F</A></B> con base la <B><A HREF="https://github.com/SerCandio/Microcontrolador-PIC18F/blob/main/LIBRERIAS%20PIC18/millis.h">libreria millis()</A></B> (Similar a la funcion <B><A HREF="https://www.arduino.cc/reference/es/language/functions/time/millis/">millis()</A></B> de la plataforma <B><A HREF="https://www.arduino.cc/">Arduino</A></B>) facilita el armado y la temporizacion de las tareas donde basicamente (como se observa en la Figura 3) :

<B>1.</B> Se <B>leen las entradas o sensores</B> y se guarda la infomacion en variables locales o glboales

<B>2.</B> Con la informacion en variables de los sensores, se hace un <B>procesamiento</B>

<B>3.</B> Se actualiza las <B>salidas</B> acorde

Entonces estructuramos 8 tareas a fin de leer el sensor de temperatura ds18b20 y la informacion de tiempo del DS1307 para luego evaluar si se ejecuta o no una alarma (4 primeras tareas) mostrando la data por LCD 16X2 y por UART :

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/3de4a245-e53e-40c5-a689-ffa261383cbf)
<I><B>Figura 4.- Flujo de las 4 primeras tareas</B></I>

Cuando el programa <B>"main.c"</B> empieza a ejecutarse, se ira mostrando la siguiente informacion de tiempo y temperatura tanto en  la LCD de 16X2 :

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/76a6b803-3c56-40e2-8291-3888d94915db)
<I><B>Figura 5.- Ejecucion de las 4 primeras tareas: Task1 -- > Task 4 (LCD de 16X2 I2C)</B></I>

Asimismo, la data e informacion tambien se muestra por la consola de usuario UART(<B>Virtual Terminal Proteus</B>):

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/f7566b88-e7c7-4eb3-9aca-95489850dcbc)
<I><B>Figura 6.- Ejecucion de las 4 primeras tareas: Task1 -- > Task 4 (Consola UART)</B></I>

A nivel de <B>Implementacion fisica</B> en una placa prototipo unidos con los modulos <B><A HREF="https://nskelectronics.in/Tiny%20RTC%20Module">Tiny RTC I2C</A></B>  y <B><A HREF="https://www.ebay.com/itm/293186602264?chn=ps&norover=1&mkevt=1&mkrid=21565-165579-560236-2&mkcid=2&itemid=293186602264&targetid=293946777986&device=c&mktype=pla&googleloc=9060924&poi=&campaignid=19723034349&mkgroupid=141566926690&rlsatarget=pla-293946777986&abcId=&merchantid=102019429&gclid=Cj0KCQiAn-2tBhDVARIsAGmStVnM0kLHaYehLvTRdnJPwh6Jd6zg_dvRAQl_C5zd7E8ZXKiwgxUjkxsaApBCEALw_wcB">LCD 16X2</A></B>  obtenemos el mismo resultado:

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/a1fc6e0c-7cb1-4d70-b2b8-d2d4194df463)
<I><B>Figura 7.- Implementacion de Task1 -- > Task 4 (LCD de 16X2 I2C)</B></I>

Para poder ver en resultado del debug en consola por puerto Serial, necesitaremos programas como <B><A HREF="https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html">putty</A></B> y los drivers de <B><A HREF="https://ftdichip.com/drivers/">FTDI</A></B>

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/02c86393-b70d-49a9-beff-c975c7057ae7)

<I><B>Figura 8.- Implementacion de Task1 -- > Task 4 (Consola UART)</B></I>

Las tareas o Task5, 6, 7 y 8 permiten modificar la fecha , hora y alarmas de tiempo y temperatura a otro valor que el usuario eliga(Task5) ; mientras que las tareas 6 y 7 escriben dicha informacion en los dispositivos externos a traves del protocolo I2C : 

<B>1.</B> El tiempo actual(Date, Month, Year,  Hour, Minute, Seconds) se escribe dirctamente en el <B><A HREF="https://www.sparkfun.com/datasheets/Components/DS1307.pdf">DS1307</A></B>

<B>2.</B> Las alarmas de tiempo(Hour, Minute, Seconds) y las de temperatura (TH, TL) se escribe dirctamente en la <B><A HREF="https://ww1.microchip.com/downloads/en/devicedoc/21061h.pdf">EEPROM 24C32</A></B> de acuerdo al siguiente mapa(ver <B><A HREF="https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/blob/main/CLOCK_CALENDAR.X/main.c">Task 7</A></B>):

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/e4c665ac-3202-43d4-aeac-bae377e8cb76)

<B>3.</B> Las alarmas son modificadas en la tarea 5 o <B>Task5</B> de manera asincrona mediante pulsadores(en configuracion pull down) conectado al puerto D : 

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/ea314a53-496c-41bf-8041-510ddb30d3f8)

<B>3.</B> Cada vez que se presione el boton <B>SHIFT_ALARM_Button_In()</B> se incrememta un contador de parametro <B>"buttonAlarmUserMode"</B> , dependiendo de su valor se pre-establece un parametro de alarma incrementandolo(se presiona <B>INC_Button_In()</B>) o decrementandolo(se presiona <B>DEC_Button_In()</B>) como se muestra :

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/1583a99b-4988-48ca-8d0e-0f56dddec409)

<I><B>Figura 9.- Seccion de codigo de la tarea 5 o "Task 5": Leer pulsadores</B></I>

Por ejemplo, si presionamos y mantenemos presionado el boton <B>SHIFT_ALARM_Button_In()</B> por primera vez, podremos modificar la alarma de tiempo el parametro "segundos": 

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/d68a618f-5677-4efb-be0a-fee6bc6a06e6)
<I><B>Figura 10.- Modificacion asincrona de la alarma de tiempo</B></I>

Analogamente , si presionamos dicho boton hasta 2 veces mas podremos modifcar los "minutos" y las  "horas" respectivamente. Tenga en cuenta que la modificacion del parametro actual se hace efectiva con los botones de incremento <B>INC_Button_In()</B> o decremento <B>DEC_Button_In()</B> pudiendo entonces establecer alguna alarma de tiempo como se ve en la Figura 10

Si seguimos presionando el boton de <B>SHIFT_ALARM_Button_In()</B>, accederemos al menu de alarmas de temperatura como se muestra en la Figura 11. Y de la misma manera si presionamos ls botones <B>INC_Button_In()</B> o <B>DEC_Button_In()</B> modificaremos los parametros respectivos:

![IMG_20240201_165725](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/37ffd91d-fdaf-41d5-bbd9-4d6d11795ae4)
<I><B>Figura 10.- Modificacion asincrona de la alarmas de temperatura: TH, TL</B></I>

Entre otros parametros que podemos modificar estan la cadencia de tiempo(<B><I>Time Cadence</I></B>) que nos define el tiempo en minutos que la alarma va a durar asi como la isteresis de temperatura ((<B><I>Temperature Ofset</I></B>)) que es el margen de error a partir del cual podria activarse la alarma de temperatura:

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/5edda146-641d-40a7-bf6b-901073e37704)
<I><B>Figura 11.- Modificacion de parametros adicionales: duracion de alarma en minutos y offset de temperatura</B></I>

<B>4.</B> Pero no ocurrira ninguna transferencia hacia la EEPROM hasta que todas las alarmas hayan sido actualizadas, por lo que <B>SHIFT_ALARM_Button_In()</B> debera presionarse unas 8 veces y posterioremente en la tarea 7 o <B>"Task 7"</B>se transfiera los 7 bytes empezando desde la direccion 0x0000 : 

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/e4e457a5-dfaf-4cd7-8970-75de729fa2a6)

<I><B>Figura 12.- Seccion de codigo de la tarea 7 o "Task 7": Escribir en la EEPROM</B></I>

Si se llega a ejecutar la tarea 7 o <B>"Task 7"</B> , el usuario recibira un aviso de confirmacion de escritura en alarmas:

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/75c6be75-84c5-4f8c-a2e0-f66e28a7b375)

<I><B>Figura 13.- Aviso de actualziacion de alarmas satisfactorio</B></I>

Tenga en cuenta que la <B><A HREF="https://ww1.microchip.com/downloads/en/devicedoc/21061h.pdf">EEPROM 24C32</A></B> tiene un modo de escritura por pagina, lo que ahorra tiempo de escritura y ahorra vida util de la memoria no volatil(ver datasheet pagina 6):

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/737a06e1-af3b-411b-b744-cc9d8b518282)

<B>5.</B> Finalmente en la tarea 8 o <B>Task 8</B> se imprime el valor actual de las alarmas cada 5 segundos por consola Serial asi como el valor actual de las alarmas acorde:

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/a35478a9-51d6-4235-8a3e-b56c8c9334d0)

<I><B>Figura 14.- Seccion de codigo de la tarea 8 o "Task 8"_ Mostrar Alarmas</B></I>

Podriamos entocnes resumir las 4 ultimas tareas como sigue:

![image](https://github.com/SerCandio/RTC-Calendar-ds1307-and-Thermostat-DS18B20/assets/106831539/49bbf923-c97a-4d60-87ea-6e0f723d7352)

(Resultado consola serial simulacion y placa)

<h2 dir="auto" tabindex="-1">Nota 1</h2>
Recuerde que el <B>ciclo de escritura</B> se tarda alrededor de <B>15 ms</B>. El usuario recibe confirmacion por la LCD que el contenido de la memoeria EEPROM fue actualizada con la informacion de las alarmas

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
