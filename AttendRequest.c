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

char *_serverFilesPath = "/root/Escritorio/YeisonCServer/Files";
char *_textPlainContentType = "text/html";
char *_filesContentType = "application/octet-stream";
char *_contentType;

/*Funcion que se encarga de atender una nueva peticion*/
void attendIncomingRequest(int pConnection, char *pServerRootPath)
{
	struct RequestInfo _requestInfo;

	initRequestInfo(&_requestInfo);
	int auxValue = parseRequestInfo(pConnection, &_requestInfo);
	if(auxValue == -1)
	{
		print("Ocurrio un error grave en el servidor el cual no fue controlado.");
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
		attendOutputRequest(pConnection, &_requestInfo, pServerRootPath);
	}
}


void attendOutputDeleteRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath)
{
	char *auxPath = calloc(strlen(pServerRootPath) + strlen(pRequestInfo->_resource) + 7 - 1, sizeof(char));
	strcat(auxPath, "rm -rf ");
	strcat(auxPath, pServerRootPath);
	strcat(auxPath, pRequestInfo->_resource);
	system(auxPath);
	free(auxPath);
	int auxValue = respondRequest(pConnection, pRequestInfo, _contentType);
	if(auxValue == -1)
	{
		print("Ocurrio un error grave en el servidor el cual no fue controlado.");
	}
}

void attendOutputPostRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath)
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
	auxValue = respondRequest(pConnection, pRequestInfo, _contentType);
	if(auxValue == -1)
	{
		print("Ocurrio un error grave en el servidor el cual no fue controlado.");
	}
}

void attendOutputPutRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath)
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
		auxValue = respondRequest(pConnection, pRequestInfo, _contentType);
		if(auxValue == -1)
		{
			print("Ocurrio un error grave en el servidor el cual no fue controlado.");
		}
	}
}

void attendOutputGetRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath)
{
	if(!strcmp(pRequestInfo->_resource, "/"))
	{
		pRequestInfo->_resource = "/index.html";
		pServerRootPath = _serverFilesPath;
		_contentType = _textPlainContentType;
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
	    	_contentType = _textPlainContentType;
	    	resource = checkResource(pRequestInfo, _serverFilesPath);
	    	pRequestInfo->_status = 404;
	    }
	}
	int auxValue = respondRequest(pConnection, pRequestInfo, _contentType);
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

void attendOutputRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath)
{
	_contentType = _filesContentType;
	if(pRequestInfo->_method == UNSUPPORTED)
	{
		pRequestInfo->_status = 501;
		int auxValue = respondRequest(pConnection, pRequestInfo, _contentType);
		if(auxValue == -1)
		{
			print("Ocurrio un error grave en el servidor el cual no fue controlado.");
		}
	}
	else if(pRequestInfo->_method == GET)
	{
		attendOutputGetRequest(pConnection, pRequestInfo, pServerRootPath);
	}
	else if(pRequestInfo->_method == POST)
	{
		pRequestInfo->_status = 205;
		attendOutputPostRequest(pConnection, pRequestInfo, pServerRootPath);
	}
	else if(pRequestInfo->_method == PUT)
	{
		pRequestInfo->_status = 201;
		attendOutputPutRequest(pConnection, pRequestInfo, pServerRootPath);
	}
	else
	{
		pRequestInfo->_status = 202;
		attendOutputDeleteRequest(pConnection, pRequestInfo, pServerRootPath);
	}
}