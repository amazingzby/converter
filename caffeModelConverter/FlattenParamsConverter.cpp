#include "FlattenParamsConverter.h"

FlattenParamsConverter::FlattenParamsConverter(shared_ptr<Layer<float> > l):LayerParamsConverter(l)
{
    LOG_IF(ERROR,strcmp(layer->type(), "Flatten") != 0)<<"The layer type should be Flatten!\n";
}
void FlattenParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    const FlattenParameter& param =  layer->layer_param().flatten_param();
    const int start_axis = param.axis();
    const int end_axis = param.end_axis();
    assert(start_axis == 1 && end_axis == -1);
    LOG_IF(ERROR,start_axis != 1 && end_axis != -1) <<"The start_axis should be 1 and the end_axis should be -1.\n";
    unsigned int nextOffset = 4 + 4;
    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);
    unsigned char type = kOpFlatten;
    fileManager->write(&type, 1, 1);
    fileManager->write(align_pads,1, 1);

}
