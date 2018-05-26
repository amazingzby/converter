/*
 * NNOpTypes.h
 *
 *  Created on: Mar 17, 2018
 *      Author: yao
 */

#ifndef LEON_NNOPTYPES_H_
#define LEON_NNOPTYPES_H_

typedef enum
{
    kOpConv                   = 0,          ///< Convolution
    kOpMaxPool                = 1,          ///< Max-Pooling
    kOpAvgPool                = 2,          ///< Average-Pooling
    kOpSoftMax                = 3,          ///< SoftMax
    kOpFC                     = 4,          ///< Fully connected layer
    kOpRelu                   = 6,          ///< rectified linear unit (Relu) rectifier
    kOpDepthConv              = 8,          ///< Depthwise Convolution
    kOpPRelu                  = 10,         ///< PReLU
    kOpSum                    = 12,         ///< Sum of input and weight
    kOpProd                   = 13,         ///< Prod of input and weight
    kOpMax                    = 14,         ///< Max between input and weight
    kOpScale                  = 15,         ///< Multiply each plane with a multiplier and add a bias
	kOpReshape                = 24,
	kOpBatchNorm              = 28,         ///< BatchNorm layer
    kOpDummy = 127,
    kOpCaffeSplit = 128,
    kOpConcat,
    kOpSlice,
    kOpFlatten,
    kOpPermuteSSD,
    kOpPriorBoxSSD,
    kOpDetectionOutputSSD,
    kOpUpSampling
}t_NNOpType;




#endif /* LEON_NNOPTYPES_H_ */
