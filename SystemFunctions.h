/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#ifndef _SYSTEMFUNCTIONS_H
#define _SYSTEMFUNCTIONS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include "ParseRequest.h"

int _result;

int readLine(int pConnection, char *pReadBuffer, int pMaxRead, int pPosContent);
int charArrayToInt(char *pArray);
int writeLine (int pConnection, char *pReadBuffer, int pNumberCharacters);
void ErrorExit(char const * pErrorMessage);
void stringUpper(char *pReadBuffer); /*Funcion que pone en mayusculas un string dado*/
void decodeURL(char *pReadBuffer);
void print(char const * pMessage);

#endif
