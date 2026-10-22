#ifndef IM2COL_H
#define IM2COL_H

ശൂന്യം im2col_cpu(ദശാംശം* data_im,
        പൂർണ്ണസംഖ്യ channels, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ width,
        പൂർണ്ണസംഖ്യ ksize, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ pad, ദശാംശം* data_col);

#ifdef GPU

ശൂന്യം im2col_gpu(ദശാംശം *im,
         പൂർണ്ണസംഖ്യ channels, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ width,
         പൂർണ്ണസംഖ്യ ksize, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ pad,ദശാംശം *data_col);

#endif
#endif
