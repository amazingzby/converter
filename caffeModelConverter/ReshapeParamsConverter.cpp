#include "ReshapeParamsConverter.h"

ReshapeParamsConverter::ReshapeParamsConverter(shared_ptr<Layer<float> > l):LayerParamsConverter(l)
{
    LOG_IF(ERROR,strcmp(layer->type(), "Reshape") != 0)<<"The layer type should be Reshape!\n";
}
void ReshapeParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    const ReshapeParameter& param =  layer->layer_param().reshape_param();
    int axis = param.axis();
    int num_axes = param.num_axes();

    LOG_IF(ERROR,axis != 0 && num_axes != -1)<<"The axis should be 0 and the num_axes should be -1.\n";

    const BlobShape& blob_shape = param.shape();
    const int shape_size = blob_shape.dim_size();
    LOG_IF(ERROR,shape_size <= 1)<<"The shape_size should be greater than 1.\n";

    int shape = (int)blob_shape.dim(0);
    LOG_IF(ERROR,shape != 0)<<"The blob shape should be zero.\n";

    int valid_shape_size = shape_size - 1;
    unsigned int nextOffset = 4 + 4 + 2*valid_shape_size;
    int pad_r = ALIGN4(nextOffset) - nextOffset;
    nextOffset += pad_r;

    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);
    unsigned char type = kOpReshape;
    fileManager->write(&type, 1, 1);
    fileManager->write(&valid_shape_size, 1, 1);

    for(int i=0;i<valid_shape_size;i++)
    {
        short shape = (short)blob_shape.dim(i+1);
        fileManager->write(&shape, 2, 1);
    }

    if(pad_r)
    {
        fileManager->write(align_pads, 1, pad_r);
    }

}
