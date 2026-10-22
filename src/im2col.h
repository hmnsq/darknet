#ifndef IM2COL_H
#define IM2COL_H

வெறுமை im2col_cpu(தசம* data_im,
        முழு channels, முழு height, முழு width,
        முழு ksize, முழு stride, முழு pad, தசம* data_col);

#ifdef GPU

வெறுமை im2col_gpu(தசம *im,
         முழு channels, முழு height, முழு width,
         முழு ksize, முழு stride, முழு pad,தசம *data_col);

#endif
#endif
