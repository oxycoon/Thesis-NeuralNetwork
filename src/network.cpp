#include "../include/network.h"

#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdlib>

//================================================
//              Constructor/Destructor
//================================================

Network::Network(int in, int hidden, int out): _countInput(in), _countHidden(hidden), _countOutput(out),
    _trainingSetAccuracy(0), _testingSetAccuracy(0), _validationSetAccuracy(0),
    _trainingSetError(0), _testingSetError(100), _validationSetError(0), _epoch(0)
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
void Network::runTraining(const std::vector<DataEntry*> &trainingSet, const std::vector<DataEntry*> &testSet, const std::vector<DataEntry*> &validationSet)
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
        /*if(oldTSMSE < _testingSetError)
        {
            std::cout << "TESTING SET ERROR INCREASING! STOPPING!" << std::endl;
            break;
        }*/
    }
    //std::cout << "Epochs ran: " << _epoch << std::endl;

    //Run validation set
    _validationSetAccuracy = getSetAccuracy(validationSet);
    _validationSetError = getSetMSE(validationSet);

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
    _hidden = std::vector<Neuron*>(_countHidden + 1);
    _output = std::vector<Neuron*>(_countOutput + 1);

    for(int i = 0; i <= _countInput; i++)
    {
        _input[i] = new Neuron();
        _input[i]->setValue(0.0);
    }

    //Last vector is bias vector
    _input[_countInput]->setValue(-1.0);

    for(int i = 0; i <= _countHidden; i++)
    {
        _hidden[i] = new Neuron();
        _hidden[i]->setValue(0.0);
    }

    //Last vector is bias vector
    _hidden[_countHidden]->setValue(-1.0);

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
        _input[i]->initializeWeights(_countHidden);
    }
    for(int i = 0; i <= _countHidden; i++)
    {
        _hidden[i]->initializeWeights(_countOutput);
    }
}

void Network::setupDeltas()
{
    for(int i = 0; i <= _countInput; i++)
    {
        _input[i]->initializeDeltas(_countHidden);
    }
    for(int i = 0; i <= _countHidden; i++)
    {
        _hidden[i]->initializeDeltas(_countOutput);
    }
}

void Network::setupErrorGradients()
{
    _hiddenErrorGradient = std::vector<double>(_countHidden + 1);
    for(int i = 0; i <= _countHidden; i++)
    {
        _hiddenErrorGradient[i] = 0;
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
        for(int j = 0; j < _countHidden; j++)
        {
            //Random weights
            double random = (double)std::rand() / (RAND_MAX);
            _input[i]->setWeight(j, ( random - 0.5) ) ;
            _input[i]->setDelta(j, 0.0);
        }
    }

    //Weights and deltas between hidden and output layer
    for(int i = 0; i <= _countHidden; i++)
    {
        for(int j = 0; j < _countOutput; j++)
        {
            //Random weights
            _hidden[i]->setWeight(j, ( (double)std::rand() / (RAND_MAX + 1) + 0.5) ) ;
            _hidden[i]->setDelta(j, 0.0);
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
void Network::runTrainingEpoch(const std::vector<DataEntry*> &set)
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

    //Calculates the hidden layer
    for(int i = 0; i < _countHidden; i++)
    {
        //Resets value
        _hidden[i]->setValue(0.0);

        //Calculate weighted sum of inputs, including bias neuron
        for(int j = 0; j <= _countInput; j++)
        {
            _hidden[i]->addToValue(_input[j]->getValue() * _input[j]->getWeight(i));
        }
        //Set to sigmoid result
        _hidden[i]->setValue(activationFunction(_hidden[i]->getValue()));
    }

    //Calculates the output layer
    for(int i = 0; i < _countOutput; i++)
    {
        //Resets value
        _output[i]->setValue(0.0);

        //Calculate weighted sum of inputs, including bias neuron
        for(int j = 0; j <= _countHidden; j++)
        {
            _output[i]->addToValue(_hidden[j]->getValue() * _hidden[j]->getWeight(i));
        }
        //Set to sigmoid result
        _output[i]->setValue(activationFunction(_output[i]->getValue()));
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

        for(int j = 0; j < _countHidden; j++)
        {
            if(!_useBatch)
            {
                _hidden[j]->setDelta(i, _learningRate * _hidden[j]->getValue() *
                                     _outputErrorGradient[i] + _momentum * _hidden[j]->getDelta(i));
            }
            else
            {
                _hidden[j]->addToDelta(i, _learningRate * _hidden[j]->getValue() *
                                       _outputErrorGradient[i]);
            }
        }
    }
    //Modify deltas between input and hidden
    for(int i = 0; i < _countHidden; i++)
    {
        _hiddenErrorGradient[i] = calculateHiddenErrorGradient(i);

        for(int j = 0; j < _countInput; j++)
        {
            if(!_useBatch)
            {
                _input[j]->setDelta(i, _learningRate * _input[j]->getValue() *
                                    _hiddenErrorGradient[i] + _momentum * _input[j]->getDelta(i));
            }
            else
            {
                _input[j]->addToDelta(i, _learningRate * _input[j]->getValue() *
                                      _hiddenErrorGradient[i]);
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
        for(int j = 0; j < _countHidden; j++)
        {
            _input[i]->addToWeight(j, _input[i]->getDelta(j));

            //Clear delta if batch is being used
            if(_useBatch)
            {
                _input[i]->setDelta(j, 0.0);
            }
        }
    }

    //Hidden to output weights
    for(int i = 0; i < _countHidden; i++)
    {
        for(int j = 0; j < _countOutput; j++)
        {
            _hidden[i]->addToWeight(j, _hidden[i]->getDelta(j));

            //Clear delta if batch is being used
            if(_useBatch)
            {
                _hidden[i]->setDelta(j, 0.0);
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
double Network::calculateHiddenErrorGradient(int index)
{
    double weightSum = 0;

    for(int i = 0; i < _countOutput; i++)
    {
        weightSum += _hidden[index]->getWeight(i) * _outputErrorGradient[i];
    }

    return _hidden[index]->getValue() * (1 - _hidden[index]->getValue()) * weightSum;
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
double Network::getSetAccuracy(const std::vector<DataEntry *> &set)
{
    double errors = 0;

    for(int i = 0; i < (int)set.size(); i++)
    {
        feedForward(set[i]->_pattern);

        bool isCorrect = true;

        for(int j = 0; j < _countOutput; j++)
        {
            if(roundOutput( _output[j]->getValue() ) != set[i]->_target[j] )
                isCorrect = false;
        }
        if(!isCorrect)
            errors++;
    }
    return 100.0 - (errors/set.size() * 100.0);
}

/**
 * @brief Network::getSetMSE
 * @param set
 * @return
 *
 *  Calculates the set's Mean Squared Error
 */
double Network::getSetMSE(const std::vector<DataEntry *> &set)
{
    double mse = 0;

    for(int i = 0; i < (int)set.size(); i++)
    {
        feedForward(set[i]->_pattern);

        for(int j = 0; j < _countOutput; j++)
        {
            mse += std::pow( (_output[j]->getValue() - set[i]->_target[j]), 2 );
        }
    }

    return mse / ( _countOutput * set.size() );
}
