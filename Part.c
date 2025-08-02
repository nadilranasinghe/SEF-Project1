#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Part.h" 
#include "Log.h"

// FUNCTION : ListallParts
// DESCRIPTION :
// This function for list all the PartElement information
// PARAMETERS :
// Parts* part : the array stored the information
// int *size : the number of part
// RETURNS :none
void ListallParts(Parts* part, int *size) {
	for (int count = 0; count < *size; count++) {
		printf("-------------------------------------\n");
		printf("PartName:%s\n", part[count].PartName);
		printf("PartNumber:%s\n", part[count].PartNumber);
		printf("PartLocate:%s\n", part[count].PartLocate);
		printf("PartCost:%.2f\n", part[count].PartCost);
		printf("QuantityOnHand:%d\n", part[count].QuantityOnHand);
		printf("PartStatus:%d\n", part[count].PartStatus);
		printf("PartID:%d\n", part[count].PartID);
		printf("-------------------------------------\n");
	}
}

// FUNCTION : SearchforPart
// DESCRIPTION :
// This function for searching specific part by id
// PARAMETERS :
// Parts* part : the array stored the information
// int *size : the number of part
// RETURNS :none
void SearchforPart(Parts* part, int *size) {
	int id;
	printf("Enter the ID to search:");
	if (scanf_s("%d", &id) != 1) {
		printf("Invalid input\n");
		//all the error message you need to change najaf, for log message i think just take the time to figure out
		logMessage(LOG_ERROR,"Invalid input");
		return;
	}
	for (int count = 0; count < *size; count++) {
		//match by part id
		if (part[count].PartID == id) {
			printf("-------------------------------------\n");
			printf("PartName:%s\n", part[count].PartName);
			printf("PartNumber:%s\n", part[count].PartNumber);
			printf("PartLocate:%s\n", part[count].PartLocate);
			printf("PartCost:%.2f\n", part[count].PartCost);
			printf("QuantityOnHand:%d\n", part[count].QuantityOnHand);
			printf("PartStatus:%d\n", part[count].PartStatus);
			printf("PartID:%d\n", part[count].PartID);
			printf("-------------------------------------\n");
		}
	}

}
// FUNCTION : AddPart
// DESCRIPTION :
// This function for adding the part information
// PARAMETERS :
// Parts* part : the array stored the information
// int *size : the number of part
// RETURNS :none
int AddPart(Parts* parts, int *size) {
	if (*size >= MAXPARTSIZE) {
		printf("Inventory full, cannot add more parts\n");
		logMessage(LOG_ERROR, "Inventory full, cannot add more parts");
		return *size;
	}

	char name[MAXIMUMLENGTH];
	char number[MAXIMUMLENGTH];
	char locate[MAXIMUMLENGTH];
	float cost;
	int quantity;
	int status;
	int id;
	char aisle[LOCATELINE];
	char shelf[LOCATELINE];
	char level[LOCATELINE];
	char bin[LOCATELINE];

	printf("Enter PartName: ");
	fgets(name, MAXIMUMLENGTH, stdin);
	//remove the newline character
	name[strcspn(name, "\n")] = '\0';
	if (strlen(name) == 0) {
		printf("Name cannot be empty\n");
		logMessage(LOG_ERROR, "Name cannot be empty");
		return *size;
	}

	printf("Enter PartNumber: ");
	fgets(number, MAXIMUMLENGTH, stdin);
	number[strcspn(number, "\n")] = '\0';
	if (strlen(number)==0) {
		printf("Number cannot be empty\n");
		logMessage(LOG_ERROR, "Number cannot be empty");
		return *size;
	}

	printf("Enter PartLocation: ");
	fgets(locate, MAXIMUMLENGTH, stdin);
	locate[strcspn(locate, "\n")] = '\0';
	if (strlen(locate) == 0) {
		printf("Locate cannot be empty\n");
		logMessage(LOG_ERROR, "Locate cannot be empty");
		return *size;
	}
	//for location validation
	int checked = sscanf_s(locate, "%9[^-]-%9[^-]-%9[^-]-%9s",
		aisle, LOCATELINE,
		shelf, LOCATELINE,
		level, LOCATELINE,
		bin, LOCATELINE);
	//error message 
	if (checked != 4) {
		printf("Invalid PartLocation format, Expected format: A12-B03-L2-BN07\n");
		logMessage(LOG_ERROR, "Invalid PartLocation format");
		return 1;
	}
	//get part cost 
	printf("Enter PartCost: ");
	if (scanf_s("%f", &cost) != 1 || cost <= 0.0f) {
		printf("Invalid cost input\n");
		// clear input buffer
		while (getchar() != '\n'); 
		logMessage(LOG_ERROR, "Invalid cost input");
		return *size;
	}

	printf("Enter PartQuantity: ");
	if (scanf_s("%d", &quantity) != 1 || quantity < 0) {
		printf("Invalid quantity input\n");
		while (getchar() != '\n');
		logMessage(LOG_ERROR, "Invalid quantity input");
		return *size;
	}
	//base on quantity set status
	if (quantity > 100) {
		status = 0;
	}
	else {
		status = 99;
	}

	printf("Enter PartID: ");
	if (scanf_s("%d", &id) != 1 || id <= 0) {
		printf("Invalid ID input\n");
		while (getchar() != '\n');
		logMessage(LOG_ERROR, "Invalid ID input");
		return *size;
	}
	// consume leftover newline
	while (getchar() != '\n');

	strcpy_s(parts[*size].PartName, MAXIMUMLENGTH, name);
	strcpy_s(parts[*size].PartNumber, MAXIMUMLENGTH, number);
	strcpy_s(parts[*size].PartLocate, MAXIMUMLENGTH, locate);
	parts[*size].PartCost = cost;
	parts[*size].QuantityOnHand = quantity;
	parts[*size].PartStatus = status;
	parts[*size].PartID = id;
	(*size)++;
	return *size;
}

