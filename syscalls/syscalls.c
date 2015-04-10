

#include <errno.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <sys/types.h>

/* errno definition */
#undef errno
extern int errno;

// For sbrk
extern int  __heap_start__;

caddr_t _sbrk(int incr) {
    static unsigned char *heap = NULL;
    unsigned char *prev_heap;

    // First time initialization
    if (heap == NULL) {
        heap = (unsigned char *) &__heap_start__;
    }
    // Store current position
    prev_heap = heap;

    // TODO: check heap size

    heap += incr;

    return (caddr_t) prev_heap;
}

int _close(int file) {
    return -1;
}

int _execve(char *name, char **argv, char **env) {
    return -1;
}

void _exit(int __status) {
  while(1);
}

int _fork() {
    return -1;
}

int _fstat(int file, struct stat *st) {
    return 0;
}

int _getpid() {
    return 1;
}

int _kill(int pid, int sig) {
    return(-1);
}

int isatty(int fildes) {
    return 1;
}
int _isatty(int fildes) {
    return 1;
}

int _link(char *old, char *new) {
    return -1;
}

int _lseek(int file, int ptr, int dir) {
    return 0;
}

int _open(const char *name, int flags, int mode) {
    return -1;
}

int _read(int file, char *ptr, int len) {
    return 0;
}

int _stat(char *file, struct stat *st) {
    return 0;
}

int _times(struct tms *buf) {
    return -1;
}

int _unlink(char *name) {
    return -1;
}

int _wait(int *status) {
    return -1;
}

int _write(int file, char *ptr, int len) {
    return -1;
}