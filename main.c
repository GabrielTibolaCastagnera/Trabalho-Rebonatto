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

void filho1(long long num)
{
    long long divisor = 1;
    int add = 1;
    if (num % 2)
    {
        add++;
    }
    long long sum = 0;
    for (divisor; divisor <= num; divisor += add)
    {
        if (num % divisor == 0)
        {
            sum += divisor;
        }
    }
    printf("Soma dos números divisores de %lld: %lld\n", num, sum);
    return;
}
void filho2(long long num)
{
    if (num % 2 == 0 || num == 1)
    {
        printf("O número %lld não é primo!\n", num);
        return;
    }
    int divisor = 3;
    double max = sqrt(num);
    for (divisor; divisor <= max; divisor += 2)
    {
        if (num % divisor == 0)
        {
            printf("O número %lld não é primo!\n", num);
            return;
        }
    }
    printf("O número %lld é primo!\n", num);
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
    long long num1 = atoll(argv[1]);
    long long num2 = atoll(argv[2]);
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
    long executionProcesses = info.procs;
    printf("\33[31m"); /* Para habilitar escrita na cor vermelha */
    printf("Informações do 'pai' e do sistema computacional\n");
    printf("Processo [%d]. Máximo de processos concorrentes: %ld\n", pidAtual, numOfProcess);
    printf("Processo [%d]. Carga de CPU no último minuto: %ld\n", pidAtual, cpuLoad);
    printf("Processo [%d]. Memória disponível: %ld Kb\n", pidAtual, freeramMemory);
    printf("\33[37m"); /* Para habilitar escrita na cor branca */
    printf("Processo [%d]. Pai tem como pai o processo: %d\n", pidAtual, parentPid);
    printf("Processo [%d]. Processos em execução: %ld\n", pidAtual, executionProcesses);

    // Armazena os pids dos filhos
    pid_t children[3];
    pid_t pid;
    time_t t;
    int status;
    // Cria o primeiro Filho
    if ((children[0] = fork()) < 0)
        perror("fork() error");
    else if (children[0] == 0)
    {
        // Filho 1
        pid_t currentPid = getpid();
        sysinfo(&info);
        printf("Processo [%d]. Filho 1 tem como pai o processo: %d.\n", currentPid, getppid());
        printf("Processo [%d]. Processos em execução: %d\n", currentPid, info.procs);
        filho1(num1);
        printf("\33[32m"); /* Para habilitar escrita na cor verde */
        printf("Processo [%d]. Filho 1 vai terminar sua execução ...\n", currentPid);
        exit(1);
    }
    else
    {
        // Cria o segundo Filho
        if ((children[1] = fork()) < 0)
            perror("fork() error");
        else if (children[1] == 0)
        {

            // Filho 2
            pid_t currentPid = getpid();
            sysinfo(&info);
            printf("Processo [%d]. Filho 2 tem como pai o processo: %d.\n", currentPid, getppid());
            printf("Processo [%d]. Processos em execução: %d\n", currentPid, info.procs);
            filho2(num2);
            printf("Processo [%d]. Filho 2 vai terminar sua execução ...\n", currentPid);
            exit(2);
        }
        else
        {
            // Cria o terceiro filho
            if ((children[2] = fork()) < 0)
                perror("fork() error");
            else if (children[2] == 0)
            {
                // Filho 3
                pid_t currentPid = getpid();
                sysinfo(&info);
                printf("Processo [%d]. Filho 3 tem como pai o processo: %d.\n", currentPid, getppid());
                printf("Processo [%d]. Processos em execução: %d\n", currentPid, info.procs);
                filho3();
                printf("Processo [%d]. Filho 3 apto novamente. Vai terminar sua execução. Fim do Filho3\n", currentPid);
                exit(3);
            }
            else
            {
                system("ps");
                // Espera um dos Filhos Terminarem e retorna o seu pid
                pid = wait(&status);
                int filho = WEXITSTATUS(status);
                // Encerra todos os filhos
                for (int i = 0; i < 3; i++)
                {
                    if ((i + 1) != filho)
                        kill(children[i], SIGTERM);
                }
                if (filho == 1)
                {
                    printf("Não foi possível terminar de verificar se o número é primo. Fim do filho 2\n");
                    printf("Não foi possível 'descansar' alguns segundo. Fim do filho 3\n");
                }
                if (filho == 2)
                {
                    printf("Não foi possível somar os divisores do numero. Fim do filho 1\n");
                    printf("Não foi possível 'descansar' alguns segundo. Fim do filho 3\n");
                }
                if (filho == 3)
                {
                    printf("Não foi possível somar os divisores do numero. Fim do filho 1\n");
                    printf("Não foi possível terminar de verificar se o número é primo. Fim do filho 2\n");
                }
            }
        }
    }
    exit(0);
}