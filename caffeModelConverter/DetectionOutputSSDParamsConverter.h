#ifndef SAVEDETECTIONOUTPUTSSDPARAMS_H
#define SAVEDETECTIONOUTPUTSSDPARAMS_H
#include "LayerParamsConverter.h"

class DetectionOutputSSDParamsConverter : public LayerParamsConverter
{
public:
    DetectionOutputSSDParamsConverter(shared_ptr<Layer<float> > l);
    void saveParams(FileManager *&fileManager, int layer_id);
};

#endif // SAVEDETECTIONOUTPUTSSDPARAMS_H
