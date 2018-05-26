#include "SoftmaxParamsConverter.h"

SoftmaxParamsConverter::SoftmaxParamsConverter(shared_ptr<Layer<float> > l):LayerParamsConverter(l)
{
    LOG_IF(ERROR,strcmp(layer->type(), "Softmax") != 0)<<"The layer type should be Softmax!\n";
}
void SoftmaxParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    unsigned int nextOffset = 4 + 4;
    fileManager->write(&nextOffset,4,1);
    fileManager->write(&layer_id,2,1);

    unsigned char type = kOpSoftMax;
    fileManager->write(&type,1,1);
    unsigned char padding=0;
    fileManager->write(&padding,1,1);
}
