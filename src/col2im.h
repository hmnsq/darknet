#ifndef COL2IM_H
#define COL2IM_H

void col2im_cpu(float* data_col,
        цео channels, цео height, цео width,
        цео ksize, цео stride, цео pad, float* data_im);

#ifdef GPU
void col2im_gpu(float *data_col,
        цео channels, цео height, цео width,
        цео ksize, цео stride, цео pad, float *data_im);
#endif
#endif
