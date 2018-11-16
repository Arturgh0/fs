#include "t2fs_internal.h"

#include <stdio.h>

int main() {

    FILE2 handle;
    char buffer[20];
    char buffer2[20];
    char buffer3[20];
    char buffer4[20];

    handle = create2("HelloWorld!");
    write2(handle, "Hello World!", 13);
    read2(handle, buffer, 13);
    printf("%s", buffer);
    seek2(handle, 2);
    read2(handle, buffer2, 11);
    printf("%s", buffer2);
    truncate2(handle);
    write2(handle, "teste", 6);
    read2(handle, buffer3, 8);
    printf("%s", buffer3);
    close2(handle);
    handle = open2("HelloWorld!");
    read2(handle, buffer4, 8);
    printf("%s", buffer4);
    delete2("HelloWorld!");
    handle = open2("HelloWorld!");
    printf("%d", handle);

}
