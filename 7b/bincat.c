#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

int file_size2(char *filename) //获取文件大小
{
    struct stat statbuf;
    stat(filename, &statbuf);
    int size = statbuf.st_size;

    return size;
}

int main()
{
    int size = file_size2("bin"); //mmap方式printf bin文件
    int fd = open("bin", O_RDWR);
    char *ptr = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    for (int i = 0; i < size; i++)
        printf("%c", ptr[i]);
    return 0;
}