#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define MAX_BUFFER 4096

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("usage: rwb <blocksize> <blockcount>\n");
        exit(1);
    }

    int block_size = atoi(argv[1]);
    int block_count = atoi(argv[2]);

    char filename[] = "tempfileXXXXXX";
    int fd = mkstemp(filename);

    if (fd == -1) {
        perror("Error creating file");
        exit(1);
    }

    printf("Created tempfile: %s\n", filename);

    char *buffer = (char*) malloc(block_size);
    if (!buffer) {
        printf("Error: failed to allocate buffer of size %d\n", block_size);
        exit(1);
    }

    memset(buffer, 0, block_size);

    struct timeval tv_start, tv_end, tv_elapsed;
    gettimeofday(&tv_start, NULL);

    struct rusage ru_start, ru_end, ru_elapsed;
    getrusage(RUSAGE_SELF, &ru_start);

    for (int i = 0; i < block_count; i++) {
        write(fd, buffer, block_size);
    }

    lseek(fd, 0, SEEK_SET);

    for (int i = 0; i < block_count; i++) {
        read(fd, buffer, block_size);
    }

    getrusage(RUSAGE_SELF, &ru_end);
    gettimeofday(&tv_end, NULL);

    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        perror("Error getting file status");
        exit(1);
    }

    close(fd);
    unlink(filename);

    timersub(&tv_end, &tv_start, &tv_elapsed);
    timersub(&ru_end.ru_utime, &ru_start.ru_utime, &ru_elapsed.ru_utime);

    printf("Elapsed time: %ld.%06ld seconds\n", tv_elapsed.tv_sec, tv_elapsed.tv_usec);
    printf("System time: %ld.%06ld seconds\n", ru_elapsed.ru_utime.tv_sec, ru_elapsed.ru_utime.tv_usec);
    printf("Minimum resident set size: %ld KB\n", ru_end.ru_minflt / 1024);
    printf("Maximum resident set size: %ld KB\n", ru_end.ru_maxrss / 1024);
    printf("Number of input/output operations: %ld\n", (ru_end.ru_inblock + ru_end.ru_oublock + file_stat.st_blocks));

    free(buffer);

    return 0;
}
