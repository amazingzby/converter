#include "ConcatParamsConverter.h"

ConcatParamsConverter::ConcatParamsConverter(shared_ptr<Layer<float> > l):LayerParamsConverter(l)
{
    LOG_IF(ERROR,strcmp(layer->type(), "Concat") != 0)<<"The layer type should be Concat!\n";
}
void ConcatParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    unsigned int nextOffset = 4 + 4;
    fileManager->write(&nextOffset,4,1);
    fileManager->write(&layer_id, 2, 1);
    unsigned char type = kOpConcat;
    fileManager->write(&type, 1, 1);
    fileManager->write(align_pads,1, 1);
}
