#ifndef PARSER_H
#define PARSER_H
#include "darknet.h"
#include "network.h"

void save_network(network net, var_na *filename);
void save_weights_double(network net, var_na *filename);

#endif
