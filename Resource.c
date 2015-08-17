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

static char _rootPath[1000] = "/root/Escritorio/YeisonCServer/Files";

int checkResource(struct RequestInfo *pRequestInfo)
{
	int auxCode;
    decodeURL(pRequestInfo->_resource);
    char *auxRootPath = calloc(strlen(_rootPath) + strlen(pRequestInfo->_resource), sizeof(char));
    strcpy(auxRootPath, _rootPath);
    strcat(auxRootPath, pRequestInfo->_resource);
    auxCode = open(auxRootPath, O_RDONLY);
    free(auxRootPath);
    return auxCode;
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