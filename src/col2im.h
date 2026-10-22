#ifndef COL2IM_H
#define COL2IM_H

शून्य col2im_cpu(भिन्न* data_col,
        पूर्णाङ्क channels, पूर्णाङ्क height, पूर्णाङ्क width,
        पूर्णाङ्क ksize, पूर्णाङ्क stride, पूर्णाङ्क pad, भिन्न* data_im);

#ifdef GPU
शून्य col2im_gpu(भिन्न *data_col,
        पूर्णाङ्क channels, पूर्णाङ्क height, पूर्णाङ्क width,
        पूर्णाङ्क ksize, पूर्णाङ्क stride, पूर्णाङ्क pad, भिन्न *data_im);
#endif
#endif
