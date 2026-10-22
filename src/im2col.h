#ifndef IM2COL_H
#define IM2COL_H

ಶೂನ್ಯ im2col_cpu(ಭಿನ್ನ* data_im,
        ಪೂರ್ಣಾಙ್ಕ channels, ಪೂರ್ಣಾಙ್ಕ height, ಪೂರ್ಣಾಙ್ಕ width,
        ಪೂರ್ಣಾಙ್ಕ ksize, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ pad, ಭಿನ್ನ* data_col);

#ifdef GPU

ಶೂನ್ಯ im2col_gpu(ಭಿನ್ನ *im,
         ಪೂರ್ಣಾಙ್ಕ channels, ಪೂರ್ಣಾಙ್ಕ height, ಪೂರ್ಣಾಙ್ಕ width,
         ಪೂರ್ಣಾಙ್ಕ ksize, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ pad,ಭಿನ್ನ *data_col);

#endif
#endif
