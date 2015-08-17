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

/*Funcion que se encarga de atender una nueva peticion*/
void attendIncomingRequest(int pConnection)
{
	struct RequestInfo _requestInfo;

	initRequestInfo(&_requestInfo);
	int auxValue = parseRequestInfo(pConnection, &_requestInfo);
	if(auxValue == -1)
	{
		print("Ocurrio un error grave en el servidor el cual no fue controlado.");
	}
	else if(auxValue == 0)
	{
		print("Al parecer el cliente se ha desconectado.");
	}
	else
	{
		attendOutputRequest(pConnection, &_requestInfo);
	}
}

void attendOutputGetRequest(int pConnection, struct RequestInfo *pRequestInfo)
{
	if(!strcmp(pRequestInfo->_resource, "/"))
	{
		pRequestInfo->_resource = "/index.html";
	}
	int resource = checkResource(pRequestInfo);
	if ( resource < 0) 
	{
	    if (errno == EACCES)
	    {
	    	pRequestInfo->_status = 401;
	    }
	    else
	    {
	    	pRequestInfo->_resource = "/notfound.html";
	    	resource = checkResource(pRequestInfo);
	    	pRequestInfo->_status = 404;
	    }
	}
	int auxValue = respondRequest(pConnection, pRequestInfo);
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

void attendOutputRequest(int pConnection, struct RequestInfo *pRequestInfo)
{
	if(pRequestInfo->_method == UNSUPPORTED)
	{
		/*Si el metodo no es soportado*/
	}
	else if(pRequestInfo->_method == GET)
	{
		attendOutputGetRequest(pConnection, pRequestInfo);
	}
	else if(pRequestInfo->_method == POST)
	{
		/*Aqui va la solicitud POST*/
	}
	else if(pRequestInfo->_method == PUT)
	{
		/*Aqui va la solicitud PUT*/
	}
	else
	{
		/*Aqui va la solicitud DELETE*/
	}
}