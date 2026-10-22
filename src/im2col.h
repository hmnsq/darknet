#ifndef IM2COL_H
#define IM2COL_H

शून्य im2col_cpu(भिन्न* data_im,
        पूर्णाङ्क channels, पूर्णाङ्क height, पूर्णाङ्क width,
        पूर्णाङ्क ksize, पूर्णाङ्क stride, पूर्णाङ्क pad, भिन्न* data_col);

#ifdef GPU

शून्य im2col_gpu(भिन्न *im,
         पूर्णाङ्क channels, पूर्णाङ्क height, पूर्णाङ्क width,
         पूर्णाङ्क ksize, पूर्णाङ्क stride, पूर्णाङ्क pad,भिन्न *data_col);

#endif
#endif
