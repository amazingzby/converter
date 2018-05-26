#ifndef SAVERELUPARAM_H
#define SAVERELUPARAM_H

#include "LayerParamsConverter.h"

class ReluParamsConverter : public LayerParamsConverter
{
public:
    ReluParamsConverter(shared_ptr<Layer<float> > l);
    void saveParams(FileManager *&fileManager,int layer_id);
};

#endif // SAVERELUPARAM_H
