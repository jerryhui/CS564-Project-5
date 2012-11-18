#include "catalog.h"


RelCatalog::RelCatalog(Status &status) :
	 HeapFile(RELCATNAME, status)
{
// nothing should be needed here
}

/***
 * 2012/11/14 JH: minor syntax debug; need to return status
 ***/
const Status RelCatalog::getInfo(const string & relation, RelDesc &record)
{
  if (relation.empty())
    return BADCATPARM;

  Status status = OK;
  Record rec;
  RID rid;
    
  /*  Open a scan on the relcat relation by invoking the startScan() method on itself. You want to look for the tuple whose first attribute matches the string relName. Then call scanNext() and getRecord() to get the desired tuple. Finally, you need to memcpy() the tuple out of the buffer pool into the return parameter record.
   */
    
    HeapFileScan* hfs;
    hfs = new HeapFileScan(RELCATNAME, status);
    
    status = hfs->startScan(0, 0, STRING, NULL, EQ);
    if (status != OK) {return status;}
    
    while (status == OK)
    {
        status = hfs->scanNext(rid);
        if (status == OK) {
            status = hfs->getRecord(rec);
            if (status != OK)
                break;
            
            if (relation.compare(((RelDesc*)rec.data)->relName) == 0)
            {
                memcpy(&record, rec.data, rec.length);
                delete hfs;
                return status;
            }
        }
    }
    delete hfs;
    if (status != OK && status != FILEEOF) return status;
    return RELNOTFOUND;
}

/***
 * 2012/11/14 JH: minor syntax debug; need to return status
 ***/
const Status RelCatalog::addInfo(RelDesc & record)
{
  RID rid;
  InsertFileScan*  ifs;
  Status status;
    

  /*  Adds the relation descriptor contained in record to the relcat relation. RelDesc represents both the in-memory format and on-disk format of a tuple in relcat. First, create an InsertFileScan object on the relation catalog table. Next, create a record and then insert it into the relation catalog table using the method insertRecord of InsertFileScan.
   */
    
    Record rec;
    rec.data = &record;
    rec.length = sizeof(RelDesc);
    
    ifs = new InsertFileScan(RELCATNAME, status);
    
    status = ifs->insertRecord(rec, rid);
    delete ifs;
    return status;
}

/***
 * 2012/11/14 JH: minor syntax debug; need to return status
 ***/
const Status RelCatalog::removeInfo(const string & relation)
{
  Status status;
  RID rid;
  HeapFileScan*  hfs;

  if (relation.empty()) return BADCATPARM;

  /*  Remove the tuple corresponding to relName from relcat. Once again, you have to start a filter scan on relcat to locate the rid of the desired tuple. Then you can call deleteRecord() to remove it.
   */
    
    Record rec;
    
    hfs = new HeapFileScan(RELCATNAME, status);
    
    status = hfs->startScan(0, 0, STRING, NULL, EQ);
    if (status != OK) {return status;}
    
    while (status == OK)
    {
        status = hfs->scanNext(rid);
        if (status == OK) {
            status = hfs->getRecord(rec);
            if (status != OK)
                break;
            
            if (relation.compare(((RelDesc*)rec.data)->relName) == 0)
            {
                hfs->deleteRecord();
                delete hfs;
                return status;
            }
        }
    }
    delete hfs;
    if (status != OK && status != FILEEOF) return status;
    return RELNOTFOUND;
}


RelCatalog::~RelCatalog()
{
// nothing should be needed here
}


AttrCatalog::AttrCatalog(Status &status) :
	 HeapFile(ATTRCATNAME, status)
{
// nothing should be needed here
}


/***
 * Returns the attribute descriptor record for attribute attrName in relation relName. Uses 
 * a scan over the underlying heapfile to get all tuples for relation and check each tuple 
 * to find whether it corresponds to attrName. (Or maybe do it the other way around !) This has 
 * to be done this way because a predicated HeapFileScan does not allow conjuncted predicates. 
 * Note that the tuples in attrcat are of type AttrDesc (structure given above).
 *
 * Return: OK if successful; otherwise, error status as they were received
 *
 * 11/12/2012 JH: First implementation.
 ***/
