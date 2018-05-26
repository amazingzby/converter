#ifndef SAVECONCATPARAMS_H
#define SAVECONCATPARAMS_H
#include "LayerParamsConverter.h"

class ConcatParamsConverter : public LayerParamsConverter
{
public:
    ConcatParamsConverter(shared_ptr<Layer<float> > l);
    void saveParams(FileManager *&fileManager,int layer_id);
};

#endif // SAVECONCATPARAMS_H
