#include "evolution_similarity_average.h"

int active_condition_sa(axl_network *mysys, int i, int j)
{
        if(mysys->a[i][j] == 1)
        {
                if((mysys->corr[i][j] > EPSILON) && (mysys->corr[i][j] < (mysys->f - EPSILON)))
                        return 1;
        }

        return 0;
}

int number_of_active_links_sa(axl_network *mysys)
{
        int i, j;
        int n = mysys->nagents;
        int number_active_links = 0;

        for(i = 0; i < n; i++)
        {
                for(j = 0; j < n; j++)
                {
                        if(active_condition_sa(mysys, i, j))
                                number_active_links++;
                }
        }
        return number_active_links;
}

int is_there_active_links_sa(axl_network *mysys)
{
        int i, j;
        int n = mysys->nagents;

        for(i = 0; i < n; i++)
        {
                for(j = 0; j < n; j++)
                {
                        if(active_condition_sa(mysys, i, j))
                                return 1;
                }
        }
        return 0;
}

int active_links_sa(axl_network *mysys, active_link *list_active_links)
{
        int i, j, k;
        int n = mysys->nagents;

        k = 0;
        for(i = 0; i < n; i++)
        {
                for(j = 0; j < n; j++)
                {
                        if(active_condition_sa(mysys, i, j))
                        {
                                list_active_links[k].source = i;
                                list_active_links[k].target = j;
                                k++;
                        }
                }
        }

        return 1;
}

int evolution_similarity_average(axl_network *mysys)
{
	int i, j, k;
	int step_n = 0;
	int number_active_links = 0;
	active_link *list_active_links;

	srand(mysys->seed);

	number_active_links = number_of_active_links_sa(mysys);
	if(number_active_links == 0)
		return 1;
	else
	{
		list_active_links = (active_link *)malloc(sizeof(active_link) * number_active_links);
		active_links_sa(mysys, list_active_links);
	}
			
	while(step_n < number_active_links)
	{
		k = rand() % number_active_links;
		i = list_active_links[k].source;
		j = list_active_links[k].target;

		if(active_condition_sa(mysys, i, j) == 1)
			increase_similarity(mysys, i, j);

		step_n++;
	}

	free(list_active_links);

        mysys->seed = rand();

	return 1;
}

int increase_similarity(axl_network *mysys, int i, int j)
{
	int k;
	int n = mysys->nagents;
	double aux, factor;

	aux = mysys->corr[i][j] + (mysys->corr[i][j]/mysys->f);
	if(aux >= mysys->f)
		aux = mysys->f;	
	if(aux < mysys->f)
	{
		factor = (mysys->corr[i][j]/mysys->f) / (mysys->f - mysys->corr[i][j]);
		for(k = 0; k < n; k++)
		{
			if((k!=i) && (k!=j))
			{
				mysys->corr[i][k] += (mysys->corr[j][k] - mysys->corr[i][k]) * factor;
				mysys->corr[k][i] = mysys->corr[i][k];
			}
		}
	}	
	else if(aux == 1.00)
	{
		for(k = 0; k < n; k++)
		{
			if((k!=i) && (k!=j))
			{
				mysys->corr[i][k] = mysys->corr[j][k];	
				mysys->corr[k][i] = mysys->corr[i][k];	
			}
		}
	}

	mysys->corr[i][j] = aux;
	mysys->corr[j][i] = mysys->corr[i][j];

	return 1;
}

