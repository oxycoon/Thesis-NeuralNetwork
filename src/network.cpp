#include "include/network.h"

#include "include/datasegment.h"
#include "include/dataresults.h"
#include "include/filewriter.h"
#include "include/cost/quadraticcost.h"

#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdlib>


//================================================
//              Constructor/Destructor
//================================================


Network::Network()
{
    //Network(1,1,1,new QuadraticCost(), DataType::UK);
}

Network::Network(int in, int out, int hidden, DataType networkType, Cost* cost, std::string name)
{

}

Network::Network(std::vector<Network *> inputs, std::vector<int> hidden, int output, Cost* cost, std::string name):
    _countHidden(hidden), _countOutput(output), _trainingSetAccuracy(0),
    _testingSetAccuracy(0), _trainingSetError(0), _testingSetError(100),
    _epoch(0), _numHiddenLayers(hidden.size()), _networkType(DataType::UK),
    _networkName(name), _costCalculator(cost)
{
    int inputCount = 0;
    for(int i = 0; i < inputs.size(); i++)
    {
        inputCount += inputs[i]->getOutputCount();
    }
    _countInput = inputCount;
    int tempIterator = 0;
    _subNetworks = inputs;

    setupNeurons();
    for(int i = 0; i < inputs.size(); i++)
    {
        modifyInputs(inputs[i]->getOutputNeurons(), tempIterator);
    }
    setupWeights();
    setupDeltas();
    setupErrorGradients();
    initWeights();

    _maxEpochs = MAX_EPOCHS;
    _targetAccuracy = TARGET_ACCURACY;
    _learningRate = LEARNING_RATE;
    _momentum = MOMENTUM;

    _trainSubnetsFirst = true;
    _useBatch = false;
    _distribution = std::normal_distribution<double>(GAUSSIAN_MEAN, GAUSSIAN_DEVIATON);

    std::cout << "Network ready for use!" << std::endl;
}

Network::Network(int in, int out, std::vector<int> hidden, Cost* cost, DataType networkType, std::string name):
    _countInput(in), _countHidden(hidden), _countOutput(out), _networkType(networkType),
    _trainingSetAccuracy(0), _testingSetAccuracy(0), _trainingSetError(0), _testingSetError(100),
    _epoch(0), _numHiddenLayers(hidden.size()), _networkName(name), _costCalculator(cost)
{
    setupNeurons();
    setupWeights();
    setupDeltas();
    setupErrorGradients();
    initWeights();

    _maxEpochs = MAX_EPOCHS;
    _targetAccuracy = TARGET_ACCURACY;
    _learningRate = LEARNING_RATE;
    _momentum = MOMENTUM;

    _useBatch = false;

    _distribution = std::normal_distribution<double>(GAUSSIAN_MEAN, GAUSSIAN_DEVIATON);

    std::cout << "Network ready for use!" << std::endl;
}


Network::~Network()
{
    std::cout << "DESTRUCTOR CALLED" << std::endl;
    for(auto it = _input.begin(); it != _input.end(); it++)
    {
        delete *it;
    }
    _input.clear();

    for(int i = 0; i < _numHiddenLayers; i++)
    {
        for(auto it = _hidden[i].begin(); it != _hidden[i].end(); it++)
        {
            delete *it;
        }
        _hidden[i].clear();
    }
    _hidden.clear();

    for(auto it = _output.begin(); it != _output.end(); it++)
    {
        delete *it;
    }
    _output.clear();

    _hiddenErrorGradient.clear();
    _outputErrorGradient.clear();
    delete _costCalculator;
}

//================================================
//                  Setters
//================================================

void Network::setLearningParameters(double learningRate, double momentum)
{
    _learningRate = learningRate;
    _momentum = momentum;
}

void Network::setMaxEpochs(unsigned int max)
{
    _maxEpochs = max;
}

void Network::setTargetAccuracy(double target)
{
    _targetAccuracy = target;
}

void Network::useBatchLearning()
{
    _useBatch = true;
}

void Network::useStochasticLearning()
{
    _useBatch = false;
}

//================================================
//                  Getters
//================================================

std::vector<Neuron *> Network::getOutputNeurons() const
{
    return _output;
}

