#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CWD "."
#define HIGHER_DIRECTORY ".."

void create_string(char*space, char*directory_1, char*directory_2, char* destination){

	strcpy(space,directory_1);
        strcat(space,"/");
        strcat(space,directory_2);
        strcpy(destination,space);
}

void searchdir(char* name, char* filename)
{
    DIR *directory;
    struct dirent *file;
    directory = opendir(name);
    char* string = malloc(sizeof(char)*1024);


    //Upon having an error nothing is returned
    if (directory == NULL)
        return;

    //if it's not the end of the directory then
     while ((file = readdir(directory))!=NULL) {
       
         //check if the type of the file is a directory
         
         if (file->d_type == DT_DIR) {
            char path[1024];
                //if entry in the directory stream is the current directory, or the upper directory in the hiearchy, then continue
                if (strcmp(file->d_name, CWD) == 0)
                continue;
                if (strcmp(file->d_name, HIGHER_DIRECTORY) == 0)
                continue;
            //otherwise store the directory name of the filename in path

            create_string(string,name,file->d_name,path);
            searchdir(path, filename);
        } else {
            if (strcmp(filename,file->d_name) == 0){
            printf("%s/%s\n", name, file->d_name);
            }

}
}
    closedir(directory);
}

int main(int argc, char *argv[]) {
 int errcode = 0;
   
 if (argc == 3){
	DIR *directory;
        if((directory = opendir(argv[1]))==NULL){
	         errcode = 1;
                 fprintf(stderr,"Invalid Directory\n");
	}
	closedir(directory);
         
    	if((strchr(argv[2],'/'))){
		 errcode = 1;
		 fprintf(stderr,"There is a directory in the filename\n");
	}

	if(errcode == 0){
   		 searchdir(argv[1],argv[2]);
        	 return 0;
	}
	}else{
       fprintf(stderr,"Not enough arguments were passed\n");
}}
  
