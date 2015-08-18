/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "Resource.h"
#include "SystemFunctions.h"

int checkResource(struct RequestInfo *pRequestInfo, char* pRootPath)
{
	int auxCode;
    decodeURL(pRequestInfo->_resource);
    char *auxRootPath = calloc(strlen(pRootPath) + strlen(pRequestInfo->_resource), sizeof(char));
    strcpy(auxRootPath, pRootPath);
    strcat(auxRootPath, pRequestInfo->_resource);
    auxCode = open(auxRootPath, O_RDONLY);
    free(auxRootPath);
    return auxCode;
}

int writeInResource(int pFile, char *pMessage)
{
	int writeBytes;
	int unwrittenBytes = strlen(pMessage);
	int result = 1;
	while (unwrittenBytes > 0) 
    {
    	writeBytes = write(pFile, pMessage, unwrittenBytes);
		if (writeBytes == -1) 
		{
			result = -1;
			break;
		}
		unwrittenBytes -= writeBytes;
		pMessage += writeBytes;
    }
    return result;
}

int createResource(char *pPathResource)
{
	int openFile = open(pPathResource, O_CREAT | O_RDWR);
	return openFile;
}

int openResource(char *pPathResource)
{
	int openFile = open(pPathResource, O_WRONLY);
	return openFile;
}

int sendResource (int pConnection, int pResource)
{
	char c;
	int controlSend;
	int result = 1;
    int  controlFlag = 1;
    while (controlFlag) 
    {
    	controlFlag = read(pResource, &c, 1);
		if ( controlFlag < 0 )
		{
			result = -1;
			break;
		}
		else if(controlFlag == 0)
		{
			break;
		}
		else
		{
			controlSend = send(pConnection, &c, 1, 0);
			if ( controlSend < 1 )
			{
				result = -1;
				break;
			}
		}
    }
    return result;
}