std::vector<double> Network::getOutputResults() const
{
    std::vector<double> results;

    for(int i = 0; i < _countOutput; i++)
    {
        results.push_back(_output[i]->getValue());
    }

    return results;
}

int Network::getOutputCount() const
{
    return _countOutput;
}

DataType Network::getNetworkType() const
{
    return _networkType;
}

//================================================
//                  Public functions
//================================================

/**
 * @brief Network::resetNetwork
 *
 *  Resets the network's training.
 */
void Network::resetNetwork()
{
    initWeights();
}

/**
 * @brief Network::runTraining
 * @param trainingSet
 * @param testSet
 * @param validationSet
 *
 *  Trains the network with the given training sets.
 */
/*void Network::runTraining(const std::vector<DataEntry*> &trainingSet, const std::vector<DataEntry*> &testSet, const std::vector<DataEntry*> &validationSet)
{
    std::cout << "Neural network training starting " << std::endl
              << "======================================================================" << std::endl
              << "Learning rate: " << _learningRate << ", Momentum: " << _momentum << ", Max epochs: " << _maxEpochs
                    << ", Target accuracy: " << _targetAccuracy << "%" << std::endl
              << "Input: " << _countInput << ", Hidden: " << _countHidden << ", Output: " << _countOutput << std::endl
              << "======================================================================" << std::endl;

    _epoch = 0;

    //Runs training using training set for training and generalized set for testing
    while((_trainingSetAccuracy < _targetAccuracy || _testingSetAccuracy < _targetAccuracy) && _epoch < _maxEpochs)
    {
        //std::cout << "New epoch, epoch #" << _epoch << std::endl;

        double oldTrA = _trainingSetAccuracy;
        double oldTSA = _testingSetAccuracy;
        double oldTSMSE = _testingSetError;

        //Train the network with the training set
        runTrainingEpoch(trainingSet);

        //Gets the generalized set accuracy and MSE
        _testingSetAccuracy = getSetAccuracy(testSet);
        _testingSetError = getSetMSE(testSet);

        //Checks for changes in the training and generalization set's accuracy, prints if there's a change
        if(std::ceil(oldTrA) != std::ceil(_trainingSetAccuracy) || std::ceil(oldTSA) != std::ceil(_testingSetAccuracy) )
        {
            std::cout << "Epoch: " << _epoch;
            std::cout << " | Training set accuracy: " << _trainingSetAccuracy << "%, MSE: " << _trainingSetError;
            std::cout << " | Generalized set accuracy: " << _testingSetAccuracy << "%, MSE: " << _testingSetError << std::endl;
        }
        //Increases epoch for next iteration.
        _epoch++;

        //Stops the training set if the generalization set's error starts increasing.
        if(oldTSMSE < _testingSetError)
        {
            std::cout << "TESTING SET ERROR INCREASING! STOPPING!" << std::endl;
            break;
        }
    }
    //std::cout << "Epochs ran: " << _epoch << std::endl;

    //Run validation set
    _validationSetAccuracy = getSetAccuracy(validationSet);
    _validationSetError = getSetMSE(validationSet);

    std::cout << std::endl << "Training Complete!!! - > Elapsed Epochs: " << _epoch << std::endl;
    std::cout << " Validation Set Accuracy: " << _validationSetAccuracy << std::endl;
    std::cout << " Validation Set MSE: " << _validationSetError << std::endl << std::endl;
    std::cout << "Closing system." << std::endl;
}*/

