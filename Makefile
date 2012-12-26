#nocommit local test
nocommit: main.o collection.o dbapi.o stringutils.o convert.o
		gcc -o nocommit main.o collection.o dbapi.o stringutils.o \
		convert.o  -ltokyocabinet

main.o: main.c collection.o
		gcc -c main.c

# client
client: client.o ncclient.o
		gcc -o client client.o ncclient.o command.o TCPClient.o stringutils.o protocol.o
		@printf '****************************************\n'
		@printf '* Build 完成！
		@printf '****************************************\n

client.o: client.c ncclient.o
		gcc -c client.c

ncclient.o: ncclient.c command.o TCPClient.o protocol.o stringutils.o
		gcc -c ncclient.c

#server
server: server.o TCPServer.o TCPListener.o protocol.o api/inet/inetdef.h\
		collection.o dbapi.o convert.o command.o
		gcc -o server server.o TCPListener.o TCPServer.o protocol.o api/inet/inetdef.h\
		 collection.o dbapi.o convert.o -ltokyocabinet

server.o: server.c
		gcc -c server.c

command.o: command.c stringutils.o 
		gcc -c command.c

#api.data
collection.o: api/data/collection.c dbapi.o convert.o 
		gcc -c api/data/collection.c 

dbapi.o: api/data/dbapi.c convert.o
		gcc -c api/data/dbapi.c -ltokyocabinet

#api.utils
stringutils.o: api/utils/stringutils.c
		gcc -c api/utils/stringutils.c

convert.o: api/utils/convert.c 
		gcc -c api/utils/convert.c

#api.inet
TCPClient.o: api/inet/TCPClient.c api/inet/inetdef.h 
		gcc -c api/inet/TCPClient.c
		
TCPListener.o: api/inet/TCPListener.c api/inet/inetdef.h 
		gcc -c api/inet/TCPListener.c

TCPServer.o: api/inet/TCPServer.c api/inet/inetdef.h 
		gcc -c api/inet/TCPServer.c

protocol.o: api/inet/protocol.c api/inet/inetdef.h 
		gcc -c api/inet/protocol.c

clean:
	rm *.o

