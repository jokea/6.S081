#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int left[2], right[2];

void child() {
    int n;
    char buf[4];
    int pid;
    int shai;

    int forked = 0;

    if (1 || pid > 0) {
        /*
        close(left[0]);
        close(left[1]);
        left[0] = dup(right[0]);
        close(right[0]);
        */

        /* first number */
        n = read(right[0], buf, 4);
        if (n == 0 || n == -1) {
            close(right[1]);
            wait((int *)0);
            exit(0);
        }

        shai = *(int *)buf;
        if (shai == 0) exit(0);
        //fprintf(1, "==<%d>== receive %d from parent\n", getpid(), shai);
        fprintf(1, "prime %d\n", shai);

        while (1) {
            char buf2[4];
            //fprintf(1, "==<%d>== try read\n", getpid());
            n = read(right[0], buf2, 4);
            if (n != 4) {
                //fprintf(1, "==<%d>== parent closed pipe\n", getpid());
                close(right[1]);
                wait((int *)0);
                exit(0);
            }

            int k = *(int *)buf2;
            if (k == shai) continue;
            if (k == 0) {
                //fprintf(1, "==<%d>== receive %d from parent\n", getpid(), k);
                char buf[4];
                memcpy(buf, &k, 4);
                write(right[1], buf, 4);
                close(right[1]);
                wait((int *)0);
                exit(0);
            }
            //fprintf(1, "==<%d>== receive %d from parent\n", getpid(), k);

            if (k % shai == 0) {
                continue;
            } else {
                //fprintf(1, "==<%d>== sending %d to child\n", getpid(), k);
                if (!forked) {
                    pid = fork();
                    forked = 1;
                }
                if (pid > 0) {
                    write(right[1], (char *)&k, 4);
                } else {
                    //sleep(10);
                    //fprintf(1, "===<%d>=== child waiting\n", getpid()); 
                    child();
                    exit(0);
                }
            }
        }
    } else {
    }

    exit(0);
}

int main(void)
{
    int pid;

    //pipe(left);
    pipe(right);

    pid = fork();
    if (pid == 0) {
        //fprintf(1, "first child: %d\n", getpid());
        //sleep(30);
        child();
        exit(0);
    } else if (pid > 0) {
        /* parent */
        //close(left[0]);
        //close(left[1]);
        close(right[0]);
        char buf[4];
        for (int i = 2; i <= 35; i++) {
            memcpy(buf, &i, 4);
            //fprintf(1, "God write %d to child in fd %d\n", i, right[1]);
            write(right[1], buf, 4);
            //sleep(2);
        }
        int k = 0;
        memcpy(buf, &k, 4);
        write(right[1], buf, 4);
        close(right[1]);
        wait((int *)0);
        exit(0);
    } else {
        fprintf(1, "fork error\n");
        exit(1);
    }

    exit(0);
}

