#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>

using namespace std;

ostream &operator<<(ostream &os, const vector<int> &vector)
{

    os << "[";
    if (vector.size() > 0)
    {
        if (vector[0] < 10)
            os << "0";
        os << vector[0];
    }

    for (long unsigned int vectorIndex = 1; (vectorIndex < vector.size()); vectorIndex++)
    {
        os << " - ";
        if (vector[vectorIndex] < 10)
            os << "0";
        os << vector[vectorIndex];
    }

    os << "]";

    return os;
}

void const displayResult(const int acessed, const vector<int> &requisitions, const int displacement)
{
    cout << "Acessou ";
    if (acessed < 10)
        cout << "0";
    cout << acessed << " " << requisitions << " Já deslocou " << displacement << endl;
}

void fillTenRequisitions(vector<int> &requisitions)
{
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(1, 99);
    requisitions.clear();
    for (int i = 0; i < 10; i++)
        requisitions.push_back(distribution(generator));
}

int diferenceBetween(int a, int b)
{
    if (a > b)
        return a - b;
    return b - a;
}

int firstComeFirstServed(vector<int> &requisitions, int header)
{
    int displacement = 0;
    cout << "========== Algoritmo FCFS! ==========\n";
    while (requisitions.size() > 0)
    {
        int acessed = requisitions.front();
        displacement += diferenceBetween(header, acessed);
        header = acessed;
        requisitions.erase(requisitions.begin());
        displayResult(acessed, requisitions, displacement);
    }
    return displacement;
}

int elevatorScan(vector<int> &requisitions, int header)
{
    cout << "========== Algoritmo SCAN(elevador)! ==========\n";
    int displacement = 0;
    bool isRight = false;
    while (requisitions.size() > 0)
    {
        int accessed = 0;
        bool foundedAccess = false;
        int foundedIndex = 0;
        for (long unsigned int requisitionIndex = 0; requisitionIndex < requisitions.size(); requisitionIndex++)
        {
            if ((requisitions[requisitionIndex] <= header && !isRight) ||
                (requisitions[requisitionIndex] >= header && isRight))
            {
                if (!foundedAccess ||
                    (isRight && requisitions[requisitionIndex] < requisitions[foundedIndex]) ||
                    (!isRight && requisitions[requisitionIndex] > requisitions[foundedIndex]))
                    foundedIndex = requisitionIndex;
                foundedAccess = true;
            }
        }
        if (foundedAccess)
        {
            accessed = requisitions[foundedIndex];
            displacement += diferenceBetween(header, accessed);
            header = accessed;
            requisitions.erase(requisitions.begin() + foundedIndex);
        }
        else
        {
            if (isRight)
            {
                isRight = false;
                displacement += diferenceBetween(header, 99);
                header = 99;
                accessed = 99;
            }
            else
            {
                isRight = true;
                displacement += diferenceBetween(header, 0);
                header = 0;
                accessed = 0;
            }
        }
        displayResult(accessed, requisitions, displacement);
    }
    return displacement;
}

void printOriginalAndPosition(vector<int> requisitions, int header)
{
    cout << endl
         << "Original " << requisitions << endl
         << "Posição inicial da cabeça de leitura e gravação " << header << endl
         << endl;
}

int main()
{
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(1, 99);
    int header = distribution(generator);
    vector<int> requisitions;

    fillTenRequisitions(requisitions);

    bool validType = false;
    while (!validType) // Enquanto não for um tipo de algoritmo válido
    {
        int algorithmType;
        cout << "Escolha um dos algoritmos:" << endl
             << endl
             << "0 - FCFS" << endl
             << "1 - SSTF" << endl
             << "2 - Scan (elevador)" << endl
             << "3 - Circular SCAN" << endl
             << "4 - C-Look" << endl
             << endl;

        cin >> algorithmType;
        switch (algorithmType)
        {
        case 0:
            validType = true;
            printOriginalAndPosition(requisitions, header);
            firstComeFirstServed(requisitions, header);
            break;

        case 1:
            validType = true;
            // printOriginalAndPosition(requisitions, header);

            break;

        case 2:
            validType = true;
            printOriginalAndPosition(requisitions, header);
            elevatorScan(requisitions, header);
            break;

        case 3:
            validType = true;
            // printOriginalAndPosition(requisitions, header);

            break;

        case 4:
            validType = true;
            // printOriginalAndPosition(requisitions, header);

            break;
        }
    }
    return 0;
}
