#include "evolution_triangle_percolation.h"

int active_condition_tp(axl_network *mysys, int i, int j)
{
        if((mysys->a[i][j] == 1) && (mysys->corr[i][j] > EPSILON))
		return 1;
	else
	        return 0;
}

int number_of_active_links_tp(axl_network *mysys)
{
        int i, j;
        int n = mysys->nagents;
        int number_active_links = 0;

        for(i = 0; i < n; i++)
        {
                for(j = 0; j < n; j++)
                {
                        if(active_condition_tp(mysys, i, j))
                                number_active_links++;
                }
        }
        return number_active_links;
}

int active_links_tp(axl_network *mysys, active_link *list_active_links)
{
        int i, j, k;
        int n = mysys->nagents;

        k = 0;
        for(i = 0; i < n; i++)
        {
                for(j = 0; j < n; j++)
                {
                        if(active_condition_tp(mysys, i, j))
                        {
                                list_active_links[k].source = i;
                                list_active_links[k].target = j;
                                k++;
                        }
                }
        }

        return 1;
}

int evolution_triangle_percolation(axl_network *mysys)
{
	int i, j, k, l;
	int number_active_links = 0;
	active_link *list_active_links;
	int changes = 1;

	while(changes != 0)
	{
		changes = 0;

		number_active_links = number_of_active_links_tp(mysys);
		if(number_active_links == 0)
			return 1;
		else
		{
			list_active_links = (active_link *)malloc(sizeof(active_link) * number_active_links);
			active_links_tp(mysys, list_active_links);
		}
			
		for(k = 0; k < number_active_links; k++)
		{
			i = list_active_links[k].source;
			j = list_active_links[k].target;

			if(active_condition_tp(mysys, i, j) == 1)
			{
				mysys->corr[i][j] = mysys->f;
				mysys->corr[j][i] = mysys->corr[i][j];

				for(l = 0; l < mysys->nagents; l++)
				{
					if((l != i) && (l != j))
					{
						if((mysys->corr[i][l] == 0.00) && (mysys->corr[j][l] == 0.00))
							continue;
						else
						{
							if(mysys->corr[i][l] != mysys->corr[j][l])
							{
								mysys->corr[i][l] = mysys->f;
								mysys->corr[j][l] = mysys->f;

								mysys->corr[l][i] = mysys->corr[i][l];
								mysys->corr[l][j] = mysys->corr[j][l];

								changes += 1;
							}
						}
				
					}
						
				}

			}
	
		}

		free(list_active_links);
	}

	return 1;
}

