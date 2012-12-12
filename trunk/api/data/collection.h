/***********************************************************************
 * CopyRright:           NoCommit Team
 * Project:              NoCommit
 * Module:               api.data.collection
 * Comments:             This file include the definition of collection
 *                       struct and the interfaces using in collection
 * Author:               capasky
 * Create Date:          2012.12.01
 * Version:              1.0
 * Plateform:            Any
 **********************************************************************/

/**
 * Revision Log:
 * @author				@date				@version
 * capasky				2012.12.01			1.0.0.1
 */
 
#ifndef COLLECTION_H_INCLUDED
#define COLLECTION_H_INCLUDED

#define DB_OK				0
#define DB_ERR				-1
#define DB_CANCEL			1

#define DB_STATE_OPEN	 	1
#define DB_STATE_CLOSE	 	0

typedef void (*Function) ( char *, char * );

/**
 * Definition of Collection struct
 * @author capasky
 * @version 1.0.1
 */
typedef struct sCollection
{
    void *  db;
    char *  dbFile;
    int     errorCode;
    int     dbState;
    int     itemCount;
} Collection;

/**
 * CollectionCreate create a collection
 * @param dbFile refer to the file name of hdb
 * @return the pointer of created collection if success, or NULL if fail
 */
Collection * Collection_Create(char * dbFile);

/**
 * DisposeCollection dispose the collection, clear memory and files
 * @param container the Collection struct
 * @return 1 if success; or 0 if fail
 */
int Collection_Dispose(Collection * container);

/**
 * Collection_Clear clear all items in collection
 * @param container the Collection struct
 * @return 1 if success; or 0 if fail
 */
int Collection_Clear(Collection * container);

/**
 * Collection_AddStr add a StringKeyValuePair into collection
 * @param container the Collection struct
 * @param key the key
 * @param value the value
 * @return 1 if success; or 0 if fail
 */
int Collection_AddStr(Collection * container, char * key, char * value);

/**
 * Collection_GetStr remove a StringKeyValuePair from collection
 * @param container the Collection struct
 * @param key
 * @return 1 if success; or 0 if fail
 */
char * Collection_GetStr(Collection * container, char * key);

/**
 * Collection_RemoveStr remove a StringKeyValuePair from collection
 * @param container the Collection struct
 * @param key
 * @return 1 if success; or 0 if fail
 */
int Collection_RemoveStr(Collection * container, char * key);

/**
 * Collection_ContainsStr test if the collection contains a key
 * @param container the Collection struct
 * @param key searching
 * @return 1 if success; or 0 if fail
 */
int Collection_ContainsStr(Collection * container, char * key);

/**
 * Collection_AddInt add a StringKeyValuePair into collection
 * @param container the Collection struct
 * @param key the key
 * @param value the value
 * @return 1 if success; or 0 if fail
 */
int Collection_AddInt(Collection * container, int key, char * value);

/**
 * Collection_GetInt remove a StringKeyValuePair from collection
 * @param container the Collection struct
 * @param key
 * @return 1 if success; or 0 if fail
 */
char * Collection_GetInt(Collection * container, int key);

/**
 * Collection_RemoveInt remove a StringKeyValuePair from collection
 * @param container the Collection struct
 * @param key
 * @return 1 if success; or 0 if fail
 */
int Collection_RemoveInt(Collection * container, int key);

/**
 * Collection_ContainsInt test if the collection contains a key
 * @param container the Collection struct
 * @param key searching
 * @return 1 if success; or 0 if fail
 */
int Collection_ContainsInt(Collection * container, int key);

/**
 * Collection_Iterator iterate all items in collection with a function
 * @param container Collection struct
 * @param function a function pointer to process each item in the
 * 			collection
 * @return void
 */
void Collection_Iterator(Collection * container, Function function);

#endif
