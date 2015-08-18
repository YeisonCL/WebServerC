/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#ifndef _ATTENDREQUEST_H
#define _ATTENDREQUEST_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "ParseRequest.h"

void attendIncomingRequest(int pConnection, char *pServerRootPath); /*Funcion que se encarga de atender una nueva peticion*/
void attendOutputRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath);
void attendOutputGetRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath);
void attendOutputPutRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath);
void attendOutputPostRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath);
void attendOutputDeleteRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath);

#endif