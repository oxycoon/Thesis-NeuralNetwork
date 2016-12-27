#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <random>

class Neuron
{
public:
    Neuron();
    ~Neuron();

    void                    setValue(double val);
    bool                    setWeight(unsigned int index, double weight);
    bool                    setDelta(unsigned int index, double delta);
    bool                    setOldDelta(unsigned int index, double delta);

    std::vector<double>     getWeights() const;
    double                  getWeight(unsigned int index) const;
    std::vector<double>     getDeltas() const;
    double                  getDelta(unsigned int index) const;
    std::vector<double>     getOldDeltas() const;
    double                  getOldDelta(unsigned int index) const;
    double                  getValue() const;

    int                     getWeightCount() const;
    int                     getDeltaCount() const;

    void                    addToValue(double val);
    bool                    addToDelta(unsigned int index, double val);
    bool                    addToWeight(unsigned int index, double val);

    void                    initializeWeights(int weights);
    void                    initializeDeltas(int deltas);
    void                    saveOptimals();
    void                    restoreOptimals();

private:
    /**
     * @brief _value
     *
     *  The value of this node in the network.
     */
    double _value;

    /**
     * @brief _weights
     *
     *  Input neurons hold weights towards the hidden layer.
     *  Hidden neurons hold weights towards the output layer.
     */
    std::vector<double> _weights;

    /**
     * @brief _deltas
     *
     *  Input neurons hold deltas towards the hidden layer.
     *  Hidden neurons hold deltas towards the output layer.
     */
    std::vector<double> _deltas;

    /**
     * @brief _deltas
     *
     *  Deltas from previous epoch.
     */
    std::vector<double> _oldDeltas;

    /**
     * @brief _weightCount
     *
     *  Number of weights in this neuron.
     */
    int _weightCount;

    /**
     * @brief _deltaCount
     *
     *  Number of deltas in this neuron.
     */
    int _deltaCount;

    /**
     * @brief _optimalWeights
     *
     *  Holds the best weights found.
     */
    std::vector<double> _optimalWeights;

    /**
     * @brief _optimalWeights
     *
     *  Holds the best deltas found.
     */
    std::vector<double> _optimalDeltas;
};

#endif // NEURON_H
