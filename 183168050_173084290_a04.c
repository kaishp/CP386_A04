/**
 * @author Kaish Panjwani
 * @file 183168050_a04.c
 * @version 2020-07-23
*/



// Include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>



// Main Driver
int main(int argc, char *argv[])
{
	if(argc<2)
	{
		printf("INPUT ERROR: 'Available' resources Data Missing... Exiting with Error Code -1\n");
		return -1;
	}

	/**
	 * ----------------------------------------------------------------
	 * Storing Available resources Data from Command line Input into 
	 * an Array
	 * ----------------------------------------------------------------
	 * */

	int available_size = argc - 1;
	int available[available_size];

	for (int i = 0; i < available_size; i++)
		available[i] = atoi(argv[i + 1]);



	/**
	 * ----------------------------------------------------------------
	 * Ask for input from user for Number of Customers
	 * ----------------------------------------------------------------
	 * */

	int customers;
	printf("Number of Customers: ");
	scanf("%d", &customers);



	/**
	 * ----------------------------------------------------------------
	 * Read data from file and insert into Maximum resources array
	 * ----------------------------------------------------------------
	 * */

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

	for (int i = 0; i < customers; i++) {
    	for (int j = 0; j < available_size; j++) {
			printf("%d", maximum[i][j]);
			
			if (j != available_size - 1)
				printf(",");
    	}

		printf("\n");
	}
}