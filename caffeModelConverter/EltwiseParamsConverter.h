#ifndef SAVEELTWISEPARAMS_H
#define SAVEELTWISEPARAMS_H

#include "LayerParamsConverter.h"

class EltwiseParamsConverter : public LayerParamsConverter
{
public:
    EltwiseParamsConverter(shared_ptr<Layer<float> > l);
    void saveParams(FileManager *&fileManager, int layer_id);
};

#endif // SAVEELTWISEPARAMS_H
