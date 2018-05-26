/*
 * NetParamsConverter.h
 *
 *  Created on: May 17, 2018
 *      Author: Zhang
 * This class converts the caffemodel to the binary file.
 *
 */


#ifndef NETPARAMSCONVERTER_H
#define NETPARAMSCONVERTER_H

#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <memory>
#include <glog/logging.h>

#include "FileManager.h"
#include "FileManager.h"
#include "ConvParamsConverter.h"
#include "ReluParamsConverter.h"
#include "PReLuParamsConverter.h"
#include "PoolingParamsConverter.h"
#include "SplitParamsConverter.h"
#include "SoftmaxParamsConverter.h"
#include "InnerProductParamsConverter.h"
#include "BatchNormParamsConverter.h"
#include "ScaleParamsConverter.h"
#include "EltwiseParamsConverter.h"
#include "ConcatParamsConverter.h"
#include "SliceParamsConverter.h"
#include "PermuteSSDParamsConverter.h"
#include "FlattenParamsConverter.h"
#include "ReshapeParamsConverter.h"
#include "PriorBoxSSDParamsConverter.h"
#include "DetectionOutputSSDParamsConverter.h"
#include "DeconvParamsConverter.h"

using namespace caffe;

class NetParamsConverter
{
public:
    NetParamsConverter(boost::shared_ptr< Net<float> > net_,const char* blobName_,bool transInput = false);
    void convertNetParams();//Convert the caffemodel to the binary file.
private:
    void writeHead();//Write magicCode and version.
    void convertBottomsAndTopsParams();//Write parameters of bottoms and tops of each layer.
    void convertLayerParams();//Write parameters of each layer.
    unsigned int sizeOfBottomsAndTops(int from, int to);
    void buildLayerNamesIndex();
private:
    static unsigned int magicCode;
    static unsigned int version;
    int extIdStart;
    double complexity;
    unsigned char align_pads[4];
    int layerSize;
    boost::shared_ptr< Net<float> > net;
    bool transInput;
    map<string, int> layer_names_index;
    map<int,int> bnToScale;
    map<int,int> topToBn;
    set<int> dummySet;
    FileManager *fileManager = FileManager::getInstance();
    const char* blobName;

};

#endif // NETPARAMSCONVERTER_H
