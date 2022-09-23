/*
Gabriel Tibola Castagnera
Willian Brun
*/

#include <stdio.h>
#include <ulimit.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    // Verifica se apenas/exatamente dois parâmetros foram informados
    if (argc < 3)
    {
        printf("Missing parameter\n");
        exit(1);
    }
    else if (argc > 3)
    {
        printf("Too many arguments\n");
        exit(1);
    }

    // //Verifica se os parâmetros contém pelo menos 10 digitos
    // if()
    // {
    //     printf("At least one of the parameters has not 10 digits.\n");
    //     exit(1);
    // }

    // Processo atual
    pid_t pidAtual, parentPid;
    pidAtual = getpid();

    // TODO:Máximo de processos concorrentes
    long numOfProcess;

    // Get sysinfo
    struct sysinfo info;
    sysinfo(&info);

    // Carga de CPU no último minuto
    long cpuLoad = info.loads[0];
    // Memória disponível
    long freeramMemory = info.freeram;

    // Pid do pai do atual
    parentPid = getppid();

    // Processos em execução
    long executionProcesses = 0;

    printf("Informações do 'pai' e do sistema computacional\n");
    printf("Processo [%d]. Máximo de processos concorrentes: %ld\n", pidAtual, numOfProcess);
    printf("Processo [%d]. Carga de CPU no último minuto: %ld\n", pidAtual, cpuLoad);
    printf("Processo [%d]. Memória disponível: %ld\n", pidAtual, freeramMemory);
    printf("Processo [%d]. Pai tem como pai o processo: %d\n", pidAtual, parentPid);
    printf("Processo [%d]. Processos em execução: %ld\n", pidAtual, sysconf(_SC_CHILD_MAX));

    pid_t pid;
    time_t t;
    int status;

    if ((pid = fork()) < 0)
        perror("fork() error");
    else if (pid == 0)
    {
        time(&t);
        printf("child (pid %d) started at %s", (int)getpid(), ctime(&t));
        sleep(5);
        time(&t);
        printf("child exiting at %s", ctime(&t));
        exit(1);
    }
    else
    {
        if ((pid = fork()) < 0)
            perror("fork() error");
        else if (pid == 0)
        {
            time(&t);
            printf("child (pid %d) started at %s", (int)getpid(), ctime(&t));
            sleep(5);
            time(&t);
            printf("child exiting at %s", ctime(&t));
            exit(2);
        }
        else
        {
            if ((pid = fork()) < 0)
                perror("fork() error");
            else if (pid == 0)
            {
                time(&t);
                printf("child (pid %d) started at %s", (int)getpid(), ctime(&t));
                sleep(5);
                time(&t);
                printf("child exiting at %s", ctime(&t));
                exit(3);
            }
            else
            {
                printf("parent has forked child with pid of %d\n", (int)pid);
                time(&t);
                printf("parent is starting wait at %s", ctime(&t));
                if ( (pid = wait(&status)) == -1)
                    perror("wait() error");
                else
                {
                    time(&t);
                    printf("parent is done waiting at %s", ctime(&t));
                    printf("the pid of the process that ended was %d\n", (int)pid);
                    if (WIFEXITED(status))
                        printf("child exited with status of %d\n", WEXITSTATUS(status));
                    else if (WIFSIGNALED(status))
                        printf("child was terminated by signal %d\n",
                               WTERMSIG(status));
                    else if (WIFSTOPPED(status))
                        printf("child was stopped by signal %d\n", WSTOPSIG(status));
                    else
                        puts("reason unknown for child termination");
                }
            }
        }
    }

    return 0;
}