// FUNCTION : UpdateInventoryforPart
// DESCRIPTION :
// This function for get the customer order for part and update the inventory
// PARAMETERS :
// Parts* part : the array stored the information
// int *size : the number of part
// RETURNS :none
void UpdateInventoryforPart(Parts part[],int *size) {
	int id;
	int partdemand;
	printf("Enter the ID to modify:");
	if (scanf_s("%d", &id) != 1) {
		printf("invalid input\n");
		logMessage(LOG_ERROR, "Invalid input");
		return;
	}
	for (int count = 0; count < *size; count++) {
		if (part[count].PartID == id) {
			printf("Current Quantity:%d\n",part[count].QuantityOnHand);
			printf("Enter the quantity the customer need (Enter the number for subtract): ");
			if (scanf_s("%d", &partdemand) != 1 || partdemand < 0) {
				printf("Invalid quantity input\n");
				logMessage(LOG_ERROR, "Invalid quantity input");
				return;
			}
			//inventory greater than partdemand, is not in deficit
			if (part[count].QuantityOnHand >= partdemand) {
				part[count].QuantityOnHand = part[count].QuantityOnHand - partdemand;
			}
			//inventory less than partdemand, is in deficit
			else {
				int deficit = part[count].QuantityOnHand - partdemand;

				if (part[count].PartStatus < 0) {
					part[count].PartStatus = part[count].PartStatus + deficit;
				}
				else {
					part[count].PartStatus = deficit;
				}
				part[count].QuantityOnHand = 0;
				printf("Order could not be fulfilled. Deficit updated\n");
				return;
			}
			//check reset status
			if (part[count].QuantityOnHand > 100) {
				part[count].PartStatus = 0;
			}
			else {
				part[count].PartStatus = 99;
			}
			printf("Inventory updated\n");
			return;
		}
	}
	printf("Part with ID %d not found\n", id);
	logMessage(LOG_ERROR, "ID not found");
}
// FUNCTION : SaveToFile
// DESCRIPTION :
// This function for save the information into specific file
// PARAMETERS :
// const char* filename : the file name
// Parts* part[] : the array of part
// int *size : the number of part
// RETURNS :none
void SaveToFile(const char* filename, Parts part[], int *size) {
	FILE* file;
	errno_t err = fopen_s(&file, filename, "w");
	if (err != 0 || file == NULL) {
		printf("Could not open file.\n");
		logMessage(LOG_ERROR, "Could not open file");
		return;
	}

	for (int count = 0; count < *size; count++) {
		//store as specific format
		fprintf(file, "%s|%s|%s|%.2f|%d|%d|%d|\n",part[count].PartName,part[count].PartNumber,part[count].PartLocate,part[count].PartCost,part[count].QuantityOnHand,part[count].PartStatus,part[count].PartID);
	}

	fclose(file);
	printf("Saved % d part(s) to file '%s'\n", *size, filename);
}

