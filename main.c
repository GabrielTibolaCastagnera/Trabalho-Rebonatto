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
#include <sys/prctl.h>
#include <errno.h>
#include <math.h>

void filho1(int num)
{
    int divisor = 1;
    int add = 1;
    if (num % 2)
    {
        add++;
    }
    float max = sqrt(num);
    long long sum = 0;
    for (divisor; divisor <= max; divisor += add)
    {
        if (num % divisor == 0)
        {
            sum += divisor + num / divisor;
        }
    }
    printf("Soma dos números divisores de %d: %lld\n", num, sum);
    return;
}
void filho2(int num)
{
    if (num % 2 == 0 || num == 1)
    {
        printf("O número %d não é primo!\n", num);
        return;
    }
    int divisor = 3;
    float max = sqrt(num);
    for (divisor; divisor <= max; divisor += 2)
    {
        if (num % divisor == 0)
        {
            printf("O número %d não é primo!\n", num);
            return;
        }
    }
    printf("O número %d é primo!\n", num);
    return;
}
void filho3()
{
    int waitfor = 1 + rand() % 7;
    printf("Filho 3 Aguardando %d segundos\n", waitfor);
    sleep(waitfor);
    return;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
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
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
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
    long numOfProcess = sysconf(_SC_CHILD_MAX);

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

    //Armazena os pids dos filhos
    pid_t children[3];
    pid_t pid;
    time_t t;
    int status;
    //Cria o primeiro Filho
    if ((children[0] = fork()) < 0)
        perror("fork() error");
    else if (children[0] == 0)
    {
        //Filho 1
        printf("Processo [%d]. Filho 1 tem como pai o processo: %d.\n", getpid(), getppid());
        filho1(num1);
        printf("Processo [%d]. Filho 1 vai terminar sua execução ...\n", getpid());
    }
    else
    {
        //Cria o segundo Filho
        if ((children[1] = fork()) < 0)
            perror("fork() error");
        else if (children[1] == 0)
        {
            //Filho 2
            printf("Processo [%d]. Filho 2 tem como pai o processo: %d.\n", getpid(), getppid());
            filho2(num2);
            printf("Processo [%d]. Filho 2 vai terminar sua execução ...\n", getpid());
        }
        else
        {
            //Cria o terceiro filho
            if ((children[2] = fork()) < 0)
                perror("fork() error");
            else if (children[2] == 0)
            {
                //Filho 3
                printf("Processo [%d]. Filho 3 tem como pai o processo: %d.\n", (int)getpid(), getppid());
                filho3();
                printf("Processo [%d]. Filho 3 apto novamente. Vai terminar sua execução. Fim do Filho3\n", getpid());
            }
            else
            {
                //Espera um dos Filhos Terminarem e retorna o seu pid
                pid = wait(&status);
                system("ps");
                //Encerra todos os filhos
                for (int i = 0; i < 3; i++)
                {
                    if (kill(children[i], SIGTERM) == -1 && errno != ESRCH)
                    {
                        /*Free resources*/
                        exit(EXIT_FAILURE);
                    }
                }
                int filho;
                //Procura qual filho Terminou
                for(int i = 0; i < 3; i++)
                {
                    if (children[i] == pid)
                        filho = i + 1;
                }
                printf("O filho %d com pid %d terminou primeiro\n", filho, pid);

                time(&t);
            }
        }
    }
    exit(0);
}