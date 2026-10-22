#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        цяло channels, цяло height, цяло width,
        цяло ksize, цяло stride, цяло pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         цяло channels, цяло height, цяло width,
         цяло ksize, цяло stride, цяло pad,float *data_col);

#endif
#endif
