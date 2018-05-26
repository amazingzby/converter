#ifndef SAVEDECONVPARAMS_H
#define SAVEDECONVPARAMS_H
#include "LayerParamsConverter.h"

class DeconvParamsConverter : public LayerParamsConverter
{
public:
    DeconvParamsConverter(shared_ptr<Layer<float> > l,int bottom_channels);
    void saveParams(FileManager *&fileManager,int layer_id);
private:
    int bottom_channels;
};

#endif // SAVEDECONVPARAMS_H
