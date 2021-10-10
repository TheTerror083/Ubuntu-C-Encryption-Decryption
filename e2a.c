#include "e2a.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, const char* argv[])
{
	if(argc < 2 || argc > 2){
		printf("argument error, the program accepts only 1 file as argument \n");
		return 0;
	}
	const char* ext = strrchr(argv[1], '.');	//get the extension of the file (NULL if there's none)
							//used to automatically determine if the file is to be encrypted or decrypted
							 //text files can also have no extension and work with this program
	struct stat fileStat;
	int i, file, out, lenght;
	char* buff;
	char* enc;
	char* newFile;					//name of the output file the progrm creates

	newFile = (char*)malloc((strlen(argv[1])+4)*sizeof(char));	//generate the new file's name
	if(newFile == NULL)
	{
		perror("malloc");
		return 0;
	}
	strcpy(newFile, argv[1]);


	file = open(argv[1], O_RDONLY, S_IRUSR);
	if(file < 0)
	{
		perror("open");
	}

	fstat(file, &fileStat);				//get the input file stats, to determine it's size
	buff=malloc(fileStat.st_size*sizeof(char));
	if( buff == NULL){
		perror("malloc");
		return 0;
	}
	

	enc=malloc(fileStat.st_size*sizeof(char));
	if(enc == NULL){
		perror("malloc");
		return 0;
	}
	if(read(file, buff, fileStat.st_size) < 0)
	{
		perror("read");
	}

	lenght = strlen(buff);

//**************************************************** DECRYPTION
	if(ext != NULL && strcmp(ext,".enc") == 0)
	{
		printf("decrypting..... \n");

		for(i = 0; i < lenght; i++)
			enc[i] = etoa(buff[i]);					//encrypt the file's content

		out = open(strcat(newFile, ".dec"), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		if(out < 0)
		{
			perror("open");
		}
	}
//****************************************************** ENCRYPTION
	else{									//only if the file is not of .enc type
		printf("encrypting..... \n");

		for(i = 0; i < lenght; i++)				//decrypt the file's content
			enc[i] = atoe(buff[i]);

		out = open(strcat(newFile, ".enc"), O_WRONLY | O_CREAT, S_IRUSR);
		if(out < 0)
		{
			perror("open");
		}
	}
	if(write(out, enc, fileStat.st_size) < 0)
		perror("write");
	free(newFile);
	free(buff);
	free(enc);

	return argc;
}