void Network::runTraining(const DataCollection &set)
{
    /*std::cout << "Neural network training starting " << std::endl
              << "======================================================================" << std::endl
              << "Learning rate: " << _learningRate << ", Momentum: " << _momentum << ", Max epochs: " << _maxEpochs
                    << ", Target accuracy: " << _targetAccuracy << "%" << std::endl
              << "Input: " << _countInput << ", Hidden: " << _countHidden << ", Output: " << _countOutput << std::endl
              << "======================================================================" << std::endl;*/
    DataResults results;
    _epoch = 0;
    if(_trainSubnetsFirst && _subNetworks.size() > 0)
    {
        for(int i = 0; i < _subNetworks.size(); i++)
        {
            _subNetworks[i]->runTraining(set);
        }
    }

    //Runs training using training set for training and generalized set for testing
    while((_trainingSetAccuracy < _targetAccuracy || _testingSetAccuracy < _targetAccuracy) && _epoch < _maxEpochs)
    {
        //std::cout << "New epoch, epoch #" << _epoch << std::endl;

        double oldTrA = _trainingSetAccuracy;
        double oldTSA = _testingSetAccuracy;
        double oldTSMSE = _testingSetError;

        //Train the network with the training set
        runTrainingEpoch(set.getTrainingSet());

        //Gets the generalized set accuracy and MSE
        _testingSetAccuracy = getSetAccuracy(set.getTestSet());
        _testingSetError = getSetError(set.getTestSet());

        results.addResult(ResultType::TMSE, _trainingSetError);
        results.addResult(ResultType::TA, _trainingSetAccuracy);
        results.addResult(ResultType::VMSE, _testingSetError);
        results.addResult(ResultType::VA, _testingSetAccuracy);

        //Checks for changes in the training and generalization set's accuracy, prints if there's a change
        if(PRINT_EPOCH_DATA)
        {
            if(PRINT_EPOCH_DATA_ON_UPDATE_ONLY)
            {
                if((std::ceil(oldTrA) != std::ceil(_trainingSetAccuracy) || std::ceil(oldTSA) != std::ceil(_testingSetAccuracy)))
                {
                    std::cout << "Epoch: " << _epoch;
                    std::cout << " | Training set accuracy: " << _trainingSetAccuracy << "%, MSE: " << _trainingSetError;
                    std::cout << " | Generalized set accuracy: " << _testingSetAccuracy << "%, MSE: " << _testingSetError << std::endl;
                }
            }
            else
            {
                std::cout << "Epoch: " << _epoch;
                std::cout << " | Training set accuracy: " << _trainingSetAccuracy << "%, MSE: " << _trainingSetError;
                std::cout << " | Generalized set accuracy: " << _testingSetAccuracy << "%, MSE: " << _testingSetError << std::endl;
            }
        }
        //Increases epoch for next iteration.
         _epoch++;

        //Stops the training set if the generalization set's error starts increasing.
        /*if(oldTSMSE < _testingSetError)
        {
            std::cout << "TESTING SET ERROR INCREASING! STOPPING!" << std::endl;
            break;
        }*/
    }
    //std::cout << "Epochs ran: " << _epoch << std::endl;

    //Run validation set
    /*_validationSetAccuracy = getSetAccuracy(set.getValidationSet());
    _validationSetError = getSetMSE(set.getValidationSet());*/

    if(WRITE_RESULTS_TO_FILE)
    {
        std::string name = _networkName + "_result.csv";
        FileWriter writer;
        writer.writeFile(name, results.toString());
    }

    std::cout << std::endl << "Training Complete!!! - > Elapsed Epochs: " << _epoch << std::endl;
    /*std::cout << " Validation Set Accuracy: " << _validationSetAccuracy << std::endl;
    std::cout << " Validation Set MSE: " << _validationSetError << std::endl << std::endl;
    std::cout << "Closing system." << std::endl;*/
}

//================================================
//                  Initializers
//================================================

void Network::setupNeurons()
{
    _input = std::vector<Neuron*>(_countInput + 1);
    _hidden = std::vector<std::vector<Neuron*>>(_countHidden.size());
    for(int i = 0; i < _countHidden.size(); i++)
    {
        _hidden[i] = std::vector<Neuron*>(_countHidden[i] + 1);
    }
    _output = std::vector<Neuron*>(_countOutput);

    for(int i = 0; i <= _countInput; i++)
    {
        _input[i] = new Neuron();
        _input[i]->setValue(0.0);
    }

    //Last vector is bias vector
    _input[_countInput]->setValue(-1.0);

    for(int j = 0; j < _numHiddenLayers; j++)
    {
        for(int i = 0; i <= _countHidden[j]; i++)
        {
            _hidden[j][i] = new Neuron();
            _hidden[j][i]->setValue(0.0);
        }
        //Last vector is bias vector
        _hidden[j][_countHidden[j]]->setValue(-1.0);
    }

    for(int i = 0; i < _countOutput; i++)
    {
        _output[i] = new Neuron();
        _output[i]->setValue(0.0);
    }
}

