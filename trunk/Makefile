#nocommit local test
nocommit: main.o collection.o dbapi.o stringutils.o convert.o
		gcc -o nocommit main.o collection.o dbapi.o stringutils.o \
		convert.o  -ltokyocabinet

main.o: main.c collection.o
		gcc -c main.c

# client
client: client.o ncclient.o
		gcc -o client client.o ncclient.o command.o TCPClient.o stringutils.o protocol.o\
		updater.o servernode.o -lpthread
		@printf '****************************************\n'
		@printf '* Build 完成！							 \n'
		@printf '****************************************\n'

client.o: client.c ncclient.o
		gcc -c client.c

ncclient.o: ncclient.c command.o TCPClient.o protocol.o stringutils.o updater.o servernode.o
		gcc -c ncclient.c

#server
server: server.o TCPServer.o TCPListener.o protocol.o api/inet/inetdef.h\
		collection.o dbapi.o convert.o command.o servercmd.o stringutils.o
		gcc -o server server.o TCPListener.o TCPServer.o protocol.o api/inet/inetdef.h\
		 collection.o dbapi.o convert.o command.o servercmd.o stringutils.o -ltokyocabinet
		@printf '****************************************\n'
		@printf '* Build 完成！							 \n'
		@printf '****************************************\n'

server.o: server.c
		gcc -c server.c

command.o: command.c stringutils.o 
		gcc -c command.c
#masterserver
masterserver: server.o TCPServer.o TCPListener.o protocol.o api/inet/inetdef.h\
		collection.o dbapi.o convert.o command.o servercmd.o stringutils.o
		gcc -o server server.o TCPListener.o TCPServer.o protocol.o api/inet/inetdef.h\
		 collection.o dbapi.o convert.o command.o servercmd.o stringutils.o -ltokyocabinet
		@printf '****************************************\n'
		@printf '* Build 完成！							 \n'
		@printf '****************************************\n'

#api
servercmd.o : api/servercmd.c protocol.o command.o
		gcc -c api/servercmd.c

#api.core
updater.o: api/core/updater.c command.o protocol.o servernode.o
		gcc -c api/core/updater.c 

servernode.o: api/core/servernode.c api/inet/inetdef.h
		gcc -c api/core/servernode.c

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

