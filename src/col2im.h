#ifndef COL2IM_H
#define COL2IM_H

void col2im_cpu(float* data_col,
        정수 channels, 정수 height, 정수 width,
        정수 ksize, 정수 stride, 정수 pad, float* data_im);

#ifdef GPU
void col2im_gpu(float *data_col,
        정수 channels, 정수 height, 정수 width,
        정수 ksize, 정수 stride, 정수 pad, float *data_im);
#endif
#endif
