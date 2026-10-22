#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        ចំនួនគត់ channels, ចំនួនគត់ height, ចំនួនគត់ width,
        ចំនួនគត់ ksize, ចំនួនគត់ stride, ចំនួនគត់ pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         ចំនួនគត់ channels, ចំនួនគត់ height, ចំនួនគត់ width,
         ចំនួនគត់ ksize, ចំនួនគត់ stride, ចំនួនគត់ pad,float *data_col);

#endif
#endif
