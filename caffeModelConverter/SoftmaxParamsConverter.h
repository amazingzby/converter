#ifndef SAVESOFTMAXPARAMS_H
#define SAVESOFTMAXPARAMS_H
#include "LayerParamsConverter.h"

class SoftmaxParamsConverter : public LayerParamsConverter
{
public:
    SoftmaxParamsConverter(shared_ptr<Layer<float> > l);
    void saveParams(FileManager *&fileManager,int layer_id);
};

#endif // SAVESOFTMAXPARAMS_H
