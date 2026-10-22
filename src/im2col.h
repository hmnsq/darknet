#ifndef IM2COL_H
#define IM2COL_H

void im2col_cpu(float* data_im,
        ကိန်းပြည့် channels, ကိန်းပြည့် height, ကိန်းပြည့် width,
        ကိန်းပြည့် ksize, ကိန်းပြည့် stride, ကိန်းပြည့် pad, float* data_col);

#ifdef GPU

void im2col_gpu(float *im,
         ကိန်းပြည့် channels, ကိန်းပြည့် height, ကိန်းပြည့် width,
         ကိန်းပြည့် ksize, ကိန်းပြည့် stride, ကိန်းပြည့် pad,float *data_col);

#endif
#endif
