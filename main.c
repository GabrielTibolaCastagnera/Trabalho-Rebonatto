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
// #include <sys/sysinfo.h>

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

    //Processo atual
    pid_t pidAtual, parentPid;
    pidAtual = getpid();

    //TODO:Máximo de processos concorrentes
    long numOfProcess;

    //Get sysinfo
    struct sysinfo info;
    sysinfo(&info);

    //Carga de CPU no último minuto
    long cpuLoad = info.loads[0];
    //Memória disponível
    long freeramMemory = info.freeram;

    //Pid do pai do atual
    parentPid = getppid();

    //Processos em execução
    long executionProcesses = 0;

    printf("Informações do 'pai' e do sistema computacional\n");
    printf("Processo [%d]. Máximo de processos concorrentes: %ld\n", pidAtual, numOfProcess);
    printf("Processo [%d]. Carga de CPU no último minuto: %ld\n", pidAtual, cpuLoad);
    printf("Processo [%d]. Memória disponível: %ld\n", pidAtual, freeramMemory);
    printf("Processo [%d]. Pai tem como pai o processo: %d\n", pidAtual, parentPid);
    printf("Processo [%d]. Processos em execução: %ld\n", pidAtual, sysconf(_SC_CHILD_MAX));

    return 0;
}