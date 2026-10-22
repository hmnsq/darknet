#അന്തര്ഭാവ "box.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <stdlib.h>

പൂർണ്ണസംഖ്യ nms_comparator(സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    ദശാംശം diff = 0;
    എങ്കിൽ(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } അല്ലെങ്കിൽ {
        diff = a.objectness - b.objectness;
    }
    എങ്കിൽ(diff < 0) തിരികെ 1;
    അല്ലെങ്കിൽ എങ്കിൽ(diff > 0) തിരികെ -1;
    തിരികെ 0;
}

ശൂന്യം do_nms_obj(detection *dets, പൂർണ്ണസംഖ്യ total, പൂർണ്ണസംഖ്യ classes, ദശാംശം thresh)
{
    പൂർണ്ണസംഖ്യ i, j, k;
    k = total-1;
    വേണ്ടി(i = 0; i <= k; ++i){
        എങ്കിൽ(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    വേണ്ടി(i = 0; i < total; ++i){
        dets[i].sort_class = -1;
    }

    qsort(dets, total, വലുപ്പം(detection), nms_comparator);
    വേണ്ടി(i = 0; i < total; ++i){
        എങ്കിൽ(dets[i].objectness == 0) തുടരുക;
        box a = dets[i].bbox;
        വേണ്ടി(j = i+1; j < total; ++j){
            എങ്കിൽ(dets[j].objectness == 0) തുടരുക;
            box b = dets[j].bbox;
            എങ്കിൽ (box_iou(a, b) > thresh){
                dets[j].objectness = 0;
                വേണ്ടി(k = 0; k < classes; ++k){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}


ശൂന്യം do_nms_sort(detection *dets, പൂർണ്ണസംഖ്യ total, പൂർണ്ണസംഖ്യ classes, ദശാംശം thresh)
{
    പൂർണ്ണസംഖ്യ i, j, k;
    k = total-1;
    വേണ്ടി(i = 0; i <= k; ++i){
        എങ്കിൽ(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    വേണ്ടി(k = 0; k < classes; ++k){
        വേണ്ടി(i = 0; i < total; ++i){
            dets[i].sort_class = k;
        }
        qsort(dets, total, വലുപ്പം(detection), nms_comparator);
        വേണ്ടി(i = 0; i < total; ++i){
            എങ്കിൽ(dets[i].prob[k] == 0) തുടരുക;
            box a = dets[i].bbox;
            വേണ്ടി(j = i+1; j < total; ++j){
                box b = dets[j].bbox;
                എങ്കിൽ (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

box float_to_box(ദശാംശം *f, പൂർണ്ണസംഖ്യ stride)
{
    box b = {0};
    b.x = f[0];
    b.y = f[1*stride];
    b.w = f[2*stride];
    b.h = f[3*stride];
    തിരികെ b;
}

dbox derivative(box a, box b)
{
    dbox d;
    d.dx = 0;
    d.dw = 0;
    ദശാംശം l1 = a.x - a.w/2;
    ദശാംശം l2 = b.x - b.w/2;
    എങ്കിൽ (l1 > l2){
        d.dx -= 1;
        d.dw += .5;
    }
    ദശാംശം r1 = a.x + a.w/2;
    ദശാംശം r2 = b.x + b.w/2;
    എങ്കിൽ(r1 < r2){
        d.dx += 1;
        d.dw += .5;
    }
    എങ്കിൽ (l1 > r2) {
        d.dx = -1;
        d.dw = 0;
    }
    എങ്കിൽ (r1 < l2){
        d.dx = 1;
        d.dw = 0;
    }

    d.dy = 0;
    d.dh = 0;
    ദശാംശം t1 = a.y - a.h/2;
    ദശാംശം t2 = b.y - b.h/2;
    എങ്കിൽ (t1 > t2){
        d.dy -= 1;
        d.dh += .5;
    }
    ദശാംശം b1 = a.y + a.h/2;
    ദശാംശം b2 = b.y + b.h/2;
    എങ്കിൽ(b1 < b2){
        d.dy += 1;
        d.dh += .5;
    }
    എങ്കിൽ (t1 > b2) {
        d.dy = -1;
        d.dh = 0;
    }
    എങ്കിൽ (b1 < t2){
        d.dy = 1;
        d.dh = 0;
    }
    തിരികെ d;
}

ദശാംശം overlap(ദശാംശം x1, ദശാംശം w1, ദശാംശം x2, ദശാംശം w2)
{
    ദശാംശം l1 = x1 - w1/2;
    ദശാംശം l2 = x2 - w2/2;
    ദശാംശം ഇടത് = l1 > l2 ? l1 : l2;
    ദശാംശം r1 = x1 + w1/2;
    ദശാംശം r2 = x2 + w2/2;
    ദശാംശം വലത് = r1 < r2 ? r1 : r2;
    തിരികെ വലത് - ഇടത്;
}

ദശാംശം box_intersection(box a, box b)
{
    ദശാംശം w = overlap(a.x, a.w, b.x, b.w);
    ദശാംശം h = overlap(a.y, a.h, b.y, b.h);
    എങ്കിൽ(w < 0 || h < 0) തിരികെ 0;
    ദശാംശം area = w*h;
    തിരികെ area;
}

ദശാംശം box_union(box a, box b)
{
    ദശാംശം i = box_intersection(a, b);
    ദശാംശം u = a.w*a.h + b.w*b.h - i;
    തിരികെ u;
}

ദശാംശം box_iou(box a, box b)
{
    തിരികെ box_intersection(a, b)/box_union(a, b);
}

ദശാംശം box_rmse(box a, box b)
{
    തിരികെ sqrt(pow(a.x-b.x, 2) + 
                pow(a.y-b.y, 2) + 
                pow(a.w-b.w, 2) + 
                pow(a.h-b.h, 2));
}

dbox dintersect(box a, box b)
{
    ദശാംശം w = overlap(a.x, a.w, b.x, b.w);
    ദശാംശം h = overlap(a.y, a.h, b.y, b.h);
    dbox dover = derivative(a, b);
    dbox di;

    di.dw = dover.dw*h;
    di.dx = dover.dx*h;
    di.dh = dover.dh*w;
    di.dy = dover.dy*w;

    തിരികെ di;
}

dbox dunion(box a, box b)
{
    dbox du;

    dbox di = dintersect(a, b);
    du.dw = a.h - di.dw;
    du.dh = a.w - di.dh;
    du.dx = -di.dx;
    du.dy = -di.dy;

    തിരികെ du;
}


ശൂന്യം test_dunion()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dunion(a,b);
    printf("Union: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    ദശാംശം inter =  box_union(a, b);
    ദശാംശം xinter = box_union(dxa, b);
    ദശാംശം yinter = box_union(dya, b);
    ദശാംശം winter = box_union(dwa, b);
    ദശാംശം hinter = box_union(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Union Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}
ശൂന്യം test_dintersect()
{
    box a = {0, 0, 1, 1};
    box dxa= {0+.0001, 0, 1, 1};
    box dya= {0, 0+.0001, 1, 1};
    box dwa= {0, 0, 1+.0001, 1};
    box dha= {0, 0, 1, 1+.0001};

    box b = {.5, .5, .2, .2};
    dbox di = dintersect(a,b);
    printf("Inter: %f %f %f %f\n", di.dx, di.dy, di.dw, di.dh);
    ദശാംശം inter =  box_intersection(a, b);
    ദശാംശം xinter = box_intersection(dxa, b);
    ദശാംശം yinter = box_intersection(dya, b);
    ദശാംശം winter = box_intersection(dwa, b);
    ദശാംശം hinter = box_intersection(dha, b);
    xinter = (xinter - inter)/(.0001);
    yinter = (yinter - inter)/(.0001);
    winter = (winter - inter)/(.0001);
    hinter = (hinter - inter)/(.0001);
    printf("Inter Manual %f %f %f %f\n", xinter, yinter, winter, hinter);
}

ശൂന്യം test_box()
{
    test_dintersect();
    test_dunion();
    box a = {0, 0, 1, 1};
    box dxa= {0+.00001, 0, 1, 1};
    box dya= {0, 0+.00001, 1, 1};
    box dwa= {0, 0, 1+.00001, 1};
    box dha= {0, 0, 1, 1+.00001};

    box b = {.5, 0, .2, .2};

    ദശാംശം iou = box_iou(a,b);
    iou = (1-iou)*(1-iou);
    printf("%f\n", iou);
    dbox d = diou(a, b);
    printf("%f %f %f %f\n", d.dx, d.dy, d.dw, d.dh);

    ദശാംശം xiou = box_iou(dxa, b);
    ദശാംശം yiou = box_iou(dya, b);
    ദശാംശം wiou = box_iou(dwa, b);
    ദശാംശം hiou = box_iou(dha, b);
    xiou = ((1-xiou)*(1-xiou) - iou)/(.00001);
    yiou = ((1-yiou)*(1-yiou) - iou)/(.00001);
    wiou = ((1-wiou)*(1-wiou) - iou)/(.00001);
    hiou = ((1-hiou)*(1-hiou) - iou)/(.00001);
    printf("manual %f %f %f %f\n", xiou, yiou, wiou, hiou);
}

dbox diou(box a, box b)
{
    ദശാംശം u = box_union(a,b);
    ദശാംശം i = box_intersection(a,b);
    dbox di = dintersect(a,b);
    dbox du = dunion(a,b);
    dbox dd = {0,0,0,0};

    എങ്കിൽ(i <= 0 || 1) {
        dd.dx = b.x - a.x;
        dd.dy = b.y - a.y;
        dd.dw = b.w - a.w;
        dd.dh = b.h - a.h;
        തിരികെ dd;
    }

    dd.dx = 2*pow((1-(i/u)),1)*(di.dx*u - du.dx*i)/(u*u);
    dd.dy = 2*pow((1-(i/u)),1)*(di.dy*u - du.dy*i)/(u*u);
    dd.dw = 2*pow((1-(i/u)),1)*(di.dw*u - du.dw*i)/(u*u);
    dd.dh = 2*pow((1-(i/u)),1)*(di.dh*u - du.dh*i)/(u*u);
    തിരികെ dd;
}


ശൂന്യം do_nms(box *boxes, ദശാംശം **probs, പൂർണ്ണസംഖ്യ total, പൂർണ്ണസംഖ്യ classes, ദശാംശം thresh)
{
    പൂർണ്ണസംഖ്യ i, j, k;
    വേണ്ടി(i = 0; i < total; ++i){
        പൂർണ്ണസംഖ്യ any = 0;
        വേണ്ടി(k = 0; k < classes; ++k) any = any || (probs[i][k] > 0);
        എങ്കിൽ(!any) {
            തുടരുക;
        }
        വേണ്ടി(j = i+1; j < total; ++j){
            എങ്കിൽ (box_iou(boxes[i], boxes[j]) > thresh){
                വേണ്ടി(k = 0; k < classes; ++k){
                    എങ്കിൽ (probs[i][k] < probs[j][k]) probs[i][k] = 0;
                    അല്ലെങ്കിൽ probs[j][k] = 0;
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
    തിരികെ encode;
}

box decode_box(box b, box anchor)
{
    box decode;
    decode.x = b.x * anchor.w + anchor.x;
    decode.y = b.y * anchor.h + anchor.y;
    decode.w = pow(2., b.w) * anchor.w;
    decode.h = pow(2., b.h) * anchor.h;
    തിരികെ decode;
}
