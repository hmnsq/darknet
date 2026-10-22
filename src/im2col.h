#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        整数 channels, 整数 height, 整数 width,
        整数 ksize, 整数 stride, 整数 pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         整数 channels, 整数 height, 整数 width,
         整数 ksize, 整数 stride, 整数 pad,float *data_col);

#endif
#endif
