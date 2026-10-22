#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        цео channels, цео height, цео width,
        цео ksize, цео stride, цео pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         цео channels, цео height, цео width,
         цео ksize, цео stride, цео pad,float *data_col);

#endif
#endif
