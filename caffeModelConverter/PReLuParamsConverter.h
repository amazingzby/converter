#ifndef SAVEPRELUPARAMS_H
#define SAVEPRELUPARAMS_H

#include "LayerParamsConverter.h"

class PReLuParamsConverter : public LayerParamsConverter
{
public:
    PReLuParamsConverter(shared_ptr<Layer<float> > l,int channels_);
    void saveParams(FileManager *&fileManager,int layer_id);
private:
    int channels;
};

#endif // SAVEPRELUPARAMS_H
