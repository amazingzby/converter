#include "NetParamsConverter.h"

unsigned int NetParamsConverter::magicCode = 0x6d76636d;//mvcm
unsigned int NetParamsConverter::version   = 0x00050004;
#define ALIGN4(x) (((x) + 0x3) & (unsigned int) ~0x3)
NetParamsConverter::NetParamsConverter(boost::shared_ptr< Net<float> > net_,const char* blobName_,bool transInput):
    net(net_),blobName(blobName_),transInput(transInput),extIdStart(32768),complexity(0)
{
    memset(align_pads,0,4);
    buildLayerNamesIndex();

    const vector<shared_ptr<Layer<float> > >& layers = net->layers();

    layerSize=(int)layers.size();
    CHECK_GT(layerSize,2)<<"The number of layers is less than 2!\n";
    for(int i=1;i<layerSize;i++)
    {
        auto layer = layers[i];
        const LayerParameter& layer_param = layer->layer_param();
        if(strcmp(layer->type(), "BatchNorm") == 0)
        {
            int top_layer_id = layer_names_index[layer_param.top(0)];
            topToBn[top_layer_id] = i;
        }else if(strcmp(layer->type(), "Scale") == 0)
        {
            int bottom_layer_id = layer_names_index[layer_param.bottom(0)];
            if(topToBn.find(bottom_layer_id) != topToBn.end())
            {
                int bn_layer_id = topToBn[bottom_layer_id];
                LOG_IF(ERROR,bnToScale.find(bn_layer_id) == bnToScale.end())<<"Can not find the correct bn layer!\n";
                bnToScale[bn_layer_id] = i;
                dummySet.insert(i);
            }
        }
    }
}

void NetParamsConverter::convertNetParams()
{
    fileManager->init(blobName);
    writeHead();
    convertBottomsAndTopsParams();
    convertLayerParams();
    fileManager->close();
}

void NetParamsConverter::writeHead()
{
    printf("Writing the magicCode and version number.\n");
    fileManager->write(&magicCode,4, 1);
    fileManager->write(&version,4, 1);
}

void NetParamsConverter::convertBottomsAndTopsParams()
{
    printf("Writing parametes of bottoms and tops of each layer.\n");
    const vector<shared_ptr<Layer<float> > >& layers = net->layers();
    //neglecting input layer,we set 'from' to 1

    unsigned int nextOffset = 4 + sizeOfBottomsAndTops(1,layerSize);
    int pad_r = ALIGN4(nextOffset) - nextOffset;
    nextOffset += pad_r;

    fileManager->write(&nextOffset, 4, 1);
    int num_layer=layerSize-1;
    fileManager->write(&num_layer,2, 1);

    for(int i=1;i<layerSize;i++)
    {
        const LayerParameter& layer_param = layers[i]->layer_param();
        int size = layer_param.bottom_size();
        fileManager->write(&size,2, 1);
        for(int j=0;j<size;j++)
        {
            int bottom_layer_id = layer_names_index[layer_param.bottom(j)];
            fileManager->write(&bottom_layer_id,2, 1);
        }
    }

    for(int i=1;i<layerSize;i++)
    {
        const LayerParameter& layer_param = layers[i]->layer_param();
        int size=layer_param.top_size();
        fileManager->write(&size,2, 1);
        for(int j=0;j<size;j++)
        {
            int top_layer_id = layer_names_index[layer_param.top(j)];
            fileManager->write(&top_layer_id,2, 1);
        }
    }

}

