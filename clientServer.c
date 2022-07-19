#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#define STR_LEN 100

// Function to catch and kill sig.
void catcher1(int signum)
{
	kill(getpid(), 22);
}

// Function to catch signal and exit.
void catch22(int signum)
{
	exit(0); // If received C+ctrl exit
}


// Void function, recevies a string and reverse it
void revstr(char* str1)  //function for reverse the string
{
	int i, len, temp;
	len = strlen(str1); // Keep string length 

	for (i = 0; i < len / 2; i++)  // Iterate over the string.
	{
		temp = str1[i];  // Keep str1[i].
		str1[i] = str1[len - i - 1];   // Replace str1[i] with str1[len-i-1].
		str1[len - i - 1] = temp;  // Replace str1[len-i-1] with temp.
	}
}


int main() 
{
  int first_pipe[2], second_pipe[2]; // 2 arrays that represent pipe
  int status1, status2; // 2 int to keep status
  char strR[STR_LEN], strW[STR_LEN]; // 2 strings
  signal(SIGTERM, catcher1); // Call catcher1 when SIGTERM appears.
  signal(22, catch22); // Catch signal 22.

  status1 = pipe(first_pipe); // Create first pipe
  status2 = pipe(second_pipe); // Create second pipe
  if (status1 == -1) // Check if creation was succseful.
  {
    printf("Unable to open pipe\n");
    exit(-1); // If not print message and exit
  }
  if (status2 == -1)  // Check if creation was succseful. 
  {
    printf("Unable to open pipe\n");
    exit(-1); // If not print message and exit
  }

  status1 = fork(); // Fork, create son process
  if (status1 == -1) // Check if sucsseful
  { 
    printf("Unable to fork\n");
    exit(-1); // If not print a message and exit
  }


if (status1 == 0)  /* server process */
{    
	close(first_pipe[1]); // Close first pipe for writing
	close(second_pipe[0]); // Close second pipe for reading
	while(1)  // infinite loop.
	{
		read(first_pipe[0], strR, STR_LEN); // Read from first pipe
		printf("----------server---------\n");
	
		if(strcmp(strR, "exit") == 0) // Check if strR equals "exit".
		{
			strcpy(strW, "Done"); // If so create the server response "Done".
			write(second_pipe[1], strW, (strlen(strW) + 1) * sizeof(char)); //Write strW to second_pipe
			printf("Server shut down.\n"); 
			exit(0); // if so after sending message exit
		}
		
		printf("Got from client: %s\n", strR);
		strcpy(strW, strR); // If the string received is not exit copy it.
		revstr(strW); // Reverse the string received
		write(second_pipe[1], strW, (strlen(strW) + 1) * sizeof(char)); // Write the reveresed message to second_pipe
		printf("Response sent...\n");	
	}
	exit(0);
  }
  
else  /* client process */
{   
	close(first_pipe[0]); // Close first_pipe for reading
	close(second_pipe[1]); // Close second_pipe for writing
	while(1) // infinite loop.
	{
		if(strcmp(strR, "Done") == 0) // Check if received done from server
		{
			exit(0); // If so exit
		}
		
		printf("----------client---------\n");
		printf("Enter to the server: ");
		scanf("%s", strW); // Get client message
	
		write(first_pipe[1], strW, (strlen(strW) + 1) * sizeof(char)); // Write message to first_pipe
		read(second_pipe[0], strR, STR_LEN); // Read message from second_pipe
		printf("----------client---------\n"); 
		printf("Received from server: %s\n\n\n", strR);	
	}
	exit(0); // End of program
  }
}
