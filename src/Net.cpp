#include "Net.h"
#include "Neuron.h"
#include <iostream>
#include <algorithm>
#include <time.h>

using namespace std;

Net::Net(int dimensions)
{
    for(int i=0; i<dimensions; i++)
    {
        m_Neurons.push_back(new Neuron(dimensions));
    }
    //ctor
}

Net::~Net()
{
    //dtor
}

int Net::stateUpdate(int j)
{
    double newState = 0;
    int change;
    for(int i=0; i < m_Neurons.size(); i++)
    {
        newState += m_Neurons[i]->getState() * m_Neurons[j]->getWeight(i);
    }
    if(newState>0)
    {
        if(m_Neurons[j]->getState() == 1) change =0;
        else
        {
            change=1;
            m_Neurons[j]->setState(1);
        }
    }
    else
    {
        if(m_Neurons[j]->getState() == -1) change =0;
        else
        {
            change=1;
            m_Neurons[j]->setState(-1);
        }
    }
    return change;
}

void Net::train(vector < vector < int > > trainning_set)
{
    double temp = 0;
    for(int i=0; i<m_Neurons.size(); i++)
    {
        for(int j=0; j<m_Neurons.size(); j++)
        {
            if(i==j) m_Neurons[i]->setWeight(0, j);
            else
            {
                temp = 0;
                for(int k = 0; k< trainning_set.size(); k++)
                {
                    temp += trainning_set[k][i]*trainning_set[k][j];
                }
                m_Neurons[i]->setWeight(temp/m_Neurons.size(), j);
            }
        }
    }
}

void Net::test(vector < int > test_set)
{
    for(int i=0; i<m_Neurons.size(); i++)
    {
        m_Neurons[i]->setState(test_set[i]);
    }
    int changes;
    srand(time(NULL));
    do
    {
        changes = 0;
        for(int k=0, i=rand() % (m_Neurons.size()); k<m_Neurons.size(); i=rand() % (m_Neurons.size()), k++)
        {
            changes += stateUpdate(i);
        }
    }
    while(changes != 0);
}
