#अन्तर्भाव "box.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <math.h>
#अन्तर्भाव <stdlib.h>

पूर्णाङ्क nms_comparator(ध्रुव शून्य *pa, ध्रुव शून्य *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    भिन्न diff = 0;
    यदि(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } अन्यथा {
        diff = a.objectness - b.objectness;
    }
    यदि(diff < 0) प्रत्यागम 1;
    अन्यथा यदि(diff > 0) प्रत्यागम -1;
    प्रत्यागम 0;
}

शून्य do_nms_obj(detection *dets, पूर्णाङ्क total, पूर्णाङ्क classes, भिन्न thresh)
{
    पूर्णाङ्क i, j, k;
    k = total-1;
    प्रति(i = 0; i <= k; ++i){
        यदि(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    प्रति(i = 0; i < total; ++i){
        dets[i].sort_class = -1;
    }

    क्विक(dets, total, परिमाण(detection), nms_comparator);
    प्रति(i = 0; i < total; ++i){
        यदि(dets[i].objectness == 0) अनुवर्त;
        box a = dets[i].bbox;
        प्रति(j = i+1; j < total; ++j){
            यदि(dets[j].objectness == 0) अनुवर्त;
            box b = dets[j].bbox;
            यदि (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                प्रति(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


शून्य do_nms_sort(detection *dets, पूर्णाङ्क total, पूर्णाङ्क classes, भिन्न thresh)
{
    पूर्णाङ्क i, j, k;
    k = total-1;
    प्रति(i = 0; i <= k; ++i){
        यदि(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    प्रति(k = 0; k < classes; ++k){
        प्रति(i = 0; i < total; ++i){
            dets[i].sort_class = k;
        }
        क्विक(dets, total, परिमाण(detection), nms_comparator);
        प्रति(i = 0; i < total; ++i){
            यदि(dets[i].prob[k] == 0) अनुवर्त;
            box a = dets[i].bbox;
            प्रति(j = i+1; j < total; ++j){
                box b = dets[j].bbox;
                यदि (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_to_box(भिन्न *f, पूर्णाङ्क stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    प्रत्यागम b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    भिन्न l1 = a.x - a.w/2;
    भिन्न l2 = b.x - b.w/2;
    यदि (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    भिन्न r1 = a.x + a.w/2;
    भिन्न r2 = b.x + b.w/2;
    यदि(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    यदि (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    यदि (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    भिन्न t1 = a.y - a.h/2;
    भिन्न t2 = b.y - b.h/2;
    यदि (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    भिन्न b1 = a.y + a.h/2;
    भिन्न b2 = b.y + b.h/2;
    यदि(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    यदि (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    यदि (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    प्रत्यागम d;
}

भिन्न overlap(भिन्न x1, भिन्न w1, भिन्न x2, भिन्न w2)
{
    भिन्न l1 = x1 - w1/2;
    भिन्न l2 = x2 - w2/2;
    भिन्न बायाँ = l1 > l2 ? l1 : l2;
    भिन्न r1 = x1 + w1/2;
    भिन्न r2 = x2 + w2/2;
    भिन्न दायाँ = r1 < r2 ? r1 : r2;
    प्रत्यागम दायाँ - बायाँ;
}

भिन्न box_intersection(box a, box b)
{
    भिन्न w = overlap(a.x, a.w, b.x, b.w);
    भिन्न h = overlap(a.y, a.h, b.y, b.h);
    यदि(w < 0 || h < 0) प्रत्यागम 0;
    भिन्न area = w*h;
    प्रत्यागम area;
}

भिन्न box_union(box a, box b)
{
    भिन्न i = box_intersection(a, b);
    भिन्न u = a.w*a.h + b.w*b.h - i;
    प्रत्यागम u;
}

भिन्न box_iou(box a, box b)
{
    प्रत्यागम box_intersection(a, b)/box_union(a, b);
}

भिन्न box_rmse(box a, box b)
{
    प्रत्यागम वर्ग_मूल(घात(a.x-b.x, 2) + 
                घात(a.y-b.y, 2) + 
                घात(a.w-b.w, 2) + 
                घात(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    भिन्न w = overlap(a.x, a.w, b.x, b.w);
    भिन्न h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    प्रत्यागम di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    प्रत्यागम du;
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
    भिन्न inter =  box_union(a, b);
    भिन्न xinter = box_union(dxa, b);
    भिन्न yinter = box_union(dya, b);
    भिन्न winter = box_union(dwa, b);
    भिन्न hinter = box_union(dha, b);
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
    भिन्न inter =  box_intersection(a, b);
    भिन्न xinter = box_intersection(dxa, b);
    भिन्न yinter = box_intersection(dya, b);
    भिन्न winter = box_intersection(dwa, b);
    भिन्न hinter = box_intersection(dha, b);
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

    भिन्न iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    म_लिखो("%f\n", iou);
    dbox d = diou(a, b);
    म_लिखो("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    भिन्न xiou = box_iou(dxa, b);
    भिन्न yiou = box_iou(dya, b);
    भिन्न wiou = box_iou(dwa, b);
    भिन्न hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    म_लिखो("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    भिन्न u = box_union(a,b);
    भिन्न i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    यदि(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        प्रत्यागम dd;
    }

    dd.dx = 2*घात((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*घात((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*घात((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*घात((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    प्रत्यागम dd;
}


शून्य do_nms(box *boxes, भिन्न **probs, पूर्णाङ्क total, पूर्णाङ्क classes, भिन्न thresh)
{
    पूर्णाङ्क i, j, k;
    प्रति(i = 0; i < total; ++i){
        पूर्णाङ्क any = 0;
        प्रति(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        यदि(!any) {
            अनुवर्त;
        }
        प्रति(j = i+1; j < total; ++j){
            यदि (box_iou(boxes[i], boxes[j]) > thresh){
                प्रति(k = 0; k < classes; ++k){
                    यदि (probs[i][k] < probs[j][k]) probs[i][k] = 0;
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
    प्रत्यागम encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = घात(2., b.w) * anchor.w;
    decode.h = घात(2., b.h) * anchor.h;
    प्रत्यागम decode;
}
