#अन्तर्भाव "box.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <math.h>
#अन्तर्भाव <stdlib.h>

पूर्णांक nms_comparator(स्थिरांक शून्य *pa, स्थिरांक शून्य *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    दशांश diff = 0;
    जर(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } अन्यथा {
        diff = a.objectness - b.objectness;
    }
    जर(diff < 0) परत 1;
    अन्यथा जर(diff > 0) परत -1;
    परत 0;
}

शून्य do_nms_obj(detection *dets, पूर्णांक total, पूर्णांक classes, दशांश thresh)
{
    पूर्णांक i, j, k;
    k = total-1;
    साठी(i = 0; i <= k; ++i){
        जर(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    साठी(i = 0; i < total; ++i){
        dets[i].sort_class = -1;
    }

    क्विक(dets, total, आकार(detection), nms_comparator);
    साठी(i = 0; i < total; ++i){
        जर(dets[i].objectness == 0) सुरूठेवा;
        box a = dets[i].bbox;
        साठी(j = i+1; j < total; ++j){
            जर(dets[j].objectness == 0) सुरूठेवा;
            box b = dets[j].bbox;
            जर (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                साठी(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


शून्य do_nms_sort(detection *dets, पूर्णांक total, पूर्णांक classes, दशांश thresh)
{
    पूर्णांक i, j, k;
    k = total-1;
    साठी(i = 0; i <= k; ++i){
        जर(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    साठी(k = 0; k < classes; ++k){
        साठी(i = 0; i < total; ++i){
            dets[i].sort_class = k;
        }
        क्विक(dets, total, आकार(detection), nms_comparator);
        साठी(i = 0; i < total; ++i){
            जर(dets[i].prob[k] == 0) सुरूठेवा;
            box a = dets[i].bbox;
            साठी(j = i+1; j < total; ++j){
                box b = dets[j].bbox;
                जर (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_to_box(दशांश *f, पूर्णांक stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    परत b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    दशांश l1 = a.x - a.w/2;
    दशांश l2 = b.x - b.w/2;
    जर (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    दशांश r1 = a.x + a.w/2;
    दशांश r2 = b.x + b.w/2;
    जर(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    जर (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    जर (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    दशांश t1 = a.y - a.h/2;
    दशांश t2 = b.y - b.h/2;
    जर (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    दशांश b1 = a.y + a.h/2;
    दशांश b2 = b.y + b.h/2;
    जर(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    जर (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    जर (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    परत d;
}

दशांश overlap(दशांश x1, दशांश w1, दशांश x2, दशांश w2)
{
    दशांश l1 = x1 - w1/2;
    दशांश l2 = x2 - w2/2;
    दशांश डावा = l1 > l2 ? l1 : l2;
    दशांश r1 = x1 + w1/2;
    दशांश r2 = x2 + w2/2;
    दशांश उजवा = r1 < r2 ? r1 : r2;
    परत उजवा - डावा;
}

दशांश box_intersection(box a, box b)
{
    दशांश w = overlap(a.x, a.w, b.x, b.w);
    दशांश h = overlap(a.y, a.h, b.y, b.h);
    जर(w < 0 || h < 0) परत 0;
    दशांश area = w*h;
    परत area;
}

दशांश box_union(box a, box b)
{
    दशांश i = box_intersection(a, b);
    दशांश u = a.w*a.h + b.w*b.h - i;
    परत u;
}

दशांश box_iou(box a, box b)
{
    परत box_intersection(a, b)/box_union(a, b);
}

दशांश box_rmse(box a, box b)
{
    परत वर्ग_मूल(घात(a.x-b.x, 2) + 
                घात(a.y-b.y, 2) + 
                घात(a.w-b.w, 2) + 
                घात(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    दशांश w = overlap(a.x, a.w, b.x, b.w);
    दशांश h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    परत di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    परत du;
}


शून्य test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    म_लिखो("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    दशांश inter =  box_union(a, b);
    दशांश xinter = box_union(dxa, b);
    दशांश yinter = box_union(dya, b);
    दशांश winter = box_union(dwa, b);
    दशांश hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    म_लिखो("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
शून्य test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    म_लिखो("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    दशांश inter =  box_intersection(a, b);
    दशांश xinter = box_intersection(dxa, b);
    दशांश yinter = box_intersection(dya, b);
    दशांश winter = box_intersection(dwa, b);
    दशांश hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    म_लिखो("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

शून्य test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    दशांश iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    म_लिखो("%f\n", iou);
    dbox d = diou(a, b);
    म_लिखो("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    दशांश xiou = box_iou(dxa, b);
    दशांश yiou = box_iou(dya, b);
    दशांश wiou = box_iou(dwa, b);
    दशांश hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    म_लिखो("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    दशांश u = box_union(a,b);
    दशांश i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    जर(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        परत dd;
    }

    dd.dx = 2*घात((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*घात((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*घात((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*घात((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    परत dd;
}


शून्य do_nms(box *boxes, दशांश **probs, पूर्णांक total, पूर्णांक classes, दशांश thresh)
{
    पूर्णांक i, j, k;
    साठी(i = 0; i < total; ++i){
        पूर्णांक any = 0;
        साठी(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        जर(!any) {
            सुरूठेवा;
        }
        साठी(j = i+1; j < total; ++j){
            जर (box_iou(boxes[i], boxes[j]) > thresh){
                साठी(k = 0; k < classes; ++k){
                    जर (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    अन्यथा probs[j][k] = 0;
                }
            }
        }
    }
}

box encode_box(box b, box anchor)
{
    box encode;
    encode.x = (b.x - anchor.x) / anchor.w;
    encode.y = (b.y - anchor.y) / anchor.h;
    encode.w = log2(b.w / anchor.w);
    encode.h = log2(b.h / anchor.h);
    परत encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = घात(2., b.w) * anchor.w;
    decode.h = घात(2., b.h) * anchor.h;
    परत decode;
}
