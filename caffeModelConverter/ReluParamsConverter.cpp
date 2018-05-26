#include "ReluParamsConverter.h"

ReluParamsConverter::ReluParamsConverter(shared_ptr<Layer<float> > l):LayerParamsConverter(l)
{
    LOG_IF(ERROR,strcmp(layer->type(), "ReLU") != 0)<<"The layer type should be ReLU\n";
}
void ReluParamsConverter::saveParams(FileManager *&fileManager,int layer_id)
{
    unsigned int nextOffset = 4 + 4 + 4;
    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);
    unsigned char type = kOpRelu;
    fileManager->write(&type, 1, 1);
    fileManager->write(align_pads,1, 1);
    float negative_slope = layer->layer_param().relu_param().negative_slope();
    int count = 1;
    fileManager->write(&count, 2, 1);
    unsigned int slope_f16 = f32Tof16(negative_slope);
    fileManager->write(&slope_f16, 2, 1);

}