void NetParamsConverter::convertLayerParams()
{
    printf("Writing parametes of each layer.\n");
    const vector<shared_ptr<Layer<float> > >& layers = net->layers();
    for(int i=1;i<layerSize;i++)
    {

        auto layer = layers[i];

        if(strcmp(layer->type(), "Convolution") == 0)
        {
            int bottom_channels = net->bottom_vecs()[i][0]->channels();
            const vector<int>& shape = net->top_vecs()[i][0]->shape();
            ConvParamsConverter saveConvParams(layer,transInput,shape,bottom_channels);
            saveConvParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "ReLU") == 0)
        {
            ReluParamsConverter saveReluParams = ReluParamsConverter(layer);
            saveReluParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "PReLU") == 0)
        {
            int channels = net->bottom_vecs()[i][0]->channels();
            PReLuParamsConverter savePReLuParams(layer,channels);
            savePReLuParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "Pooling") == 0)
        {
            PoolingParamsConverter savePoolingParams(layer);
            savePoolingParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "Split") == 0)
        {
            SplitParamsConverter saveSplitParams(layer);
            saveSplitParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "Softmax") == 0)
        {
            SoftmaxParamsConverter saveSoftmaxParams(layer);
            saveSoftmaxParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "InnerProduct") == 0)
        {
            const vector<int>& shape = net->bottom_vecs()[i][0]->shape();
            InnerProductParamsConverter saveInnerProductParams(layer,transInput,shape);
            saveInnerProductParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "BatchNorm") == 0)
        {
            BatchNormParamsConverter saveBatchNormParams(layer,transInput,bnToScale,net);
            saveBatchNormParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "Scale") == 0)
        {
            vector<Blob<float>*> bottom = net->bottom_vecs()[i];
            ScaleParamsConverter saveScaleParams(layer,bottom,dummySet);
            saveScaleParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "Eltwise") == 0)
        {
            EltwiseParamsConverter saveEltwiseParams(layer);
            saveEltwiseParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "Concat") == 0)
        {
            ConcatParamsConverter saveConcatParams(layer);
            saveConcatParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "Slice") == 0)
        {
            vector<Blob<float>*> bottom = net->bottom_vecs()[i];
            SliceParamsConverter saveSliceParams(layer,bottom);
            saveSliceParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "Permute")==0)
        {
            vector<Blob<float>*> bottom = net->bottom_vecs()[i];
            PermuteSSDParamsConverter savePermuteSSDParams(layer,bottom);
            savePermuteSSDParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "Flatten") == 0)
        {
            FlattenParamsConverter saveFlattenParams(layer);
            saveFlattenParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "Reshape") == 0)
        {
            ReshapeParamsConverter saveReshapeParams(layer);
            saveReshapeParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "PriorBox") == 0)
        {
            PriorBoxSSDParamsConverter savePriorBoxSSDParams(layer);
            savePriorBoxSSDParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "DetectionOutput") == 0)
        {
            DetectionOutputSSDParamsConverter saveDetectionOutputSSDParams(layer);
            saveDetectionOutputSSDParams.saveParams(fileManager,i);
        }
        else if(strcmp(layer->type(), "Deconvolution") == 0)
        {
            int bottom_channels = net->bottom_vecs()[i][0]->channels();
            DeconvParamsConverter saveDeconvParams(layer,bottom_channels);
            saveDeconvParams.saveParams(fileManager,i);
        }
        else
        {
           LOG_IF(ERROR,1)<<"Layer type not supported:"<<layer->type()<<"\n";
        }
    }
}

unsigned int NetParamsConverter::sizeOfBottomsAndTops(int from, int to)
{
    const vector<shared_ptr<Layer<float> > >& layers = net->layers();
    unsigned int totalSize = 2;
    for(int i=from;i<to;i++)
    {
        const LayerParameter& layer_param = layers[i]->layer_param();

        int size = layer_param.bottom_size();
        totalSize += (2 + 2*size);

        size = layer_param.top_size();
        totalSize += (2 + 2*size);
    }
    return totalSize;
}

void NetParamsConverter::buildLayerNamesIndex()
{
    auto layerNames = net->layer_names();
    for (int layer_id = 0; layer_id < layerNames.size(); ++layer_id) {
        layer_names_index[layerNames[layer_id]] = layer_id;
    }

    const vector<shared_ptr<Layer<float> > >& layers = net->layers();
    for(int i=1;i<layers.size();i++)
    {
        //map split layer's tops to it's layer_id
        if(strcmp(layers[i]->type(),"Split") == 0)
        {
            const LayerParameter& layer_param = layers[i]->layer_param();

            for(int j=0;j<layer_param.top_size();j++)
            {
                const string& top_name =  layer_param.top(j);
                layer_names_index[top_name] = i;
            }
        }
        //map slice layer's tops to extend ids respectively
        if(strcmp(layers[i]->type(),"Slice") == 0)
        {
            const LayerParameter& layer_param = layers[i]->layer_param();

            for(int j=0;j<layer_param.top_size();j++)
            {
                const string& top_name =  layer_param.top(j);
                layer_names_index[top_name] = extIdStart++;
            }
        }

    }
}
