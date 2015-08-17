
all: libancillary.a main.o SystemFunctions.o AttendRequest.o ParseRequest.o Resource.o RespondRequest.o WebServer.o
	gcc -Wall -std=c99 main.o SystemFunctions.o AttendRequest.o ParseRequest.o  Resource.o RespondRequest.o WebServer.o libancillary.a -o WebServerExtreme 

libancillary.a: fd_send.o fd_recv.o
	ar cr $@ fd_send.o fd_recv.o
	ranlib $@

fd_send.o: lib/ancillary.h
	gcc -c -ansi -pedantic -Wall -std=c99 lib/fd_send.c -o $@

fd_recv.o: lib/ancillary.h
	gcc -c -ansi -pedantic -Wall -std=c99 lib/fd_recv.c -o $@

main.o: main.c WebServer.h
	gcc -c -ansi -pedantic -D_SVID_SOURCE -Wall -std=c99 main.c -o $@

SystemFunctions.o: SystemFunctions.c SystemFunctions.h
	gcc -c -ansi -pedantic -Wall -std=c99 SystemFunctions.c -o $@

AttendRequest.o: AttendRequest.c AttendRequest.h ParseRequest.h SystemFunctions.h Resource.h RespondRequest.h
	gcc -c -ansi -pedantic -Wall -std=c99 AttendRequest.c -o $@

ParseRequest.o: ParseRequest.c ParseRequest.h SystemFunctions.h
	gcc -c -ansi -pedantic -Wall -std=c99 ParseRequest.c -o $@

Resource.o: Resource.c Resource.h SystemFunctions.h
	gcc -c -ansi -pedantic -Wall -std=c99 Resource.c -o $@	

RespondRequest.o: RespondRequest.c RespondRequest.h SystemFunctions.h
	gcc -c -ansi -pedantic -Wall -std=c99 RespondRequest.c -o $@

WebServer.o: WebServer.c WebServer.h SystemFunctions.h AttendRequest.h
	gcc -c -ansi -pedantic -D_SVID_SOURCE -Wall -std=c99 WebServer.c -o $@