#ifndef SAVEINNERPRODUCTPARAMS_H
#define SAVEINNERPRODUCTPARAMS_H

#include "LayerParamsConverter.h"

class InnerProductParamsConverter : public LayerParamsConverter
{
public:
    InnerProductParamsConverter(shared_ptr<Layer<float> > l,bool transInput,const vector<int> shape);
    void saveParams(FileManager *&fileManager,int layer_id);
private:
    bool transInput;
    const vector<int> shape;
};

#endif // SAVEINNERPRODUCTPARAMS_H
