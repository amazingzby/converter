#include "LayerParamsConverter.h"

using namespace caffe;

LayerParamsConverter::LayerParamsConverter(shared_ptr<Layer<float> > l):layer(l)
{
    memset(align_pads,0,4);
}
