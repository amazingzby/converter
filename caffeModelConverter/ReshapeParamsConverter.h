#ifndef SAVERESHAPEPARAMS_H
#define SAVERESHAPEPARAMS_H
#include "LayerParamsConverter.h"

class ReshapeParamsConverter : public LayerParamsConverter
{
public:
    ReshapeParamsConverter(shared_ptr<Layer<float> > l);
    void saveParams(FileManager *&fileManager, int layer_id);
};

#endif // SAVERESHAPEPARAMS_H