void Network::setupWeights()
{
    for(int i = 0; i <= _countInput; i++)
    {
        _input[i]->initializeWeights(_countHidden[0]);
    }
    for(int j = 0; j < _numHiddenLayers; j++)
    {
        for(int i = 0; i <= _countHidden[j]; i++)
        {
            if(j == _numHiddenLayers - 1)
                _hidden[j][i]->initializeWeights(_countOutput);
            else
                _hidden[j][i]->initializeWeights(_countHidden[j+1]);
        }
    }
}

void Network::setupDeltas()
{
    for(int i = 0; i <= _countInput; i++)
    {
        _input[i]->initializeDeltas(_countHidden[0]);
    }
    for(int j = 0; j < _numHiddenLayers; j++)
    {
        for(int i = 0; i <= _countHidden[j]; i++)
        {
            //_hidden[j][i]->initializeDeltas(_countOutput);
            if(j == _numHiddenLayers - 1)
                _hidden[j][i]->initializeDeltas(_countOutput);
            else
                _hidden[j][i]->initializeDeltas(_countHidden[j]);
        }
    }
}

void Network::setupErrorGradients()
{
    _hiddenErrorGradient = std::vector<std::vector<double>>(_numHiddenLayers);
    for(int i = 0; i < _numHiddenLayers; i++)
    {
        _hiddenErrorGradient[i] = std::vector<double>(_countHidden[i] + 1);
    }

    for(int j = 0; j < _numHiddenLayers; j++)
    {
        for(int i = 0; i <= _countHidden[j]; i++)
        {
            _hiddenErrorGradient[j][i] = 0;
        }
    }


    _outputErrorGradient = std::vector<double>(_countOutput + 1);
    for(int i = 0; i <= _countOutput; i++)
    {
        _outputErrorGradient[i] = 0;
    }
}

/**
 * @brief Network::initWeights
 *
 *  Initializes weights with random values between -0.5 and 0.5, and deltas to 0.
 */
void Network::initWeights()
{
    srand( (unsigned int) std::time(0) );

    //Weights and deltas between input and hidden layer
    for(int i = 0; i <= _countInput; i++)
    {
        for(int j = 0; j < _countHidden[0]; j++)
        {
            //Random weights
            double random = (double)std::rand() / (RAND_MAX);
            _input[i]->setWeight(j, ( random - 0.5) ) ;
            _input[i]->setDelta(j, 0.0);
        }
    }

    //Weights and deltas between hidden layers if there are more than 1 hidden
    //layeres
    if(_numHiddenLayers > 1)
    {
        for(int i = 0; i < _numHiddenLayers-1; i++)
        {
            for(int j = 0; j <= _countHidden[i]; j++)
            {
                for(int k = 0; k < _countHidden[i+1]; k++)
                {
                    _hidden[i][j]->setWeight(k, ( (double)std::rand() / (RAND_MAX + 1) - 0.5) );
                    _hidden[i][j]->setDelta(k, 0.0);
                }
            }
        }
    }

    //Weights and deltas between hidden and output layer
    for(int i = 0; i <= _countHidden[_numHiddenLayers-1]; i++)
    {
        for(int j = 0; j < _countOutput; j++)
        {
            //Random weights
            _hidden[_numHiddenLayers-1][i]->setWeight(j, ( (double)std::rand() / (RAND_MAX + 1) + 0.5) ) ;
            _hidden[_numHiddenLayers-1][i]->setDelta(j, 0.0);
        }
    }
}

/**
 * @brief Network::modifyInputs
 * @param inputs
 * @param startIndex
 */
void Network::modifyInputs(std::vector<Neuron *> inputs, int &startIndex)
{
    for(int i = 0; i < inputs.size(); i++)
    {
        _input[startIndex] = inputs[i];
        startIndex++;
    }
}

//================================================
//            Epoch training functions
//================================================

/**
 * @brief Network::runTrainingEpoch
 * @param set
 *
 *  Runs a training epoch on the given set.
 */
