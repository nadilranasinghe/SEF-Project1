#ifndef PARTS_H
#define PARTS_H
#define MAXIMUMLENGTH 51
#define MAXPARTSIZE 100
#define MAXLINE 100
#define LOCATELINE 10
typedef struct {
	char PartName[MAXIMUMLENGTH];
	char PartNumber[MAXIMUMLENGTH];
	char PartLocate[MAXIMUMLENGTH];
	float PartCost;
	int QuantityOnHand;
	int PartStatus;
	int PartID;
}Parts;
//list all the part information
void ListallParts(Parts* part, int *size);
// Get part by ID
void SearchforPart(Parts* part, int *size);
// Add a new part
int AddPart(Parts* part, int *size);

void UpdateInventoryforPart(Parts part[], int *size);

void SaveToFile(const char* filename, Parts part[], int *size);

void loadfromfile(const char* filename, Parts part[], int* size);
#endif

