#include "EltwiseParamsConverter.h"

EltwiseParamsConverter::EltwiseParamsConverter(shared_ptr<Layer<float> > l)
        :LayerParamsConverter(l)
{
    LOG_IF(ERROR,strcmp(layer->type(), "Eltwise") != 0)<<"The layer type should be Eltwise\n";
}
void EltwiseParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    int opType = layer->layer_param().eltwise_param().operation();
    bool condition=opType == EltwiseParameter_EltwiseOp_SUM || layer->layer_param().eltwise_param().coeff_size() != 0;
    LOG_IF(ERROR,condition)<<"The operation type is sum or the coff_size is not zero!\n";
    unsigned int nextOffset = 4 + 4;
    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);

    unsigned char type = kOpSum;
    switch (opType) {
    case EltwiseParameter_EltwiseOp_PROD:
        type=kOpProd;
        break;
    case EltwiseParameter_EltwiseOp_MAX:
        type=kOpMax;
        break;
    default:
        break;
    }
    fileManager->write(&type, 1, 1);
    fileManager->write(align_pads,1, 1);
}
