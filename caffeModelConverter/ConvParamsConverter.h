#ifndef SAVECONVPARAMS_H
#define SAVECONVPARAMS_H

#include "LayerParamsConverter.h"

class ConvParamsConverter : public LayerParamsConverter
{
public:
    ConvParamsConverter(shared_ptr<Layer<float> > l,bool transInput,const vector<int> shape_,int bottom_channels);
    void saveParams(FileManager *&fileManager,int layer_id);
private:
    bool hasBias;
    int biasCount;
    int kernelH;
    int kernelW;
    int kernelSize;
    int outChannels;
    int inChannels;
    bool transInput;
    const vector<int> shape;
    int bottom_channels;
private:
    void getConvStrides(unsigned char strides[2],ConvolutionParameter& conv_param);
    void getConvPads(unsigned char pads[2],ConvolutionParameter& conv_param);
};

#endif // SAVECONVPARAMS_H
