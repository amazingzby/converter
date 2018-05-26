#include "ConvParamsConverter.h"

ConvParamsConverter::ConvParamsConverter(shared_ptr<Layer<float> > l,bool transInput,const vector<int> shape_,int bottom_channels_)
    :LayerParamsConverter(l),transInput(transInput),shape(shape_),bottom_channels(bottom_channels_)
{
    LOG_IF(ERROR,strcmp(layer->type(), "Convolution") != 0)<<"The layer type should be Convolution\n";
    const vector<int>& convShape = layer->blobs()[0]->shape();
    hasBias = layer->layer_param().convolution_param().bias_term();
    biasCount = hasBias?layer->blobs()[1]->shape()[0]:0;
    kernelH = convShape[2];
    kernelW = convShape[3];
    kernelSize = kernelH*kernelW;
    outChannels = convShape[0];
    inChannels  = convShape[1];
    complexity = outChannels*inChannels*kernelH*kernelW*shape[2]*shape[3];
    offset = 4 + 14 + 2*(kernelSize*outChannels*inChannels + biasCount);
}

void ConvParamsConverter::saveParams(FileManager *&fileManager,int layer_id)
{
    unsigned char type = kOpConv;

    int group = layer->layer_param().convolution_param().group();
    if(group > 1 && (group == bottom_channels))
    {
        type = kOpDepthConv;
    }

    unsigned int nextOffset = 4 + 14 + 2*(kernelSize*outChannels*inChannels + biasCount);
    int pad_r = ALIGN4(nextOffset) - nextOffset;
    nextOffset += pad_r;
    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);
    fileManager->write(&type, 1, 1);
    fileManager->write(&hasBias,1, 1);
    fileManager->write(&outChannels, 2, 1);
    fileManager->write(&inChannels, 2, 1);
    if(!transInput)
    {
        fileManager->write(&kernelH,1,1);
        fileManager->write(&kernelW,1,1);
    }
    else
    {
        fileManager->write(&kernelW,1,1);
        fileManager->write(&kernelH,1,1);
    }
    ConvolutionParameter conv_param = layer->layer_param().convolution_param();
    unsigned char strides[2]={0,0};
    getConvStrides(strides,conv_param);
    fileManager->write(strides,1, 2);
    unsigned char pads[2] = {0,0};
    getConvPads(pads,conv_param);
    fileManager->write(pads,1,2);
    const float* weights = layer->blobs()[0]->cpu_data();
    if(!transInput)
    {
        for(int y=0;y<kernelH;y++)
        {
            for(int x=0;x<kernelW;x++)
            {
                for(int j=0;j<inChannels;j++)
                {
                    for(int i=0;i<outChannels;i++)
                    {
                        int pos = i*inChannels*kernelSize + j*kernelSize + y*kernelW + x;
                        unsigned int weight_f16 = f32Tof16(weights[pos]);
                        fileManager->write(&weight_f16, 2, 1);
                    }
                }
            }
        }
    }
    else
    {
        for(int x=0;x<kernelW;x++)
        {
            for(int y=0;y<kernelH;y++)
            {
                for(int j=0;j<inChannels;j++)
                {
                    for(int i=0;i<outChannels;i++)
                    {
                        int pos = i*inChannels*kernelSize + j*kernelSize + y*kernelW + x;
                        unsigned int weight_f16 = f32Tof16(weights[pos]);
                        fileManager->write(&weight_f16, 2, 1);
                    }
                }
            }
        }
    }
    if(hasBias)
    {
        const float* biases=layer->blobs()[1]->cpu_data();
        for(int i=0;i<biasCount;i++)
        {
            unsigned int weight_f16 = f32Tof16(biases[i]);
            fileManager->write(&weight_f16, 2, 1);
        }
    }
    if(pad_r)
    {
        fileManager->write(align_pads, 1, pad_r);
    }
}

void ConvParamsConverter::getConvStrides(unsigned char strides[2],ConvolutionParameter& conv_param)
{
    if(conv_param.has_stride_h()||conv_param.has_stride_w())
    {
        strides[0]=conv_param.stride_h();
        strides[1]=conv_param.stride_w();
    }
    else
    {
        const int num_stride_dims = conv_param.stride_size();
        const int kDefaultStride = 1;

        for(int i=0;i<2;++i)
        {
            strides[i] = (num_stride_dims == 0) ? kDefaultStride :
            conv_param.stride((num_stride_dims == 1) ? 0 : i);
        }
    }
}

void ConvParamsConverter::getConvPads(unsigned char pads[2],ConvolutionParameter& conv_param)
{
    if (conv_param.has_pad_h() || conv_param.has_pad_w()) {

        pads[0] = conv_param.pad_h();
        pads[1] = conv_param.pad_w();
    } else {
        const int num_pad_dims = conv_param.pad_size();
        const int kDefaultPad = 0;
        for (int i = 0; i < 2; ++i) {
            pads[i] = (num_pad_dims == 0) ? kDefaultPad :
            conv_param.pad((num_pad_dims == 1) ? 0 : i);
        }
    }
}
