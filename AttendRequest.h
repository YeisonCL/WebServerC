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

void attendIncomingRequest(int pConnection, char *pRootPath, int pPort, int pProcessMax, int pActiveProcess);
void attendIncomingHttpRequest(int pConnection, char *pServerRootPath); /*Funcion que se encarga de atender una nueva peticion*/
void attendIncomingFtpRequest(int pConnection, int pActiveProcess, int pProcessMax);
void attendIncomingTelnetRequest(int pConnection);
void attendIncomingDNSRequest(int pConnection);
void attendIncomingSSHRequest(int pConnection);
void attendIncomingSMTPRequest(int pConnection);
void attendIncomingSNMPRequest(int pConnection);
void doNothing(int pConnection);
void attendOutputRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType);
void attendOutputGetRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType);
void attendOutputPutRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType);
void attendOutputPostRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType);
void attendOutputDeleteRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pServerRootPath, char *pContentType);

#endif