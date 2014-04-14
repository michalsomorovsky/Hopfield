#ifndef NEURON_H
#define NEURON_H

#include <vector>

using namespace std;

class Neuron
{
    public:
        Neuron(int dimensions);
        ~Neuron();
        int getState() { return m_State; }
        void setState(int val) { m_State = val; }
        double getWeight(int i) { return m_Weights[i]; }
        void setWeight(double val, int i) { m_Weights[i] = val; }
        double getTreshold() { return m_Treshold; }
        void setTreshold(double val) { m_Treshold = val; }
    protected:
    private:
        int m_State;
        vector < double > m_Weights;
        double m_Treshold;
};

#endif // NEURON_H
