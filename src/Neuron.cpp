#include "Neuron.h"

Neuron::Neuron(int dimensions)
{
    m_State = 0;
    m_Treshold = 0;
    for(int i=0; i<dimensions; i++)
    {
        m_Weights.push_back(0);
    }
    //ctor
}

Neuron::~Neuron()
{
    //dtor
}
