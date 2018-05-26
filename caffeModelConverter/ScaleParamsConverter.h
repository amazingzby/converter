#ifndef SAVESCALEPARAMS_H
#define SAVESCALEPARAMS_H

#include "LayerParamsConverter.h"

class ScaleParamsConverter : public LayerParamsConverter
{
public:
    ScaleParamsConverter(shared_ptr<Layer<float> > l,vector<Blob<float>*> bottom,set<int> dummySet);
    void saveParams(FileManager *&fileManager,int layer_id);
private:
    vector<Blob<float>*> bottom;
    set<int> dummySet;
    void saveDummyParams(int layer_id,FileManager *&fileManager);
    void saveScaleBody(int layer_id, int scaleDim,bool hasBias,FileManager *&fileManager);
};

#endif // SAVESCALEPARAMS_H
