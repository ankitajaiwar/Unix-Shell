#define _POSIX_C_SOURCE 200112L /* for error implicit declaration of setenv*/
#include <stdio.h>/*stdin, stdout, stderr */
#include <stdlib.h>/*fgets, exit*/
#include <unistd.h>/*fork*/
#include <string.h>/* strtok*/
#include <sys/wait.h>/* wait, waitpid */
#include <signal.h> /* signal child ignore*/

#include <fcntl.h> /*open*/

#define MAX_LENGTH 80
#define PATH_LENGHT 150
#define EXITCMD "exit"
#define CDCMD "cd"
#define SETCMD "set"
#define MYLSCMD "myls"
#define PWDCMD "pwd"

int main(int argc, char *argv[]) 
{
  char line[MAX_LENGTH];
  char *argsvalue[MAX_LENGTH];
  
  int argscount;
  char *token = (char*)malloc(80* sizeof(char));
  char *settoken;
  char *setpath;
  int index;
  int status;
  int cppid;
  int pid;
  int cpid;
  int cippid;
  int ccpid;
  int background = 0;
  int pipe_count =0;
  int b_count = 0;
  int in_count = 0;
  int out_count = 0;
  int index_i;
  int index_o;
  int statloc;
  int i;
  int icmd_argc;
  int output_file;
  int stdin_dup;
  int stdout_dup;
  int input_file;

  status = 1;
  signal(SIGCHLD, SIG_IGN);
  do
   {	
	    printf("$ ");	
		
		
		 if(fgets(line, MAX_LENGTH, stdin) != NULL )
	    {
	    	if(line[0]=='\n') 
	    		{
	    			/*printf("Empty Command \n");*/
	    			continue;
	    		}
			/*printf("%s", line);*/
			argscount = 0;
			line[strlen(line) - 1] = '\0';/*used otherwise path had extra spaces and command not execute */
			token = strtok(line, " ");
			while(token != NULL)
			{
				argsvalue[argscount++]= token;
				token = strtok(NULL, " ");
			}

			for ( i = 0; i < argscount; ++i)
			{

				if (strcmp(argsvalue[i], "|") == 0)
				{/*HANDLE PIPES*/
					pipe_count = pipe_count + 1;
					index = i;
				}
				else if (strcmp(argsvalue[i], "<") == 0)
				{
					in_count = in_count + 1;
					index_i = i;
					
				}
				else if (strcmp(argsvalue[i], ">") == 0)
				{
					out_count = out_count + 1;
					index_o = i;
					

				}
				else if (strcmp(argsvalue[i], "&") == 0)
				{
					b_count = b_count + 1;
					index = i;
				}
			}

			if ((pipe_count > 0 && in_count > 0) || (pipe_count > 0 && out_count > 0) || (pipe_count > 0 && b_count > 0))
			{
				fprintf(stderr,"Pipe and Input-Output Direction/Background not supposed to be come together \n");
				pipe_count = 0;
				in_count = 0;
				out_count = 0;
				b_count = 0;


				continue;

			}

			if (( in_count > 0 && out_count > 0))

			{
				if (index_i < index_o)
				{

					if (argscount < 3) {


						fprintf(stderr, "Invalid Input Redirection syntax.\n");
						in_count = 0;
						continue;

					}


					char *icmd_args[index_i + 1];
					icmd_argc = index_i;
					for (i = 0; i < icmd_argc; ++i)
					{

						icmd_args[i] = argsvalue[i];

					}

					icmd_args[icmd_argc] = NULL;
					stdin_dup = dup(0);
					input_file = open(argsvalue[index_i + 1], O_RDONLY);
					stdout_dup = dup(1);/* duplicate stdin file descriptor */
					output_file = open(argsvalue[index_o + 1], O_RDWR | O_CREAT | O_TRUNC, 0777);

					if (input_file < 0 || output_file < 0) {

						fprintf(stderr, "Failed to open file.\n");
						in_count = 0;
						out_count = 0;
						continue;

					}



					else {
						cippid = fork();

						dup2(input_file, 0);
						dup2(output_file, 1);
						if (cippid == 0)
						{
							execvp(icmd_args[0], icmd_args);
							fprintf(stderr, "Execution Failed");
							exit(EXIT_FAILURE);
						}
						else {
							waitpid(cippid, &statloc, WUNTRACED);
						}
						close(input_file);
						close(output_file);

						dup2(stdin_dup, 0);
						dup2(stdout_dup, 1);




					}


					in_count = 0;
					out_count = 0;
					continue;
				}
				else
				{
					if (argscount < 3) {


						fprintf(stderr, "Invalid Input Redirection syntax.\n");
						in_count = 0;
						continue;

					}


					char *icmd_args[index_o + 1];
					icmd_argc = index_o;
					for (i = 0; i < icmd_argc; ++i)
					{

						icmd_args[i] = argsvalue[i];

					}

					icmd_args[icmd_argc] = NULL;
					stdin_dup = dup(0);
					input_file = open(argsvalue[index_i + 1], O_RDONLY);
					stdout_dup = dup(1);/* duplicate stdin file descriptor */
					output_file = open(argsvalue[index_o + 1], O_RDWR | O_CREAT | O_TRUNC, 0777);

					if (input_file < 0 || output_file < 0) {

						fprintf(stderr, "Failed to open file.\n");
						in_count = 0;
						out_count = 0;
						continue;

					}



					else {
						cippid = fork();

						dup2(input_file, 0);
						dup2(output_file, 1);
						if (cippid == 0)
						{
							execvp(icmd_args[0], icmd_args);
							fprintf(stderr, "Execution Failed");
							exit(EXIT_FAILURE);
						}
						else {
							waitpid(cippid, &statloc, WUNTRACED);
						}
						close(input_file);
						close(output_file);

						dup2(stdin_dup, 0);
						dup2(stdout_dup, 1);




					}


					in_count = 0;
					out_count = 0;
					continue;
				}

			}

			if (b_count != 0)
			{


				if (index != argscount - 1)
				{
					fprintf(stderr,"Ampersand should be at the end of the command\n");
					b_count = 0;
					continue;
				}
				else
				{
					background = 1;
					argsvalue[argscount - 1] = NULL;
					argscount--;
				}
				b_count = 0;
			}

			if (pipe_count != 0)
			{
				int fd[2];

				int fd_write = -1;
				int pipe_no = -1;
				int counter;
				int fd_show = -1;
				char *left_args[256];

				int pcmd_argc;
				if (argscount < 3) {


					fprintf(stderr,"Invalid Pipe syntax\n");
					pipe_count = 0;
					continue;

				}
				else if (index == argscount - 1) {


					fprintf(stderr," | is not placed correctly, Final command is missing\n");
					pipe_count = 0;
					continue;

				}
				for ( i = 0; i < argscount; ++i) {

					if (strcmp(argsvalue[i], "|") == 0 || i == argscount - 1) {



						pcmd_argc = 0;

						if (pipe_no ==0 || pipe_no ==-1) {

							counter = 0;
							

						}
						else {

						

							counter = pipe_no + 1;

						}



						if (i < argscount - 1) {

							

							pipe_no = i;



						

							for (counter = counter; counter < i; counter++) {

								left_args[pcmd_argc++] = argsvalue[counter];

							}

							left_args[pcmd_argc] = NULL;
							pipe(fd);
							fd_show = fd[1];
						}
						else
						{
							for (counter = counter; counter <= i; ++counter)
							{

								left_args[pcmd_argc++] = argsvalue[counter];

							}

							left_args[pcmd_argc] = NULL;


							fd_show = -1;

						}
						pid = fork();

						if (pid < 0) {

							pipe_count = 0;
							fprintf(stderr,"Fork Failed in Pipe");
							continue;


						}
						else if (pid == 0) {




							if (fd_write > 0) {


								dup2(fd_write, 0);

								close(fd_write);

							}



							if (fd_show != -1 && fd_show != 1) {


								dup2(fd_show, 1);

								close(fd_show);

							}

							execvp(left_args[0], left_args);

							exit(EXIT_SUCCESS);

						}
						else {


							waitpid(pid, NULL, 0);

							close(fd_write);

							close(fd_show);

							fd_write = fd[0];

						}
						
						


					}
				}
				pipe_count = 0;
				continue;

			}
			if (in_count != 0)
			{
				if (argscount < 3) {


					fprintf(stderr,"Invalid Input Redirection syntax.\n");
					in_count = 0;
					continue;

				}
				else if (index_i != argscount - 2) {


					fprintf(stderr," < is not placed correctly.\n");
					in_count = 0;
					continue;

				}

				char *icmd_args[index_i + 1];
				icmd_argc = index_i;
				for ( i = 0; i < icmd_argc; ++i)
				{

					icmd_args[i] = argsvalue[i];

				}

				icmd_args[icmd_argc] = NULL;
				stdin_dup = dup(0);
				input_file = open(argsvalue[argscount - 1], O_RDONLY);

				if (input_file < 0) {

					fprintf(stderr,"Failed to open file %s.\n", argsvalue[argscount - 1]);
					in_count = 0;
					continue;

				}
				else {
					cippid = fork();

					dup2(input_file, 0);
					if (cippid == 0)
					{
						execvp(icmd_args[0], icmd_args);
						fprintf(stderr,"Execution Failed");
						exit(EXIT_FAILURE);
					}
					else {
						waitpid(cippid, &statloc, WUNTRACED);
					}
					close(input_file);
				
					dup2(stdin_dup, 0);



					
				}
				in_count = 0;
				continue;
			}
			if (out_count != 0)
			{
				if (argscount < 3 ){
					

					fprintf(stderr,"Invalid Output Redirection syntax.\n");
					out_count = 0;

					continue;

				}
				else if (index_o != argscount - 2) {
					

					fprintf(stderr," > is not placed correctly.\n");
					out_count = 0;
					continue;

				}

				char *ocmd_args[index_o + 1];
				int ocmd_argc = index_o;
				for (i = 0; i < ocmd_argc; ++i)
				{

					ocmd_args[i] = argsvalue[i];

				}

				ocmd_args[ocmd_argc] = NULL;/* ocmd_args contains all commands on the left of > */
				stdout_dup = dup(1);/* duplicate stdin file descriptor */
				output_file = open(argsvalue[argscount - 1], O_RDWR | O_CREAT | O_TRUNC, 0777);

				if (output_file < 0) {

					fprintf(stderr, "Cannot open file ");
					out_count =0;
					continue;

				}
				else {
					cppid = fork();

					dup2(output_file, 1);
					if (cppid == 0)
					{
						execvp(ocmd_args[0], ocmd_args);
						fprintf(stderr,"Execution Failed \n");
						exit(EXIT_FAILURE);
					}
					else {
						waitpid(cppid, &statloc, WUNTRACED);
					}
					close(output_file);
					
					dup2(stdout_dup, 1);
						
					}		
				out_count =0;
				continue;
			}
			
			
			if (strcmp(EXITCMD, argsvalue[0])==0)/* EXIT CMD*/
			{
					
				status = 0;
			}
			else if (strcmp(CDCMD, argsvalue[0])==0)
			{
				chdir(argsvalue[1]);
			}
			else if (strcmp(PWDCMD, argsvalue[0]) == 0)
			{
				char pwd[1024];
				getcwd(pwd, sizeof(pwd));
				printf(" %s \n", pwd);
			}

			else if (strcmp(SETCMD, argsvalue[0])==0)
			{
				settoken = strtok(argsvalue[1], "=");/*??????????????????????HOW TO USE MULTIPLE DELIMETERS???????????????????????????*/
				setpath = strtok(NULL, "=");

				/*printf("%s \n", settoken);*/
				
				
				/*printf("%s \n", setpath);*/

				if(settoken == NULL || setpath == NULL) 
				{
					fprintf(stderr,"Wrong set syntax, make sure there are no whitespaces \n");
					continue;
				}
				else{


				setenv(settoken, setpath,1);/* overwrite if exist*/
			}

			}

			else if (strcmp(MYLSCMD, argsvalue[0])==0)
			{
				
				char *mypath = getenv("MYPATH");/*get value of environmetn variable MYPATH*/
				int flagfound = 0;
				char test_path[256];
				char *lsargvs[argscount + 1];
				if (mypath == NULL) 
				{
					fprintf(stderr,"Mypath not set \n");
					continue;
				}
				else if(argscount>2)
				{
					fprintf(stderr,"Only 1 argument is valid \n");
					continue;

				}
				
				
				strcpy (test_path,mypath);
				strcat (test_path,"/myls");
				/*printf("%s", test_path);*/
				/*sprintf(test_path, "%s/%s", mypath,"myls");*/
				
				if(access(test_path, F_OK)!=-1){/*test for the existence of file*/
					flagfound = 1;
					
				}
					
				if (flagfound == 1) /* if executable found in path*/
				{
					
					lsargvs[0] = test_path;
					
					if (argscount > 1) {

						lsargvs[1] = argsvalue[1];

						lsargvs[2] = NULL;

					}
					else {

						lsargvs[1] = NULL;

					}
					cpid = fork();
					if (cpid == 0)
					{
						#if defined (_POSIX_counterOB_CONTROL)
											setpgid(cpid, 0);
													
							
							#endif
					execv(lsargvs[0], lsargvs);
					}
					else
					{
						if(background)
						{
							waitpid(cpid, &statloc, WNOHANG);
							background = 0;
						}
						else
						{
							
							waitpid(cpid, &statloc, WUNTRACED);
						}
					}
				}
				else
				{
					fprintf(stderr,"Executable file not found \n");
				}
				
			}
			
			
			else 
			{
				ccpid = fork();
				if (ccpid == 0)
				{	
					
					if (background)
					{
							#if defined (_POSIX_counterOB_CONTROL)
											setpgid(ccpid, 0);
													
							#else
													
							#endif
						

					}

					
					argsvalue[argscount] = NULL;
					execvp(argsvalue[0], argsvalue);
					if (!feof(stdin)) {


						fprintf(stderr,"Execution Failed, Not a valid command \n");
						exit(EXIT_FAILURE);
					}

				}
				else
				{
					
					if (background != 0)
					{
						waitpid(ccpid, &statloc, WNOHANG);
						
						background = 0;
					}
					else
					{
						waitpid(ccpid, &statloc, WUNTRACED);
					}
				}
			}



			
			

			
			
			
			
		}

		else
		{
			/*printf(" Please write a command \n");*/
			continue;
		}

		
		
  } while (status && !feof(stdin));
  /*waitpid(ccpid, &statloc, WUNTRACED);
  waitpid(cppid, &statloc, WUNTRACED);
  waitpid(pid, &statloc, WUNTRACED);*/
  
  return EXIT_SUCCESS;

}
