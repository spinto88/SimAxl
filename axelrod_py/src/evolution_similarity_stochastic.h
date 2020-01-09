#ifndef EVOLUTION_SIMILARITY_STOCHASTIC_H
#define EVOLUTION_SIMILARITY_STOCHASTIC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "axelrod.h"

#define EPSILON 10E-5

int increase_similarity_ss(axl_network *, int, int);
int is_there_active_links_ss(axl_network *);

int evolution_similarity_stochastic(axl_network *);

#endif
