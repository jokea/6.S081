#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char **argv) {
    int n, pid;
    char buf[64];
    int cnt = 0;
    char *args[MAXARG];

    for (int i = 0; i < argc - 1; i++) {
        args[i] = malloc(strlen(argv[i + 1]) + 1);
        strcpy(args[i], argv[i + 1]);
    }

    int pos = 0;
    memset(buf, 0, sizeof buf);
    while (1) {
        char c;
        n = read(0, &c, 1);
        if (n && c != '\n') buf[pos++] = c;
        else if (n == 0 || c == '\n') {
            if (n == 0 && pos == 0) break;
            args[argc - 1] = malloc(strlen(buf) + 1);
            strcpy(args[argc - 1], buf);
            pid = fork();
            if (pid == 0) {
                exec(argv[1], args); 
                exit(0);
            } else if (pid > 0) {
                wait((int *)0);
                memset(buf, 0, sizeof buf);
                pos = 0;
            } else if (pid < 0) {
                fprintf(2, "fork error\n"); 
                exit(1);
            }
        }

        if (n == 0) break;
    }
    
    exit(0);

}
