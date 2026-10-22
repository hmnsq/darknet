#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        số_nguyên channels, số_nguyên height, số_nguyên width,
        số_nguyên ksize, số_nguyên stride, số_nguyên pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         số_nguyên channels, số_nguyên height, số_nguyên width,
         số_nguyên ksize, số_nguyên stride, số_nguyên pad,float *data_col);

#endif
#endif
