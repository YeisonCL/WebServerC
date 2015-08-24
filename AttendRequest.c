/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "AttendRequest.h"
#include "ParseRequest.h"
#include "SystemFunctions.h"
#include "Resource.h"
#include "RespondRequest.h"
#include "CGIExecution.h"

char *_serverFilesPath = "/root/Escritorio/YeisonCServer/WebServer/CodigoFuente/Files";


void attendIncomingRequest(int pConnection, char *pRootPath, int pPort, int pProcessMax, int pActiveProcess)
{
	if(pPort == 80)
	{
		attendIncomingHttpRequest(pConnection, pRootPath);
	}
	else if(pPort == 21)
	{
		attendIncomingFtpRequest(pConnection, pActiveProcess, pProcessMax);
	}
	else if(pPort == 22)
	{
		attendIncomingSSHRequest(pConnection);
	}
	else if(pPort == 23)
	{
		attendIncomingTelnetRequest(pConnection);
	}
	else if(pPort == 25)
	{
		attendIncomingSMTPRequest(pConnection);
	}
	else if(pPort == 53)
	{
		attendIncomingDNSRequest(pConnection);
	}
	else if(pPort == 161)
	{
		attendIncomingSNMPRequest(pConnection);
	}
	else
	{
		attendIncomingHttpRequest(pConnection, pRootPath);
	}
}

void attendIncomingDNSRequest(int pConnection)
{
	char *welcomeDNS = "NOT IMPLEMENTED.\r\n";
	writeLine(pConnection, welcomeDNS, strlen(welcomeDNS));
	doNothing(pConnection);
}

void attendIncomingSSHRequest(int pConnection)
{
	char *welcomeSSH = "The authenticity of host 'Rayquaza Server' can't be established.\r\n";
	char *rsa = "RSA key fingerprint is 97:4f:66:f5:96:ba:6d:b2:ef:65:35:45:18:0d:cc:29 \r\n";
	char *continueServer = "Are you sure you want to continue connecting (yes/no)?\r\n";
	writeLine(pConnection, welcomeSSH, strlen(welcomeSSH));
	writeLine(pConnection, rsa, strlen(rsa));
	writeLine(pConnection, continueServer, strlen(continueServer));
	doNothing(pConnection);
}

/*Funcion que se encarga de atender una nueva peticion*/
void attendIncomingHttpRequest(int pConnection, char *pServerRootPath)
{
	struct RequestInfo _requestInfo;
	char *_contentType = "text/html";

	initRequestInfo(&_requestInfo);
	int auxValue = parseRequestInfo(pConnection, &_requestInfo);
	if(auxValue == -1)
	{
		_requestInfo._status = 500;
		auxValue = respondRequest(pConnection, &_requestInfo, _contentType);
		if(auxValue == -1)
		{
			print("Ocurrio un error grave en el servidor el cual no fue controlado.");
		}
	}
	else if(auxValue == 0)
	{
		print("Al parecer el cliente se ha desconectado.");
	}
	else
	{
		attendOutputRequest(pConnection, &_requestInfo, pServerRootPath, _contentType);
	}
}

void doNothing(int pConnection)
{
	char c;
	int valueAux;
	do
	{
		valueAux = recv(pConnection, &c, 1, 0);
	}while(valueAux != 0);
}

void attendIncomingFtpRequest(int pConnection, int pActiveProcess, int pProcessMax)
{
	int activeProcessInt = pActiveProcess;
	char activeProcessStr[21];
	char maxProcessStr[21];
	sprintf(activeProcessStr, "%d", activeProcessInt);
	sprintf(maxProcessStr, "%d", pProcessMax);
	char ftpResponse[85] = "220 FTP server ready, ";
	strcat(ftpResponse, activeProcessStr);
	strcat(ftpResponse, " active clients of ");
	strcat(ftpResponse, maxProcessStr);
	strcat(ftpResponse, " simultaneous clients allowed.\r\n");
	writeLine(pConnection, ftpResponse, strlen(ftpResponse));
	doNothing(pConnection);
}

void attendIncomingSNMPRequest(int pConnection)
{
	char *versionSNMP = "Version: 1\r\n";
	char *community = "Community: public \r\n";
	char *pdu = "PDU type: GET\r\n";
	char *request = "Request Id: 0\r\n";
	char *error = "Error Status: NO ERROR\r\n";
	char *errorIndex = "Error Index: 0\r\n";
	writeLine(pConnection, versionSNMP, strlen(versionSNMP));
	writeLine(pConnection, community, strlen(community));
	writeLine(pConnection, pdu, strlen(pdu));
	writeLine(pConnection, request, strlen(request));
	writeLine(pConnection, error, strlen(error));
	writeLine(pConnection, errorIndex, strlen(errorIndex));
	doNothing(pConnection);
}