void Network::runTrainingEpoch(const std::vector<DataSegment> &set)
{
    double incorrectPatterns = 0;
    double meanSquaredError = 0;

    //Runs training for every pattern
    for(int i = 0; i < set.size(); i++)
    {
        std::vector<double> inputDataVector;

        if(_networkType != DataType::UK)
        {
            inputDataVector = set[i].getDataOfType(_networkType, true);
            /*std::vector<double> tempDataEntryVector = set[i].getDataOfType(_networkType);
            if(_networkType == DataType::ACCELEROMETER)
            {
                inputDataVector.push_back(set[i].getTotalAccelerometer());
            }
            else if(_networkType == DataType::GYRO)
            {
                inputDataVector.push_back(set[i].getTotalGyroscope());

            }
            else if(_networkType == DataType::COMPASS)
            {
                inputDataVector.push_back(set[i].getTotalMagnetometer());
            }
            else if(_networkType == DataType::BAROMETER)
            {
                inputDataVector.push_back(set[i].getTotalBarometer());
            }

            for(int j = 0; j < tempDataEntryVector.size(); j++)
            {
                inputDataVector.push_back(tempDataEntryVector[j]);
            }*/
        }
        else
        {
            if(_subNetworks.size() > 0 && _trainSubnetsFirst)
            {
                for(int j = 0; j < _subNetworks.size(); j++)
                {
                    _subNetworks[j]->feedForward(set[i].getDataOfType(_subNetworks[j]->_networkType, true));
                    std::vector<double> tmp = _subNetworks[j]->getOutputResults();
                    for(int k = 0; k < tmp.size(); k++)
                    {
                        inputDataVector.push_back(tmp[k]);
                    }
                }
            }
        }
        feedForward(inputDataVector);
        feedBackward(set[i].getTargets());

        bool patternCorrect = true;

        for(int j = 0; j < _countOutput; j++)
        {
            //Checks if the output value matches the target
            if(PRINT_TRAINING_DATA)
            {
                std::cout << "Training set #" << i << ", output #" << j <<" - Target: " <<
                         set[i].getTargets()[j] << " | Rounded: " << roundOutput(_output[j]->getValue()) <<
                         "| Pure: " << _output[j]->getValue() << std::endl;
            }

            if(roundOutput(_output[j]->getValue() ) != set[i].getTarget(j) )
            {
                patternCorrect = false;
            }

            //Calculates error
            meanSquaredError += _costCalculator->calculateCost(_output[j]->getValue(), set[i].getTarget(j));
        }
        if(!patternCorrect)
        {
            incorrectPatterns++;
        }

    }
    //Updates weights here if batch learning is used.
    if(_useBatch)
    {
        updateWeights();
    }

    //update training accuracy and MSE
    _trainingSetAccuracy = 100 - (incorrectPatterns / set.size() * 100);
    _trainingSetError = meanSquaredError / (_countOutput * set.size());
}

/**
 * @brief Network::feedForward
 * @param input
 *
 *  Feeds the input forward for an output
 */
void Network::feedForward(std::vector<double> input)
{
    //Sets input neurons to input values
    for(int i = 0; i < _countInput; i++)
    {
        if(USE_GAUSSIAN_NOISE)
        {
            double gausNoise = input[i] * getGaussianNoise(GAUSSIAN_MEAN, GAUSSIAN_DEVIATON);
            _input[i]->setValue(input[i] + gausNoise);
        }
        else
        {
            _input[i]->setValue(input[i]);
        }
    }

    //Calculates the hidden layers
    for(int k = 0; k < _numHiddenLayers; k++)
    {
        for(int i = 0; i < _countHidden[k]; i++)
        {
            //Resets value
            _hidden[k][i]->setValue(0.0);

            //Calculate weighted sum of inputs, including bias neuron
            if(k == 0) // k is the first layer
            {
                for(int j = 0; j <= _countInput; j++)
                {
                    _hidden[k][i]->addToValue(_input[j]->getValue() * _input[j]->getWeight(i));
                }

            }
            else // k is any other layer
            {
                for(int j = 0; j <= _countHidden[k-1]; j++)
                {
                    _hidden[k][i]->addToValue(_hidden[k-1][j]->getValue() * _hidden[k-1][j]->getWeight(i));
                }
            }
            //Set to sigmoid result
            _hidden[k][i]->setValue(sigmoidFunction(_hidden[k][i]->getValue()));
        }
    }

    //Calculates the output layer
    for(int i = 0; i < _countOutput; i++)
    {
        //Resets value
        _output[i]->setValue(0.0);

        //Calculate weighted sum of inputs, including bias neuron
        for(int j = 0; j <= _countHidden[_numHiddenLayers-1]; j++)
        {
            _output[i]->addToValue(_hidden[_numHiddenLayers-1][j]->getValue() * _hidden[_numHiddenLayers-1][j]->getWeight(i));
        }
        //Set to sigmoid result
        _output[i]->setValue(sigmoidFunction(_output[i]->getValue()));
    }
}

