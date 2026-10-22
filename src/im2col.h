#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        จำนวนเต็ม channels, จำนวนเต็ม height, จำนวนเต็ม width,
        จำนวนเต็ม ksize, จำนวนเต็ม stride, จำนวนเต็ม pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         จำนวนเต็ม channels, จำนวนเต็ม height, จำนวนเต็ม width,
         จำนวนเต็ม ksize, จำนวนเต็ม stride, จำนวนเต็ม pad,float *data_col);

#endif
#endif
