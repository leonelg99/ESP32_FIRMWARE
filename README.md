<h1><b>Proyecto MAW</b></h1>

<p>Este es un proyecto para la materia Taller de Proyecto I de la carrera de Ingenieria en Computación de la Universidad Nacional de La Plata. Consiste en un vehiculo con cuatro motores, el cual es controlado mediante un joystick. Además, esta compuesto de una cámara con la que realiza una transmisión de video para poder facilitar el control del vehiculo.</p>
<p>En este caso, se utiliza el ESP32 en conjunto con la placa EDU-CIAA-NXP de forma Master-Slave, indicandole a la placa las instrucciones a realizar.</p>

<h2><b>ESP32 FIRMWARE</b></h2>
<p>Esta biblioteca se utiliza para configurar el ESP32-CAM como un Access Point, en el cual otros dispositivos puedan conectarse a el e intercambiar información. Mientras haya un cliente conectado, recibirá los mensajes, los procesará y según quien sea el encargado de realizar esas acciones, ya sea la EDU-CIAA-NXP o el ESP32 mismo, los almacenará en un buffer o se encargará de realizar las tareas requeridas.</p>
<p>Concurrentemente, estará intercambiando información con la EDU-CIAA-NXP mediante UART, en el caso de que le lleguen mensajes, los almacenará en un buffer para luego enviarlos al dispositivo conectado. </p>
<p>Finalmente, se realiza la correspondiente configuración de la cámara para que capture frames con una calidad media SVGA, y las envía utilizando HTTP, para luego realizar una transmisión de video en el dispositivo que las reciba.</p>

<h2>Desarrolladores</h2>
<p>Este proyecto fue desarrollado por Guerrico Leonel, Ossola Florencia, y Pérez Balcedo Octavio</p>
