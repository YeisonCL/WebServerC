/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "WebServer.h"
#include "SystemFunctions.h"
#include "AttendRequest.h"
#include "lib/ancillary.h"

/*Funcion que se encarga de crear el socket que sera utilizado por el web server.*/
void createSocket()
{
	_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if(_socketServer == -1)
	{
		ErrorExit("Se ha presentado un error al intentar crear el socket de conexión.");
	}
	print("Socket principal creado con exito.");
}

/*Avisamos al sistema operativo que hemos creado un socket y queremos que una nuestro programa a el.*/
void bindSocket()
{
	_serverAdress.sin_family = AF_INET;
    _serverAdress.sin_addr.s_addr = htonl(INADDR_ANY);
    _serverAdress.sin_port = htons(_serverPort);
    if(bind(_socketServer, (struct sockaddr *) &_serverAdress, sizeof(_serverAdress)) == -1)
    {
    	ErrorExit("Se ha presentado un error al intentar asociar el socket de conexion.");
    }
    print("Socket principal asociado con exito.");
}

/*Funcion que se encarga de escuchar las conexiones entrantes al servidor.*/
void listeningConnections()
{
	if(listen(_socketServer, MAX_CONNECTIONS) == -1)
	{
		ErrorExit("Se ha producido un error al intentar escuchar conexiones.");
	}
	print("Escuchando nuevas conexiones entrantes.");
}

void closeChildProcess()
{
	for(int i = 0; i < _processMax; i++)
	{
		kill(_childProcess[i], SIGKILL);
	}
	free(_childProcess);
}

void exitServer()
{
	print("Cerrando socket principal.");
	close(_socketServer);
	print("Cerrando sockets de conexion interna.");
	close(_socketPair[1]);
	print("Cerrando los procesos hijos creados.");
	closeChildProcess();
	print("Hasta pronto.");
    exit(EXIT_SUCCESS); 

}

void detectCloseSignal()
{
	struct sigaction closeSignal;
	closeSignal.sa_handler = exitServer;
	sigemptyset(&closeSignal.sa_mask);
	closeSignal.sa_flags = 0;

   sigaction(SIGINT, &closeSignal, NULL);
}

void initializeWebServer(int argc, char *argv[])
{
	int option;
	int controlFlag = 0;
	int flagHelp = 0;
	while((option = getopt(argc,argv,"n:w:p:h")) != -1)
	{
		controlFlag = 1;
		switch (option)
		{
			case 'n':
				_processMax = charArrayToInt(optarg);
				break;
			case 'w':
				strcpy(_rootPath, optarg);
				break;
			case 'p':
				_serverPort = charArrayToInt(optarg);
				break;
			case 'h':
				flagHelp = 1;
				print("-n \t Cantidad de procesos a utilizar para la administracion de conexiones entrantes en el servidor.");
				print("-w \t Path principal donde se alojara el servidor.");
				print("-p \t Puerto donde se iniciara el servidor.");
				print("Ejemplo: ./preforked-webserver -n 20 -w /root/http/www -p 8081");
				break;
		}
	}
	if(controlFlag == 0)
	{
		print("Por favor digite los parametros de configuracion del servidor, utilice la opcion -h para mas informacion.");
	}
	else if(flagHelp == 0)
	{
		startWebServer();
	}
}

/*Funcion que se encarga de crear los procesos que se utilizaran para atender las conexiones entrantes*/
void startWebServer()
{
	_processNumber = 0;
	_pidFork = -1;
	createSocketPair();
	_childProcess = malloc(_processMax * sizeof(int));
	while(_processNumber < _processMax)
	{
		_pidFork = fork();
		if(_pidFork == 0)
		{
			break;
		}
		else
		{
			_childProcess[_processNumber] = _pidFork;
			_processNumber++;
			waitpid(-1, NULL, WNOHANG);
		}
	}
	int keyActiveProcess = ftok(FILE_KEY, KEY_ACTIVE_PROCESS);
	int keyIncomingConnection = ftok(FILE_KEY, KEY_INCOMING_CONNECTION);
	int idZoneActiveProcess = shmget(keyActiveProcess, sizeof(int) * _processMax, 0777 | IPC_CREAT);
	int idZoneIncomingConnection = shmget(keyIncomingConnection, sizeof(int), 0777 | IPC_CREAT);
	_activesProcess = shmat(idZoneActiveProcess, 0, 0);
	_incomingConnection = shmat(idZoneIncomingConnection, 0, 0);
	detectCloseSignal();
	acceptIncomingConnections();
}

