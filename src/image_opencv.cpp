#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    цео x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    цео step = disp->widthStep;
    за(y = 0; y < im.h; ++y){
        за(x = 0; x < im.w; ++x){
            за(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned char)(val*255);
            }
        }
    }
    врати disp;
}

image ipl_to_image(IplImage* src)
{
    цео h = src->height;
    цео w = src->width;
    цео c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned char *data = (unsigned char *)src->imageData;
    цео step = src->widthStep;
    цео i, j, k;

    за(i = 0; i < h; ++i){
        за(k= 0; k < c; ++k){
            за(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    врати im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    ако(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    врати m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    врати im;
}

void *open_video_stream(const char *f, цео c, цео w, цео h, цео fps)
{
    VideoCapture *cap;
    ако(f) cap = new VideoCapture(f);
    инако cap = new VideoCapture(c);
    ако(!cap->isOpened()) врати 0;
    ако(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    ако(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    ако(fps) cap->set(CV_CAP_PROP_FPS, w);
    врати (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    ако(m.empty()) врати make_empty_image(0,0,0);
    врати mat_to_image(m);
}

image load_image_cv(char *filename, цео channels)
{
    цео flag = -1;
    ако (channels == 0) flag = -1;
    инако ако (channels == 1) flag = 0;
    инако ако (channels == 3) flag = 1;
    инако {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    ако(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        char buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        врати make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    врати im;
}

цео show_image_cv(image im, const char* name, цео ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    цео c = waitKey(ms);
    ако (c != -1) c = c%256;
    врати c;
}

void make_window(char *name, цео w, цео h, цео fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    ако (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } инако {
        resizeWindow(name, w, h);
        ако(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
