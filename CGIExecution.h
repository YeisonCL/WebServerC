#ifndef CGI_EXECUTION
#define CGI_EXECUTION

char* executeCGI(char* pPath, char **pArgs, char **pArgenv);
char* executeGetCGI(char* pPath, char* pQueryString);
char* executePostCGI(char* pPath, char* pMessage);

#endif