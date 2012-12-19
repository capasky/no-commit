nocommit: main.o collection.o dbapi.o stringutils.o convert.o
	gcc -o nocommit main.o collection.o dbapi.o stringutils.o convert.o -ltokyocabinet

client: TCPClient.o protocol.o api/inet/inetdef.h cmddef.h
		gcc -o client TCPClient.o protocol.o api/inet/inetdef.h cmddef.h

main.o: main.c collection.o
	gcc -c main.c

collection.o: api/data/collection.c dbapi.o stringutils.o convert.o
	gcc -c api/data/collection.c

dbapi.o: api/data/dbapi.c convert.o
	gcc -c api/data/dbapi.c -ltokyocabinet

stringutils.o: api/utils/stringutils.c
	gcc -c api/utils/stringutils.c

convert.o: api/utils/convert.c
	gcc -c api/utils/convert.c


TCPClient.o: api/inet/TCPClient.c 



clean:
	rm *.o
