class GraphNode(object):
    def __init__(self,layer):
        self.in_edges=list()
        self.out_edges=list()
        self.layer=layer
        self.covered=False
        self.real_name=self.name
    @property
    def name(self):
        assert False
    @property
    def variable_name(self):
        return self.real_name.replace('/','_').replace('-','_').replace('[','_').replace(']','_')
    @property
    def real_variable_name(self):
        return self.real_name.replace('/', '_').replace('-', '_').replace('[', '_').replace(']', '_')

class Graph(object):
    def __init__(self,model):
        self.layer_map={}
        self.input_layers=list()
        self.output_layers=list()
        self.layer_name_map=dict()
        self.topological_sort=list()
        self.model=model
    def build(self):
        self._make_input_layers()
        self._make_output_layers()
        self._get_topological_sort()
    def _make_input_layers(self):