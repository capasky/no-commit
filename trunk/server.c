/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               Server
 * Comments:             This file include the implement of server
 * Author:               yellhb
 * Create Date:          2012.12.15
 * Version:              1.0
 * Plateform:            Linux
 **********************************************************************/

/**
 * Revision Log:
 * @author              @date               @version
 * yellhb               2012.12.15          1.0.0.1
 */

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

#include "api/inet/inetdef.h"
#include "api/inet/protocol.h"
#include "api/inet/TCPListener.h"

#define PORT 				5533
#define IP_ADDR 			"192.168.130.194"
#define MAX_CONNECT_QUEUE	1024
#define MAX_BUF_SIZE		1024

int main ( int arg, char** argv )
{
	int 		sockfd = -1;
	char 		buf[MAX_BUF_SIZE];
	struct		sockaddr_in clientaddr;
	struct		sockaddr_in addr;
	int 		ret;
	socklen_t	clientaddr_len = sizeof ( struct sockaddr_in );
	NCProtocol	*ncprotocol;
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons ( PORT );
	addr.sin_addr.s_addr = inet_addr ( IP_ADDR );
	memset ( &addr.sin_zero, 0, 8 );

	sockfd = socket ( PF_INET, SOCK_STREAM, 0 );
	ret = bind ( sockfd, (struct sockaddr *) &addr, sizeof ( struct sockaddr ));
	if ( ret == -1 )
	{
		fprintf ( stderr, "Bind Error, %s : %d,", __FILE__, __LINE__ );
		fprintf ( stderr, "%s : %d\n", ( char* ) inet_ntoa ( addr.sin_addr), 
				ntohs ( addr.sin_port ));
		close ( sockfd );
		return -1;
	}

	ret = listen ( sockfd, MAX_CONNECT_QUEUE );

	while ( 1 )
	{
		int idata;
		int newfd = accept ( sockfd, (struct sockaddr * ) &clientaddr, &clientaddr_len );
		if ( newfd == -1 )
		{
			fprintf ( stderr, "Accept Error, %s : %d\n", __FILE__, __LINE__ );
		}
		else 
		{
			ret = recv ( newfd, buf, MAX_BUF_SIZE, 0 );
			
			if ( ret > 0 )
			{	
				ncprotocol = NCProtocol_Parse ( buf );
				printf ( "Recv Message from %s : %d\n Command: %d	chunkCount: %d ", 
						( char* ) inet_ntoa ( clientaddr.sin_addr), 
						ntohs ( clientaddr.sin_port ), 
						ncprotocol->command, 
						ncprotocol->chunkCount );
				for ( idata = 0; idata < ncprotocol->chunkCount; ++idata )
				{
					printf ( " Data%d : %s ", idata + 1, ncprotocol->dataChunk[idata]->data );
				}
				printf ( "\n" );
			}

			ret = send ( newfd, "hi", sizeof ( "hi" ), 0 );
			if ( ret > 0 )
			{
				printf ( "rely %s : %d\n", 
						( char* ) inet_ntoa ( clientaddr.sin_addr ), ntohs ( clientaddr.sin_port ));
			}
			close ( newfd );
		}
	}
	close ( sockfd );

	return 0;
}
