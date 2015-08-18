WebServerC
===========
## Descripción
WebServer creado en su totalidad en lenguaje C utilizando una metodología PRE-FORKED.

##Running
Compilar usando el MakeFile disponible y ejecutar con las opciones disponibles.

'''
make
./preforked-webserver -h
'''

##Options

###-n
Define la cantidad de procesos a utilizar para la administración de conexiones entrantes en el servidor.

###-w
Define el path principal donde se alojará el servidor.

###-p
Define el puerto donde iniciará el servidor.

##Signals Handled

###SIGINT

Use CTRL + C para apagar el web server.

##Notes
El servidor por defecto se inicia en la dirección IP local asignada a la computadora.


