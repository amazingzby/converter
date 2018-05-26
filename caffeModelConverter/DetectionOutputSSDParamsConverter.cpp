#include "DetectionOutputSSDParamsConverter.h"

DetectionOutputSSDParamsConverter::DetectionOutputSSDParamsConverter(shared_ptr<Layer<float> > l)
        :LayerParamsConverter(l)
{
    LOG_IF(ERROR,strcmp(layer->type(), "DetectionOutput") != 0)<<"The layer type should be DetectionOutput\n";
}
void DetectionOutputSSDParamsConverter::saveParams(FileManager *&fileManager, int layer_id)
{
    const DetectionOutputParameter& detection_output_param =
                                    layer->layer_param().detection_output_param();
    bool share_location = detection_output_param.share_location();
    int num_classes = detection_output_param.num_classes();
    int background_label_id = detection_output_param.background_label_id();
    int keep_top_k = detection_output_param.keep_top_k();
    int top_k = -1;
    if(detection_output_param.nms_param().has_top_k())
    {
        top_k = detection_output_param.nms_param().top_k();
    }
    int code_type = (int)detection_output_param.code_type();
    bool variance_encoded_in_target =
        detection_output_param.variance_encoded_in_target();
    float confidence_threshold = detection_output_param.has_confidence_threshold() ?
        detection_output_param.confidence_threshold() : 0.0f;
    float nms_threshold = detection_output_param.nms_param().nms_threshold();

    float eta = detection_output_param.nms_param().eta();
    unsigned int nextOffset = 4 + 28;
    fileManager->write(&nextOffset, 4, 1);
    fileManager->write(&layer_id, 2, 1);
    unsigned char type = kOpDetectionOutputSSD;
    fileManager->write(&type, 1, 1);
    fileManager->write(&share_location,1, 1);
    fileManager->write(&num_classes, 2, 1);
    fileManager->write(&background_label_id, 2, 1);
    fileManager->write(&keep_top_k, 4, 1);
    fileManager->write(&top_k, 4, 1);
    fileManager->write(&code_type, 1, 1);
    fileManager->write(&variance_encoded_in_target,1, 1);
    unsigned int param_f16 = f32Tof16(confidence_threshold);
    fileManager->write(&param_f16, 2, 1);
    fileManager->write(&nms_threshold, 4, 1);
    fileManager->write(&eta, 4, 1);
}
