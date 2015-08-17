/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "SystemFunctions.h"

void ErrorExit(char const * pErrorMessage)
{
	fprintf(stderr, "Web Server: %s\n", pErrorMessage);
    exit(EXIT_FAILURE);
}

void print(char const * pMessage)
{
	fprintf(stdout, "Web Server: %s\n", pMessage);
}

void stringUpper(char *pReadBuffer) 
{
    while (*pReadBuffer != '\0') 
    {
		*pReadBuffer = toupper(*pReadBuffer);
		pReadBuffer++;
    }
}

int charArrayToInt(char *pArray)
{
	return atoi(pArray);
}

void decodeURL(char *pReadBuffer) 
{
    char asciiNum[3] = {0};
    int c;
    int i = 0;
    
    while ( pReadBuffer[i] ) 
    {
		if ( pReadBuffer[i] == '+' )
		{
			pReadBuffer[i] = ' ';
		}
		else if ( pReadBuffer[i] == '%' ) 
		{
		    asciiNum[0] = pReadBuffer[i + 1];
		    asciiNum[1] = pReadBuffer[i + 2];
		    pReadBuffer[i] = strtol(asciiNum, NULL, 16);
		    c = i + 1;
		    do 
		    {
				pReadBuffer[c] = pReadBuffer[c + 2];
		    } while (pReadBuffer[2 + (c++)]);
		}
		i++;
    }
}

int writeLine (int pConnection, char *pReadBuffer, int pNumberCharacters)
{
	int sentBytes;
	int unsentBytes = pNumberCharacters;

	_result = 1;

    while (unsentBytes > 0) 
    {
    	sentBytes = send(pConnection, pReadBuffer, unsentBytes, 0);
		if (sentBytes <= 0 ) 
		{
		    if (errno == EINTR)
		    {
		    	sentBytes = 0;
		    }
		    else
		    {
		    	_result = -1;
		    	break;
		    }
		}
		unsentBytes -= sentBytes;
		pReadBuffer += sentBytes;
    }
    return _result;
}

int readLine(int pConnection, char *pReadBuffer, int pMaxRead, int pPosContent)
{
	_result = -1;
	char c;
    for (int i = 0; i < pMaxRead; i++) 
    {	
    	int valueAux = recv(pConnection, &c, 1, 0);
		if (valueAux == 1 ) 
		{
		    *pReadBuffer++ = c;
		    if (c == '\n')
		    {
		    	_result = 1;
		    	break;
		    }
		    else if(pPosContent != 0)
		    {
		    	if(i + 1 == pPosContent)
		    	{
		    		_result = 1;
		    		break;	
		    	}
		    }
		}
		else if (valueAux == 0 ) 
		{
			_result = 0;
			break;
		}
		else 
		{
			_result = -1;
			break;
		}
    }
    return _result;
}