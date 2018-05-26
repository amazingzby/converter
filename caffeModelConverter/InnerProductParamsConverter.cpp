#include "InnerProductParamsConverter.h"

InnerProductParamsConverter::InnerProductParamsConverter(shared_ptr<Layer<float> > l,bool transInput,const vector<int> shape)
    :LayerParamsConverter(l),transInput(transInput),shape(shape)
{
    LOG_IF(ERROR,strcmp(layer->type(), "InnerProduct") != 0)<<"The layer type should be InnerProduct!\n";
}
void InnerProductParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    int axis = layer->layer_param().inner_product_param().axis();
    LOG_IF(ERROR,axis!=1)<<"Axis should equal to 1.\n";
    LOG_IF(ERROR,shape.size()!=4||shape.size()!=2)<<"The layer shape should be 2 or 4!\n";
    int channels=shape[1];
    int height=1,width=1;
    if(shape.size()==4)
    {
        height=shape[2];
        width=shape[3];
    }
    const vector<int>& wShape = layer->blobs()[0]->shape();

    bool hasBias   = layer->layer_param().inner_product_param().bias_term();
    bool transpose = layer->layer_param().inner_product_param().transpose();

    int biasCount = hasBias?layer->blobs()[1]->shape()[0]:0;

    int num_output=wShape[0];
    int num_input =wShape[1];

    unsigned int nextOffset = 4 + 8 + 2*(num_output*num_input + biasCount);

    int pad_r=ALIGN4(nextOffset) - nextOffset;
    nextOffset+=pad_r;
    fileManager->write(&nextOffset,4,1);
    fileManager->write(&layer_id,2,1);
    unsigned char type = kOpFC;
    fileManager->write(&type,1,1);
    fileManager->write(&hasBias,1,1);
    fileManager->write(&num_output,2,1);
    fileManager->write(&num_input,2,1);

    const float* weights = (float*)layer->blobs()[0]->cpu_data();
    if(transpose)
    {
        for(int i=0;i<num_output;i++)
        {
            const float* row = weights + i*num_input;
            if(transInput)
            {
                for(int x=0;x<width;x++)
                {
                    for(int y=0;y<height;y++)
                    {
                        for(int c=0;c<channels;c++)
                        {
                            unsigned int weight_f16 = f32Tof16(row[c*height*width + y*width + x]);
                            fileManager->write(&weight_f16,2,1);
                        }
                    }
                }
            }
            else
            {
                for(int y=0;y<height;y++)
                {
                    for(int x=0;x<width;x++)
                    {
                        for(int c=0;c<channels;c++)
                        {
                            unsigned int weight_f16 = f32Tof16(row[c*height*width + y*width + x]);
                            fileManager->write(&weight_f16,2,1);
                        }
                    }
                }
            }
        }
    }
    else
    {
        for(int i=0;i<num_input;i++)
        {
            for(int j=0;j<num_output;j++)
            {
                const float* row = weights + j*num_input;
                int c = i%channels;
                int x,y;
                if(transInput)
                {
                    y = (i/channels)%height;
                    x = (i/(channels*height))%width;
                }
                else
                {
                    x = (i/channels)%width;
                    y = (i/(channels*width))%height;
                }
                unsigned int weight_f16 = f32Tof16(row[c*height*width + y*width + x]);
                fileManager->write(&weight_f16,2,1);
            }
        }
    }
    if(hasBias)
    {
        const float* biases = layer->blobs()[1]->cpu_data();
        for(int i=0;i<biasCount;i++)
        {
            unsigned int weight_f16 = f32Tof16(biases[i]);
            fileManager->write(&weight_f16,2,1);
        }
    }
    if(pad_r)
    {
        fileManager->write(align_pads,1,pad_r);
    }

}
