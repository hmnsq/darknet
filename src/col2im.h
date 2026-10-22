#ifndef COL2IM_H
#define COL2IM_H

void col2im_cpu(float* data_col,
        จำนวนเต็ม channels, จำนวนเต็ม height, จำนวนเต็ม width,
        จำนวนเต็ม ksize, จำนวนเต็ม stride, จำนวนเต็ม pad, float* data_im);

#ifdef GPU
void col2im_gpu(float *data_col,
        จำนวนเต็ม channels, จำนวนเต็ม height, จำนวนเต็ม width,
        จำนวนเต็ม ksize, จำนวนเต็ม stride, จำนวนเต็ม pad, float *data_im);
#endif
#endif
