#include "evolution_axelrod.h"

int homophily(axl_agent a, axl_agent b)
{
        int i;
        int f = a.f; /* Number of features.*/
        int hab = 0; /* Homophily.*/

        /* Go over all features. */
        for(i = 0; i < f; i++)
        {
                /* If a given feature is shared, the homophily increases. */
                if(a.feat[i] == b.feat[i])
                        hab += 1;
        }

        /* Return the number of shared features */
        return hab;
}


int active_condition_a(axl_agent a, axl_agent b)
{
	int hab;

	hab = homophily(a, b);

	if((hab > 0) && (hab < a.f))
		return 1;
	else
		return 0;
}

int number_of_active_links_a(axl_network* mysys)
{
	int i, j;
	int count = 0;
        int n = mysys->nagents;

	for(i = 0; i < mysys->nagents; i++)
	{
		for(j = 0; j < n; j++)
		{
			if(mysys->a[i][j])
			{
				if(active_condition_a(mysys->agent[i], mysys->agent[j]))
					count++;
			}
		}
	}
	
	return count;
}

int is_there_active_links_a(axl_network* mysys)
{
	int i, j;
        int n = mysys->nagents;

	for(i = 0; i < mysys->nagents; i++)
	{
		for(j = 0; j < n; j++)
		{
			if(mysys->a[i][j])
			{
				if(active_condition_a(mysys->agent[i], mysys->agent[j]))
					return 1;
			}
		}
	}
	
	return 0;
}


void evolution_axelrod(axl_network *mysys)
{
	int i, j, k, r;	
	int step_n, h_ab;
	int n_active_links;
	active_link *active_links;

	/* Set the random seed */
	srand(mysys->seed);

	/* Look for active links */
	n_active_links = number_of_active_links_a(mysys);

	active_links = (active_link *)malloc(n_active_links * sizeof(active_link));

	k = 0;
	for(i = 0; i < mysys->nagents; i++)
	{
		for(j = 0; j < mysys->nagents; j++)
		{
			if(mysys->a[i][j])
			{
				if(active_condition_a(mysys->agent[i], mysys->agent[j]))
				{
					active_links[k].source = i;
					active_links[k].target = j;
					k++;
				}
			}
		}
	}
	
	for(step_n = 0; step_n < n_active_links; step_n++)
	{
		/* Choose a random agent */
		k = rand() % n_active_links;

		i = active_links[k].source;
		j = active_links[k].target;

		/* Compute the homophily */
		h_ab = homophily(mysys->agent[i], mysys->agent[j]);
					   
		/* Take a random number */ 
    		r = rand() % mysys->f;

		/* If the interaction takes place, go into the next if */
	 	if((r < h_ab) && (active_condition_a(mysys->agent[i], mysys->agent[j])))
		{
			/* Take a random feature */
			r = rand() % mysys->agent[i].f;

			/* If the two agents share this feature, take the closest not equal */
			while(mysys->agent[i].feat[r] == mysys->agent[j].feat[r])
				r = (r+1) % mysys->agent[i].f;
      	    			        
			/* Agent i copies a feature from agent j */
			mysys->agent[i].feat[r] = mysys->agent[j].feat[r];
		}

		mysys->seed = rand();
	}

	free(active_links);

	return;
}
