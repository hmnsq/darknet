#ifdef OPENCV

#include "stdio.h"
#include "stdlib.h"
#include "opencv2/opencv.hpp"
#include "image.h"

using namespace cv;

extern "C" {

IplImage *image_to_ipl(image im)
{
    ціле x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    ціле step = disp->widthStep;
    для(y = 0; y < im.h; ++y){
        для(x = 0; x < im.w; ++x){
            для(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned char)(val*255);
            }
        }
    }
    повернути disp;
}

image ipl_to_image(IplImage* src)
{
    ціле h = src->height;
    ціле w = src->width;
    ціле c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned char *data = (unsigned char *)src->imageData;
    ціле step = src->widthStep;
    ціле i, j, k;

    для(i = 0; i < h; ++i){
        для(k= 0; k < c; ++k){
            для(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    повернути im;
}

Mat image_to_mat(image im)
{
    image copy = copy_image(im);
    constrain_image(copy);
    якщо(im.c == 3) rgbgr_image(copy);

    IplImage *ipl = image_to_ipl(copy);
    Mat m = cvarrToMat(ipl, true);
    cvReleaseImage(&ipl);
    free_image(copy);
    повернути m;
}

image mat_to_image(Mat m)
{
    IplImage ipl = m;
    image im = ipl_to_image(&ipl);
    rgbgr_image(im);
    повернути im;
}

void *open_video_stream(const char *f, ціле c, ціле w, ціле h, ціле fps)
{
    VideoCapture *cap;
    якщо(f) cap = new VideoCapture(f);
    інакше cap = new VideoCapture(c);
    якщо(!cap->isOpened()) повернути 0;
    якщо(w) cap->set(CV_CAP_PROP_FRAME_WIDTH, w);
    якщо(h) cap->set(CV_CAP_PROP_FRAME_HEIGHT, w);
    якщо(fps) cap->set(CV_CAP_PROP_FPS, w);
    повернути (void *) cap;
}

image get_image_from_stream(void *p)
{
    VideoCapture *cap = (VideoCapture *)p;
    Mat m;
    *cap >> m;
    якщо(m.empty()) повернути make_empty_image(0,0,0);
    повернути mat_to_image(m);
}

image load_image_cv(char *filename, ціле channels)
{
    ціле flag = -1;
    якщо (channels == 0) flag = -1;
    інакше якщо (channels == 1) flag = 0;
    інакше якщо (channels == 3) flag = 1;
    інакше {
        fprintf(stderr, "OpenCV can't force load with %d channels\n", channels);
    }
    Mat m;
    m = imread(filename, flag);
    якщо(!m.data){
        fprintf(stderr, "Cannot load image \"%s\"\n", filename);
        char buff[256];
        sprintf(buff, "echo %s >> bad.list", filename);
        system(buff);
        повернути make_image(10,10,3);
        //exit(0);
    }
    image im = mat_to_image(m);
    повернути im;
}

ціле show_image_cv(image im, const char* name, ціле ms)
{
    Mat m = image_to_mat(im);
    imshow(name, m);
    ціле c = waitKey(ms);
    якщо (c != -1) c = c%256;
    повернути c;
}

void make_window(char *name, ціле w, ціле h, ціле fullscreen)
{
    namedWindow(name, WINDOW_NORMAL); 
    якщо (fullscreen) {
        setWindowProperty(name, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    } інакше {
        resizeWindow(name, w, h);
        якщо(strcmp(name, "Demo") == 0) moveWindow(name, 0, 0);
    }
}

}

#endif
