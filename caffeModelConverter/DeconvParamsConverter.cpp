#include "DeconvParamsConverter.h"

DeconvParamsConverter::DeconvParamsConverter(shared_ptr<Layer<float> > l,int bottom_channels)
    :LayerParamsConverter(l),bottom_channels(bottom_channels)
{
    LOG_IF(ERROR,strcmp(layer->type(), "Deconvolution") != 0)<<"The layer type should be Deconvolution\n";
}

void DeconvParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    int numParams = layer->layer_param().param().size();
    LOG_IF(ERROR,numParams != 1)<<"The parameter numParams should be 1.\n";

    float lr_mult = layer->layer_param().param(0).lr_mult();
    float decay_mult = layer->layer_param().param(0).decay_mult();

    LOG_IF(ERROR,lr_mult != 0 && decay_mult != 0)<<"Parameters lr_mult and decay_mult are not correct.\n";

    bool hasBias = layer->layer_param().convolution_param().bias_term();
    LOG_IF(ERROR,hasBias != false)<<"The parameter hasBias should be false.\n";


    //bottom channels
    int group = layer->layer_param().convolution_param().group();
    int num_output = layer->layer_param().convolution_param().num_output();
    LOG_IF(ERROR,group != bottom_channels && num_output != bottom_channels)<<"Parameters group and num_output should be equal to bottom_channels.\n";

    int  stride_size = layer->layer_param().convolution_param().stride_size();
    LOG_IF(ERROR,stride_size != 1)<<"The parameter stride_size should be 1.\n";


    int stride = layer->layer_param().convolution_param().stride(0);
    int  kernel_size_size = layer->layer_param().convolution_param().kernel_size_size();
    assert(kernel_size_size == 1);
    LOG_IF(ERROR,kernel_size_size != 1)<<"The parameter kernel_size_size should be 1.\n";

    int kernel_size = layer->layer_param().convolution_param().kernel_size(0);
    assert(kernel_size == (2 * stride - stride % 2));
    LOG_IF(ERROR,kernel_size == (2 * stride - stride % 2))<<"The parameter kernel_size should be equal to 2 * stride - stride % 2.\n";

    LOG_IF(ERROR,stride != 2)<<"The parameter stride should be 2.\n";

    const float* weights = layer->blobs()[0]->cpu_data();
    unsigned int nextOffset = 4 + 4 + 32;
    //    int pad_r = ALIGN4(nextOffset) - nextOffset;
    //    nextOffset += pad_r;

    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);
    unsigned char type = kOpUpSampling;
    fileManager->write(&type, 1, 1);
    fileManager->write(align_pads,1, 1);
    for(int i=0;i<16;i++)
    {
        unsigned int weight_f16 = f32Tof16(weights[i]);
        fileManager->write(&weight_f16, 2, 1);
    }

}
