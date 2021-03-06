#include "t2fs_internal.h"

#include <string.h>


int t2fs_init(){
    if ( !t2fs_initialized ){

        // le informa�oes do superbloco
		if( t2fs_read_superblock(&t2fs_superbloco_info) != 0 ) {
			return -1;
		}

        // seta o current path para o diretorio raiz
		t2fs_cwd_cluster_num = t2fs_superbloco_info.RootDirCluster;
		strcpy(t2fs_cwd_path, "/");

		// calcula numero m�ximo de arquivos por diretorio
		t2fs_maximum_files_in_directory = t2fs_superbloco_info.SectorsPerCluster * 4;

		// inicializa array de diret�rios abertos (zera tudo)
		memset((void*)t2fs_open_directories, 0, sizeof(t2fs_open_directories));

		// inicializa array de arquivos abertos (zera tudo)
		memset((void*)t2fs_open_files, 0, sizeof(t2fs_open_files));

        // carrega o diretorio raiz
        // TODO

        t2fs_initialized = 1;
    }
    return 0;
}
