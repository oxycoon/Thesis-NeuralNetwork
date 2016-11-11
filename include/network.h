#ifndef NETWORK_H
#define NETWORK_H

#include "neuron.h"
#include "dataentry.h"
#include "datacollection.h"

#include <vector>
#include <random>

#define LEARNING_RATE 0.001
#define MOMENTUM 0.90
#define MAX_EPOCHS 1500
#define TARGET_ACCURACY 90
#define PRINT_EPOCH_DATA true
#define PRINT_EPOCH_DATA_ON_UPDATE_ONLY false
#define PRINT_TRAINING_DATA false
#define WRITE_RESULTS_TO_FILE false

#define USE_GAUSSIAN_NOISE true
#define GAUSSIAN_MEAN 0.0
#define GAUSSIAN_DEVIATON 0.15

//Inspired by https://takinginitiative.wordpress.com/2008/04/23/basic-neural-network-tutorial-c-implementation-and-source-code/

class Network
{
public:
    Network(int in, int out, int hidden, DataType networkType);
    Network(int in, int out, std::vector<int> hidden, DataType networkType);
    ~Network();

    void setLearningParameters(double learningRate, double momentum);
    void setMaxEpochs(unsigned int max);
    void setTargetAccuracy(double target);

    void useBatchLearning();
    void useStochasticLearning();

    void resetNetwork();

    //void runTraining(const std::vector<DataEntry*> &trainingSet, const std::vector<DataEntry*> &generalizedSet, const std::vector<DataEntry*> &validationSet);
    void runTraining(const DataCollection &set);

    /*std::vector<Neuron*>    getOutputNeurons();
    void                    setInputNeurons(std::vector<Neuron*> &input);*/

private:
    int _countInput, _countOutput;
    std::vector<int> _countHidden;
    int _numHiddenLayers;

    DataType _networkType;

    std::vector<Neuron*>                _input;
    std::vector<std::vector<Neuron*>>   _hidden;
    std::vector<Neuron*>                _output;

    std::vector<std::vector<double>>    _hiddenErrorGradient;
    std::vector<double>                 _outputErrorGradient;

    unsigned int _epoch;
    unsigned int _maxEpochs;

    double _targetAccuracy;
    double _learningRate;
    double _momentum;

    double _trainingSetAccuracy;
    double _validationSetAccuracy;
    double _testingSetAccuracy;
    double _trainingSetError;
    double _validationSetError;
    double _testingSetError;

    bool _useBatch;

    std::default_random_engine _generator;
    std::normal_distribution<double> _distribution;

    //Initializers
    void setupNeurons();
    void setupWeights();
    void setupDeltas();
    void setupErrorGradients();



    void initWeights();

    //Epoch training related functions
    //void runTrainingEpoch(const std::vector<DataEntry*> &set);
    void runTrainingEpoch(const std::vector<DataSegment> &set);
    void feedForward(std::vector<double> inputs);
    //void feedForward(DataEntry* input);
    void feedBackward(std::vector<double> targets);
    void updateWeights();

    double activationFunction(double x);
    double calculateOutputErrorGradient(double target, double actual);
    double calculateHiddenErrorGradient(int layer, int index);

    int roundOutput(double output);

    //Non training set related functions
    double getSetAccuracy(const std::vector<DataSegment>  &set);
    double getSetMSE(const std::vector<DataSegment>  &set);

    double getGaussianNoise(double mean, double standardDeviation);

};

#endif // NETWORK_H
