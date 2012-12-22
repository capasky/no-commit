/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.data.collection
 * Comments:             This file include the implementation of
 * 						 collection struct and the interfaces using
 * 						 in collection
 * Author:               capasky
 * Create Date:          2012.12.07
 * Version:              1.0
 * Plateform:            Linux
 ***********************************************************************/

/**
 * Revision Log:
 * @author				@date				@version
 * capasky				2012.12.07			1.0.0.1
 * yellhb				2012.12.16			1.0.1.0
 * yellhb				2012.12.22			1.0.2.1
 */
 
#include <tcutil.h>
#include <tchdb.h>
#include <string.h>
#include <stdbool.h>

#include "collection.h"
#include "dbapi.h"
#include "../utils/stringutils.h"
#include "../utils/convert.h"

/**
 * CollectionCreate create a collection
 */
Collection * Collection_Create ( char * dbFile )
{
	Collection * container = (Collection *)
								malloc ( sizeof ( struct sCollection ));
    container->itemCount = 0;
    container->dbFile = strdup(dbFile);
	container->db = createDB();

	/* 开启TCHDB的读写锁  */
	tchdbsetmutex (( TCHDB* ) container->db );

	container->errorCode = openDB ( container->db, container->dbFile );
	if ( container->errorCode == DB_OK )
	{
		container->dbState = DB_STATE_OPEN;
	}
	else
	{
		container->dbState = DB_STATE_CLOSE;
		deleteDB ( container->db );
		container = NULL;
	}
    return container;
}

/**
 * DisposeCollection dispose the collection, clear memory and files
 */
int Collection_Dispose ( Collection * container )
{
	if ( container->dbState == DB_STATE_OPEN )
	{
		container->errorCode = closeDB ( container->db );
	}
	deleteDB ( container->db );
	free ( container->dbFile );
	free ( container );
	return DB_OK;
}

/**
 * Collection_Clear clear all items in collection
 */
int Collection_Clear ( Collection * container )
{
    char *key;
    initDB ( container->db );

    while (( key = tchdbiternext2 ( container->db )) != NULL )
    {
        if ( !tchdbout2 ( container->db, key ))
            return DB_ERR;
    }

    return DB_OK;
}

/**
 * Collection_AddStr add a StringKeyValuePair into collection
 */
int Collection_AddStr ( Collection * container, char * key, char * value )
{
    int ecode;

	if ( !tchdbput2 (( TCHDB * ) container->db, key, value ))
	{
		ecode = tchdbecode (( TCHDB * ) container->db );
		fprintf ( stderr, "put error:%s\n", tchdberrmsg ( ecode ));
		return DB_ERR;
	}

	return DB_OK;
}

/**
 * Collection_GetStr remove a StringKeyValuePair from collection
 */
char * Collection_GetStr ( Collection * container, char * key )
{
    char* value;
	int ecode;
	value = tchdbget2 (( TCHDB *) container->db, key );
	if ( !value )
	{
		ecode = tchdbecode (( TCHDB *) container->db );
		fprintf ( stderr, "get error:%s\n", tchdberrmsg ( ecode ));
		return NULL;
	}

	return value;
}

/**
 * Collection_RemoveStr remove a StringKeyValuePair from collection
 */
int Collection_RemoveStr ( Collection * container, char * key )
{
    if ( !tchdbout2 ( container->db, key ))
        return DB_ERR;

    return DB_OK;
}

/**
 * Collection_ContainsStr test if the collection contains a key
 */
int Collection_ContainsStr ( Collection * container, char * key )
{
    if ( Collection_GetStr ( container, key ) != NULL )
        return DB_OK;

    return DB_ERR;
}

/**
 * Collection_AddInt add a StringKeyValuePair into collection
 */
int Collection_AddInt ( Collection * container, int key, char * value )
{
    int ecode;

	if ( !tchdbput2 (( TCHDB * ) container->db, Convert_IntToString ( key ) , value ))
	{
		ecode = tchdbecode (( TCHDB * ) container->db );
		fprintf ( stderr, "put error:%s\n", tchdberrmsg ( ecode ));
		return DB_ERR;
	}

	return DB_OK;
}

/**
 * Collection_GetInt remove a StringKeyValuePair from collection
 */
char * Collection_GetInt ( Collection * container, int key )
{
    char*   value;
	int     ecode;
	value = tchdbget2 (( TCHDB *) container->db, Convert_IntToString ( key ));
	if ( !value )
	{
		ecode = tchdbecode (( TCHDB *) container->db );
		fprintf ( stderr, "get error:%s\n", tchdberrmsg ( ecode ));
		return NULL;
	}

	return value;
}

/**
 * Collection_RemoveInt remove a StringKeyValuePair from collection
 */
int Collection_RemoveInt ( Collection * container, int key )
{
    if ( !tchdbout2 ( container->db, Convert_IntToString ( key )))
        return DB_ERR;

    return DB_OK;
}

/**
 * Collection_ContainsInt test if the collection contains a key
 */
int Collection_ContainsInt ( Collection * container, int key )
{
    if ( Collection_GetStr ( container, Convert_IntToString ( key )) != NULL )
        return DB_OK;

    return DB_ERR;
}

/**
 * Collection_Iterator iterate all items in collection with a function
 */
void Collection_Iterator(Collection * container, Function function)
{
    char *key;
    char *value;
    tchdbiterinit (( TCHDB * ) container->db );

    while((key = tchdbiternext2(( TCHDB * ) container->db )) != NULL )
    {
        value = tchdbget2 (( TCHDB * ) container->db, key );
        if ( value )
        {
            function ( key, value );
        }
    }
}

