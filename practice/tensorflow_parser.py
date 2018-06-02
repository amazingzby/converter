import numpy as np
import tensorflow

class TensorflowParser(object):
    def __init__(self,meta_file,checkpoint_file):
        super(TensorflowParser,self).__init__()
        if meta_file:
            model=self._load_meta()
        if checkpoint_file:
            self.ckpt_data=self._load_weights(checkpoint_file)
            self.weight_loaded=True
    def load_protobuf_from_file(self,container,filename):
        with open(filename,'rb') as fin:
            file_content=fin.read()
        try:
            container.ParseFromString(file_content)
            print("Parse file [%s] with binary format successfully." % (filename))
            return container
        except Exception as e:
            print ("Info: Trying to parse file [%s] with binary format but failed with error [%s]." % (
            filename, str(e)))
        try:
            from google.protobuf import text_format
            text_format.Parse(file_content.decode('UTF-8'),container, allow_unknown_extension=True)
            print("Parse file [%s] with text format successfully." % (filename))
        except text_format.ParseError as e:
            raise IOError("Cannot parse file %s: %s." % (filename, str(e)))
        return container
    def _load_meta(self,model_network_path):
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
        from mmdnn.conversion.common.IR.IR_graph import load_protobuf_from_file

        meta_graph = meta_graph_pb2.MetaGraphDef()
        load_protobuf_from_file(meta_graph, model_network_path)
        graph = meta_graph.graph_def

        print ("Tensorflow model file [%s] loaded successfully." % model_network_path)
        return graph

if __name__=='__main__':
    model_network_path = "/home/zby/Desktop/project/converter/TFModelConverter_practice/resNet/imagenet_resnet_v2_152.ckpt.meta"
    model_weight_path = "/home/zby/Desktop/project/converter/TFModelConverter_practice/resNet/resnet_v2_152.ckpt"
    tensorflowParser=TensorflowParser()
    model=tensorflowParser._load_meta(model_network_path)
    print(model)