void attendIncomingTelnetRequest(int pConnection)
{
	char *welcomeTelnet = "Welcome to Rayquaza Telnet Service\r\n";
	char *loginMsg = "Login: \r\n";
	char *passwordMsg = "Password: \r\n";
	writeLine(pConnection, welcomeTelnet, strlen(welcomeTelnet));
	writeLine(pConnection, loginMsg, strlen(loginMsg));
	writeLine(pConnection, passwordMsg, strlen(passwordMsg));
	doNothing(pConnection);
}

void attendIncomingSMTPRequest(int pConnection)
{
	char *welcomeSMTP = "220 Server SMTP\r\n";
	writeLine(pConnection, welcomeSMTP, strlen(welcomeSMTP));
	doNothing(pConnection);
}

void attendOutputDeleteRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType)
{
	char *auxPath = calloc(strlen(pServerRootPath) + strlen(pRequestInfo->_resource) + 7 - 1, sizeof(char));
	strcat(auxPath, "rm -rf ");
	strcat(auxPath, pServerRootPath);
	strcat(auxPath, pRequestInfo->_resource);
	system(auxPath);
	free(auxPath);
	int auxValue = respondRequest(pConnection, pRequestInfo, pContentType);
	if(auxValue == -1)
	{
		print("Ocurrio un error grave en el servidor el cual no fue controlado.");
	}
}

int testCGIRequest(struct RequestInfo *pRequestInfo)
{
	if(!strncmp(pRequestInfo->_resource, "/cgi-bin", 8) && *(pRequestInfo->_resource + 8) != '\0')
	{
		if(*(pRequestInfo->_resource + 8) == '/' && *(pRequestInfo->_resource + 9) == '\0')
		{
			return -2;
		}
		else
		{
			return checkResource(pRequestInfo, "/root/Escritorio/YeisonCServer/Server");
		}
	}
	else
	{
		return -2;
	}
}

void attendOutputPostRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType)
{
	int verifyCGI = testCGIRequest(pRequestInfo);
	fprintf(stderr, "VerifyCGI: %d\n", verifyCGI);
	if(verifyCGI >= 0)
	{
		pRequestInfo->_status = 200;
		fprintf(stderr, "Es un archivo valido\n");
		close(verifyCGI);
		pRequestInfo->_resource++;
		fprintf(stderr, "Resource tiene: %c\n", *pRequestInfo->_resource);
		char *message = executePostCGI(pRequestInfo->_resource, pRequestInfo->_content);
		fprintf(stderr, "Mensaje del CGI: %s\n", message);
		checkContentType(pRequestInfo->_resource, &pContentType);
		fprintf(stderr, "Ya hice chek del content type\n");
		respondRequest(pConnection, pRequestInfo, pContentType);
		fprintf(stderr, "Mande los headers\n");
		writeLine(pConnection, message, strlen(message));
		fprintf(stderr, "Mande el mensaje\n");
	}
	else if(verifyCGI == -1)
	{
		fprintf(stderr, "No es un archivo valido\n");
		pRequestInfo->_resource = "/404.html";
	    int resource = checkResource(pRequestInfo, _serverFilesPath);
	    pRequestInfo->_status = 404;
	    fprintf(stderr, "Aca1\n");
	    checkContentType(pRequestInfo->_resource, &pContentType);
	    fprintf(stderr, "Aca2\n");
		respondRequest(pConnection, pRequestInfo, pContentType);
		fprintf(stderr, "Aca3\n");
		sendResource(pConnection, resource);
		fprintf(stderr, "Aca4\n");
	}
	else
	{
		int auxValue;
		char *auxPath = calloc(strlen(pServerRootPath) + strlen(pRequestInfo->_resource) - 1, sizeof(char));
		strcat(auxPath, pServerRootPath);
		strcat(auxPath, pRequestInfo->_resource);
		int openFile = openResource(auxPath);
		if (openFile < 0) 
		{
			pRequestInfo->_status = 304;
		}
		else
		{
			if(writeInResource(openFile, pRequestInfo->_content) == -1)
			{
				print("Ocurrio un error grave en el servidor el cual no fue controlado.");
				pRequestInfo->_status = 500;
			}
			close(openFile);
		}
		free(auxPath);
		auxValue = respondRequest(pConnection, pRequestInfo, pContentType);
		if(auxValue == -1)
		{
			print("Ocurrio un error grave en el servidor el cual no fue controlado.");
		}
	}
}

void attendOutputPutRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType)
{
	int auxValue;
	char *resource = pRequestInfo->_resource;
	char *auxPath = calloc(strlen(pServerRootPath) + strlen(pRequestInfo->_resource) - 1, sizeof(char));
	strcat(auxPath, pServerRootPath);
	strcat(auxPath, "/");
	if(strlen(resource) == 1)
	{
		print("La direccion dada es incorrecta.");
		pRequestInfo->_status = 500;
	}
	else
	{
		resource++;
		while(*resource != '\0')
		{
			char *auxPtr = strchr(resource, '/');
			if(auxPtr != NULL)
			{
				int len = auxPtr - resource;
				auxPtr = calloc(len, sizeof(char));
	    		strncpy(auxPtr, resource, len);
	    		resource = resource + len + 1;
	    		strcat(auxPath, auxPtr);
	    		auxValue = mkdir(auxPath, 0700);
	    		if( auxValue == -1 && errno != EEXIST)
	    		{
	    			print("Ocurrio un error grave en el servidor el cual no fue controlado.");
	    			free(auxPtr);
	    			pRequestInfo->_status = 500;
	    			break;
	    		}
	    		free(auxPtr);
	    		strcat(auxPath, "/");
			}
			else
			{
				auxPtr = strchr(resource, '\0');
				int len = auxPtr - resource;
				auxPtr = calloc(len, sizeof(char));
	    		strncpy(auxPtr, resource, len);
	    		resource = resource + len;
	    		strcat(auxPath, auxPtr);
	    		free(auxPtr);
	    		int fileOpen = createResource(auxPath);
	    		if(fileOpen == -1)
	    		{
	    			print("Ocurrio un error grave en el servidor el cual no fue controlado.");
	    			pRequestInfo->_status = 500;
	    			break;
	    		}
	    		else
	    		{
	    			if(writeInResource(fileOpen, pRequestInfo->_content) == -1)
	    			{
	    				close(fileOpen);
	    				print("Ocurrio un error grave en el servidor el cual no fue controlado.");
	    				pRequestInfo->_status = 500;
	    				break;
	    			}
	    			close(fileOpen);
	    		}
			}
		}
		free(auxPath);
		auxValue = respondRequest(pConnection, pRequestInfo, pContentType);
		if(auxValue == -1)
		{
			print("Ocurrio un error grave en el servidor el cual no fue controlado.");
		}
	}
}

void attendOutputGetRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType)
{
	if(!strcmp(pRequestInfo->_resource, "/"))
	{
		pRequestInfo->_resource = "/index.html";
		pServerRootPath = _serverFilesPath;
	}
	int resource = checkResource(pRequestInfo, pServerRootPath);
	if ( resource < 0) 
	{
	    if (errno == EACCES)
	    {
	    	pRequestInfo->_status = 401;
	    }
	    else
	    {
	    	pRequestInfo->_resource = "/404.html";
	    	resource = checkResource(pRequestInfo, _serverFilesPath);
	    	pRequestInfo->_status = 404;
	    }
	}
	checkContentType(pRequestInfo->_resource, &pContentType);
	int auxValue = respondRequest(pConnection, pRequestInfo, pContentType);
	if(auxValue == -1)
	{
		print("Ocurrio un error grave en el servidor el cual no fue controlado.");
	}
	else
	{
		if (pRequestInfo->_status == 200 || pRequestInfo->_status == 404) 
		{
			if (sendResource(pConnection, resource) < 0)
			{
				print("No se pudo enviar el archivo al destinatario.");
			}
			close(resource);
		}
	}
}

void attendOutputRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType)
{
	if(pRequestInfo->_method == UNSUPPORTED)
	{
		pRequestInfo->_status = 501;
		int auxValue = respondRequest(pConnection, pRequestInfo, pContentType);
		if(auxValue == -1)
		{
			print("Ocurrio un error grave en el servidor el cual no fue controlado.");
		}
	}
	else if(pRequestInfo->_method == GET)
	{
		attendOutputGetRequest(pConnection, pRequestInfo, pServerRootPath, pContentType);
	}
	else if(pRequestInfo->_method == POST)
	{
		pRequestInfo->_status = 205;
		attendOutputPostRequest(pConnection, pRequestInfo, pServerRootPath, pContentType);
	}
	else if(pRequestInfo->_method == PUT)
	{
		pRequestInfo->_status = 201;
		attendOutputPutRequest(pConnection, pRequestInfo, pServerRootPath, pContentType);
	}
	else
	{
		pRequestInfo->_status = 202;
		attendOutputDeleteRequest(pConnection, pRequestInfo, pServerRootPath, pContentType);
	}
}