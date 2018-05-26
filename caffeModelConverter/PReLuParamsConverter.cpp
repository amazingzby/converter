#include "PReLuParamsConverter.h"

PReLuParamsConverter::PReLuParamsConverter(shared_ptr<Layer<float> > l,int channels_):LayerParamsConverter(l),channels(channels_)
{
    LOG_IF(ERROR,strcmp(layer->type(), "PReLU") != 0)<<"The layer type should be PReLU\n";

}
void PReLuParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    int slopeCount = layer->blobs()[0]->count();
    unsigned int nextOffset = 4 + 4 + (2 + 2*slopeCount);
    int pad_r = ALIGN4(nextOffset) - nextOffset;
    nextOffset += pad_r;
    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);
    unsigned char type = kOpPRelu;
    fileManager->write(&type, 1, 1);
    fileManager->write(align_pads,1,1);

    const float* slopes = layer->blobs()[0]->cpu_data();
    PReLUParameter prelu_param = layer->layer_param().prelu_param();
    bool channelShared = prelu_param.channel_shared();

    if(channelShared)
        assert(slopeCount == 1);
    else
        assert(slopeCount == channels);

    fileManager->write(&slopeCount,2, 1);

    for(int i=0;i<slopeCount;i++)
    {
        unsigned int slope_f16 = f32Tof16(slopes[i]);
        fileManager->write(&slope_f16, 2, 1);
    }
    if(pad_r)
    {
       fileManager->write(align_pads, 1, pad_r);
    }
}
