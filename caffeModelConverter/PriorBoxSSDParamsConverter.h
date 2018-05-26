#ifndef SAVEPRIORBOXSSDPARAMS_H
#define SAVEPRIORBOXSSDPARAMS_H

#include "LayerParamsConverter.h"

class PriorBoxSSDParamsConverter : public LayerParamsConverter
{
public:
    PriorBoxSSDParamsConverter(shared_ptr<Layer<float> > l);
    void saveParams(FileManager *&fileManager,int layer_id);
};

#endif // SAVEPRIORBOXSSDPARAMS_H
