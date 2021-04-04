#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    int n, pid;
    char buf[64];
    int pd1[2], pd2[2];

    pipe(pd1);
    pipe(pd2);
    pid = fork();
    if (pid == 0) {
        /* child */
        close(pd1[1]);
        close(pd2[0]);
        n = read(pd1[0], buf, 64);
        if (n == 0) {
            fprintf(2, "parent closed connection\n");
            exit(1);
        } else if (n < 0) {
            fprintf(2, "child read error\n");
            exit(1);
        } else {
            int pid = getpid();
            fprintf(1, "%d: received ping\n", pid);
            write(pd2[1], buf, 1);
            exit(0);
        }
    } else if (pid > 0) {
        /* parent */
        close(pd1[0]);
        close(pd2[1]);
        write(pd1[1], "a", 1);
        n = read(pd2[0], buf, 64);
        if (n == 1) {
            fprintf(1, "%d: received pong\n", getpid());
            wait((int *)0);
            exit(0);
        }

    } else {
        fprintf(2, "fork error\n");
        exit(1);
    }

    exit(0);

}
