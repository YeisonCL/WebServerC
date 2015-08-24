WebServerC
===========
## Descripción
WebServer creado en su totalidad en lenguaje C utilizando una metodología PRE-FORKED y PRE-THREAD el cual cuenta además con soporte para la ejecución de archivos tipo CGI.

##Running
Compilar usando el MakeFile disponible y ejecutar con las opciones disponibles.

```
make
./RayquazaWebServer
```

##Options

###-n
Define la cantidad de procesos a utilizar para la administración de conexiones entrantes en el servidor.

```
./RayquazaWebServer -n 50
```

###-f
Inicia el WebServer en modo PRE-FORKED

```
./RayquazaWebServer -n 50 -f
```

###-t
Inicia el WebServer en modo PRE-THREAD

```
./RayquazaWebServer -n 50 -t
```

###-w
Define el path principal donde se alojará el servidor, como sugerencia utilice la carpeta llamada "HardDrive" proporcionada.

```
./preforked-webserver -n 50 -f -w /root/www/http
```

###-p
Define el puerto donde iniciará el servidor, por defecto los puertos disponibles son 80:HTTP, 21:FTP, 22:SSH, 23:Telnet, 25:SMTP, 53:DNS y 161:SMNP cualquier otro dado sera tratado como protocolo HTTP

```
./preforked-webserver -n 50 -f -w /root/www/http -p 80
```

###-h
Brinda ayuda disponible sobre las opciones de configuración antes mencionadas.

```
./preforked-webserver -h
```

##Signals Handled

###SIGINT | SIGTERM

Use CTRL + C para apagar el web server.

##Notes
- El servidor por defecto se inicia en la dirección IP local asignada a la computadora.
- Sólo son soportadas las funciones GET, POST, PUT y DELETE del protocolo HTTP.
- Al utilizar la funcion PUT sólo es posible crear archivos de texto plano.
- El servidor funciona única y exclusivamente en un ambiente GNU/Linux.
- Por cuestiones de tiempo el modo PRE-THREAD presenta problemas de concurrencia que no fueron reparados, por ende es posible que el servidor se cierre sin ninguna explicación, el modo PRE-FORKED funciona perfectamente.
- El código no está completamente comentado.
- Para hacer funcionar el programa en alguna otra computadora se debe ir al documento "AttendRequest.c" y cambiar la direccion de la variable "_serverFilesPath" por la dirección en su computadora que apunte a la misma carpeta "Files".

