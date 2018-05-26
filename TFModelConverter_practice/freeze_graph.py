import tensorflow as tf
from tensorflow.python.framework import graph_util
from google.protobuf import text_format
model_network_path="./resNet/imagenet_resnet_v2_152.ckpt.meta"
model_weight_path="./resNet/resnet_v2_152.ckpt"
with tf.Session() as sess:
    saver = tf.train.import_meta_graph(model_network_path, clear_devices=True)
    saver.restore(sess, model_weight_path)
    #graph_def = sess.graph.as_graph_def()
    #output_graph_def = graph_def.convert_variables_to_constants(
    #    sess, tf.get_default_graph().as_graph_def(), [y.name.split(':')[0]])
    graph_location='./resGraph'
    train_writer = tf.summary.FileWriter(graph_location)
    train_writer.add_graph(tf.get_default_graph())
    all_vars = tf.trainable_variables()
    #print(all_vars["resnet_v2_152/conv1/weights:0"])
    for v in all_vars:
        print(v.name)
    #graph_def = sess.graph.as_graph_def()
    #names=[node for op in sess.node]
    #for i in names:
    for node in sess.graph_def.node:
        if node.name=="resnet_v2_152/conv1/weights/Initializer/truncated_normal/shape":
            print(node.attr["value"])
            print(node)





"""
sess = tf.Session()  
   #将保存的模型文件解析为GraphDef  
   model_f = gfile.FastGFile("model.pb",'rb')  
   graph_def = tf.GraphDef()  
   graph_def.ParseFromString(model_f.read())  
   c = tf.import_graph_def(graph_def,return_elements=["add:0"])  
   print(sess.run(c))  
   #[array([ 11.], dtype=float32)]  
"""