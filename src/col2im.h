#ifndef COL2IM_H
#define COL2IM_H

void col2im_cpu(float* data_col,
        ціле channels, ціле height, ціле width,
        ціле ksize, ціле stride, ціле pad, float* data_im);

#ifdef GPU
void col2im_gpu(float *data_col,
        ціле channels, ціле height, ціле width,
        ціле ksize, ціле stride, ціле pad, float *data_im);
#endif
#endif
