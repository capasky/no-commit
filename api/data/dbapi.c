/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.data.dbapi
 * Comments:             This file include the implementation of 
 *						 database interface
 * Author:               yellhb
 * Create Date:          2012.11.29
 * Version:              1.0
 * Plateform:            Linux
 **********************************************************************/

/**
 * Revision Log:
 * @author				@date				@version
 * yellhb				2012.11.29			1.0.0.1
 */

#include <tcutil.h>
#include <tchdb.h>

/**
 * Create new Database instance
 */
void * createDB ()
{
	return tchdbnew();
}

/**
 * open Database according the filename
 */
int openDB ( void *pdb, char *filename )
{
	int ecode;
	if (!tchdbopen (( TCHDB *) pdb, filename, HDBOWRITER | HDBOCREAT))
	{
		ecode = tchdbecode ( pdb );
		fprintf ( stderr, "open error: %s\n", tchdberrmsg ( ecode));
		return -1;
	}
	return 0;
}

/**
 * initalize Database
 */
void initDB ( void *pdb )
{
	tchdbiterinit (( TCHDB * ) pdb );
}

/**
 * Insert key-value pair into Database
 */
int insertKV ( void *pdb, int key, char *value )
{
	int ecode;

	if ( !tchdbput2 (( TCHDB * ) pdb, itochar ( key ), value ))
	{
		ecode = tchdbecode (( TCHDB * ) pdb );
		fprintf ( stderr, "put error:%s\n", tchdberrmsg ( ecode ));
		return -1;
	}

	return 0;
}

/**
 * Get value by key
 */
char * getKV ( void *pdb, int key )
{
	char* value;
	int ecode;
	value = tchdbget2 (( TCHDB *) pdb, itochar ( key ));
	if ( !value )
	{
		ecode = tchdbecode (( TCHDB *) pdb );
		fprintf ( stderr, "get error:%s\n", tchdberrmsg ( ecode ));
		return NULL;
	}

	return value;
}

/**
 * Close Database
 */
int closeDB ( void *pdb )
{
	int ecode;
	if ( !tchdbclose (( TCHDB * ) pdb ))
	{
		ecode = tchdbecode (( TCHDB * ) pdb );
		fprintf ( stderr, "close err: %s\n", tchdberrmsg ( ecode ));
		return -1;
	}
	return 0;
}

/**
 * delete Databse
 */
void deleteDB ( void *pdb )
{
	tchdbdel (( TCHDB * ) pdb );
}
