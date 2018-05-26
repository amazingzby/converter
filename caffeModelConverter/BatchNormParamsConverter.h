#ifndef SAVEBATCHNORMPARAMS_H
#define SAVEBATCHNORMPARAMS_H

#include "LayerParamsConverter.h"

class ScaleParam
{
public:
    shared_ptr<Layer<float> > layer;
    int layer_id;
    int scaleDim;
    bool hasBias;
public:
    ScaleParam(shared_ptr<Layer<float> > layer,int layer_id,int scaleDim,bool hasBias)
    :layer(layer),layer_id(layer_id),scaleDim(scaleDim),hasBias(hasBias)
    {
    }
};




class BatchNormParamsConverter : public LayerParamsConverter
{
public:
    BatchNormParamsConverter(shared_ptr<Layer<float> > l,bool transInput,map<int,int> bnToScale,boost::shared_ptr< Net<float> > net);
    void saveParams(FileManager *&fileManager,int layer_id) ;
private:
    bool transInput;
    map<int,int> bnToScale;
    boost::shared_ptr< Net<float> > net;
private:
    void saveBatchNormBody(int channels,FileManager *&fileManager);
    void saveScaleBody(const shared_ptr<Layer<float> > layer, int layer_id, int scaleDim,
                                 bool hasBias,FileManager *&fileManager);
    shared_ptr<ScaleParam> scaleParamOf(int bn_layer_id);
};

#endif // SAVEBATCHNORMPARAMS_H
