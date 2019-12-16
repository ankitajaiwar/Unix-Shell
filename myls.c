
#define  _POSIX_C_SOURCE 200112L
#include <dirent.h> /* struct dirent(d_ino = file serial number, d_name = name of entry and typedef DIR*/
#include <sys/stat.h> /* struct stat gives all info about file */
#include <sys/types.h>


#include <stdio.h> 

#include <stdlib.h>

#include <string.h> 
#include <grp.h>

#include <pwd.h> 


#include <time.h> 







	int main(int argc, char *argv[])

{
	struct dirent *directory_struct;
	DIR *directory; /* directory stream pointer*/
	struct stat file_statistics;
	struct stat fileStat;

	
	char size_path[512];
	char directory_name[256];

	char file_path[512];
	
	int size;

	char time_stamp[256];
	struct group *gr;
	int lstatus;
	struct passwd *pw;
	


	if (argc > 2)
	{
		fprintf(stderr,"Invalid syntax for myls, only one directory name can be given \n");
		exit(EXIT_FAILURE);
	}
	if (argc > 1)
	{

		strcpy(directory_name, argv[1]);/* myls for which directory*/

	}
	else {

		strcpy(directory_name, ".");/* current directory */

	}



	directory = opendir(directory_name);/* open directory of typedef DIR */

	/************************************************************************************/
	if (directory!= NULL)
	{
		while ((directory_struct = readdir(directory)) != NULL)
		{
			if (directory_struct->d_name[0] != '.') { /* Ignore hidden files */
				/* Create the path to stat */
				
				sprintf(size_path, "%s/%s", directory_name, directory_struct->d_name);

				lstatus = lstat(size_path, &fileStat);
				if (lstatus<0)
				{
				
					fprintf(stderr,"stat %s failed (probably file does not exist).\n",size_path);
					exit(EXIT_FAILURE);
				}
				
				size += fileStat.st_blocks;
			}
		}
		

	}

	size = size / 2;
	printf("Total 1024B blocks: %d \n", size);
	/************************************************************************************/
	directory = opendir(directory_name);

	if (directory != NULL) { /* if directory exits*/

		while ((directory_struct = readdir(directory)) != NULL) {/*returns NULL at the end of the directory */

			if (directory_struct->d_name[0] == '.') {

			/* skip hidden files */

				continue;

			}

			
			sprintf(file_path, "%s/%s", directory_name, directory_struct->d_name); 

			lstatus = lstat(file_path, &file_statistics);
			if(lstatus <0)
			{

				fprintf(stderr,"stat %s failed (probably file does not exist).\n", file_path);
				exit(EXIT_FAILURE);
			}


			
			
			if (S_ISDIR(file_statistics.st_mode)) {/* IS FILE A DIRECTORY? */

				
				printf("d");

			}

			else if (S_ISREG(file_statistics.st_mode)) { /* IS FILE REGULAR? */

			

				printf("-");

			}
			
			else if (S_ISBLK(file_statistics.st_mode)) {/* BLOCK SPECIAL */

				

				printf("b");

			}
			else if (S_ISCHR(file_statistics.st_mode)) {/* SPECIAL CHARACTER */

				

				printf("c");

			}
			
			
			else if (S_ISFIFO(file_statistics.st_mode)) {/*FIFO */

				

				printf("p");

			}
			else if (S_ISSOCK(file_statistics.st_mode)) {/* SOCKET */

				printf("s");

			}
			else if (S_ISLNK(file_statistics.st_mode)) {/*symbolik link */

				printf("l");

			}

			/*file_per = (file_statistics.st_mode & ~S_IFMT); // keep only permission bits */
			printf("%c", (S_IRUSR & file_statistics.st_mode) ? 'r' : '-');
			printf("%c", (S_IWUSR & file_statistics.st_mode) ? 'w' : '-');
			printf("%c", (S_IXUSR & file_statistics.st_mode) ? 'x' : '-');
			printf("%c", (S_IRGRP & file_statistics.st_mode) ? 'r' : '-');
			printf("%c", (S_IWGRP & file_statistics.st_mode) ? 'w' : '-');
			printf("%c", (S_IXGRP & file_statistics.st_mode) ? 'x' : '-');
			printf("%c", (S_IROTH & file_statistics.st_mode) ? 'r' : '-');
			printf("%c", (S_IWOTH & file_statistics.st_mode) ? 'w' : '-');
			printf("%c ", (S_IXOTH & file_statistics.st_mode) ? 'x' : '-');



			printf("%ld ", file_statistics.st_nlink);/*Numbr of Links */


			pw = getpwuid(file_statistics.st_uid);/* username */


			printf("%s ", pw->pw_name);

			gr = getgrgid(file_statistics.st_gid);/*group name */

			printf("%s ", gr->gr_name);



			

			

			printf("%6ld ", (long)file_statistics.st_size);



			strcpy(time_stamp, ctime(&file_statistics.st_mtime));

			

			time_stamp[strlen(time_stamp) - 1] = '\0';/* we don;t want newline*/

			

			printf("%25s ", time_stamp);



			/* print file name */

			printf("%s\n", directory_struct->d_name);

		}



		closedir(directory);

	}
	else {

		fprintf( stderr,"Cannot Open directory %s\n", directory_name);

		return EXIT_FAILURE;

	}

	return EXIT_SUCCESS;

}

