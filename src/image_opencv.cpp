#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    puur_naa_nka x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    puur_naa_nka step = disp->widthStep;
    for(y = 0; y < im.h; ++y){
        for(x = 0; x < im.w; ++x){
            for(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned var_na)(val*255);
            }
        }
    }
    ᤀᤡᤴ disp;
}

image ipl_to_image(IplImage* src)
{
    puur_naa_nka h = src->height;
    puur_naa_nka w = src->width;
    puur_naa_nka c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned var_na *data = (unsigned var_na *)src->imageData;
    puur_naa_nka step = src->widthStep;
    puur_naa_nka i, j, k;

    for(i = 0; i < h; ++i){
        for(k= 0; k < c; ++k){
            for(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    ᤀᤡᤴ im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    ᤀᤡ᤺(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    ᤀᤡᤴ m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    ᤀᤡᤴ im;
}

void *open_video_stream(const var_na *f, puur_naa_nka c, puur_naa_nka w, puur_naa_nka h, puur_naa_nka fps)
{
    VideoCapture *cap;
    ᤀᤡ᤺(f) cap = new VideoCapture(f);
    ᤀᤡ᤺ᤆᤡ cap = new VideoCapture(c);
    ᤀᤡ᤺(!cap->isOpened()) ᤀᤡᤴ 0;
    ᤀᤡ᤺(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    ᤀᤡ᤺(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    ᤀᤡ᤺(fps) cap->set(CV_CAP_PROP_FPS, w);
    ᤀᤡᤴ (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    ᤀᤡ᤺(m.empty()) ᤀᤡᤴ make_empty_image(0,0,0);
    ᤀᤡᤴ mat_to_image(m);
}

image load_image_cv(var_na *filename, puur_naa_nka channels)
{
    puur_naa_nka flag = -1;
    ᤀᤡ᤺ (channels == 0) flag = -1;
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (channels == 1) flag = 0;
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (channels == 3) flag = 1;
    ᤀᤡ᤺ᤆᤡ {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    ᤀᤡ᤺(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        var_na buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        ᤀᤡᤴ make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    ᤀᤡᤴ im;
}

puur_naa_nka show_image_cv(image im, const var_na* name, puur_naa_nka ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    puur_naa_nka c = waitKey(ms);
    ᤀᤡ᤺ (c != -1) c = c%256;
    ᤀᤡᤴ c;
}

void make_window(var_na *name, puur_naa_nka w, puur_naa_nka h, puur_naa_nka fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    ᤀᤡ᤺ (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } ᤀᤡ᤺ᤆᤡ {
        resizeWindow(name, w, h);
        ᤀᤡ᤺(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
