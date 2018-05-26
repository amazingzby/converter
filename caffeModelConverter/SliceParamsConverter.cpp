#include "SliceParamsConverter.h"

SliceParamsConverter::SliceParamsConverter(shared_ptr<Layer<float> > l,vector<Blob<float>*> bottom):
    LayerParamsConverter(l),bottom(bottom)
{
    LOG_IF(ERROR,strcmp(layer->type(), "Slice") != 0)<<"The layer type should be Slice!\n";
    LOG_IF(ERROR,bottom[0]->shape().size()!=4)<<"The bottom size should be 4!\n";
}
void SliceParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    const SliceParameter& slice_param = layer->layer_param().slice_param();
    vector<int> slice_point_vec;
    slice_point_vec.clear();
    std::copy(slice_param.slice_point().begin(),
              slice_param.slice_point().end(),
              std::back_inserter(slice_point_vec));

    int num_slice_points = (int)slice_point_vec.size();

    if(num_slice_points==0)
    {
        int num_top = (int)layer->layer_param().top_size();
        int channels = bottom[0]->shape(1);
        for(int i=1;i<num_top;i++)
        {
            slice_point_vec.push_back(i*(channels/num_top));
        }
        num_slice_points=num_top-1;
    }
    unsigned int nextOffset = 4 + 4 + num_slice_points*2;
    int pad_r = ALIGN4(nextOffset) - nextOffset;
    nextOffset += pad_r;

    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);

    unsigned char type = kOpSlice;

    fileManager->write(&type, 1, 1);
    fileManager->write(&num_slice_points, 1, 1);

    for(int i=0;i<num_slice_points;i++)
    {
        int slice_point = slice_point_vec[i];
        fileManager->write(&slice_point, 2, 1);
    }
    if(pad_r)
    {
        fileManager->write(align_pads, 1, pad_r);
    }
}
