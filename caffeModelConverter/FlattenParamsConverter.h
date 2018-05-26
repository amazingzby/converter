#ifndef SAVEFLATTENPARAMS_H
#define SAVEFLATTENPARAMS_H
#include "LayerParamsConverter.h"

class FlattenParamsConverter : public LayerParamsConverter
{
public:
    FlattenParamsConverter(shared_ptr<Layer<float> > l);
    void saveParams(FileManager *&fileManager, int layer_id);
};

#endif // SAVEFLATTENPARAMS_H
