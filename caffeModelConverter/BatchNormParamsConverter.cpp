#include "BatchNormParamsConverter.h"

BatchNormParamsConverter::BatchNormParamsConverter(shared_ptr<Layer<float> > l,bool transInput,map<int,int> bnToScale,boost::shared_ptr< Net<float> > net)
    :LayerParamsConverter(l),transInput(transInput),bnToScale(bnToScale),net(net)
{
    LOG_IF(ERROR,strcmp(layer->type(), "BatchNorm") != 0)<<"The layer type should be BatchNorm!\n";
}
void BatchNormParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    const vector<int>& shape = layer->blobs()[0]->shape();
    int channels=shape[0];

    int scaleOffset = 0;
    shared_ptr<ScaleParam> sp = scaleParamOf(layer_id);
    //assert(sp);
    LOG_IF(ERROR,!sp)<<"Scale paramete is empty!\n";
    //assert(sp->hasBias);
    LOG_IF(ERROR,!sp->hasBias)<<"Scale parametes need the parameter 'bias'!\n";

    scaleOffset += (4 + 2*sp->scaleDim + (sp->hasBias?sp->scaleDim*2:0));
    unsigned int nextOffset = 4 + 4 + (2 + 4*channels) + scaleOffset;
    int pad_r = ALIGN4(nextOffset) - nextOffset;
    nextOffset += pad_r;

    fileManager->write(&nextOffset,4,1);
    fileManager->write(&layer_id,2,1);

    unsigned char type = kOpBatchNorm;
    fileManager->write(&type,1,1);
    fileManager->write(align_pads,1,1);

    saveBatchNormBody(channels,fileManager);
    saveScaleBody(sp->layer,sp->layer_id,sp->scaleDim,sp->hasBias,fileManager);

    if(pad_r)
    {
        fileManager->write(align_pads,1,pad_r);
    }
}
void BatchNormParamsConverter::saveBatchNormBody(int channels, FileManager *&fileManager)
{
    LOG_IF(ERROR,layer->blobs()[2]->cpu_data()[0] == 0)<<"layer->blobs()[2]->cpu_data()[0] should not be zero!\n";
    const float scale_factor = 1.0f / layer->blobs()[2]->cpu_data()[0];

    const float* mean_mva     = layer->blobs()[0]->cpu_data();
    const float* variance_mva = layer->blobs()[1]->cpu_data();

    BatchNormParameter param=layer->layer_param().batch_norm_param();
    float eps=param.eps();

    assert(eps > (1e-5)-(1e-6));

    fileManager->write(&channels,2,1);
    for(int i=0;i<channels;i++)
    {
        unsigned int mean_f16 = f32Tof16(mean_mva[i]*scale_factor);
        fileManager->write(&mean_f16,2,1);
    }

    for(int i=0;i<channels;i++)
    {
        float variance = variance_mva[i];
        if(variance < 0.0f) variance = 0.0f;
        float rsigma = 1.0f/sqrtf(variance*scale_factor + eps);
        unsigned int rsigma_f16 = f32Tof16(rsigma);
        fileManager->write(&rsigma_f16,2,1);
    }
}
shared_ptr<ScaleParam> BatchNormParamsConverter::scaleParamOf(int bn_layer_id)
{
    shared_ptr<ScaleParam> p;

    if(bnToScale.find(bn_layer_id) != bnToScale.end())
    {
        int scale_layer_id = bnToScale[bn_layer_id];
        const vector<shared_ptr<Layer<float> > >& layers = net->layers();

        auto scale_layer = layers[scale_layer_id];

        const ScaleParameter& param = scale_layer->layer_param().scale_param();
        vector<Blob<float>*> bottom = net->bottom_vecs()[scale_layer_id];

        Blob<float>* scaleBlob = (bottom.size() > 1) ? bottom[1] : scale_layer->blobs()[0].get();

        int scaleDim = scaleBlob->count();
        bool hasBias = param.bias_term();

        p.reset(new ScaleParam(scale_layer,scale_layer_id,scaleDim,hasBias));

    }

    return p;
}

void BatchNormParamsConverter::saveScaleBody(const shared_ptr<Layer<float> > layer, int layer_id, int scaleDim,
                             bool hasBias,FileManager *&fileManager)
{
    assert(strcmp(layer->type(), "Scale") == 0);

//    const ScaleParameter& param = layer->layer_param().scale_param();
    vector<Blob<float>*> bottom = net->bottom_vecs()[layer_id];

//    int axis = bottom[0]->CanonicalAxisIndex(param.axis());
//    const int num_axes = param.num_axes();

    //bottom's shape must be 4!
    //assert(bottom[0]->shape().size() == 4 && axis == 1 && num_axes == 1);

    Blob<float>* scaleBlob = (bottom.size() > 1) ? bottom[1] : layer->blobs()[0].get();

    //write scaleDim
    fileManager->write(&scaleDim, 2, 1);

    const float* scale_data = scaleBlob->cpu_data();
    for(int i=0;i<scaleDim;i++)
    {
        unsigned int weight_f16 = f32Tof16(scale_data[i]);
        fileManager->write(&weight_f16, 2, 1);
    }

    //write biasCount
    int count = hasBias?scaleDim:0;
    fileManager->write(&count, 2, 1);

    if(hasBias)
    {
        int size = (int)layer->blobs().size() ;
        Blob<float>* biasBlob = layer->blobs()[size - 1].get();

        int biasCount = biasBlob->count();
        assert(biasCount == scaleDim);
        const float* bias_data = biasBlob->cpu_data();
        for(int i=0;i<biasCount;i++)
        {
            unsigned int weight_f16 = f32Tof16(bias_data[i]);
            fileManager->write(&weight_f16, 2, 1);
        }

    }
}
