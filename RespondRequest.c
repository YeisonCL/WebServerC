/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "RespondRequest.h"
#include "SystemFunctions.h"

int respondRequest(int pConnection, struct RequestInfo *pRequestInfo)
{
	char bufferAux[100];
    sprintf(bufferAux, "HTTP/1.1 %d OK\r\n", pRequestInfo->_status);
	char *headersArray[5] = {bufferAux, "Server: YACLServer V0.1\r\n", "Content-Type: text/html\r\n", "Connection: keep-alive\r\n", "\r\n"};
	int auxValue = 1;
	int count = 0;
	while(auxValue != -1 && count < 5)
	{
		auxValue = writeLine(pConnection, headersArray[count], strlen(headersArray[count]));
		count++;
	}
    return auxValue;
}