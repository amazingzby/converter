import tensorflow as tf
"""
from conversion.tensorflow_parser import TensorflowParser
model_network_path="/home/zby/Desktop/project/TFModelConverter_practice/resNet/imagenet_resnet_v2_152.ckpt.meta"
model_weight_path="/home/zby/Desktop/project/TFModelConverter_practice/resNet/resnet_v2_152.ckpt"
tensorflowParser=TensorflowParser(model_network_path,model_weight_path,None)
tensorflowParser.gen_IR()
print(tensorflowParser.src_graph)
#binary_file='./resNet/model.bin'
#with open(binary_file,'wb') as fileManager:
"""
model_network_path="/home/zby/Desktop/project/TFModelConverter_practice/resNet/imagenet_resnet_v2_152.ckpt.meta"
model_weight_path="/home/zby/Desktop/project/TFModelConverter_practice/resNet/resnet_v2_152.ckpt"
with tf.Session() as sess:
    saver=tf.train.import_meta_graph(model_network_path)
    saver.restore(sess,model_weight_path)
