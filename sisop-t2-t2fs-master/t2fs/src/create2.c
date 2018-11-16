#include "t2fs_internal.h"
#include "apidisk.h"

#include <string.h>

int create2(char* filename) {
	t2fs_init();

	FILE2 handle;
    void* cluster_data;
	unsigned int cluster_registry = 0;
	unsigned int free_cluster;
	unsigned int sector_buffer [SECTOR_SIZE / 4];
    unsigned int sector;
    unsigned int sector_offset;

	// nome inv�lido
	if (strlen(filename) > 51)
        return -1;

    // verifica se filename n�o � um path
    if (strchr(filename, '/') != NULL)
        return -1;

    // tenta abrir o arquivo, verificando se j� existe
    handle = open2(filename);

    // diret�rio de handles cheio
    if (handle == -2)
        return -1;

    // arquivo ainda n�o existe
    if (handle == -1) {
        // abre cluster do diret�rio corrente e procura primeira posi��o livre
        cluster_data = t2fs_read_cluster( t2fs_cwd_cluster_num );
        while( cluster_registry < t2fs_maximum_files_in_directory &&
               (unsigned int) ((struct t2fs_record*)cluster_data+cluster_registry)->TypeVal != (unsigned int) TYPEVAL_INVALIDO) {
            cluster_registry++;
        }

        // diret�rio cheio
        if( cluster_registry == t2fs_maximum_files_in_directory ) {
            free(cluster_data);
            return -1;
        }

        // inicializa entrada
        ((struct t2fs_record*)cluster_data+cluster_registry)->TypeVal = (BYTE) TYPEVAL_REGULAR;
        strcpy( ((struct t2fs_record*)cluster_data+cluster_registry)->name, filename );
        ((struct t2fs_record*)cluster_data+cluster_registry)->bytesFileSize = 0;

        // encontra cluster livre e aloca-o
        free_cluster = t2fs_get_free_cluster();
        if ((int) free_cluster == -1)
            return -1;
        sector = free_cluster / (SECTOR_SIZE / 4);
        sector_offset = free_cluster % (SECTOR_SIZE / 4);
        if (read_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
            return -1;
        sector_buffer[sector_offset] = 0xFFFFFFFF;

        // associa cluster livre ao arquivo
        ((struct t2fs_record*)cluster_data+cluster_registry)->clustersFileSize = 1;
        ((struct t2fs_record*)cluster_data+cluster_registry)->firstCluster = free_cluster;

        // escreve no disco
        if (write_sector(sector + t2fs_superbloco_info.pFATSectorStart, (unsigned char*) sector_buffer) != 0)
            return -1;
        free(cluster_data);

        return open2(filename);
    }

    // arquivo j� existe, conte�do � removido
    seek2(handle, 0);
    truncate2(handle);

    return handle;
}
