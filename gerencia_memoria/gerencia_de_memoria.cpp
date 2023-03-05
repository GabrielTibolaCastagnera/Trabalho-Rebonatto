/*
Gabriel Tibola Castagnera
Willian Brun
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <utility>
#include <iostream>
using namespace std;

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
    virtual int insertProcessOnMemory(vector<int> &memory, int process) const = 0;
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
    int insertProcessOnMemory(vector<int> &memory, int process) const override
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

/**
 * @brief Classe que implementa a interface PushType para inserir o processo no maior lugar possível que encontrar.
 *
 */
class WorstFit : public PushType
{
public:
    /**
     * @brief Insere o processo na memória.
     * @param memory Memória a ser inserida o processo.
     * @param process Processo a ser inserido.
     * @returns Se caso o processo foi inserido, retorna qual partição onde foi inserida, se não retorna -1.
     */
    int insertProcessOnMemory(vector<int> &memory, int process) const override
    {
        int worstAddress = -1;
        for (int address = 0; address < memory.size(); address++)
        {
            if (memory[address] >= process)
            {
                if (worstAddress == -1)
                {
                    worstAddress = address;
                }
                else if (memory[worstAddress] < memory[address])
                {
                    worstAddress = address;
                }
            }
        }
        if (worstAddress == -1)
            return -1;
        memory[worstAddress] -= process;
        return worstAddress;
    }
};

/**
 * @brief Classe que implementa a interface PushType para inserir o processo no primeiro lugar que encontrar.
 * Inicia a busca no último endereço que foi inserido outro processo. 
 *
 */
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
    int insertProcessOnMemory(vector<int> &memory, int process) const override
    {
        int stardedAddress = _address;
        while (1)
        {
            if (memory[_address] >= process)
            {
                memory[_address] -= process;
                int aux = _address;
                if(memory[_address] == 0){
                    if(_address == memory.size() - 1)
                        _address = 0;
                }
                return aux;
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
    int insertProcessOnMemory(vector<int> &memory, int process) const override
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
    void setPushType(unique_ptr<PushType> pushType_)
    {
        _pushType = move(pushType_);
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
            if (_lastAddedProcessStatus != -1 && _memory[_lastAddedProcessStatus] == 0)
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

    /**
     * @brief Gera 10 solicitações de memória (aleatoriamente), com tamanhos variando maior que zero (0) e menor que 100.
     * Mostra o número e o tamanho da requisição (entre colchetes) em seguida o conjunto de regiões que estão livres.
     */
    void requestMemoryRandomly()
    {
        default_random_engine generator(rd());

        // Generate n random integers between 1 and 100 and add them to the vector
        uniform_int_distribution<int> distribution(1, 100);

        for (size_t i = 0; i < 10; i++)
        {
            int randomNumber = distribution(generator);
            pushProcessOnMemory(randomNumber);
            cout << "[" << i << " - " << randomNumber << "] " << *this << endl;
        }
    }

    void clearProcessStatus()
    {
        _lastAddedProcessStatus = -2;
    }

    // Inicia a memória com 9 à 15 partições com tamanho variando entre 1 e 100
    MemoryManagement(unique_ptr<PushType> pushType_)
    {
        _pushType = move(pushType_);
        default_random_engine generator(rd());

        uniform_int_distribution<int> distribution(1, 100);
        uniform_int_distribution<int> memorySize(8, 16);

        for (int address = 0; address < memorySize(generator); address++)
        {
            _memory.push_back(distribution(generator));
        }
    }

    // Sobrescreve o operador << para mostrar a memória.
    // Caso a última tentativa de inserção do processo for sucesso, estará destacado a partição.
    friend ostream &operator<<(ostream &os, const MemoryManagement &memoryManagementUnit)
    {
        if (memoryManagementUnit._isPerfectFit)
        {
            os << "\033[1;48;5;208;32m"
               << "Encaixe Perfeito"
               << "\033[0m";
            return os;
        }
        if (memoryManagementUnit._lastAddedProcessStatus == -1)
        {
            os << "\033[1;48;5;208;32m"
               << "Solicitação não pode ser atendida"
               << "\033[0m";
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
    random_device rd;
    vector<int> _memory;
    unique_ptr<PushType> _pushType;
    int _lastAddedProcessStatus = -2;
    bool _isPerfectFit = false;
};

int main()
{
    MemoryManagement memoryManagementUnit(make_unique<FirstFit>());
    int algorithmType;

    cout << "===== Gerenciador de memoria por particoes dinamicas =====" << endl
         << "Escolha um dos algoritmos:" << endl
         << "0 - First-fit" << endl
         << "1 - Best-fit" << endl
         << "2 - Worst-fit" << endl
         << "3 - Circular-fit" << endl
         << endl
         << "9 - Exit" << endl;

    cin >> algorithmType;

    switch (algorithmType)
    {
    case 0:
        cout << "===== Algoritmo First-fit! =====" << endl;
        break;

    case 1:
        cout << "===== Algoritmo Best-fit! =====" << endl;
        memoryManagementUnit.setPushType(make_unique<BestFit>());
        break;

    case 2:
        cout << "===== Algoritmo Worst-fit! =====" << endl;
        memoryManagementUnit.setPushType(make_unique<WorstFit>());
        break;

    case 3:
        cout << "===== Algoritmo Circular-fit! =====" << endl;
        memoryManagementUnit.setPushType(make_unique<CircularFit>());
        break;

    case 9:
        cout << "Exited!" << endl;
        exit(0);
        break;

    default:
        cout << "Out of range!" << endl;
        exit(0);
        break;
    }
    cout << "Original " << memoryManagementUnit << endl;
    memoryManagementUnit.requestMemoryRandomly();
    return 0;
}