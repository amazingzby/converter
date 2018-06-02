import sys as _sys
import google.protobuf.text_format as text_format
from six import text_type as _text_type

def _convert(args):
    if args.dstNodeName is None:
        raise ValueError("Need to provide the output node of Tensorflow model.")
    parser = TensorflowParser(args.network, args.weights, args.frozen_pb, args.dstNodeName)
    parser.run(args.dstPath)
    return 0

def _get_parser():
    import argparse
    parser=argparse.ArgumentParser(description='Convert other model file formats to binary format.')
    parser.add_argument(
        '--weights','-w','-iw',
        type=_text_type,
        default=None,
        help='Path to the model weights file'
    )

    parser.add_argument(
        '--network','-n','-in',
        type=_text_type,
        default=None,
        help='Path to the model network file'
    )
    parser.add_argument(
        '--dstPath','-d','-o',
        type=_text_type,
        required=True,
        help='Path to save the binary file.'
    )
    parser.add_argument(
        '--inNodeName', '-inode',
        type=_text_type,
        default='input',
        help="[Tensorflow] Input nodes' name of the graph."
    )
    parser.add_argument(
        '--dstNodeName','-node',
        type=_text_type,
        default=None,
        help="[Tensorflow] Output nodes' name of the graph."
    )
def _main():
    parser=_get_parser()
    args=parser.parse_args()
    ret=_convert(args)
    _sys.exit(int(ret))
if __name__ == '__main__':
    _main()