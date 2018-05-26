#include "PoolingParamsConverter.h"

PoolingParamsConverter::PoolingParamsConverter(shared_ptr<Layer<float> > l):LayerParamsConverter(l)
{
        LOG_IF(ERROR,strcmp(layer->type(), "Pooling") != 0)<<"The layer type should be Pooling\n";
}
void PoolingParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    PoolingParameter pool_param=layer->layer_param().pooling_param();
    unsigned char type = kOpMaxPool;
    if(pool_param.pool() == PoolingParameter_PoolMethod_AVE)
    {
        type = kOpAvgPool;
    }

    bool global_pooling = pool_param.global_pooling();
    unsigned int nextOffset = 4 + 4 + (global_pooling?0:6);
    int pad_r = ALIGN4(nextOffset) - nextOffset;
    nextOffset += pad_r;
    fileManager->write(&nextOffset,4,1);
    fileManager->write(&layer_id,2,1);
    fileManager->write(&type,1,1);
    fileManager->write(&global_pooling,1,1);
    if(!global_pooling)
    {
        int kernelH,kernelW;
        if(pool_param.has_kernel_size())
        {
            kernelH=kernelW=pool_param.kernel_size();
        }
        else
        {
            kernelH=pool_param.kernel_h();
            kernelW=pool_param.kernel_w();
        }
        fileManager->write(&kernelH,1,1);
        fileManager->write(&kernelW,1,1);

        unsigned char strides[2] = {0,0};
        if(!pool_param.has_stride_h())
        {
            strides[0]=strides[1]=pool_param.stride();
        }
        else
        {
            strides[0]=pool_param.stride_h();
            strides[1]=pool_param.stride_w();
        }
        fileManager->write(strides,1,2);
        unsigned char pads[2] = {0,0};
        if(!pool_param.has_pad_h())
        {
            pads[0]=pads[1]=pool_param.pad();
        }
        else
        {
            pads[0]=pool_param.pad_h();
            pads[1]=pool_param.pad_w();
        }
        fileManager->write(pads,1,2);
    }
    if(pad_r)
    {
        fileManager->write(align_pads,1,pad_r);
    }
}
