#ifndef EVOLUTION_AXELROD_H
#define EVOLUTION_AXELROD_H

#include "axelrod.h"

int homophily(axl_agent, axl_agent);
int is_there_active_links(axl_agent *);

void evolution_axelrod(axl_network *);

#endif
