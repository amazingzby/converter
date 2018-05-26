#include "SplitParamsConverter.h"

SplitParamsConverter::SplitParamsConverter(shared_ptr<Layer<float> > l):LayerParamsConverter(l)
{
    LOG_IF(ERROR,strcmp(layer->type(), "Split") != 0)<<"The layer type should be Split!\n";
}
void SplitParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    unsigned int nextOffset = 4 + 4;
    fileManager->write(&nextOffset,4,1);
    unsigned char type = kOpCaffeSplit;
    fileManager->write(&type,1,1);
    const LayerParameter& layer_param = layer->layer_param();
    unsigned char num_split = (unsigned char)layer_param.top_size();
    fileManager->write(&num_split,1,1);
}
