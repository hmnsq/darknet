#ifndef COL2IM_H
#define COL2IM_H

ಶೂನ್ಯ col2im_cpu(ಭಿನ್ನ* data_col,
        ಪೂರ್ಣಾಙ್ಕ channels, ಪೂರ್ಣಾಙ್ಕ height, ಪೂರ್ಣಾಙ್ಕ width,
        ಪೂರ್ಣಾಙ್ಕ ksize, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ pad, ಭಿನ್ನ* data_im);

#ifdef GPU
ಶೂನ್ಯ col2im_gpu(ಭಿನ್ನ *data_col,
        ಪೂರ್ಣಾಙ್ಕ channels, ಪೂರ್ಣಾಙ್ಕ height, ಪೂರ್ಣಾಙ್ಕ width,
        ಪೂರ್ಣಾಙ್ಕ ksize, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ pad, ಭಿನ್ನ *data_im);
#endif
#endif
