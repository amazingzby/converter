#ifndef SAVESLICEPARAMS_H
#define SAVESLICEPARAMS_H

#include "LayerParamsConverter.h"

class SliceParamsConverter : public LayerParamsConverter
{
public:
    SliceParamsConverter(shared_ptr<Layer<float> > l,vector<Blob<float>*> bottom);
    void saveParams(FileManager *&fileManager, int layer_id);
private:
    vector<Blob<float>*> bottom;
};

#endif // SAVESLICEPARAMS_H
