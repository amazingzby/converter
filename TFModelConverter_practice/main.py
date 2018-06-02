from tensorflow_parser import TensorflowParser
import tensorflow_graph
model_network_path="./resNet/imagenet_resnet_v2_152.ckpt.meta"
model_weight_path="./resNet/resnet_v2_152.ckpt"
model_frozen_file=None
i=0
tf_parser=TensorflowParser(model_network_path,model_weight_path,model_frozen_file)
for layer in tf_parser.src_graph.topological_sort:
    current_node=tf_parser.src_graph.get_node(layer)
    node_type=current_node.type
    i=i+1
    print("Layer %s"%str(i))
    print(node_type)
print(tf_parser)