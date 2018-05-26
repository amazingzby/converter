QT += core
QT -= gui

CONFIG += c++11

TARGET = caffeModelConverter
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    BatchNormParamsConverter.cpp \
    ConcatParamsConverter.cpp \
    ConvParamsConverter.cpp \
    DeconvParamsConverter.cpp \
    DetectionOutputSSDParamsConverter.cpp \
    EltwiseParamsConverter.cpp \
    FileManager.cpp \
    FlattenParamsConverter.cpp \
    Fp16Convert.cpp \
    InnerProductParamsConverter.cpp \
    LayerParamsConverter.cpp \
    NetParamsConverter.cpp \
    PermuteSSDParamsConverter.cpp \
    PoolingParamsConverter.cpp \
    PReLuParamsConverter.cpp \
    PriorBoxSSDParamsConverter.cpp \
    ReluParamsConverter.cpp \
    ReshapeParamsConverter.cpp \
    ScaleParamsConverter.cpp \
    SliceParamsConverter.cpp \
    SoftmaxParamsConverter.cpp \
    SplitParamsConverter.cpp
#caffe
INCLUDEPATH += /home/zby/work/ssd/include /home/zby/work/ssd/src /home/zby/work/ssd/distribute/include
INCLUDEPATH += /home/zby/work/software/opencv-3.2.0/include \
/home/zby/work/software/opencv-3.2.0/include/opencv \
/home/zby/work/software/opencv-3.2.0/include/opencv2
LIBS += -L/home/zby/work/ssd/build/lib
LIBS += -lcaffe
#opencv
LIBS += /home/zby/work/software/opencv-3.2.0/build/lib/libopencv_calib3d.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_core.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_features2d.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_flann.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_highgui.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_imgcodecs.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_imgproc.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_ml.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_objdetect.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_photo.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_shape.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_stitching.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_superres.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_videoio.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_video.so \
/home/zby/work/software/opencv-3.2.0/build/lib/libopencv_videostab.so
#other dependencies
LIBS += -lglog -lgflags -lprotobuf -lboost_system -lboost_thread -llmdb -lleveldb -lstdc++  -lcblas -latlas

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    BatchNormParamsConverter.h \
    ConcatParamsConverter.h \
    ConvParamsConverter.h \
    DeconvParamsConverter.h \
    DetectionOutputSSDParamsConverter.h \
    EltwiseParamsConverter.h \
    FileManager.h \
    FlattenParamsConverter.h \
    Fp16Convert.h \
    InnerProductParamsConverter.h \
    LayerParamsConverter.h \
    NetParamsConverter.h \
    NNOpTypes.h \
    PermuteSSDParamsConverter.h \
    PoolingParamsConverter.h \
    PReLuParamsConverter.h \
    PriorBoxSSDParamsConverter.h \
    ReluParamsConverter.h \
    ReshapeParamsConverter.h \
    ScaleParamsConverter.h \
    SliceParamsConverter.h \
    SoftmaxParamsConverter.h \
    SplitParamsConverter.h

SUBDIRS += \
    caffeModelConverter.pro \
    caffeModelConverter.pro

DISTFILES += \
    caffeModelConverter.pro.user \
    caffeModelConverter.pro.user \
    caffeModelConverter.pro.autosave
