#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        ціле channels, ціле height, ціле width,
        ціле ksize, ціле stride, ціле pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         ціле channels, ціле height, ціле width,
         ціле ksize, ціле stride, ціле pad,float *data_col);

#endif
#endif
