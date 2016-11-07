#include "../include/neuron.h"

Neuron::Neuron()
{
}

Neuron::~Neuron()
{
    _deltaCount = 0;
    _weightCount = 0;
}

/**
 * @brief Neuron::setValue
 * @param val
 *
 *  Sets a new value for the node
 */
void Neuron::setValue(double val)
{
    _value = val;
}

/**
 * @brief Neuron::setWeight
 * @param index Item index
 * @param weight New weight
 * @return Returns true if successful, returns false if index is out of scope.
 *
 *  Sets the weight at index.
 */
bool Neuron::setWeight(unsigned int index, double weight)
{
    if(index < _weightCount)
    {
        _weights[index] = weight;
        return true;
    }
    else
        return false;
}

/**
 * @brief Neuron::setDelta
 * @param index Item index
 * @param delta New delta
 * @return Returns true if successful, returns false if index is out of scope.
 *
 *  Sets the weight at index
 */
bool Neuron::setDelta(unsigned int index, double delta)
{
    //Else set delta at index to input
    if(index < _deltaCount)
    {
        _deltas[index] = delta;
        return true;
    }
    //Else fail
    else
        return false;
}

/**
 * @brief Neuron::getWeights
 * @return Pointer to array of weights
 *
 *  Gets the array of weights
 */
std::vector<double> Neuron::getWeights() const
{
    return _weights;
}

/**
 * @brief Neuron::getWeight
 * @param index
 * @return Weight at index or 0 if the index is out of scope
 */
double Neuron::getWeight(unsigned int index) const
{
    if(index < _weightCount)
        return _weights[index];
    else
        return 0.0;
}

std::vector<double> Neuron::getDeltas() const
{
    return _deltas;
}

double Neuron::getDelta(unsigned int index) const
{
    if(index < _deltaCount)
        return _deltas[index];
    else
        return 0.0;
}

double Neuron::getValue() const
{
    return _value;
}

int Neuron::getWeightCount() const
{
    return _weightCount;
}

int Neuron::getDeltaCount() const
{
    return _deltaCount;
}

void Neuron::addToValue(double val)
{
    _value += val;
}

bool Neuron::addToDelta(unsigned int index, double val)
{
    /*if(_deltaCount == 1)
    {
        _deltas += val;
        return true;
    }*/
    if(index < _deltaCount)
    {
        _deltas[index] += val;
        return true;
    }
    else
        return false;
}

bool Neuron::addToWeight(unsigned int index, double val)
{
    if(index < _weightCount)
    {
        _weights[index] += val;
        return true;
    }
    else
        return false;
}

void Neuron::initializeWeights(int weights)
{
    _weightCount = weights;
    _weights = std::vector<double>(weights);
    for(int i = 0; i < weights; i++)
    {
        _weights[i] = 0.0;
    }
}

void Neuron::initializeDeltas(int deltas)
{
    _deltaCount = deltas;
    _deltas = std::vector<double>(deltas);
    for(int i = 0; i < _deltaCount; i++)
    {
        _deltas[i] = 0.0;
    }
}
