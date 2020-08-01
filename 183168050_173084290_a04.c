/**
 * @author Kaish Panjwani
 * @author Ravish Virani
 * @file 183168050_173084290_a04.c
 * @version 2020-07-23
*/



// Include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h> //For sleep
#include <pthread.h>

// Define
#define MAXCUSTOMER 5
#define MAXRESOURCE 4
#define MAXIN 200

int needed[MAXCUSTOMER][MAXRESOURCE];
int maxarray[MAXCUSTOMER][MAXRESOURCE];
int allocation[MAXCUSTOMER][MAXRESOURCE]; //customers allocated in a 2d array
void needed_resources(int, int, int aloc[MAXCUSTOMER][MAXRESOURCE], int maxi[MAXCUSTOMER][MAXRESOURCE]);


// Main Driver
int main(int argc, char *argv[])
{
	if(argc<2)
	{
		printf("INPUT ERROR: 'Available' resources Data Missing... Exiting with Error Code -1\n");
		return -1;
	}
	
	//Printing Number of Customers
	printf("Number of Customers: %d\n", MAXCUSTOMER);

	/**
	 * ----------------------------------------------------------------
	 * Storing Available resources Data from Command line Input into 
	 * an Array
	 * ----------------------------------------------------------------
	 * */

	int available_size = MAXRESOURCE;
	int available[available_size];

	for (int i = 0; i < available_size; i++)
		available[i] = atoi(argv[i + 1]);



	/**
	 * ----------------------------------------------------------------
	 * Ask for input from user for Number of Customers
	 * ----------------------------------------------------------------
	 * 

	int customers;
	printf("Number of Customers: ");
	scanf("%d", &customers);
	
	*/



	/**
	 * ----------------------------------------------------------------
	 * Read data from file and insert into Maximum resources array
	 * ----------------------------------------------------------------
	 * */

	int maximum[MAXCUSTOMER][available_size]; // Declare 2D Array

	// Initialize maximum array to zeros
	for(int m = 0; m < MAXCUSTOMER; m++) {
    	for(int n = 0; n < available_size; n++) {
    		maximum[m][n] = 0;
    	}
	}

	// Open File
	FILE *in = fopen("sample4_in.txt", "r");
	
	if(!in)
	{ // Check if the file opened or not
		printf("FILE OPEN ERROR: File Not Opened... Exiting with Error Code -1\n");
		return -1;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char* fileContent = (char*)malloc(((int) st.st_size + 1)* sizeof(char));
	fileContent[0]='\0';	
	
	// Storing data from File into fileContent string
	while(!feof(in))
	{
		char line[100];
		if(fgets(line, 100, in) != NULL)
			strncat(fileContent, line, strlen(line));
	}

	fclose(in);

	char* lines[MAXCUSTOMER];
	char *command = NULL;
	int i = 0;
	command = strtok(fileContent,"\r\n");

	// Storing data from fileContent string to 'lines' array
	while(command!=NULL)
	{
		lines[i] = malloc(sizeof(command)*sizeof(char));
		strcpy(lines[i],command);
		i++;
		command = strtok(NULL,"\r\n");
	}

	// Tokenize each line and add individual numbers into the array
	for(int k = 0; k < MAXCUSTOMER; k++)
	{
		char* token = NULL;
		int l = 0;
		token =  strtok(lines[k],",");

		while(token!=NULL)
		{
			maximum[k][l] = atoi(token);
			l++;
			token = strtok(NULL,",");
		}
	}



	/**
	 * ----------------------------------------------------------------
	 * Print Currently Available resources
	 * ----------------------------------------------------------------
	 * */

	printf("Currently Available resources:");

	for (int i = 0; i < available_size; i++)
		printf(" %d", available[i]);

	printf("\n");



	/**
	 * ----------------------------------------------------------------
	 * Print Maximum resources from file
	 * ----------------------------------------------------------------
	 * */

	printf("Maximum resources from file:\n");
	
		for (int i = 0; i < MAXCUSTOMER; i++) {
    	for (int j = 0; j < available_size; j++) {
			printf("%d", maximum[i][j]);
			
			if (j != available_size - 1)
				printf(",");
    	}

		printf("\n");
	}
	
	//char
	
	char wrd[MAXIN];
    char com[MAXIN];
    char req[MAXIN] = "RQ"; //resource requesting
    char rel[MAXIN] = "RL"; //resource releasing
    char str[MAXIN] = "*\n";
    char run[MAXIN] = "run\n";
    char exit_prog[MAXIN] = "exit\n";
	
	/**
	 * ----------------------------------------------------------------
	 * Asking for initial request to initially run while loop
	 * ----------------------------------------------------------------
	 * */
	 
	 printf("Enter Request: ");
	 fgets(wrd, MAXIN, stdin);
	 strcpy(com, wrd);
	 int work = 0;
	 
	 /**
	 * ----------------------------------------------------------------
	 * Programs main logic when exit has not been initiated
	 * ----------------------------------------------------------------
	 * */
	 
	 while (strcmp(com,exit_prog) != 0)
	 {
		 if (work > 0)
		 {
			printf("Enter Command: ");
            fgets(wrd, sizeof wrd, stdin);
            strcpy(com, wrd);
        }
		
		//wrd splitting
		int total_wrd = 0;
		for (int z = 0; com[z] != '\0'; z++)
		{
			if (com[z] == ' ' || com[z] == '\n' || comm [w] == '\t')
			{
				total_wrd++;
			}
		}
		
		// parser initialization
		char *tkn = strtok(com, " ");
		char *input_str[MAXIN];
		
		int i=0;
		if (total_wrd >= 2)
		{
			while (tkn != NULL $$ i <= MAXCUSTOMER)
			{
				input_str[i] = tkn;
				tkn = strtok(NULL, " ");
				i += 1;
			}
		}
		else {
		strcpy(input_str[0], com);}
		
		int str_len = i;
		i = 0;
		
		// end of parser
		
	 /**
	 * ----------------------------------------------------------------
	 * Resource Request
	 * ----------------------------------------------------------------
	 * */
		
		if (strcmp(input_str[0], req) == 0) {
			if (atoi (input_str[1]) >= MAXCUSTOMER) {
				printf("Allocation array cannot be larger then Max number of customers\n");
				continue;
			}
			for (int y = 2; y < (str_len); y++) {
				allocation[atoi(input_str[1])][y-2] = atoi(input_str[y]);
			}
			printf("Request is satisfied\n");
		}
		
	 /**
	 * ----------------------------------------------------------------
	 * Resource Release
	 * ----------------------------------------------------------------
	 * */		
		
		if (strcmp(input_str[0], rel) == 0)
        {
            int quit_if;
            for (int q = 2; q < (length_string); q++)
            {
                int releasevalue;
                //release value from allocation 2d array
                releasevalue = allocation[atoi(input_str[1])][q - 2] - atoi(input_str[q]);

                //error when releasing
                if (releasevalue < 0) {
                    printf("Release if unsatisfied\n");
                    quit_if = 1;
                    break;
                }
                else {
                    allocation[atoi(input_str[1])][q - 2] = releasevalue;
                }
            }
            if (quit_if == 1) {
                continue;
            }

            printf("Release is satisfied\n");
        }
		
	 /**
	 * ----------------------------------------------------------------
	 * Show all details with * command
	 * ----------------------------------------------------------------
	 * */			
		
		if (strcmp(input_str[0], str) == 0) {
			// Current state
			printf("Showing current state of arrays.\n");
			int l, p;
			printf("Currently Available Resources: ");
			for (i = 1; i < MAXCUSTOMER; l++) {
				printf("%d\n", available[l]);
			}
			
			//Maximum
			printf("Maximum Resources: \n");
			
			for (int i = 0; i < MAXCUSTOMER; i++) {
			for (int j = 0; j < available_size; j++) {
				printf("%d", maximum[i][j]);
			
				if (j != available_size - 1)
					printf(",");
			}

			printf("\n");
			}
		}
		
		//Allocation Resources
		printf("Allocation Resources: \n");
		for (l = 0; l < MAXCUSTOMER; l++) {
			for (p = 0; p < MAXRESOURCE; p++) {
				printf("%d ", allocation[l][p]);
                }
				printf("\n");
        }
        printf("\n");}
		
		//Needed Resources
		printf("Needed Resources: \n");
		needed_resources(MAXRESOURCE, MAXCUSTOMER, allocation, maxarray);
		for (l = 0; l < MAXCUSTOMER; l++) {
			for (p = 0; p < MAXRESOURCE; p++) {
				printf("%d ", needed[l][p]);
			}
			printf("\n");
		}
		printf("\n");
		
}

void needed_resources(int i, int j, int aloc[i][j], int maxi[i][j]) {
	int m, n;
	
	for (m = 0, m < j; m++) {
		for (n = 0; n < i; n++){
			needed[m][n] = maxi[m][n] - aloc[i][j];
		}
	}
}