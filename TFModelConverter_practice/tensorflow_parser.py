import numpy as np
import tensorflow as tf
from tensorflow_graph import TensorflowGraph
from load_protobuf_from_file import load_protobuf_from_file
class TensorflowParser():

    @property
    def src_graph(self):
        return self.tf_graph

    @staticmethod
    def _load_meta(model_network_path):
        """Load a tensorflow meta file from disk

        Parameters
        ----------
        model_network_path: str
            Path where the model network path is (protobuf meta file)

        Returns
        -------
        model: A tensorflow protobuf file
        """
        from tensorflow.core.protobuf import meta_graph_pb2
        meta_graph=meta_graph_pb2.MetaGraphDef()
        load_protobuf_from_file(meta_graph,model_network_path)
        graph=meta_graph.graph_def
        print("Tensorflow model file [%s] loaded successfully." % model_network_path)
        return graph

    @staticmethod
    def _load_weights(model_weight_path):
        """Load a tensorflow checkpoint file from disk

        Parameters
        ----------
        model_weight_path: str
            Path where the weight path is (checkpoint file)

        Returns
        -------
        model: tensor name --> ndarry
        """
        reader = tf.train.NewCheckpointReader(model_weight_path)
        var_to_shape_map = reader.get_variable_to_shape_map()
        data=dict()
        for name in var_to_shape_map:
            tensor=reader.get_tensor(name)
            data[name]=tensor
        print("Tensorflow checkpoint file [%s] loaded successfully. [%d] variables loaded." % (\
                model_weight_path, len(data)))
        return data

    def gen_IR(self):
        for layer in self.src_graph.topological_sort:
            current_node=self.src_graph.get_node(layer)
            if self._skip_node(current_node):
                continue
            node_type=current_node.type
            if hasattr(self,"rename_"+node_type):
                func=getattr(self,"rename_"+node_type)
                func=(current_node)
            else:
                self.rename_UNKNOWN(current_node)
    @classmethod
    def skip_node(cls,source_node):
        if source_node.covered:
            return True
        for prefix in cls.skip_prefix:
            if source_node.name.startswith(prefix):
                return True

        scopes=TensorflowParser._get_scopes(source_node.name)
        for s in scopes:
            if s in cls.skip_scope:
                return True
        return False

    @staticmethod
    def _get_scopes(layer_name):
        return layer_name.split('/')


    def __init__(self,meta_file,checkpoint_file,frozen_file,dest_nodes=None):
        super(TensorflowParser, self).__init__()
        if meta_file:
            model=TensorflowParser._load_meta(meta_file)
        if checkpoint_file:
            self.ckpt_data=TensorflowParser._load_weights(checkpoint_file)
            self.weight_loaded=True
        if dest_nodes!=None:
            from tensorflow.python.framework.graph_util import extract_sub_graph
            model=extract_sub_graph(model,dest_nodes.split(','))
        self.tf_graph=TensorflowGraph(model)
        self.tf_graph.build()

