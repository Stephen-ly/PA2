/* We want POSIX.1-2008 + XSI, i.e. SuSv4, features */
#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

//declaring the boolean type
typedef int bool;
enum { false, true };

//declaring a global pointer to the visited filepath pointers array
char ** visited;

//index for the number of filepaths that have been added to array (end of array)
int num_filepaths = 0;

//Check whether a filepath is within the global array of filepath pointers
bool check_visited (const char *filepath, const char *namelink){
	if (strlen(namelink) >= strlen(filepath)){
	int i=0;
	//index through the filepath pointers	
	for (i = 0; i <= num_filepaths; i++){	
		if (strcmp(visited[i],filepath)){
			return true;
		}

	}
}
	return false;

}

//Adds the filepath that is given to the the visited pointer array
void mark_visited(const char *filepath){
	//create a filename pointer that has a max of 100 characters
	char *pointer = malloc(sizeof(char)*100);
	strcpy(pointer,filepath);
	//add the pointer to the array
	visited[num_filepaths]=pointer;
	//increment the index
	num_filepaths += 1;
}

//Goes through each node that is given to the array, upon reaching a symbollic link, find the target and
//check if the target is within the pointer array
int node_function(const char *filepath, const struct stat *info,
                const int typeflag, struct FTW *pathinfo){

	//mark the directory that you're in
	mark_visited(filepath);

	//if the directory is a symbollic link then execute
	if (typeflag == FTW_SL) {
        char   *pointed_to;
        int max_length = 1024;
        int length;

	//incrememnt the size of the allocated heapsize if the directory name is too long
        while (1) {
	
	    //add one for the null indicator
            pointed_to = malloc(max_length + 1);
            if (pointed_to == NULL)
                fprintf(stderr,"Not enough memory");
	    
	    
            length = readlink(filepath, pointed_to, max_length);

            if (length >= max_length) {
                free(pointed_to);
                max_length += 1024;
                continue;
            }

            pointed_to[length] = '\0';
            break;
        }

        if (check_visited(pointed_to,filepath) == true){
		return 2;
	}
	
	
        free(pointed_to);
	}	     
    return 0;
    }


int tree_walk(const char *const filepath){
   
    int result;

    //Check for empty filepath
    if (filepath == NULL || *filepath == '\0')
        fprintf(stderr, "invalid path");

    result = nftw(filepath, node_function,10, FTW_PHYS);
 
    
    if (result == 0){
	printf("There is no cycle in the directory \n");
   	return 0;
    }else{ if(result == 2){
	printf("There is a cycle in the directory\n");
	return 0;
}
}
}
int main(int argc, char*argv[]){

    int i;
    visited = malloc(sizeof(char**)*1000);
	
	if(argc == 1){
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL){
            tree_walk(cwd);
        }
	else{
            fprintf(stderr,"CWD Error");
        } 
	}else if (tree_walk(argv[1])) {
            fprintf(stderr,"ERROR");
            return EXIT_FAILURE;
        }
}
	

