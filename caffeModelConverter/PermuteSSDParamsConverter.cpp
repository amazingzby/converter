#include "PermuteSSDParamsConverter.h"

PermuteSSDParamsConverter::PermuteSSDParamsConverter(shared_ptr<Layer<float> > l,vector<Blob<float>*> bottom):
    LayerParamsConverter(l),bottom(bottom)
{
    LOG_IF(ERROR,strcmp(layer->type(), "Permute") != 0)<<"The layer type should be Permute!\n";
}
void PermuteSSDParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    const PermuteParameter& permute_param = layer->layer_param().permute_param();
    int order_size = permute_param.order_size();
    LOG_IF(ERROR,order_size!=4)<<"The order_size should be equal to 4!\n";
    int orderDigit=0;
    for(int i = 0; i < order_size; ++i)
    {
        int order = permute_param.order(i);
        orderDigit += (order << (i*4));
    }
    LOG_IF(ERROR,orderDigit != 0x1320)<<"The orderDigit should be equal to 0x1320!\n";
    saveDummyParams(layer_id,fileManager);

}
void PermuteSSDParamsConverter::saveDummyParams(int layer_id, FileManager *&fileManager)
{
    unsigned int nextOffset = 4 + 4;
    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);
    unsigned char type = kOpDummy;
    fileManager->write(&type, 1, 1);
    fileManager->write(align_pads,1, 1);
}
