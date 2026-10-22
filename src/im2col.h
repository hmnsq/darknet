#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        целое channels, целое height, целое width,
        целое ksize, целое stride, целое pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         целое channels, целое height, целое width,
         целое ksize, целое stride, целое pad,float *data_col);

#endif
#endif
