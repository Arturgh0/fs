#include "t2fs.h"

#include <stdio.h>

void ll(char* path) {

	DIR2 dir = opendir2(path);

	if( dir < 0 ) {
		printf("Could not open directory '%s'\n", path);
	}
	else {
		printf("Directory '%s':\n", path);
		DIRENT2 dir_entry;
		while( readdir2( dir, &dir_entry ) == 0 ) {
			printf("%d\t%s\t%d\n", dir_entry.fileType, dir_entry.name, dir_entry.fileSize);
		}
		closedir2(dir);
	}

}

int main() {

	char cwd[300];

	getcwd2(cwd, 300);
	printf("current working directory: '%s'\n", cwd);
	ll(".");
	chdir2("dir1");
	getcwd2(cwd, 300);
	printf("current working directory: '%s'\n", cwd);
	ll(".");


}
