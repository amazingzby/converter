#include "ScaleParamsConverter.h"

ScaleParamsConverter::ScaleParamsConverter(shared_ptr<Layer<float> > l,vector<Blob<float>*> bottom,set<int> dummySet):
    LayerParamsConverter(l),bottom(bottom),dummySet(dummySet)
{
    LOG_IF(ERROR,strcmp(layer->type(), "Scale") != 0)<<"The layer type should be Scale!\n";
}

void ScaleParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    if(dummySet.find(layer_id) != dummySet.end())
    {
        saveDummyParams(layer_id, fileManager);
        return;
    }
    const ScaleParameter& param = layer->layer_param().scale_param();

    Blob<float>* scaleBlob = (bottom.size() > 1) ? bottom[1] : layer->blobs()[0].get();

    int scaleDim = scaleBlob->count();
    bool hasBias = param.bias_term();

    unsigned int nextOffset = 4 + 4 + (4 + 2*scaleDim + (hasBias?scaleDim*2:0));
    int pad_r = ALIGN4(nextOffset) - nextOffset;
    nextOffset += pad_r;
    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);
    unsigned char type = kOpScale;
    fileManager->write(&type, 1, 1);
    fileManager->write(align_pads,1, 1);
    saveScaleBody(layer_id,scaleDim,hasBias,fileManager);
    if(pad_r)
    {
        fileManager->write(align_pads,1,pad_r);
    }
}

void ScaleParamsConverter::saveDummyParams(int layer_id, FileManager *&fileManager)
{
    unsigned int nextOffset = 4 + 4;
    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);
    unsigned char type = kOpDummy;
    fileManager->write(&type, 1, 1);
    fileManager->write(align_pads,1, 1);
}
void ScaleParamsConverter::saveScaleBody(int layer_id, int scaleDim,bool hasBias,FileManager *&fileManager)
{
    Blob<float>* scaleBlob = (bottom.size() > 1) ? bottom[1] : layer->blobs()[0].get();
    fileManager->write(&scaleDim, 2, 1);
    const float* scale_data = scaleBlob->cpu_data();
    for(int i=0;i<scaleDim;i++)
    {
        unsigned int weight_f16 = f32Tof16(scale_data[i]);
        fileManager->write(&weight_f16, 2, 1);
    }
    int count=hasBias?scaleDim:0;
    fileManager->write(&count, 2, 1);
    if(hasBias)
    {
        int size=(int)layer->blobs().size() ;
        Blob<float>* biasBlob = layer->blobs()[size - 1].get();

        int biasCount=biasBlob->count();
        const float* bias_data = biasBlob->cpu_data();
        for(int i=0;i<biasCount;i++)
        {
            unsigned int weight_f16 = f32Tof16(bias_data[i]);
            fileManager->write(&weight_f16, 2, 1);
        }
    }
}
