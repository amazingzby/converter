import tensorflow as tf
import numpy as np
meta_path='./models/model.ckpt.meta'
latest_checkpoint='./models'
with tf.Session() as sess:
    new_saver = tf.train.import_meta_graph(meta_path)#load graph
    for var in tf.trainable_variables():
        print(var.name)
    new_saver.restore(sess,tf.train.latest_checkpoint(latest_checkpoint))
    all_vars=tf.trainable_variables()
    for v in all_vars:
        name=v.name
        fname
