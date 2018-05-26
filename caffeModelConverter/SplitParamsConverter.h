#ifndef SAVESPLITPARAMS_H
#define SAVESPLITPARAMS_H

#include "LayerParamsConverter.h"

class SplitParamsConverter : public LayerParamsConverter
{
public:
    SplitParamsConverter(shared_ptr<Layer<float> > l);
    void saveParams(FileManager *&fileManager,int layer_id);
};

#endif // SAVESPLITPARAMS_H
