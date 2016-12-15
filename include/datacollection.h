#ifndef DATACOLLECTION_H
#define DATACOLLECTION_H



#include "include/dataentry.h"
#include "include/datasegment.h"

#include "include/enum/exercise.h"

#include <QtCore>

#include <vector>

class DataCollection : public QObject
{
    Q_OBJECT
public:
    DataCollection();
    ~DataCollection();

    DataEntry*                  getEntry(const int index) const;
    int                         getCollectionSize() const;
    int                         getTrainingSetSize() const;
    int                         getTestSetSize() const;
    Exercise                    getExercise() const;
    void                        setExercise(const Exercise ex);
    std::vector<DataSegment>    getTestSet() const;
    std::vector<DataSegment>    getTrainingSet() const;
    std::string                 getName() const;

    void                        setName(const std::string name);

    void                        printCollection();

    void                        createTrainingTestSets(int setSize, double trainingSize);
    void                        addToCollection(DataEntry* data);
    //void                        addCollection(DataCollection &collection);

private:
    std::vector<DataEntry*>     _collection;
    std::vector<DataSegment>    _trainingSet;
    std::vector<DataSegment>    _testSet;

    std::string                 _name;

    int                         _trainingSetSize;
    int                         _testSetSize;

    bool                        getDataSegment(int startIndex, int size, DataSegment &output) const;

signals:
    void        signDataCollectionConsoleOutput(const QString &msg);
};

#endif // DATACOLLECTION_H
