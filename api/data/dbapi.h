/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.data.dbapi
 * Comments:             This file include the defination of database
 *						 interfaces
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

#ifndef DBAPI_H_INCLUDED
#define DBAPI_H_INCLUDED

/**
 * Create new Database instance
 * input	: None
 * output	: instance reference
 * in/out	: None
 * return	: void *
 */
void * createDB ();

/**
 * open Database according the filename
 * input	: *filename
 * output	: None
 * in/out	: *pdb
 * return	: state
 */
int openDB ( void *pdb, char *filename );

/**
 * initalize Database
 * input	: None
 * output	: Node
 * in/out	: *pdb
 * return	: None
 */
void initDB ( void *pdb );

/**
 * Insert key-value pair into Database
 * input	: key, *value
 * output	: None
 * in/out	: *pdb
 * return	: state
 */
int insertKV ( void *pdb, int key, char* value );

/**
 * Get value by key
 * input	: key, *pdb
 * output	: None
 * in/out	: Node
 * return	: value
 */
char* getKV ( void *pdb, int key );

/**
 * Close Database
 * input	: None
 * output	: Node
 * in/out	: *pdb
 * return	: state
 */
int closeDB ( void *pdb );

/**
 * delete Databse
 * input	: Node
 * output	: None
 * in/out	: *pdb
 * return	: None
 */
void deleteDB ( void *pdb );

#endif
