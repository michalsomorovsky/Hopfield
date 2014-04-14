#include "Net.h"
#include "Neuron.h"
#include <iostream>

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

void Net::stateUpdate(int j)
{
    double newState = 0;
    for(int i=0; i < m_Neurons.size(); i++)
    {
        //cout<<m_Neurons[i]->getState()<<" * "<<m_Neurons[j]->getWeight(i)<<endl;
        newState += m_Neurons[i]->getState() * m_Neurons[j]->getWeight(i);
    }
    //cout<<"Neuron "<<j<<" : "<<newState<<endl;
    if(newState>0) m_Neurons[j]->setState(1);
    else return m_Neurons[j]->setState(-1);
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
        //cout<<"Testset : "<<test_set[i]<<endl;
        //cout<<"State : "<<m_Neurons[i]->getState()<<endl;
    }
    for(int i=0; i<m_Neurons.size(); i++)
    {
        stateUpdate(i);
    }
}
