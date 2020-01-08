#ifndef EVOLUTION_SIMILARITY_AVERAGE_H
#define EVOLUTION_SIMILARITY_AVERAGE_H

#include <stdio.h>
#include <stdlib.h>
#include "axelrod.h"

#define EPSILON 10E-5

int increase_similarity(axl_network *, int, int);
int is_there_active_links(axl_network *);

int evolution_similarity_average(axl_network *);

#endif
