#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        ཧྲིལ channels, ཧྲིལ height, ཧྲིལ width,
        ཧྲིལ ksize, ཧྲིལ stride, ཧྲིལ pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         ཧྲིལ channels, ཧྲིལ height, ཧྲིལ width,
         ཧྲིལ ksize, ཧྲིལ stride, ཧྲིལ pad,float *data_col);

#endif
#endif