// FUNCTION : loadfromfile
// DESCRIPTION :
// This function for load the information from specific file
// PARAMETERS :
// const char* filename : the file name
// Parts* part[] : the array of part
// int *size : the number of part
// RETURNS :none
void loadfromfile(const char* filename, Parts part[], int* size) {
	FILE* file;
	errno_t err = fopen_s(&file, filename, "r");
	if (err != 0 || file == NULL) {
		printf("Could not open file.\n");
		logMessage(LOG_ERROR, "Could not open file");
		return;
	}

	char line[MAXLINE];
	int partcount = 0;
	while (fgets(line, MAXLINE, file) != NULL && partcount < MAXPARTSIZE) {
		//removes the newline character
		line[strcspn(line, "\n")] = '\0';
		//temparary variable stand for each partelement
		char tempPartname[MAXIMUMLENGTH];
		char tempPartNumber[MAXIMUMLENGTH];
		char tempPartLocate[MAXIMUMLENGTH];
		float tempPartCost;
		int tempPartQuantity;
		int tempPartStatus;
		int tempPartId;

		int fields = sscanf_s(line, "%50[^|]|%50[^|]|%50[^|]|%f|%d|%d|%d|", tempPartname, MAXIMUMLENGTH, tempPartNumber, MAXIMUMLENGTH, tempPartLocate, MAXIMUMLENGTH, &tempPartCost, &tempPartQuantity, &tempPartStatus, &tempPartId);
		
		if (fields == 7) {
			//save each element into part
			strcpy_s(part[partcount].PartName, MAXIMUMLENGTH, tempPartname);
			strcpy_s(part[partcount].PartNumber, MAXIMUMLENGTH, tempPartNumber);
			strcpy_s(part[partcount].PartLocate, MAXIMUMLENGTH, tempPartLocate);
			part[partcount].PartCost = tempPartCost;
			part[partcount].QuantityOnHand = tempPartQuantity;
			part[partcount].PartStatus = tempPartStatus;
			part[partcount].PartID = tempPartId;
			partcount++;
		}
		//error message
		else {
			printf("Invalid line format, skipping:\n");
			printf("%s\n", line);
			logMessage(LOG_ERROR, "Load from data, invalid line format");
		}
	}
	fclose(file);
	//update part size
	*size = partcount;
	printf("Loaded %d parts from the file",partcount);
}

int main(void) {
	Parts parts[MAXPARTSIZE];
	int size = 0;
	int option;
	logMessage(LOG_INFO, "Program started");
	while (1) {
		printf("\nMenu:\n1. Add Part\n2. List All Parts\n3. Search for Part\n4. Update Inventory\n5. Load From File\n6. Save To File\n7. Exit\n");
		printf("Choose option: ");
		if (scanf_s("%d", &option) != 1) {
			printf("Invalid input.\n");
			while (getchar() != '\n'); 
			continue;
		}
		getchar(); 

		switch (option) {
		case 1:
			AddPart(parts, &size);
			break;
		case 2:
			ListallParts(parts, &size);
			break;
		case 3:
			SearchforPart(parts, &size);
			break;
		case 4:
			UpdateInventoryforPart(parts, &size);
			break;
		case 5:
			loadfromfile("parts.db.txt", parts, &size);
				printf("Loaded successfully.\n");
			break;
		case 6:
			SaveToFile("parts.db.txt", parts, &size);
			printf("Save successful\n");
			break;
		case 7:
			printf("Exiting...\n");
			logMessage(LOG_ERROR, "Program Exit");
			closeLogger();
			return 0;
		default:
			printf("Invalid option.\n");
		}
	}
	logMessage(LOG_ERROR, "Program stop");
	closeLogger();
	return 0;
}