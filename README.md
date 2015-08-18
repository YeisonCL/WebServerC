WebServerC
===========
## Descripción
WebServer creado en su totalidad en lenguaje C utilizando una metodología PRE-FORKED.

##Running
Compilar usando el MakeFile disponible y ejecutar con las opciones disponibles.

```
make
./preforked-webserver
```

##Options

###-n
Define la cantidad de procesos a utilizar para la administración de conexiones entrantes en el servidor.

```
./preforked-webserver -n 50
```

###-w
Define el path principal donde se alojará el servidor.

```
./preforked-webserver -n 50 -w /root/www/http
```

###-p
Define el puerto donde iniciará el servidor.

```
./preforked-webserver -n 50 -w /root/www/http -p 8081
```

###-h
Brinda ayuda disponible sobre las opciones de configuración antes mencionadas.

```
./preforked-webserver -h
```

##Signals Handled

###SIGINT

Use CTRL + C para apagar el web server.

##Notes
- El servidor por defecto se inicia en la dirección IP local asignada a la computadora.
- Sólo son soportadas las funciones GET, POST, PUT y DELETE del protocolo HTTP.
- Al utilizar la funcion PUT sólo es posible crear archivos de texto plano.


