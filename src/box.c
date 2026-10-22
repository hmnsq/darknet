#समावेश "box.h"
#समावेश <stdio.h>
#समावेश <math.h>
#समावेश <stdlib.h>

पूर्णांक nms_comparator(स्थिर व्योम *pa, स्थिर व्योम *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    भग्न diff = 0;
    अगर(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } अन्यथा {
        diff = a.objectness - b.objectness;
    }
    अगर(diff < 0) वापस 1;
    अन्यथा अगर(diff > 0) वापस -1;
    वापस 0;
}

व्योम do_nms_obj(detection *dets, पूर्णांक योग_2, पूर्णांक classes, भग्न thresh)
{
    पूर्णांक i, j, k;
    k = योग_2-1;
    क्रम(i = 0; i <= k; ++i){
        अगर(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    योग_2 = k+1;

    क्रम(i = 0; i < योग_2; ++i){
        dets[i].sort_class = -1;
    }

    क्विक(dets, योग_2, माप(detection), nms_comparator);
    क्रम(i = 0; i < योग_2; ++i){
        अगर(dets[i].objectness == 0) जारी;
        box a = dets[i].bbox;
        क्रम(j = i+1; j < योग_2; ++j){
            अगर(dets[j].objectness == 0) जारी;
            box b = dets[j].bbox;
            अगर (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                क्रम(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


व्योम do_nms_sort(detection *dets, पूर्णांक योग_2, पूर्णांक classes, भग्न thresh)
{
    पूर्णांक i, j, k;
    k = योग_2-1;
    क्रम(i = 0; i <= k; ++i){
        अगर(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    योग_2 = k+1;

    क्रम(k = 0; k < classes; ++k){
        क्रम(i = 0; i < योग_2; ++i){
            dets[i].sort_class = k;
        }
        क्विक(dets, योग_2, माप(detection), nms_comparator);
        क्रम(i = 0; i < योग_2; ++i){
            अगर(dets[i].prob[k] == 0) जारी;
            box a = dets[i].bbox;
            क्रम(j = i+1; j < योग_2; ++j){
                box b = dets[j].bbox;
                अगर (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_to_box(भग्न *f, पूर्णांक stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    वापस b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    भग्न l1 = a.x - a.w/2;
    भग्न l2 = b.x - b.w/2;
    अगर (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    भग्न r1 = a.x + a.w/2;
    भग्न r2 = b.x + b.w/2;
    अगर(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    अगर (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    अगर (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    भग्न t1 = a.y - a.h/2;
    भग्न t2 = b.y - b.h/2;
    अगर (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    भग्न b1 = a.y + a.h/2;
    भग्न b2 = b.y + b.h/2;
    अगर(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    अगर (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    अगर (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    वापस d;
}

भग्न overlap(भग्न x1, भग्न w1, भग्न x2, भग्न w2)
{
    भग्न l1 = x1 - w1/2;
    भग्न l2 = x2 - w2/2;
    भग्न बायाँ = l1 > l2 ? l1 : l2;
    भग्न r1 = x1 + w1/2;
    भग्न r2 = x2 + w2/2;
    भग्न दायाँ = r1 < r2 ? r1 : r2;
    वापस दायाँ - बायाँ;
}

भग्न box_intersection(box a, box b)
{
    भग्न w = overlap(a.x, a.w, b.x, b.w);
    भग्न h = overlap(a.y, a.h, b.y, b.h);
    अगर(w < 0 || h < 0) वापस 0;
    भग्न area = w*h;
    वापस area;
}

भग्न box_union(box a, box b)
{
    भग्न i = box_intersection(a, b);
    भग्न u = a.w*a.h + b.w*b.h - i;
    वापस u;
}

भग्न box_iou(box a, box b)
{
    वापस box_intersection(a, b)/box_union(a, b);
}

भग्न box_rmse(box a, box b)
{
    वापस वर्ग_मूल(घात(a.x-b.x, 2) + 
                घात(a.y-b.y, 2) + 
                घात(a.w-b.w, 2) + 
                घात(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    भग्न w = overlap(a.x, a.w, b.x, b.w);
    भग्न h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    वापस di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    वापस du;
}


व्योम test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    म_लिखो("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    भग्न inter =  box_union(a, b);
    भग्न xinter = box_union(dxa, b);
    भग्न yinter = box_union(dya, b);
    भग्न winter = box_union(dwa, b);
    भग्न hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    म_लिखो("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
व्योम test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    म_लिखो("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    भग्न inter =  box_intersection(a, b);
    भग्न xinter = box_intersection(dxa, b);
    भग्न yinter = box_intersection(dya, b);
    भग्न winter = box_intersection(dwa, b);
    भग्न hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    म_लिखो("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

व्योम test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    भग्न iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    म_लिखो("%f\n", iou);
    dbox d = diou(a, b);
    म_लिखो("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    भग्न xiou = box_iou(dxa, b);
    भग्न yiou = box_iou(dya, b);
    भग्न wiou = box_iou(dwa, b);
    भग्न hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    म_लिखो("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    भग्न u = box_union(a,b);
    भग्न i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    अगर(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        वापस dd;
    }

    dd.dx = 2*घात((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*घात((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*घात((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*घात((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    वापस dd;
}


व्योम do_nms(box *boxes, भग्न **probs, पूर्णांक योग_2, पूर्णांक classes, भग्न thresh)
{
    पूर्णांक i, j, k;
    क्रम(i = 0; i < योग_2; ++i){
        पूर्णांक any = 0;
        क्रम(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        अगर(!any) {
            जारी;
        }
        क्रम(j = i+1; j < योग_2; ++j){
            अगर (box_iou(boxes[i], boxes[j]) > thresh){
                क्रम(k = 0; k < classes; ++k){
                    अगर (probs[i][k] < probs[j][k]) probs[i][k] = 0;
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
    वापस encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = घात(2., b.w) * anchor.w;
    decode.h = घात(2., b.h) * anchor.h;
    वापस decode;
}
