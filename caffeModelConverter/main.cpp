#include "NetParamsConverter.h"
int main()
{
    Caffe::set_mode(Caffe::CPU);
    shared_ptr<Net<float> > net;
    net.reset(new Net<float>("/home/zby/Desktop/mobilenet-ssd/deploy.prototxt",TEST));
    net->CopyTrainedLayersFrom("/home/zby/Desktop/mobilenet-ssd/MobileNetSSD_deploy.caffemodel");
    const char* blobFile=  "/home/zby/Desktop/mobilenet-ssd/mobel_blob.bin";
    NetParamsConverter netParamsConverter(net,blobFile);
    netParamsConverter.convertNetParams();

    return 0;
}
