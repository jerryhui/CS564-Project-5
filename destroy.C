#include "catalog.h"

/***
 * Destroys a relation. It performs the following steps:
 *
 * removes the catalog entry for the relation
 * destroys the heap file containing the tuples in the relation
 *
 * Returns:
 *   OK on success
 *   error code otherwise
 *
 * 2012/11/14 JH: First implementation.
 * 2012/11/18 JH: major debug--must destroy relation tuple last; remove correct file
 ***/

const Status RelCatalog::destroyRel(const string & relation)
{
    Status status;

    if (relation.empty() || 
        relation == string(RELCATNAME) || 
        relation == string(ATTRCATNAME))
    return BADCATPARM;

    status = attrCat->dropRelation(relation);
    if (status!=OK) return status;
    
    status = removeInfo(relation);
    if (status!=OK) return status;
    
    return destroyHeapFile(relation);
}


/***
 *
 * Drops a relation. It performs the following steps:
 *
 * removes the catalog entries for the relation
 * Returns:
 *  OK on success
 *  error code otherwise
 *
 * 2012/11/14 JH: First implementation.
 ***/
const Status AttrCatalog::dropRelation(const string & relation)
{
    Status status;
    AttrDesc *attrs;
    int attrCnt, i;

    if (relation.empty()) return BADCATPARM;

    // Obtain info of all atrributes first
    status = getRelInfo(relation, attrCnt, attrs);
    if (status != OK) return status;
    
    // for each attribute, remove attribute
    for (i=0; i<attrCnt; i++) {
        status = removeInfo(relation, attrs[i].attrName);
    }
    
    return status;
}


