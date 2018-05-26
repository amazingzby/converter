#ifndef SAVELAYERPARAMS_H
#define SAVELAYERPARAMS_H

#include<caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "FileManager.h"
#include "Fp16Convert.h"
#include "NNOpTypes.h"
#include "glog/logging.h"

using namespace caffe;

#define ALIGN4(x) (((x) + 0x3) & (unsigned int) ~0x3)

class LayerParamsConverter
{
public:
    LayerParamsConverter(shared_ptr<Layer<float> > l);
    virtual void saveParams(FileManager *&fileManager,int layer_id)=0;
protected:
    const shared_ptr<Layer<float> > layer;
    unsigned int offset;
    double complexity;
    unsigned char align_pads[4];
};

#endif // SAVELAYERPARAMS_H
