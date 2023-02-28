/*
Este trabalho vai computar nota para a média semestral. Além, disso sua não realização irá ocasionar falta aos estudantes no dia 25/02/2023.

Implementar um programa que simule parte de um gerenciador de memória por partições dinâmicas. O programa deve ser escrito em linguagem C/C++
que seja executado no sistema operacional linux de 64 bits.

O programa deve produzir uma lista de  espaços de memória livre, contendo entre oito (8) e 15 regiões sendo cada uma com tamanho variando entre
um (1) e 100. A quantidade de regiões e o tamanho de cada uma deve ser gerado de forma aleatória.

O lista deve ser mostrada, com cada uma das regiões de memória livre  separadas por espaço em branco, traço e espaço em branco (REG1 - REG2 - REG3).
As expressões REG<numero> são lacunas de memória disponível. Ver exemplo de solução.

O usuário deve escolher o algoritmo a ser usado para percorrer a lista, dentre os clássicos descritos na literatura. O programa deve suportar os
quatro algoritmos comumente citados.

Após a definição do algoritmo, deve-se gerar 10 solicitações de memória (aleatoriamente), com tamanhos variando maior que zero (0) e menor que 100.
Deve-se mostrar o número e o tamanho da requisição (entre colchetes) em seguida o conjunto de regiões que estão livres, após atender a solicitação.

Caso a região não seja completamente preenchida, o espaço que sobrou deve ser inserido como uma nova região no conjunto, no mesmo local da região
original, sendo destacado em negrito.
Caso a região for completamente preenchida, a lista deve diminuir uma posição. Nesse caso, uma solicitação preenche completamente uma região
(perfect fit), a mesma deve ser excluída do conjunto de regiões de memória. Mostrar "Encaixe perfeito" ao final da lista, em negrito.

Caso não seja possível atender uma requisição por falta de um espaço contíguo, mostrar "Solicitação não pode ser atendida" e não mostrar a lista.
Orientações:

O trabalho pode ser realizado de forma individual ou em dupla. Informe em comentário no inicio do código fonte o nome dos responsáveis pelo trabalho.
Trabalhos que não compilam serão avaliados com nota zero (0).
Trabalhos iguais, mais de uma dupla terão a nota dividida pela quantidade de duplas.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <utility>
#include <iostream>

class PushType
{
public:
    virtual int insertProcessOnMemory(std::vector<int> &memory, int process) const = 0;
};

class FirstFit : public PushType
{
public:
    int insertProcessOnMemory(std::vector<int> &memory, int process) const override
    {
        for (int adress = 0; adress < memory.size(); adress++)
        {
            if (memory[adress] >= process)
            {
                memory[adress] -= process;
                return adress;
            }
        }
        return -1;
    }
};

class BestFit : public PushType
{
public:
    int insertProcessOnMemory(std::vector<int> &memory, int process) const override
    {
        int bestAdress = -1;
        for (int adress = 0; adress < memory.size(); adress++)
        {
            if (memory[adress] >= process)
            {
                if (bestAdress == -1)
                {
                    bestAdress = adress;
                }
                else if (memory[bestAdress] > memory[adress])
                {
                    bestAdress = adress;
                }
            }
        }
        if (bestAdress == -1)
            return -1;
        memory[bestAdress] -= process;
        return bestAdress;
    }
};

class MemoryManagement
{
public:
    void setPushType(std::unique_ptr<PushType> pushType_)
    {
        _pushType = std::move(pushType_);
    }

    int pushProcessOnMemory(int process)
    {
        if (_pushType)
        {
            return _pushType->insertProcessOnMemory(_memory, process);
        }
        return -1;
    }

    MemoryManagement(std::unique_ptr<PushType> pushType_)
    {
        _pushType = std::move(pushType_);
        // Seed the random number generator
        std::random_device rd;
        std::default_random_engine generator(rd());

        // Generate n random integers between 1 and 100 and add them to the vector
        std::uniform_int_distribution<int> distribution(1, 100);
        std::uniform_int_distribution<int> memorySize(8, 16);

        for (int adress = 0; adress < memorySize(generator); adress++)
        {
            _memory.push_back(distribution(generator));
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const MemoryManagement &memoryManagement)
    {
        os << "[";
        bool flag = false;
        for (int adress = 0; adress < memoryManagement._memory.size(); adress++)
        {
            if (flag)
            {
                os << " - ";
            }
            os << memoryManagement._memory[adress];
            flag = true;
        }
        os << "]";
        return os;
    }

    int length() const
    {
        return _memory.size();
    }

private:
    std::vector<int> _memory;
    std::unique_ptr<PushType> _pushType;
};

int main()
{
    MemoryManagement memoryManagement(std::make_unique<FirstFit>());
    std::cout << "length(" << memoryManagement.length() << "): " << memoryManagement << std::endl;

    int a = memoryManagement.pushProcessOnMemory(20);
    std::cout << a << ": " << memoryManagement << std::endl;

    std::cout << "length(" << memoryManagement.length() << "): " << memoryManagement << std::endl;
    memoryManagement.setPushType(std::make_unique<BestFit>());
    a = memoryManagement.pushProcessOnMemory(5);
    std::cout << a << ": " << memoryManagement << std::endl;
    return 0;
}