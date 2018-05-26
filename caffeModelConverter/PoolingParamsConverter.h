#ifndef SAVEPOOLINGPARAMS_H
#define SAVEPOOLINGPARAMS_H
#include"LayerParamsConverter.h"

class PoolingParamsConverter:public LayerParamsConverter
{
public:
    PoolingParamsConverter(shared_ptr<Layer<float> > l);
    void saveParams(FileManager *&fileManager,int layer_id);
};

#endif // SAVEPOOLINGPARAMS_H