/**
 * @brief Network::feedBackward
 * @param targets The desired values for the training sets.
 *
 *  Feeds backwards, adjust deltas and calculate the error gradients.
 */
void Network::feedBackward(std::vector<double> targets)
{
    //Modify deltas between hidden and output
    for(int i = 0; i < _countOutput; i++)
    {
        _outputErrorGradient[i] = calculateOutputErrorGradient(targets[i], _output[i]->getValue());
        for(int j = 0; j < _countHidden[_numHiddenLayers-1]; j++)
        {
            if(!_useBatch)
            {
                _hidden[_numHiddenLayers-1][j]->setDelta(i, _learningRate * _hidden[_numHiddenLayers-1][j]->getValue() *
                                     _outputErrorGradient[i] + _momentum * _hidden[_numHiddenLayers-1][j]->getDelta(i));
            }
            else
            {
                _hidden[_numHiddenLayers-1][j]->addToDelta(i, _learningRate * _hidden[_numHiddenLayers-1][j]->getValue() *
                                           _outputErrorGradient[i]);
            }
        }
    }
    //Modify deltas between hidden layers
    if(_numHiddenLayers > 1)
    {
        for(int i = _numHiddenLayers-1; i >= 1; i--)
        {
            for(int j = 0; j < _countHidden[i]; j++)
            {
                _hiddenErrorGradient[i][j] = calculateHiddenErrorGradient(i,j);

                for(int k = 0; k < _countHidden[i-1]; k++)
                {
                    if(!_useBatch)
                    {
                        _hidden[i-1][k]->setDelta(j, _learningRate * _hidden[i-1][k]->getValue() *
                                                _hiddenErrorGradient[i][j] + _momentum * _hidden[i-1][k]->getDelta(j));
                    }
                    else
                    {
                        _hidden[i-1][k]->setDelta(j, _learningRate * _hidden[i-1][k]->getValue() *
                                                _hiddenErrorGradient[i][j]);
                    }
                }
            }
        }
    }

    //Modify deltas between input and hidden
    for(int i = 0; i < _countHidden[0]; i++)
    {
        _hiddenErrorGradient[0][i] = calculateHiddenErrorGradient(0, i);

        for(int j = 0; j < _countInput; j++)
        {
            if(!_useBatch)
            {
                _input[j]->setDelta(i, _learningRate * _input[j]->getValue() *
                                    _hiddenErrorGradient[0][i] + _momentum * _input[j]->getDelta(i));
            }
            else
            {
                _input[j]->addToDelta(i, _learningRate * _input[j]->getValue() *
                                      _hiddenErrorGradient[0][i]);
            }
        }

    }

    //If using stochastic learning, update weights now
    if(!_useBatch)
    {
        updateWeights();
    }
}

/**
 * @brief Network::updateWeights
 *
 *  Updates the weights of the network.
 */
void Network::updateWeights()
{
    //Input to hidden weights
    for(int i = 0; i <= _countInput; i++)
    {
        for(int j = 0; j < _countHidden[0]; j++)
        {
            _input[i]->addToWeight(j, _input[i]->getDelta(j));

            //Clear delta if batch is being used
            if(_useBatch)
            {
                _input[i]->setDelta(j, 0.0);
            }
        }
    }

    if(_numHiddenLayers > 1)
    {
        for(int i = 0; i < _numHiddenLayers-1; i++)
        {
            for(int j = 0; j < _countHidden[i]; j++)
            {
                for(int k = 0; k < _countHidden[i+1]; k++)
                {
                    _hidden[i][j]->addToWeight(k, _hidden[i][j]->getDelta(k));
                    if(_useBatch)
                    {
                        _hidden[i][j]->setDelta(k, 0.0);
                    }
                }
            }
        }
    }

    //Hidden to output weights
    for(int i = 0; i < _countHidden[_numHiddenLayers-1]; i++)
    {
        for(int j = 0; j < _countOutput; j++)
        {
            _hidden[_numHiddenLayers-1][i]->addToWeight(j, _hidden[_numHiddenLayers-1][i]->getDelta(j));

            //Clear delta if batch is being used
            if(_useBatch)
            {
                _hidden[_numHiddenLayers-1][i]->setDelta(j, 0.0);
            }
        }
    }
}

