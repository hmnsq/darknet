#ifndef COL2IM_H
#define COL2IM_H

void col2im_cpu(float* data_col,
        整数 channels, 整数 height, 整数 width,
        整数 ksize, 整数 stride, 整数 pad, float* data_im);

#ifdef GPU
void col2im_gpu(float *data_col,
        整数 channels, 整数 height, 整数 width,
        整数 ksize, 整数 stride, 整数 pad, float *data_im);
#endif
#endif
