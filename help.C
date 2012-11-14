#include <sys/types.h>
#include <functional>
#include <string.h>
#include <stdio.h>
using namespace std;

#include "error.h"
#include "utility.h"
#include "catalog.h"

// define if debug output wanted


//
// Retrieves and prints information from the catalogs about the for the
// user. If no relation is given (relation is NULL), then it lists all
// the relations in the database, along with the width in bytes of the
// relation, the number of attributes in the relation, and the number of
// attributes that are indexed.  If a relation is given, then it lists
// all of the attributes of the relation, as well as its type, length,
// and offset, whether it's indexed or not, and its index number.
//
// Returns:
// 	OK on success
// 	error code otherwise
//

const Status RelCatalog::help(const string & relation)
{
  Status status;
  RelDesc rd;
  AttrDesc *attrs;
  int attrCnt;

  if (relation.empty()) return UT_Print(RELCATNAME);

    else
    {
        if (relation.empty()) relation = RELCATNAME;
        
        // get relation data
        if ((status = relCat->getInfo(relation, rd)) != OK) return status;

        // get attribute data
        if ((status = attrCat->getRelInfo(rd.relName, attrCnt, attrs)) != OK)
            return status;

        // compute width of output columns

        int *attrWidth;
        if ((status = UT_computeWidth(attrCnt, attrs, attrWidth)) != OK)
            return status;

        //DM:  Print the relation name
        cout << "Relation name: " << rd.relName << endl << endl;

        //DM:  Print out a list of the attributes
        int i;
        for(i = 0; i < attrCnt; i++) {
            printf("%-*.*s ", attrWidth[i], attrWidth[i],
                   attrs[i].attrName);
        }
        printf("\n");

        //DM:  Print out a line of "-" characters
        for(i = 0; i < attrCnt; i++) {
            for(int j = 0; j < attrWidth[i]; j++)
                putchar('-');
            printf("  ");
        }
        printf("\n");
        
        
        
        
    }

  return OK;
}
