#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        정수 channels, 정수 height, 정수 width,
        정수 ksize, 정수 stride, 정수 pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         정수 channels, 정수 height, 정수 width,
         정수 ksize, 정수 stride, 정수 pad,float *data_col);

#endif
#endif
