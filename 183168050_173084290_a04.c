/**
 * @author Kaish Panjwani
 * @author Ravish Virani
 * @file 183168050_173084290_a04.c
 * @version 2020-08-01
 **/


/**
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * 						Link to GitHub Repository:
 * 				   https://github.com/kaishp/CP386_A04
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 **/



// Include
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>



// Structure
typedef struct customer {
	int customers;
	int record_size;
} Customer;



// Variables
int *available;
int **allocated;
int **needed;

Customer *cust;

#define MAXIN 200

sem_t sem_even; // Initializing Even Semaphore
sem_t sem_odd;// Initializing Odd Semaphore



// Declaration
int getCustCount(char *filename);
void calcNeeded(int i, int j, int **allocated, int maximum[i][j], int **needed);
void reCalcCurrent();
int bankerAlgorithm(int m, int n, int **allocated, int maximum[n][m], int available[m], int **needed, int arr[n]);
void *threadRun(void *p);



/**
 * ================================================================
 * main - Main Driver for the program
 * ================================================================
 **/
int main(int argc, char *argv[])
{
	if(argc<2)
	{
		printf("INPUT ERROR: 'Available' resources Data Missing... Exiting with Error Code -1\n");
		return -1;
	}



	// Intializing Semaphores
	sem_init(&sem_even, 0, 0);
	sem_init(&sem_odd, 0 ,0);



	/**
	 * ----------------------------------------------------------------
	 * Calculating Available Size
	 * ----------------------------------------------------------------
	 **/

	int available_size = argc - 1;
	
	

	/**
	 * ----------------------------------------------------------------
	 * Find Number of Customers for given file and print
	 * ----------------------------------------------------------------
	 **/

	int customers = getCustCount("sample4_in.txt") + 1;
	printf("Number of Customers: %d\n", customers);


	
	/**
	 * ----------------------------------------------------------------
	 * Initializing Arrays
	 * ----------------------------------------------------------------
	 **/

	cust = (Customer*)malloc(sizeof(Customer));
	cust->record_size = available_size;
	cust->customers = customers;


	available = (int*)malloc(available_size * sizeof(int)); // Dynamically Allocating array for Available
	if (available == NULL) { 
        printf("ERROR: Malloc Failure\n"); 
        return -1;
    }

	// Intializing available array to argv values
	for (int i = 0; i < available_size; i++)
		available[i] = atoi(argv[i + 1]);


	// Dynamically Allocating 2D array for Allocated
	allocated = (int **)malloc(customers * sizeof(int *));
	
	for (int i = 0; i < customers; i++)
		allocated[i] = (int *)malloc(available_size * sizeof(int));

	
	// Initialize allocated array to zeros
	for(int m = 0; m < customers; m++) {
    	for(int n = 0; n < available_size; n++)
    		allocated[m][n] = 0;
	}
	

	// Dynamically Allocating 2D array for Needed
	needed = (int **)malloc(customers * sizeof(int *));
	
	for (int i = 0; i < customers; i++)
		needed[i] = (int *)malloc(available_size * sizeof(int));
	
	
	int arr[customers]; // Temporary array for storing data



	/**
	 * ----------------------------------------------------------------
	 * Read data from file and insert into Maximum resources array
	 * ----------------------------------------------------------------
	 **/

	int maximum[customers][available_size]; // Declare 2D Array

	// Initialize maximum array to zeros
	for(int m = 0; m < customers; m++) {
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

	char* lines[customers];
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
	for(int k = 0; k < customers; k++)
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
	 **/

	printf("Currently Available resources:");

	for (int i = 0; i < available_size; i++)
		printf(" %d", available[i]);

	printf("\n");



	/**
	 * ----------------------------------------------------------------
	 * Print Maximum resources from file
	 * ----------------------------------------------------------------
	 **/

	printf("Maximum resources from file:\n");

	for (int i = 0; i < customers; i++) {
    	for (int j = 0; j < available_size; j++) {
			printf("%d", maximum[i][j]);
			
			if (j != available_size - 1)
				printf(",");
    	}

		printf("\n");
	}



	/**
	 * ----------------------------------------------------------------
	 * Initializing Command Strings (char arrays)
	 * ----------------------------------------------------------------
	 **/

	char string[MAXIN];
    char com[MAXIN];
    char req[MAXIN] = "RQ"; //Resource Requesting
    char rel[MAXIN] = "RL"; //Resource Releasing
    char str[MAXIN] = "*\n"; // New Line
    char run[MAXIN] = "run\n"; // Run Safe Sequence
    char exit_prog[MAXIN] = "exit\n"; // Exit Program
	


	/**
	 * ----------------------------------------------------------------
	 * Asking for initial request to initially run while loop
	 * ----------------------------------------------------------------
	 **/

	printf("Enter Command: ");
	fgets(string, MAXIN, stdin);
	strcpy(com, string);
	int work = 0;



	/**
	 * ----------------------------------------------------------------
	 * Programs main logic when exit has not been initiated
	 * ----------------------------------------------------------------
	 **/

	while (strcmp(com,exit_prog) != 0) {
		 if (work > 0)
		 {
			printf("Enter Command: ");
            fgets(string, sizeof string, stdin);
            strcpy(com, string);
        }
		
		// String splitting
		int total_string = 0;
		for (int z = 0; com[z] != '\0'; z++)
		{
			if (com[z] == ' ' || com[z] == '\n' || com [z] == '\t')
				total_string++;
		}
		
		// parser initialization
		char *tkn = strtok(com, " ");
		char *input_str[MAXIN];
		
		int i=0;
		if (total_string >= 2)
		{
			while (tkn != NULL && i <= customers)
			{
				input_str[i] = tkn;
				tkn = strtok(NULL, " ");
				i += 1;
			}
		}
		
		else
			strcpy(input_str[0], com);
		
		int str_len = i;
		i = 0;
		
		// end of parser
		
	 /**
	 * ----------------------------------------------------------------
	 * COMMAND: RQ - Resource Request
	 * ----------------------------------------------------------------
	 * */
		
		if (strcmp(input_str[0], req) == 0) {
			if (atoi (input_str[1]) >= customers) { // Idiot Proofing the RQ command
				printf("Allocated index cannot be larger then Max Number of Customers\n");
				// continue;
			}

			else {
				for (int y = 2; y < (str_len); y++) // Store requested resource data
					allocated[atoi(input_str[1])][y-2] = atoi(input_str[y]); // Convert inputted allocated resource information from string to integer

				printf("Request is satisfied\n");
			}
		}
		
	 /**
	 * ----------------------------------------------------------------
	 * COMMAND: RL - Resource Release
	 * ----------------------------------------------------------------
	 * */		
		
		else if (strcmp(input_str[0], rel) == 0)
        {
            int quit_flag; // Used when a Requested Release is unsatisfied

			if (atoi (input_str[1]) >= customers) { // Idiot Proofing the RQ command
				printf("Allocated index cannot be larger then Max Number of Customers\n");
				// continue;
			}

			else {

				for (int q = 2; q < (str_len); q++)
				{
					int release_value;
					release_value = allocated[atoi(input_str[1])][q - 2] - atoi(input_str[q]); //Calculating Release value from Allocated 2D array

					if (release_value < 0) { // Idiot Proofing the RL command, to avoid Allocated Resource value to fall below 0
						printf("Release is unsatisfied\n");
						quit_flag = 1; 
						break;
					}
					
					else
						allocated[atoi(input_str[1])][q - 2] = release_value; // Changing the value of currently pointed release value

					if (q = str_len - 1)
						printf("Release is satisfied\n");
				}

				// Leave the loop and start over from the while loop (Reject value inputted)

				if (quit_flag == 1)
					continue;
			}
        }
		
	 /**
	 * ----------------------------------------------------------------
	 * COMMAND: * - Display details
	 * ----------------------------------------------------------------
	 * */			
		
		else if (strcmp(input_str[0], str) == 0) {
			
			// Print Current state
			printf("Showing current state of arrays:\n\n");
			

			// Print Currently Available Resources
			printf("Currently Available Resources: ");

			reCalcCurrent();


			for (int l = 0; l < available_size; l++)
				printf("%d ", available[l]);


			printf("\n\n");
			

			// Print Maximum Resources
			printf("Maximum Resources: \n");
			
			for (int l = 0; l < customers; l++) {
				for (int p = 0; p < available_size; p++)
					printf("%d ", maximum[l][p]);

				printf("\n");
			}


			printf("\n");


			//  Print Allocated Resources
			printf("Allocated Resources: \n");

			for (int l = 0; l < customers; l++) {
				for (int p = 0; p < available_size; p++)
					printf("%d ", allocated[l][p]);
				
				printf("\n");
        	}


			printf("\n");
		

			// Print Needed Resources
			printf("Needed Resources: \n");
			
			calcNeeded(available_size, customers, allocated, maximum, needed);

			for (int l = 0; l < customers; l++) {
				for (int p = 0; p < available_size; p++)
					printf("%d ", needed[l][p]);

				printf("\n");
			}

			printf("\n");
		}
		
		
		
	/**
	 * ----------------------------------------------------------------
	 *  COMMAND: run - Run threads and find safe sequence
	 * ----------------------------------------------------------------
	 * */
		
		else if (strcmp(input_str[0], run) == 0) {
			printf("Executing run command\n");
			
			calcNeeded(available_size, customers, allocated, maximum, needed);
			int respond = bankerAlgorithm(available_size, customers, allocated, maximum, available, needed, arr);
			
			if (respond != 0) {
				printf("Safe sequence not found... :(\n");
				return -1;
			}

			for (int g = 0; g < customers; g++) {
				int ind = arr[g];
				printf("--> Customer/Thread %d\n", ind);
				
				// Running the thread
				pthread_t tid;
				pthread_create(&tid, NULL, threadRun, &ind);
				pthread_join(tid, NULL);
			}
		}
		
		work += 1;
	}



	// Free Dynamically Stored Data
	free(available);
	free(allocated);
	free(needed);

	return 0;
}



/**
 * ================================================================
 * getCustCount - Count number of "\n" occurances in the given file
 * ================================================================
 **/
int getCustCount(char *filename) {
	
	// Variable Declarations
	FILE *fileptr;
    int count_lines = 0;
    char c;
 
    fileptr = fopen(filename, "r"); // Open File

    c = getc(fileptr); // Extract Character from File and store in variable 'c'
    
	while (c != EOF)
    {
		// Increment Count Lines when "\n" is present
        if (c == '\n')
            count_lines++;

        c = getc(fileptr);
    }

    fclose(fileptr); // Close file

    return count_lines;
}



/**
 * ================================================================
 * calcNeeded - Calculates Needed Resources
 * ================================================================
 **/
void calcNeeded(int i, int j, int **allocated, int maximum[j][i], int **needed) {

	for (int m = 0; m < j; m++) {
		for (int n = 0; n < i; n++)
			needed[m][n] = maximum[m][n] - allocated[m][n];
	}
}



/**
 * ================================================================
 * reCalcCurrent - Calculates Current Resources
 * ================================================================
 **/
void reCalcCurrent() {

	int temp;
	int temp2;

	for (int m = 0; m < cust->record_size; m++) {
		temp = 0;

		for (int n = 0; n < cust->customers; n++)
			temp = temp + allocated[n][m];
		
		temp2 = available[m] - temp;
		available[m] = temp2;
	}
}



/**
 * ================================================================
 * bankerAlgorithm - Performs the Banker's Algorithm
 * ================================================================
 **/
int bankerAlgorithm(int m, int n, int **allocated, int maximum[n][m], int available[m], int **needed, int arr[n])
{
    int i, j, k;
    int safe = 1;

    int f[n], ind = 0;
    for (k = 0; k < n; k++)
        f[k] = 0;

    int y = 0;
    for (k = 0; k < m; k++)
    {
        for (i = 0; i < n; i++)
        {
            if (f[i] == 0)
            {

                int flag = 0;
                for (j = 0; j < m; j++)
                {
                    if (needed[i][j] > available[j])
                    {
                        flag = 1;
                        break;
                    }
                }

                if (flag == 0)
                {
                    arr[ind++] = i;

                    for (y = 0; y < m; y++)
                        available[y] += allocated[i][y];

                    f[i] = 1;
                }
            }
        }
    }

    for (i = 0; i < n; i++)
    {
        if (f[i] == 0)
            safe = 0;
    }

    if (safe == 0)
        return -1;

    printf("Safe Sequence is: <");

    for (i = 0; i < n - 1; i++)
        printf("%d ", arr[i]);

    printf("%d>\n", arr[n - 1]);
	printf("Now going to executing the threads:\n\n\n");

    return 0;
}



/**
 * ================================================================
 * threadRun - This function is run by a thread
 * ================================================================
 **/
void *threadRun(void *p) {

	int *client = (int *)p;

	printf("\tAllocated Resources:\t");
	for (int p = 0; p < cust->record_size; p++)
		printf("%d ", allocated[*client][p]);
	

	printf("\n");
    
	
	printf("\tNeeded:\t");
    for (int i=0; i< cust->record_size; i++){
        printf("%d ", needed[*client][i]);
    }


    printf("\n");
    
	
	printf("\tAvailable:\t");
    for (int i=0; i< cust->record_size; i++){
        printf("%d ", available[i]);
    }


    printf("\n");


    printf("\tThread has started\n");
    printf("\tThread has finished\n");
    printf("\tThread is realeasing resources\n");
    

	printf("\tNew Available:\t");
    
	for (int i=0; i< cust->record_size; i++){
        available[i] =  available[i] + allocated[*client][i];
        printf("%d ", available[i]);
    }


    printf("\n");
	
	return NULL;

}