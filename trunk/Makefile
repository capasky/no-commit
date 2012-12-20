#nocommit local test
nocommit: main.o collection.o dbapi.o stringutils.o convert.o
		gcc -o nocommit main.o collection.o dbapi.o stringutils.o \
		convert.o base.h -ltokyocabinet

main.o: main.c collection.o
		gcc -c main.c

# client
client: client.o TCPClient.o protocol.o api/inet/inetdef.h command.o
		gcc -o client client.o TCPClient.o protocol.o command.o stringutils.o

client.o: client.c TCPClient.o protocol.o api/inet/inetdef.h command.o  \
		 stringutils.o
		gcc -c client.c

#server
server: server.o TCPServer.o TCPListener.o protocol.o api/inet/inetdef.h\
		collection.o dbapi.o convert.o command.o base.h
		gcc -o server server.o TCPListener.o TCPServer.o protocol.o api/inet/inetdef.h\
		 collection.o dbapi.o convert.o base.h -ltokyocabinet

server.o: server.c
		gcc -c server.c

command.o: command.c stringutils.o base.h
		gcc -c command.c

#api.data
collection.o: api/data/collection.c dbapi.o convert.o base.h
		gcc -c api/data/collection.c base.h

dbapi.o: api/data/dbapi.c convert.o
		gcc -c api/data/dbapi.c -ltokyocabinet

#api.utils
stringutils.o: api/utils/stringutils.c
		gcc -c api/utils/stringutils.c

convert.o: api/utils/convert.c base.h
		gcc -c api/utils/convert.c

#api.inet
TCPClient.o: api/inet/TCPClient.c api/inet/inetdef.h base.h
		gcc -c api/inet/TCPClient.c
		
TCPListener.o: api/inet/TCPListener.c api/inet/inetdef.h base.h
		gcc -c api/inet/TCPListener.c

TCPServer.o: api/inet/TCPServer.c api/inet/inetdef.h base.h
		gcc -c api/inet/TCPServer.c

protocol.o: api/inet/protocol.c api/inet/inetdef.h base.h
		gcc -c api/inet/protocol.c

clean:
	rm *.o

