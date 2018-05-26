#include "PriorBoxSSDParamsConverter.h"

PriorBoxSSDParamsConverter::PriorBoxSSDParamsConverter(shared_ptr<Layer<float> > l):LayerParamsConverter(l)
{
    LOG_IF(ERROR,strcmp(layer->type(), "PriorBox") != 0)<<"The layer type should be PriorBox\n";
}
void PriorBoxSSDParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    const PriorBoxParameter& prior_box_param = layer->layer_param().prior_box_param();

    bool clip = prior_box_param.clip();

    vector<float> min_sizes;
    vector<float> max_sizes;
    vector<float> aspect_ratios;
    vector<float> variance;

    for(int i = 0; i < prior_box_param.min_size_size(); ++i)
    {
        min_sizes.push_back(prior_box_param.min_size(i));
    }
    if (prior_box_param.max_size_size() > 0)
    {
        for (int i = 0; i < prior_box_param.max_size_size(); ++i)
        {
            max_sizes.push_back(prior_box_param.max_size(i));
        }
    }

    bool flip = prior_box_param.flip();
    for (int i = 0; i < prior_box_param.aspect_ratio_size(); ++i)
    {
        float ar = prior_box_param.aspect_ratio(i);
        bool already_exist = false;
        for (int j = 0; j < aspect_ratios.size(); ++j)
        {
            if (fabs(ar - aspect_ratios[j]) < 1e-6)
            {
                already_exist = true;
                break;
            }
        }
        if(fabs(ar - 1) < 1e-6)
        {
            already_exist = true;
        }
        if(!already_exist)
        {
            aspect_ratios.push_back(ar);
            if(flip)
            {
                aspect_ratios.push_back(1./ar);
            }
        }
    }
    if(prior_box_param.variance_size() > 1)
    {
        for (int i = 0; i < prior_box_param.variance_size(); ++i)
        {
            variance.push_back(prior_box_param.variance(i));
        }
    }
    else if(prior_box_param.variance_size() == 1)
    {
        variance.push_back(prior_box_param.variance(0));
    }
    else
    {
        variance.push_back(0.1);
    }

    int img_w = 0,img_h = 0;
    float step_w = 0.0f,step_h = 0.0f;

    if (prior_box_param.has_img_h() || prior_box_param.has_img_w())
    {
        img_h = prior_box_param.img_h();
        img_w = prior_box_param.img_w();
    }
    else if(prior_box_param.has_img_size())
    {
        const int img_size = prior_box_param.img_size();
        img_h = img_size;
        img_w = img_size;
    }

    if(prior_box_param.has_step_h() || prior_box_param.has_step_w())
    {
        step_h = prior_box_param.step_h();
        step_w = prior_box_param.step_w();
    }
    else if(prior_box_param.has_step())
    {
        const float step = prior_box_param.step();
        step_h = step;
        step_w = step;
    }

    float offset=prior_box_param.offset();

    int min_size_num = (int)min_sizes.size();
    int max_size_num = (int)max_sizes.size();
    int aspect_ratio_num = (int)aspect_ratios.size();
    int variance_num = (int)variance.size();

    unsigned int nextOffset = 4 + 14 + 8 +
        2*(min_size_num + max_size_num + aspect_ratio_num + variance_num);
    int pad_r = ALIGN4(nextOffset) - nextOffset;
    nextOffset += pad_r;

    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);
    unsigned char type = kOpPriorBoxSSD;
    fileManager->write(&type, 1, 1);
    fileManager->write(&clip, 1, 1);
    fileManager->write(&img_w, 2, 1);
    fileManager->write(&img_h, 2, 1);

    unsigned int step_f16 = f32Tof16(step_w);
    fileManager->write(&step_f16, 2, 1);
    step_f16 = f32Tof16(step_h);
    fileManager->write(&step_f16, 2, 1);

    unsigned int offset_f16 = f32Tof16(offset);
    fileManager->write(&offset_f16, 2, 1);

    fileManager->write(&min_size_num, 2, 1);
    for(int i=0;i<min_size_num;i++)
    {
        unsigned int weight_f16 = f32Tof16(min_sizes[i]);
        fileManager->write(&weight_f16, 2, 1);
    }

    fileManager->write(&max_size_num, 2, 1);
    for(int i=0;i<max_size_num;i++)
    {
        unsigned int weight_f16 = f32Tof16(max_sizes[i]);
        fileManager->write(&weight_f16, 2, 1);
    }

    fileManager->write(&aspect_ratio_num, 2, 1);
    for(int i=0;i<aspect_ratio_num;i++)
    {
        unsigned int weight_f16 = f32Tof16(aspect_ratios[i]);
        fileManager->write(&weight_f16, 2, 1);
    }
    fileManager->write(&variance_num, 2, 1);
    for(int i=0;i<variance_num;i++)
    {
        unsigned int weight_f16 = f32Tof16(variance[i]);
        fileManager->write(&weight_f16, 2, 1);
    }
    if(pad_r)
    {
        fileManager->write(align_pads, 1, pad_r);
    }

}
