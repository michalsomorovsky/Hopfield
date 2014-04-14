#ifndef NET_H
#define NET_H

#include "Neuron.h"

class Net
{
    public:
        Net(int dimensions);
        ~Net();
        vector < Neuron* > GetNeurons() { return m_Neurons; }
        void SetNeurons(vector < Neuron* > val) { m_Neurons = val; }
        void train(vector < vector < int > > trainning_set);
        void stateUpdate(int neuron);
        void test(vector < int > test_set);
        int getSize() { return m_Neurons.size(); }
    protected:
    private:
        vector < Neuron* > m_Neurons;
};

#endif // NET_H