/**
 * @brief Network::sigmoidPrimeFunction
 * @param x
 * @return Derivative Sigmoid function
 */
double Network::sigmoidPrimeFunction(double x)
{
    return sigmoidFunction(x)*(1-sigmoidFunction(x));
}

/**
 * @brief Network::sigmoidFunction
 * @param x
 * @return Sigmoid function
 */
double Network::sigmoidFunction(double x)
{
    //Sigmoid function
    return 1 / (1 + std::exp(-x));
    //return x;
}

/**
 * @brief Network::calculateOutputErrorGradient
 * @param target Target value
 * @param actual Actual value
 * @return Error between the target and actual
 *
 *  Calculates the error gradient between the target and actual value.
 *  SigmoidPrime
 */
double Network::calculateOutputErrorGradient(double target, double actual)
{
    //return actual * (1 - actual) * (target - actual);
    return _costCalculator->errorDelta(actual, target);
}

/**
 * @brief Network::calculateHiddenErrorGradient
 * @param index
 * @return
 *
 *  Calculates the error gradient at the hidden node at index between the hidden layer
 *  and input layer.
 */
double Network::calculateHiddenErrorGradient(int layer, int index)
{
    double weightSum = 0;

    if(layer == (_numHiddenLayers - 1)) // calculates between the output layer and hidden layer
    {
        for(int i = 0; i < _countOutput; i++)
        {
            weightSum += _hidden[layer][index]->getWeight(i) * _outputErrorGradient[i];
        }
    }
    else
    {
        for(int i = 0; i < _countHidden[layer+1]; i++)
        {
            weightSum += _hidden[layer][index]->getWeight(i) * _hiddenErrorGradient[layer+1][i];
        }
    }

    return _hidden[layer][index]->getValue() * (1 - _hidden[layer][index]->getValue()) * weightSum;
}

/**
 * @brief Network::roundOutput
 * @param output
 * @return
 *
 *  Rounds the output for a boolean result
 */
int Network::roundOutput(double output)
{
    //TODO PROBABLY SOME PROBLEM HERE
    if(output < 0.1) return 0;//Exercise::WALKING;
    else if(output > 0.9) return 1;//Exercise::UNKNOWN;
    else return -1;
    //std::cout << output << std::endl;
}

//================================================
//        Non training set related functions
//================================================

/**
 * @brief Network::getSetAccuracy
 * @param set
 * @return
 *
 *  Calculates the accuracy of the given set.
 */
double Network::getSetAccuracy(const std::vector<DataSegment> &set)
{
    double errors = 0;

    for(int i = 0; i < (int)set.size(); i++)
    {
        feedForward(set[i].getDataOfType(_networkType, true));

        bool isCorrect = true;

        for(int j = 0; j < _countOutput; j++)
        {
            if(roundOutput( _output[j]->getValue() ) != set[i].getTarget(j) )
                isCorrect = false;
        }
        if(!isCorrect)
            errors++;
    }
    return 100.0 - (errors/set.size() * 100.0);
    //return 0.0;
}

/**
 * @brief Network::getSetMSE
 * @param set
 * @return
 *
 *  Calculates the set's Mean Squared Error
 */
double Network::getSetError(const std::vector<DataSegment> &set)
{
    double mse = 0;

    for(int i = 0; i < set.size(); i++)
    {
        feedForward(set[i].getDataOfType(_networkType, true));
        for(int j = 0; j < _countOutput; j++)
        {
            mse += _costCalculator->calculateCost(_output[j]->getValue(),set[i].getTarget(j));
        }
    }

    return mse / ( _countOutput * set.size() );
    //return 0.0;
}

double Network::getGaussianNoise(double mean, double standardDeviation)
{
    return _distribution(_generator);
}

