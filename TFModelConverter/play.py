import tensorflow as tf
from conversion.tensorflow_graph import TensorflowGraph
from play_function import *
model_network_path="/home/zby/Desktop/project/TFModelConverter_practice/resNet/imagenet_resnet_v2_152.ckpt.meta"
model_weight_path="/home/zby/Desktop/project/TFModelConverter_practice/resNet/resnet_v2_152.ckpt"
fileManager=open('/home/zby/Desktop/tfnet.txt','w')
if model_network_path:
    model=load_meta(model_network_path)
if model_weight_path:
    ckpt_data=load_weights(model_weight_path)

tf_graph=TensorflowGraph(model)
tf_graph.build()

for layer in tf_graph.topological_sort:
    current_node=self.src_graph.get_node(layer)
    if

"""
import sys
i=0
stdout_backup=sys.stdout
log_file=open('/home/zby/Desktop/data.log','w')
sys.stdout=log_file
print(ckpt_data)
#if(i>500):
#    break
log_file.close()
"""
"""
import sys
stdout_backup=sys.stdout
log_file=open('/home/zby/Desktop/message.log','w')
sys.stdout=log_file
print(model)
log_file.close()
"""