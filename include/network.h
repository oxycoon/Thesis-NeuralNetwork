#ifndef NETWORK_H
#define NETWORK_H

#include "include/neuron.h"
#include "include/dataentry.h"
#include "include/datacollection.h"
#include "include/cost/cost.h"

#include <QtCore>

#include <vector>
#include <random>

#define LEARNING_RATE 0.01
#define MOMENTUM 0.90
#define MAX_EPOCHS 5000
#define TARGET_ACCURACY 80
#define PRINT_EPOCH_DATA true
#define PRINT_EPOCH_DATA_ON_UPDATE_ONLY false
#define PRINT_TRAINING_DATA false
#define WRITE_RESULTS_TO_FILE true

#define GAUSSIAN_MEAN 0.0
#define GAUSSIAN_DEVIATON 0.10



class Network : public QObject, public QRunnable
{
    Q_OBJECT
public:
    Network();
    Network(int in, int hidden, int out, Cost* cost, DataType networkType, std::string name="");
    Network(std::vector<Network*> inputs, std::vector<int> hidden, int out, Cost* cost,  std::string name="");
    Network(int in, std::vector<int> hidden, int out, Cost* cost, DataType networkType = DataType::UK, std::string name="");
    ~Network();

    void setLearningParameters(double learningRate, double momentum);
    void setMaxEpochs(unsigned int max);
    void setTargetAccuracy(double target);
    void setInputNeurons(std::vector<Neuron*> &input);
    void useBatchLearning();
    void useStochasticLearning();
    void enableNoise(bool enable);
    void setNoiseParameters(double noiseDeviation, double noiseMean = 0.0);
    void setTrainSubNetworksFirst(bool enable);
    void setDataCollection(DataCollection* collection);
    void doTraining(bool enable);
    void editNetwork(int in, std::vector<int> hidden, int out,
                      std::string name, DataType type, Cost* calc);
    void doTesting(bool enable);

    std::vector<Neuron*>    getOutputNeurons() const;
    std::vector<double>     getOutputResults() const;
    int                     getInputCount() const;
    std::vector<int>        getHiddenCount() const;
    int                     getOutputCount() const;
    DataType                getNetworkType() const;
    CostCalc                getNetworkCostCalc() const;
    std::string             getNetworkName() const;
    bool                    hasSubNetworks() const;
    std::vector<Network*>   getSubNetworks() const;
    int                     getNetworkDepth(int depth = 1) const;
    int                     getNetworkID() const;




    void resetNetwork();
    void initNetwork();
    void initNetworkFromSub(std::vector<Network*> inputs);

    //void runTraining(const std::vector<DataEntry*> &trainingSet, const std::vector<DataEntry*> &generalizedSet, const std::vector<DataEntry*> &validationSet);
    void runTraining(DataCollection *set);
    void runTesting(DataCollection *set);

    void run();

protected:
    DataType _networkType;
    static int UNIQUE_ID;

    void feedForward(std::vector<double> inputs);
    void feedBackward(std::vector<double> targets);

private:
    //******************************
    //  Network name
    //******************************
    std::string                         _networkName;
    int                                 _id;

    //******************************
    //  Network Neurons variables
    //******************************
    /**
     * @brief _input
     *
     *  Input layer neurons, if the network consists of
     *  sub networks these will be represented by the
     *  ouput Neurons in the underlying networks.
     */
    std::vector<Neuron*>                _input;
    /**
     * @brief _hidden
     *
     *  Hidden layer neurons.
     */
    std::vector<std::vector<Neuron*>>   _hidden;
    /**
     * @brief _output
     *
     *  Output layer neurons.
     */
    std::vector<Neuron*>                _output;
    /**
     * @brief _subNetworks
     *
     *  Subnetworks within this network.
     */
    std::vector<Network*>               _subNetworks;

    int                                 _countInput;
    int                                 _countOutput;
    std::vector<int>                    _countHidden;
    int                                 _numHiddenLayers;


    //******************************
    //  Network learning settings
    //******************************
    unsigned int    _epoch;
    unsigned int    _maxEpochs;
    double          _targetAccuracy;
    double          _learningRate;
    double          _momentum;
    bool            _doTraining;
    bool            _useBatch;
    bool            _trainSubnetsFirst;
    bool            _useNoise;
    bool            _isTrained;
    bool            _doTesting;
    double          _noiseMean;
    double          _noiseDeviation;

    DataCollection* _dataCollection;

    //******************************
    //  Training and testing error and accuracy
    //******************************
    double                              _trainingSetAccuracy;
    double                              _testingSetAccuracy;
    double                              _trainingSetError;
    double                              _testingSetError;
    std::vector<std::vector<double>>    _hiddenErrorGradient;
    std::vector<double>                 _outputErrorGradient;
    Cost*                               _costCalculator;

    //******************************
    //  Gaussian noise variables
    //******************************
    std::default_random_engine          _generator;
    std::normal_distribution<double>    _distribution;

    //******************************
    //  Initializers
    //******************************
    void setupNeurons();
    void setupWeights();
    void setupDeltas();
    void setupErrorGradients();
    void initWeights();
    void modifyInputs(std::vector<Neuron*> inputs, int &startIndex);

    //Epoch training related functions
    //void runTrainingEpoch(const std::vector<DataEntry*> &set);
    void runTrainingEpoch(const std::vector<DataSegment> &set);
    void updateWeights();


    double sigmoidFunction(double x);
    double sigmoidPrimeFunction(double x);


    double calculateOutputErrorGradient(double target, double actual);
    double calculateHiddenErrorGradient(int layer, int index);

    int roundOutput(double output);

    //Non training set related functions
    double getSetAccuracy(const std::vector<DataSegment> &set);
    double getSetError(const std::vector<DataSegment> &set);

    double getGaussianNoise();


signals:
    void signNetworkEpochComplete(int id, int epoch, double trainingError, double trainingAccuracy,
                                  double testingError, double testingAccuracy);
    void signNetworkConsoleOutput(const QString &message);
    void signNetworkTrainingComplete();

};

#endif // NETWORK_H
