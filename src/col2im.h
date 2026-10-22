#ifndef COL2IM_H
#define COL2IM_H

ശൂന്യം col2im_cpu(ദശാംശം* data_col,
        പൂർണ്ണസംഖ്യ channels, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ width,
        പൂർണ്ണസംഖ്യ ksize, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ pad, ദശാംശം* data_im);

#ifdef GPU
ശൂന്യം col2im_gpu(ദശാംശം *data_col,
        പൂർണ്ണസംഖ്യ channels, പൂർണ്ണസംഖ്യ height, പൂർണ്ണസംഖ്യ width,
        പൂർണ്ണസംഖ്യ ksize, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ pad, ദശാംശം *data_im);
#endif
#endif