/*Funcion encargada de colocar la memoria compartida de procesos activos a ceros*/
void fillToZero()
{
	for(int i = 0; i < _processMax; i++)
	{
		*(_activesProcess + i) = 0;
	}
}

/*Funcion que verifica si existe algun proceso disponible para ser asignado a una nueva conexion entrante*/
int verifyProcessAvailable()
{
	int value = -1;
	for(int i = 0; i < _processMax; i++)
	{
		if(*(_activesProcess + i) == 0)
		{
			value = i;
			break;
		}
	}
	return value;
}

/*Funcion encargada de crear los sockets de comunicacion interna*/
void createSocketPair()
{
	if(socketpair(PF_UNIX, SOCK_STREAM, 0, _socketPair)) 
	{
		ErrorExit("Se ha presentado un error al intentar crear los sockets de comunicacion interna.");
    } 
    else 
    {
		print("Sockets de comunicacion interna establecidos correctamente.");
    }
}

/*Funcion encargada de aceptar conexiones entrantes y crear un nuevo proceso para cada una de ellas.*/
void acceptIncomingConnections()
{
	if(_pidFork != 0)
	{
		createSocket();
		bindSocket();
		listeningConnections();
		fillToZero();
		close(_socketPair[1]);
		while(1)
		{
			*_incomingConnection = accept(_socketServer, NULL, NULL);
			print("Nueva conexion entrante recibida.");
			if(*_incomingConnection == -1)
			{
				ErrorExit("Se ha presentado un error al intentar aceptar una nueva conexion entrante.");
			}
			else
			{
				int processAvailable = verifyProcessAvailable();
				if(processAvailable >= 0)
				{
					ancil_send_fd(_socketPair[0], *_incomingConnection);
					*(_activesProcess + processAvailable) = 1; /*Colocamos el proceso en activo*/
					while(*_incomingConnection != 0)
					{
						sleep(1); /*Espere un segundo a que el proceso hijo tome la conexion entrante*/
					}
				}
				else
				{
					print("Conexion rechazada por exceso de carga en el servidor.");
					char *errorMessage = "Ups, estamos al maximo de carga permitida en el servidor, por favor intentalo nuevamente en un momento.";
					send(*_incomingConnection, errorMessage, strlen(errorMessage), 0);
					if (shutdown(*_incomingConnection, 2) == -1)
					{
						ErrorExit("Se ha producido un error al intentar cerrar la conexion en el proceso padre.");
					}
					else
					{
						close(*_incomingConnection);
						print("Conexion en el proceso padre cerrada correctamente.");
					}
				}
			}
		}
	}
	else
	{
		int keyActiveProcess = ftok(FILE_KEY, KEY_ACTIVE_PROCESS);
		int keyIncomingConnection = ftok(FILE_KEY, KEY_INCOMING_CONNECTION);
		int idZoneActiveProcess = shmget(keyActiveProcess, sizeof(int) * _processMax, 0777 | IPC_CREAT);
		int idZoneIncomingConnection = shmget(keyIncomingConnection, sizeof(int), 0777 | IPC_CREAT);
		_activesProcess = shmat(idZoneActiveProcess, 0, 0);
		_incomingConnection = shmat(idZoneIncomingConnection, 0, 0);
		close(_socketPair[0]);
		while(1)
		{
			if(*(_activesProcess + _processNumber) == 1)
			{
				ancil_recv_fd(_socketPair[1], &_incomingConnectionFileHandler);
				*_incomingConnection = 0; /*Liberamos al proceso padre para que pueda aceptar mas conexiones*/
				attendIncomingRequest(_incomingConnectionFileHandler, _rootPath);
				shutdown(_incomingConnectionFileHandler, 2);
				close(_incomingConnectionFileHandler);
				print("Se ha cerrado una conexion ya atendida.");
				*(_activesProcess + _processNumber) = 0; /*Liberamos el proceso nuevamente y lo dejamos disponible para atender otra conexion nuevamente*/
			}
			else
			{
				sleep(1); /*Duerma el proceso por un segundo*/
			}
		}
	}
}

