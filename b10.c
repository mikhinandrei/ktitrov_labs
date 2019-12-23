/* Запускаю, читаю STDIN,
после специального символа направляю
дочернему процессу и как-то
красиво из него вывожу*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd[2];
    int state = pipe(fd);
    if (state == -1) printf("Piping error\n");
    pid_t p;

    char **inputs = (char**)malloc(20*sizeof(char*));
    int i = 0;

    char input[1024];
    char got[1024];

    switch (p = fork()) {
        case -1:
            printf("error\n");
            break;
        case 0:
            read(fd[0], got, 1024);
            while(strncmp(got, "end", 1024) != 0) {
                printf("Child process got string: %s\n", got);
                read(fd[0], got, 1024);
            }
            close(fd[0]);
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("child process' PID = %d\n", p);
            printf("%s\n", "Введите send чтобы отправить дочернему процессу\nВведите end чтобы завершить");
            scanf("%s", input);
            while (strncmp(input, "end", 1024) != 0) {
                if (strncmp(input, "send", 1024) == 0) {
                    for (int j = 0; j < i; j++) {
                        write(fd[1], inputs[j], 1024);
                        free(inputs[j]);
                    }
                    i = 0;
                    free(inputs);
                    inputs = (char**)malloc(20*sizeof(char*));
                }
                else {
                  inputs[i] = (char*)malloc((strlen(input)+1)*sizeof(char*));
                  strcpy(inputs[i], input);
                  i++;
                }
                scanf("%s", input);
            }
            write(fd[1], input, 1024);
            for (int j = 0; j < i; j++) {
                free(inputs[j]);
            }
            free(inputs);
            close(fd[1]);
            printf("CLOSED\n");
            exit(EXIT_SUCCESS);
            break;
    }

    return 0;
}
