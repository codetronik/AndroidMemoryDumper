#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        printf("\ndump [pid] [start address] [end address]\n");
        return 0;
    }
    char* end;
    int pid = atoi(argv[1]);
    intptr_t startAddr = strtoll(argv[2], &end, 16);
    intptr_t endAddr = strtoll(argv[3], &end, 16);
    int memSize = endAddr - startAddr;
    printf("\npid : %d\nstart addr %.10lX\n  end addr %.10lX\nsize : %x\n", pid, startAddr, endAddr, memSize);
    
    unsigned char* pMem = (unsigned char*)malloc(memSize);
    memset(pMem, 0, memSize);
    int fd = open("memory", O_WRONLY | O_TRUNC | O_CREAT, 0777);
    if (-1 == fd)
    {
        printf("cannot create file.\n");
        return 0;
    }
    int success = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    if (-1 == success)
    {
        printf("process attach error\n");
        return 0;
    }

    for (int i = 0; i < memSize; i = i + sizeof(long))
    {
        long data;
        data = ptrace(PTRACE_PEEKDATA, pid, (void*)(startAddr + i), NULL);
        if (-1 == data)
        {          
            printf("peekdata error! address : %zx (%d)\n", startAddr + i, errno);
        }

        memcpy(pMem + i, &data, sizeof(long));

        ssize_t written = write(fd, &data, sizeof(long));
        if (-1 == written)
        {
            printf("writing error!\n");
            return 0;
        }
    }

    ptrace(PTRACE_DETACH, pid, NULL, NULL);
    return 0;
}