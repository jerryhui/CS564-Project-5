#include "catalog.h"


RelCatalog::RelCatalog(Status &status) :
	 HeapFile(RELCATNAME, status)
{
// nothing should be needed here
}


const Status RelCatalog::getInfo(const string & relation, RelDesc &record)
{
  if (relation.empty())
    return BADCATPARM;

  Status status;
  Record rec;
  RID rid;




}


const Status RelCatalog::addInfo(RelDesc & record)
{
  RID rid;
  InsertFileScan*  ifs;
  Status status;




}

const Status RelCatalog::removeInfo(const string & relation)
{
  Status status;
  RID rid;
  HeapFileScan*  hfs;

  if (relation.empty()) return BADCATPARM;



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
            while (!found && status != FILEEOF) {
                status = hfs->scanNext(rid);
                if (status!=OK) break;
                
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


const Status AttrCatalog::addInfo(AttrDesc & record)
{
  RID rid;
  InsertFileScan*  ifs;
  Status status;





}


const Status AttrCatalog::removeInfo(const string & relation, 
			       const string & attrName)
{
  Status status;
  Record rec;
  RID rid;
  AttrDesc record;
  HeapFileScan*  hfs;

  if (relation.empty() || attrName.empty()) return BADCATPARM;

}


const Status AttrCatalog::getRelInfo(const string & relation, 
				     int &attrCnt,
				     AttrDesc *&attrs)
{
  Status status;
  RID rid;
  Record rec;
  HeapFileScan*  hfs;

  if (relation.empty()) return BADCATPARM;




}


AttrCatalog::~AttrCatalog()
{
// nothing should be needed here
}

