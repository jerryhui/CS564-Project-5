#include <stdio.h>
#include <string.h>
#include <iostream>

int errno;
using namespace std;

typedef struct {
  int unique1;
  int unique2;
  int hundred1;
  int hundred2;
  char dummy[84];
} Rels;


int main()
{
  FILE *fp;
    int i;
  
  fp = fopen("rel500.data","r");

  Rels rel;
  for (i = 0; i < 500; i++) {    
	if (fread((char*)&rel, sizeof(rel), 1, fp) < 1)
		fprintf(stderr, "Error in reading file\n");
    	cout << rel.unique1 << "\t" << rel.unique2 << "\t"
	     << rel.hundred1 << "\t" << rel.hundred2 << "\t"
	     << rel.dummy << endl;
  }
  fclose(fp);

  fp = fopen("rel1000.data","r");

  for (i = 0; i < 1000; i++) {    
        if (fread((char*)&rel, sizeof(rel), 1, fp) < 1)
                fprintf(stderr, "Error in reading file\n");
        cout << rel.unique1 << "\t" << rel.unique2 << "\t"
             << rel.hundred1 << "\t" << rel.hundred2 << "\t"
             << rel.dummy << endl;

  }        
  fclose(fp);

    return (1);
}
