#include "t2fs_internal.h"

#include <string.h>

FILE2 open2 (char *filename) {
	t2fs_init();

    int file_handler = 0;
    char* end;
    struct t2fs_record reg_aux;

	// pega registro do filepath
	struct t2fs_record registro = t2fs_find_file( t2fs_cwd_cluster_num, filename );
	if( registro.TypeVal != TYPEVAL_REGULAR ) {
		return -1;
	}

    // acha posi��o livre no array de arquivos abertos
	while( file_handler < MAXIMUM_OPEN_FILES && t2fs_open_files[file_handler].file_record != NULL) {
		file_handler++;
	}

	// numero m�ximo de arquivos abertos atingido
	if( file_handler == MAXIMUM_OPEN_FILES ) {
		return -2;
	}

	// encontra cluster do diret�rio que cont�m o arquivo
	end = strrchr(filename, '/');
	// arquivo est� no diret�rio corrente
    if (end == NULL)
        t2fs_open_files[file_handler].dir_cluster = t2fs_cwd_cluster_num;
    // arquivo est� no diret�rio ra�z
    else if (end == filename)
            t2fs_open_files[file_handler].dir_cluster = t2fs_superbloco_info.RootDirCluster;
         else {
            // remove nome do arquivo do path, deixando apenas caminho para o diret�rio
            end[0] = '\0';
            // obt�m informa��es do diret�rio
            reg_aux = t2fs_find_file( t2fs_cwd_cluster_num, filename );
            t2fs_open_files[file_handler].dir_cluster = reg_aux.firstCluster;
         }


    // inicializa outros dados do handler
	t2fs_open_files[file_handler].file_record = (struct t2fs_record*) malloc(sizeof(struct t2fs_record));
	t2fs_open_files[file_handler].file_record->TypeVal = registro.TypeVal;
    t2fs_open_files[file_handler].file_record->name = registro.name;
	t2fs_open_files[file_handler].file_record->bytesFileSize = registro.bytesFileSize;
	t2fs_open_files[file_handler].file_record->clustersFileSize = registro.clustersFileSize;
	t2fs_open_files[file_handler].file_record->firstCluster = registro.firstCluster;
	t2fs_open_files[file_handler].current_Pointer = 0;

	return file_handler + 1;
}
