/*
Willian Brun
Gabriel Tibola Castagnera
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>
#include <bits/stdc++.h>

using namespace std;

ostream &operator<<(ostream &os, vector<int> &vector)
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

void displayResult(int acessed, vector<int> &requests, int displacement)
{
    cout << "Acessou ";
    if (acessed < 10)
        cout << "0";
    cout << acessed << " " << requests << " Já deslocou " << displacement << endl;
}

void printOriginalAndPosition(vector<int> requests, int header)
{
    cout << endl
         << "Original " << requests << endl
         << "Posição inicial da cabeça de leitura e gravação " << header << endl
         << endl;
}

void fillTenRequests(vector<int> &requests)
{
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(1, 99);
    requests.clear();
    for (int i = 0; i < 10; i++)
        requests.push_back(distribution(generator));
}

int diferenceBetween(int a, int b)
{
    if (a > b)
        return a - b;
    return b - a;
}

int firstComeFirstServed(vector<int> &requests, int header)
{
    int displacement = 0;
    cout << "========== Algoritmo FCFS! ==========" << endl;
    while (requests.size() > 0)
    {
        int acessed = requests.front();
        displacement += diferenceBetween(header, acessed);
        header = acessed;
        requests.erase(requests.begin());
        displayResult(acessed, requests, displacement);
    }
    return displacement;
}

int shortestSeekTimeFirst(vector<int> &requests, int header)
{
    cout << "========== Algoritmo SSTF! ==========" << endl;

    int totalDisplacement = 0;
    vector<int> remainingRequests = requests;

    while (!remainingRequests.empty())
    {
        int minDisplacement = INT_MAX;
        int indexToAccess = -1;
        for (size_t i = 0; i < remainingRequests.size(); ++i)
        {
            int displacement = abs(remainingRequests[i] - header);
            if (displacement < minDisplacement)
            {
                minDisplacement = displacement;
                indexToAccess = i;
            }
        }
        int accessed = remainingRequests[indexToAccess];
        totalDisplacement += minDisplacement;
        header = accessed;
        remainingRequests.erase(remainingRequests.begin() + indexToAccess);
        displayResult(accessed, remainingRequests, totalDisplacement);
    }

    return totalDisplacement;
}

int elevatorScan(vector<int> &requests, int header)
{
    cout << "========== Algoritmo SCAN(elevador)! ==========" << endl;
    int displacement = 0;
    bool isRight = false;
    while (requests.size() > 0)
    {
        int accessed = 0;
        bool foundedAccess = false;
        int foundedIndex = 0;
        for (long unsigned int requisitionIndex = 0; requisitionIndex < requests.size(); requisitionIndex++)
        {
            if ((requests[requisitionIndex] <= header && !isRight) ||
                (requests[requisitionIndex] >= header && isRight))
            {
                if (!foundedAccess ||
                    (isRight && requests[requisitionIndex] < requests[foundedIndex]) ||
                    (!isRight && requests[requisitionIndex] > requests[foundedIndex]))
                    foundedIndex = requisitionIndex;
                foundedAccess = true;
            }
        }
        if (foundedAccess)
        {
            accessed = requests[foundedIndex];
            displacement += diferenceBetween(header, accessed);
            header = accessed;
            requests.erase(requests.begin() + foundedIndex);
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
        displayResult(accessed, requests, displacement);
    }
    return displacement;
}

int circularScan(vector<int> &requests, int header)
{
    cout << "========== Algoritmo Circular SCAN! ==========" << endl;
    int displacement = 0;
    vector<int> left, right;
    for (size_t i = 0; i < requests.size(); i++)
    {
        if (requests[i] < header)
            left.push_back(requests[i]);
        if (requests[i] >= header)
            right.push_back(requests[i]);
    }
    // Sorting left and right vectors
    stable_sort(left.begin(), left.end());
    stable_sort(right.begin(), right.end());
    // Right
    for (size_t i = 0; i < right.size(); i++)
    {

        int cur_track = right[i];
        int distance = abs(cur_track - header);
        displacement += distance;
        header = cur_track;

        size_t j = 0;
        for (j = 0; j < requests.size(); j++)
        {
            if (requests[j] == cur_track)
            {
                break;
            }
        }
        requests.erase(requests.begin() + j);
        displayResult(cur_track, requests, displacement);
    }
    if (!requests.empty())
    {
        displacement += abs(header - 99);
        header = 0;
        if (header != 99)
            displayResult(99, requests, displacement);
        displacement += 99;
        displayResult(header, requests, displacement);
    }

    // Left
    for (size_t i = 0; i < left.size(); i++)
    {
        int cur_track = left[i];
        int distance = abs(cur_track - header);
        displacement += distance;
        header = cur_track;

        size_t j = 0;
        for (j = 0; j < requests.size(); j++)
        {
            if (requests[j] == cur_track)
            {
                break;
            }
        }
        requests.erase(requests.begin() + j);
        displayResult(cur_track, requests, displacement);
    }
    return displacement;
}

int cLook(vector<int> &requests, int header)
{
    cout << "========= Algoritmo C-look ==========" << endl;

    int displacement = 0;
    int distance, cur_track;
    vector<int> left, right;

    for (size_t i = 0; i < requests.size(); i++)
    {
        if (requests[i] < header)
            left.push_back(requests[i]);
        if (requests[i] >= header)
            right.push_back(requests[i]);
    }

    // Sorting left and right vectors
    stable_sort(left.begin(), left.end());
    stable_sort(right.begin(), right.end());

    // Right
    for (size_t i = 0; i < right.size(); i++)
    {
        cur_track = right[i];
        distance = abs(cur_track - header);
        displacement += distance;
        header = cur_track;

        size_t j = 0;
        for (j = 0; j < requests.size(); j++)
        {
            if (requests[j] == cur_track)
            {
                break;
            }
        }
        requests.erase(requests.begin() + j);
        displayResult(cur_track, requests, displacement);
    }
    // left
    for (size_t i = 0; i < left.size(); i++)
    {
        cur_track = left[i];
        distance = abs(cur_track - header);
        displacement += distance;
        header = cur_track;

        size_t j = 0;
        for (j = 0; j < requests.size(); j++)
        {
            if (requests[j] == cur_track)
            {
                break;
            }
        }
        requests.erase(requests.begin() + j);
        displayResult(cur_track, requests, displacement);
    }

    return displacement;
}

int main()
{
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(1, 99);
    int header = distribution(generator);
    vector<int> requests;
    int totalRequests = 0;

    fillTenRequests(requests);

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
             << "4 - C-Look" << endl;

        cin >> algorithmType;
        switch (algorithmType)
        {
        case 0:
            validType = true;
            printOriginalAndPosition(requests, header);
            totalRequests = firstComeFirstServed(requests, header);
            cout << "Quantidade total de deslocamentos: " << totalRequests << endl;
            break;

        case 1:
            validType = true;
            printOriginalAndPosition(requests, header);
            totalRequests = shortestSeekTimeFirst(requests, header);
            cout << "Quantidade total de deslocamentos: " << totalRequests << endl;
            break;

        case 2:
            validType = true;
            printOriginalAndPosition(requests, header);
            totalRequests = elevatorScan(requests, header);
            cout << "Quantidade total de deslocamentos: " << totalRequests << endl;
            break;

        case 3:
            validType = true;
            printOriginalAndPosition(requests, header);
            totalRequests = circularScan(requests, header);
            cout << "Quantidade total de deslocamentos: " << totalRequests << endl;
            break;

        case 4:
            validType = true;
            printOriginalAndPosition(requests, header);
            totalRequests = cLook(requests, header);
            cout << "Quantidade total de deslocamentos: " << totalRequests << endl;
            break;
        default:
            cout << "Escolha errada!\nEscolha um valor entre 0 e 4\n\n";
            break;
        }          
    }
    return 0;
}
