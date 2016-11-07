#include "../include/network.h"

#include "../include/datasegment.h"

#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdlib>

//================================================
//              Constructor/Destructor
//================================================


Network::Network(int in, int out, int hidden, DataType networkType)
{

}

Network::Network(int in, int out, std::vector<int> hidden, DataType networkType = DataType::UK):
    _countInput(in), _countHidden(hidden), _countOutput(out), _networkType(networkType),
    _trainingSetAccuracy(0), _testingSetAccuracy(0), _validationSetAccuracy(0),
    _trainingSetError(0), _testingSetError(100), _validationSetError(0), _epoch(0),
    _numHiddenLayers(hidden.size())
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

    std::cout << "Network ready for use!" << std::endl;
}


Network::~Network()
{
    /*for(auto it = _input.begin(); it != _input.end(); it++)
    {
        delete *it;
    }
    _input.clear();

    for(auto it = _hidden.begin(); it != _hidden.end(); it++)
    {
        delete *it;
    }
    _hidden.clear();

    for(auto it = _output.begin(); it != _output.end(); it++)
    {
        delete *it;
    }
    _output.clear();

    _hiddenErrorGradient.clear();
    _outputErrorGradient.clear();*/

    /*delete[] _hidden;
    delete[] _output;
    delete[] _input;

    delete[] _hiddenErrorGradient;
    delete[] _outputErrorGradient;*/
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

    _epoch = 0;

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
        _testingSetError = getSetMSE(set.getTestSet());

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
        /*if(oldTSMSE < _testingSetError)
        {
            std::cout << "TESTING SET ERROR INCREASING! STOPPING!" << std::endl;
            break;
        }*/
    }
    //std::cout << "Epochs ran: " << _epoch << std::endl;

    //Run validation set
    _validationSetAccuracy = getSetAccuracy(set.getValidationSet());
    _validationSetError = getSetMSE(set.getValidationSet());

    std::cout << std::endl << "Training Complete!!! - > Elapsed Epochs: " << _epoch << std::endl;
    std::cout << " Validation Set Accuracy: " << _validationSetAccuracy << std::endl;
    std::cout << " Validation Set MSE: " << _validationSetError << std::endl << std::endl;
    std::cout << "Closing system." << std::endl;
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
    _output = std::vector<Neuron*>(_countOutput + 1);

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
                _hidden[j][i]->initializeWeights(_countHidden[j]);
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
                    _hidden[i][j]->setWeight(k, ( (double)std::rand() / (RAND_MAX + 1) + 0.5) );
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

//================================================
//            Epoch training functions
//================================================

/**
 * @brief Network::runTrainingEpoch
 * @param set
 *
 *  Runs a training epoch on the given set.
 */
/*void Network::runTrainingEpoch(const std::vector<DataEntry*> &set)
{
    double incorrectPatterns = 0;
    double meanSquaredError = 0;

    //Runs training for every pattern
    for(int i = 0; i < (int)set.size(); i++)
    {
        feedForward(set[i]->_pattern);
        feedBackward(set[i]->_target);

        bool patternCorrect = true;

        for(int j = 0; j < _countOutput; j++)
        {
            //Checks if the output value matches the target
            std::cout << "Training set #" << i << ", output #" << j <<" - Target: " << set[i]->_target[j] << " | Rounded: " << roundOutput(_output[j]->getValue()) <<
                         "| Pure: " << _output[j]->getValue() << std::endl;

            if(roundOutput(_output[j]->getValue() ) != set[i]->_target[j] )
            {
                patternCorrect = false;
            }

            //Calculates mean square error
            meanSquaredError += std::pow((_output[j]->getValue() - set[i]->_target[j]), 2);
        }
        std::cout << std::endl;

        //If pattern does not match, add to sum of incorrect.
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
}*/

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

        }
        feedForward(inputDataVector);
        feedBackward(set[i].getTargets());

        bool patternCorrect = true;

        for(int j = 0; j < _countOutput; j++)
        {
            //Checks if the output value matches the target
            std::cout << "Training set #" << i << ", output #" << j <<" - Target: " <<
                         set[i].getTargets()[j] << " | Rounded: " << roundOutput(_output[j]->getValue()) <<
                         "| Pure: " << _output[j]->getValue() << std::endl;

            if(roundOutput(_output[j]->getValue() ) != set[i].getTarget(j) )
            {
                patternCorrect = false;
            }
            //Calculates mean square error
            meanSquaredError += std::pow((_output[j]->getValue() - set[i].getTarget(j)), 2);
        }


    }
    //Updates weights here if batch learning is used.
    if(_useBatch)
    {
        updateWeights();
    }

    //update training accuracy and MSE
    //_trainingSetAccuracy = 100 - (incorrectPatterns / set.size() * 100);
    //_trainingSetError = meanSquaredError / (_countOutput * set.size());
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
        _input[i]->setValue(input[i]);
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
            _hidden[k][i]->setValue(activationFunction(_hidden[k][i]->getValue()));
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
        _output[i]->setValue(activationFunction(_output[i]->getValue()));
    }
}

/*void Network::feedForward(DataEntry* input)
{
    //Sets inputneuros to input values
    std::vector<double> inputVectors = input->getEntriesOfDataType(_networkType);

    for(int i = 0; i < inputVectors.size(); i++)
    {
        _input[i]->setValue(inputVectors[i]);
    }

    //Calculates hidden layer
    for(int i = 0; i < _countHidden; i++)
    {
        //Reset value
        _hidden[i]->setValue(0.0);

        //Calculate weighted sum of inputs, including bias neuron
        for(int j = 0; j <= _countInput; i++)
        {
            _hidden[i]->addToValue(_input[j]->getValue() * _input[j]->getWeight(i));
        }
        //Set sigmoid result
        _hidden[i]->setValue(activationFunction(_hidden[i]->getValue()));
    }
}*/

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
                        _hidden[i][k]->setDelta(j, _learningRate * _hidden[i][k]->getValue() *
                                                _hiddenErrorGradient[i][j] + _momentum * _hidden[i][j]->getDelta(j));
                    }
                    else
                    {
                        _hidden[i][k]->setDelta(j, _learningRate * _hidden[i][k]->getValue() *
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
 *  Updates thje weights of the network.
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
        //TODO:
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


double Network::activationFunction(double x)
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
 */
double Network::calculateOutputErrorGradient(double target, double actual)
{
    return actual * (1 - actual) * (target - actual);
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
    if(output < 0.1) return 0;
    else if(output > 0.9) return 1;
    else return -1;
    //std::cout << output << std::endl;

    /*if(output < 0.05) return 0; //empty classroom
    else if(output > 0.20 && output < 0.25) return 1;// one person
    else if(output > 0.40 && output < 0.45) return 2;// group
    else if(output > 0.60 && output < 0.5) return 3;// class
    else if(output > 0.95) return 4;// lecture
    else return -1; //unknown*/

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
    return 0.0;
}

/**
 * @brief Network::getSetMSE
 * @param set
 * @return
 *
 *  Calculates the set's Mean Squared Error
 */
double Network::getSetMSE(const std::vector<DataSegment> &set)
{
    double mse = 0;

    for(int i = 0; i < (int)set.size(); i++)
    {
        feedForward(set[i].getDataOfType(_networkType, true));
        for(int j = 0; j < _countOutput; j++)
        {
            mse += std::pow((_output[j]->getValue() - set[i].getTarget(j)), 2 );
        }
    }

    return mse / ( _countOutput * set.size() );
    return 0.0;
}

