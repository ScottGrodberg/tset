#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_OUT_PASS "\x1b[1;37;42m"
#define ANSI_OUT_FAIL "\x1b[1;37;41m"
#define ANSI_OUT_RESET "\x1b[0m"

const int maxtests = 16;

int debug =0;


/**
 * param char * argv[1] The name of the exe you want to test
 * param char * argv[2] The name of the testcases file
 */
int main (int argc, char **argv) {

	const int MAXLINELEN = 100;
	char line[MAXLINELEN];

	char input[100];
	char expected[100];
	char actual[100]; /* this field is not used by this progam as of 3/3/16. */
	
	char command[100];

	line[0] ='\0';

    	if (argc !=3){
		printf("This program expects 2 arguments. 0=exename, 1=testcases filename\n");
		exit (1);
	}
	
	/* create and open the fd */
	FILE *fp;
	fp = fopen(argv[2], "r");

	int i;
	i=0;
	while(fgets(line, MAXLINELEN, fp)){

		if (*line && i) {
			/* get a line, up to MAXLINELEN chars from fp.  done if NULL */
			sscanf (line, "%[^|]|%[^|]|%[^\n]", input, expected, actual);
			

			if (debug){
				/* convert the string to a long int */
				printf ("%s | %s | %s\n", input, expected, actual);
			}

			/* we have the program args and what is expected */
			/* build the command for popen() */
			snprintf(command, sizeof(command), "./%s %s 2>&1", argv[1], input);
		
			if (debug){
				/* show the built-up command string and test color output */ 
				printf ("Here is the command: %s\n", command);
			}

			FILE *pp;
			pp = popen(command,"r");
			char output[100];

			while (fgets(output, MAXLINELEN, pp)) {

				char *result;
				
				/* this assumes there is a newline that needs to get removed, but we will add a null char */
				int len_output = strlen(output);
				if (debug) printf("here is the line length of the result from the tested program %d\n",len_output);


				result = (char*)malloc(sizeof(char) * len_output); 
				
				sscanf(output, "%[^\n]", result);
				result[len_output] = '\0';

				if (debug) printf("Here is the command with newline removed: %s\n", result);
			
				printf ("Input: %s  Expected: %s  Actual: %s ", input, expected, result);
				if (strcmp(result,expected) ==0 )
					printf(ANSI_OUT_PASS "PASS" ANSI_OUT_RESET "\n");
				else
					printf (ANSI_OUT_FAIL "FAIL" ANSI_OUT_RESET "\n");
	
			}

			pclose(pp);
		


		
		}
		i++;
	}
	fclose(fp);  

	/* reset the output ansi color and done */
	printf("Done!\n");

	return 0;
}



