##include "bwfs.h"

void encode_inode(const char *path){

	char * pathname = malloc(sizeof(path)+1);
	strcpy(pathname, path);

	filetype * file = filetype_from_path(pathname);

    while(file != NULL){

      char* string = file->blocks;
      size_t length = strlen(string);
      size_t i = 0;
      for (; i < length; i++) {
          //printf("%c", string[i]);
          string[i] = string[i] - spblock.key;
      }
      strcpy(file->blocks, string );
     	(file->blocks)++;
    }

}

void decode_inode(const char *path){

	char * pathname = malloc(sizeof(path)+1);
	strcpy(pathname, path);

	filetype * file = filetype_from_path(pathname);

		while(file != NULL){

			char* string = file->blocks;
			size_t length = strlen(string);
			size_t i = 0;
			for (; i < length; i++) {
					//printf("%c", string[i]);
					string[i] = string[i] + spblock.key;
			}
			strcpy(file->blocks, string );
			(file->blocks)++;
		}

}
