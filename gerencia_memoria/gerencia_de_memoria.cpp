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
/**
 * @brief Uma interface para abstrair cada jeito de colocar o processo na memória.
 */
class PushType
{
public:
    /**
     * @brief Abstração da funcão que insere um processo na memória.
     * @param memory Memória a ser inserida o processo.
     * @param process Processo a ser inserido.
     * @returns Um inteiro.
     * @returns Se caso o processo foi inserido, retorna qual partição onde foi inserida, se não retorna -1.
     */
    virtual int insertProcessOnMemory(std::vector<int> &memory, int process) const = 0;
};

/**
 * @brief Classe que implementa a interface PushType para inserir o processo no primeiro lugar que encontrar.
 *
 */
class FirstFit : public PushType
{
public:
    /**
     * @brief Insere o processo na memória.
     * @param memory Memória a ser inserida o processo.
     * @param process Processo a ser inserido.
     * @returns Se caso o processo foi inserido, retorna qual partição onde foi inserida, se não retorna -1.
     */
    int insertProcessOnMemory(std::vector<int> &memory, int process) const override
    {
        for (int address = 0; address < memory.size(); address++)
        {
            if (memory[address] >= process)
            {
                memory[address] -= process;
                return address;
            }
        }
        return -1;
    }
};

class CircularFit : public PushType
{
    mutable int _address = 0;

public:
    /**
     * @brief Insere o processo na memória.
     * @param memory Memória a ser inserida o processo.
     * @param process Processo a ser inserido.
     * @returns Se caso o processo foi inserido, retorna qual partição onde foi inserida, se não retorna -1.
     */
    int insertProcessOnMemory(std::vector<int> &memory, int process) const override
    {
        int stardedAddress = _address;
        while (1)
        {
            if (memory[_address] >= process)
            {
                memory[_address] -= process;
                return _address;
            }
            if (_address == memory.size() - 1)
                if (_address != stardedAddress)
                    _address = -1;
                else
                    return -1;
            _address++;
            if (_address == stardedAddress)
                return -1;
        }
        return -1;
    }
};

/**
 * @brief Classe que implementa a interface PushType para inserir o processo no menor lugar possível que encontrar.
 *
 */
class BestFit : public PushType
{
public:
    /**
     * @brief Insere o processo na memória.
     * @param memory Memória a ser inserida o processo.
     * @param process Processo a ser inserido.
     * @returns Se caso o processo foi inserido, retorna qual partição onde foi inserida, se não retorna -1.
     */
    int insertProcessOnMemory(std::vector<int> &memory, int process) const override
    {
        int bestAddress = -1;
        for (int address = 0; address < memory.size(); address++)
        {
            if (memory[address] >= process)
            {
                if (bestAddress == -1)
                {
                    bestAddress = address;
                }
                else if (memory[bestAddress] > memory[address])
                {
                    bestAddress = address;
                }
            }
        }
        if (bestAddress == -1)
            return -1;
        memory[bestAddress] -= process;
        return bestAddress;
    }
};

/**
 * @brief Classe de gereciamento de memória que depende de uma implementação de inserção na memória.
 *
 */
class MemoryManagement
{
public:
    /**
     * @brief Seta o método desejado para inserir os processo na memória
     */
    void setPushType(std::unique_ptr<PushType> pushType_)
    {
        _pushType = std::move(pushType_);
    }

    /**
     * @brief Insere o processo na memória de acordo com o método inserido.
     * @param process Processo a ser inserido.
     * @returns Se caso o processo foi inserido, retorna qual partição onde foi inserida, se não retorna -1.
     */
    int pushProcessOnMemory(int process)
    {
        if (_pushType)
        {
            _lastAddedProcessStatus = _pushType->insertProcessOnMemory(_memory, process);
            if(_lastAddedProcessStatus != -1 && _memory[_lastAddedProcessStatus] == 0)
            {
                _memory.erase(_memory.begin() + _lastAddedProcessStatus);
                _isPerfectFit = true;
            }
            else
                _isPerfectFit = false;
            return _lastAddedProcessStatus;
        }
        return -1;
    }

    void clearProcessStatus(){
        _lastAddedProcessStatus = -2;
    }

    // Inicia a memória com 9 à 15 partições com tamanho variando entre 1 e 100
    MemoryManagement(std::unique_ptr<PushType> pushType_)
    {
        _pushType = std::move(pushType_);
        // Seed the random number generator
        std::random_device rd;
        std::default_random_engine generator(rd());

        // Generate n random integers between 1 and 100 and add them to the vector
        std::uniform_int_distribution<int> distribution(1, 100);
        std::uniform_int_distribution<int> memorySize(8, 16);

        for (int address = 0; address < memorySize(generator); address++)
        {
            _memory.push_back(distribution(generator));
        }
    }

    // Sobrescreve o operador << para mostrar a memória.
    // Caso a última tentativa de inserção do processo for sucesso, estará destacado a partição.
    friend std::ostream &operator<<(std::ostream &os, const MemoryManagement &memoryManagementUnit)
    {
        if(memoryManagementUnit._isPerfectFit)
        {
            os << "\033[1;48;5;208;32m" << "Encaixe Perfeito" << "\033[0m";
            return os;
        }
        if(memoryManagementUnit._lastAddedProcessStatus == -1)
        {
            os << "\033[1;48;5;208;32m" << "Solicitação não pode ser atendida" << "\033[0m";
            return os;
        }
        os << "[";
        bool flag = false;
        for (int address = 0; address < memoryManagementUnit._memory.size(); address++)
        {
            if (flag)
            {
                os << " - ";
            }
            if (address == memoryManagementUnit._lastAddedProcessStatus)
                os << "\033[1;48;5;208;32m";
            os << memoryManagementUnit._memory[address] << "\033[0m";
            flag = true;
        }
        os << "]";
        return os;
    }

    /**
     * @return Quantidade de partições na memória.
     */
    int length() const
    {
        return _memory.size();
    }

private:
    std::vector<int> _memory;
    std::unique_ptr<PushType> _pushType;
    int _lastAddedProcessStatus = -2;
    bool _isPerfectFit = false;
};

int main()
{
    MemoryManagement memoryManagementUnit(std::make_unique<FirstFit>());
    std::cout << "length(" << memoryManagementUnit.length() << "): " << memoryManagementUnit << std::endl;

    int lastAddedProcessStatus = memoryManagementUnit.pushProcessOnMemory(20);
    std::cout << lastAddedProcessStatus << ": " << memoryManagementUnit << std::endl;
    std::cout << "length(" << memoryManagementUnit.length() << "): " << memoryManagementUnit << std::endl;
    memoryManagementUnit.setPushType(std::make_unique<BestFit>());
    lastAddedProcessStatus = memoryManagementUnit.pushProcessOnMemory(100);
    std::cout << lastAddedProcessStatus << ": " << memoryManagementUnit << std::endl;
    memoryManagementUnit.setPushType(std::make_unique<CircularFit>());
    lastAddedProcessStatus = memoryManagementUnit.pushProcessOnMemory(50);
    std::cout << lastAddedProcessStatus << ": " << memoryManagementUnit << std::endl;
    lastAddedProcessStatus = memoryManagementUnit.pushProcessOnMemory(100);
    std::cout << lastAddedProcessStatus << ": " << memoryManagementUnit << std::endl;
    return 0;
}