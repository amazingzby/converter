#ifndef SAVEPERMUTESSDPARAMS_H
#define SAVEPERMUTESSDPARAMS_H

#include "LayerParamsConverter.h"

class PermuteSSDParamsConverter : public LayerParamsConverter
{
public:
    PermuteSSDParamsConverter(shared_ptr<Layer<float> > l,vector<Blob<float>*> bottom);
    void saveParams(FileManager *&fileManager,int layer_id);
private:
    vector<Blob<float>*> bottom;
    void saveDummyParams(int layer_id,FileManager *&fileManager);
};

#endif // SAVEPERMUTESSDPARAMS_H
