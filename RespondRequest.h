/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#ifndef _RESPONDREQUEST_H
#define _RESPONDREQUEST_H

#include <unistd.h>
#include <stdio.h>

#include "ParseRequest.h"

int respondRequest(int pConnection, struct RequestInfo *pRequestInfo);

#endif