#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    цяло x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    цяло step = disp->widthStep;
    за(y = 0; y < im.h; ++y){
        за(x = 0; x < im.w; ++x){
            за(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned char)(val*255);
            }
        }
    }
    върни disp;
}

image ipl_to_image(IplImage* src)
{
    цяло h = src->height;
    цяло w = src->width;
    цяло c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned char *data = (unsigned char *)src->imageData;
    цяло step = src->widthStep;
    цяло i, j, k;

    за(i = 0; i < h; ++i){
        за(k= 0; k < c; ++k){
            за(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    върни im;
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
    върни m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    върни im;
}

void *open_video_stream(const char *f, цяло c, цяло w, цяло h, цяло fps)
{
    VideoCapture *cap;
    ако(f) cap = new VideoCapture(f);
    иначе cap = new VideoCapture(c);
    ако(!cap->isOpened()) върни 0;
    ако(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    ако(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    ако(fps) cap->set(CV_CAP_PROP_FPS, w);
    върни (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    ако(m.empty()) върни make_empty_image(0,0,0);
    върни mat_to_image(m);
}

image load_image_cv(char *filename, цяло channels)
{
    цяло flag = -1;
    ако (channels == 0) flag = -1;
    иначе ако (channels == 1) flag = 0;
    иначе ако (channels == 3) flag = 1;
    иначе {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    ако(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        char buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        върни make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    върни im;
}

цяло show_image_cv(image im, const char* name, цяло ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    цяло c = waitKey(ms);
    ако (c != -1) c = c%256;
    върни c;
}

void make_window(char *name, цяло w, цяло h, цяло fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    ако (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } иначе {
        resizeWindow(name, w, h);
        ако(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
