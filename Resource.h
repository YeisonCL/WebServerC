/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#ifndef _RECOURSE_H
#define _RECOURSE_H

#include <fcntl.h>
#include <unistd.h>

#include "ParseRequest.h"

int checkResource(struct RequestInfo *pRequestInfo, char* pRootPath);
int createResource(char *pPathResource);
int writeInResource(int pFile, char *pMessage);
int openResource(char *pPathResource);
int sendResource (int pConnection, int pResource);

#endif