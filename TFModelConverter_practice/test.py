import tensorflow as tf
import numpy as np

with tf.Session() as sess:
    new_saver = tf.train.import_meta_graph('./models/model.ckpt.meta')
    for var in tf.trainable_variables():
        print
        var.name
    new_saver.restore(sess, tf.train.latest_checkpoint('./models/'))
    all_vars = tf.trainable_variables()
    for v in all_vars:
        name = v.name
        fname = name + '.prototxt'
        fname = fname.replace('/', '_')
        print
        fname
        v_4d = np.array(sess.run(v))
        if v_4d.ndim == 4:
            # v_4d.shape [ H, W, I, O ]
            v_4d = np.swapaxes(v_4d, 0, 2)  # swap H, I
            v_4d = np.swapaxes(v_4d, 1, 3)  # swap W, O
            v_4d = np.swapaxes(v_4d, 0, 1)  # swap I, O
            # v_4d.shape [ O, I, H, W ]
            f = open(fname, 'w')
            vshape = v_4d.shape[:]
            v_1d = v_4d.reshape(v_4d.shape[0] * v_4d.shape[1] * v_4d.shape[2] * v_4d.shape[3])
            f.write('  blobs {\n')
            for vv in v_1d:
                f.write('    data: %8f' % vv)
                f.write('\n')
            f.write('    shape {\n')
            for s in vshape:
                f.write('      dim: ' + str(s))  # print dims
                f.write('\n')
            f.write('    }\n')
            f.write('  }\n')
        elif v_4d.ndim == 1:  # do not swap
            f = open(fname, 'w')
            f.write('  blobs {\n')
            for vv in v_4d:
                f.write('    data: %.8f' % vv)
                f.write('\n')
            f.write('    shape {\n')
            f.write('      dim: ' + str(v_4d.shape[0]))  # print dims
            f.write('\n')
            f.write('    }\n')
            f.write('  }\n')
f.close()