const Status AttrCatalog::getInfo(const string & relation,
				  const string & attrName,
				  AttrDesc &record)
{

    Status status;
    RID rid;
    Record rec;
    HeapFileScan*  hfs;
    
    AttrDesc* iDesc;
    bool found=false;

    if (relation.empty() || attrName.empty()) return BADCATPARM;
    
    // open AttrCatalog file
    hfs = new HeapFileScan(ATTRCATNAME, status);
    if (status==OK) {
        if ((status = hfs->startScan(0, 0, STRING, "", EQ))==OK) {

            // scan for requested relation
            while (!found) {
                status = hfs->scanNext(rid);
                if (status!=OK) break;  // this includes FILEEOF
                
                if ( (status = hfs->HeapFile::getRecord(rid, rec))==OK ) {
                    iDesc = (AttrDesc*)rec.data;
                    
                    // if found, return relation                    
                    if ( attrName.compare(iDesc->attrName)==0
                        && relation.compare(iDesc->relName)==0 ) {
                        record = *iDesc;
                        found = true;
                    }
                }
            }
            
        }
    }
    
    // clear memory
    delete hfs;
    return status;
}

/***
 * Adds a tuple (corresponding to an attribute of a relation) to the attrcat relation.
 *
 * 2012/11/12 JH: First implementation.
 ***/
const Status AttrCatalog::addInfo(AttrDesc & record)
{
    RID rid;
    InsertFileScan*  ifs;
    Status status;
    Record newRec;
    
    ifs = new InsertFileScan(ATTRCATNAME, status);
    if (status!=OK) return status;
    
    newRec.data = &record;
    newRec.length = sizeof(record);
    status = ifs->insertRecord(newRec, rid);
    
    delete ifs;     // clear memory
    return status;
}

/***
 * Removes the tuple from attrcat that corresponds to attribute attrName of relation.
 *
 * 2012/11/12 JH: First implementation.
 * 2012/11/14 JH: Deleted unused variable.
 ***/
const Status AttrCatalog::removeInfo(const string & relation, 
			       const string & attrName)
{
    Status status;
    Record rec;
    RID rid;
    HeapFileScan*  hfs;

    if (relation.empty() || attrName.empty()) return BADCATPARM;

    bool found=false;
        
    hfs = new HeapFileScan(ATTRCATNAME, status);
    if (status==OK) {
        if ((status = hfs->startScan(0, 0, STRING, "", EQ))==OK) {
            while (!found && status!=FILEEOF) {
                status = hfs->scanNext(rid);
                if (status!=OK) break;
             
                if ( (status=hfs->HeapFile::getRecord(rid, rec))==OK ) {
                    if ( relation.compare(((AttrDesc*)rec.data)->relName)==0
                        && attrName.compare(((AttrDesc*)rec.data)->attrName)==0 ) {
                        //remove this record
                        status = hfs->deleteRecord();
                        found = true;
                    }
                }
            }
        }
    }
    
    delete hfs;
    return status;
}

/***
 * While getInfo() above returns the description of a single attribute, this method 
 * returns (by reference) descriptors for all attributes of the relation via attr, 
 * an array of AttrDesc structures,  and the count of the number of attributes in attrCnt. 
 * The attrs array is allocated by this function, but it should be deallocated by the caller.
 *
 * Return: OK if successful; otherwise, error status as they were received
 *
 * Assumption: info about the given relation is already in RelCat
 *
 * 2012/11/12 JH: First implementation.
 * 2012/11/14 JH: Change method of allocating attrs (use array).
 ***/
const Status AttrCatalog::getRelInfo(const string & relation, 
				     int &attrCnt,
				     AttrDesc *&attrs)
{
    Status status;
    RID rid;
    Record rec;
    HeapFileScan*  hfs;

    if (relation.empty()) return BADCATPARM;

    AttrDesc* iDesc;
    RelDesc relDesc;
    int iAttrD;
    
    // look up number of attributes from relation catalog
    status = relCat->getInfo(relation, relDesc);
    if (status!=OK) return status;
    attrCnt = relDesc.attrCnt;
    
    // open AttrCatalog file
    hfs = new HeapFileScan(ATTRCATNAME, status);
    if (status==OK) {
        if ((status = hfs->startScan(0, 0, STRING, "", EQ))==OK) {
            // allocate memory for return values
            attrs = new AttrDesc[attrCnt];
            iAttrD=0;
            
            // scan for requested relation
            while (iAttrD<attrCnt) {
                status = hfs->scanNext(rid);
                if (status!=OK) break;  // this includes FILEEOF
                
                if ( (status = hfs->HeapFile::getRecord(rid, rec))==OK ) {
                    iDesc = (AttrDesc*)rec.data;
                    
                    // if found, return relation
                    if ( relation.compare(iDesc->relName)==0 ) {
                        attrs[iAttrD] = *iDesc;
                        iAttrD++;
                    }
                }
            }
            
        }
    }
    
    // clear memory
    delete hfs;
    return status;
}


AttrCatalog::~AttrCatalog()
{
// nothing should be needed here
}

