#include "evolution_similarity_stochastic.h"

int active_condition_ss(axl_network *mysys, int i, int j)
{
        if(mysys->a[i][j] == 1)
        {
                if((mysys->corr[i][j] > EPSILON) && (mysys->corr[i][j] < (mysys->f - EPSILON)))
                        return 1;
        }

        return 0;
}

int number_of_active_links_ss(axl_network *mysys)
{
        int i, j;
        int n = mysys->nagents;
        int number_active_links = 0;

        for(i = 0; i < n; i++)
        {
                for(j = 0; j < n; j++)
                {
                        if(active_condition_ss(mysys, i, j))
                                number_active_links++;
                }
        }
        return number_active_links;
}

int is_there_active_links_ss(axl_network *mysys)
{
        int i, j;
        int n = mysys->nagents;

        for(i = 0; i < n; i++)
        {
                for(j = 0; j < n; j++)
                {
                        if(active_condition_ss(mysys, i, j))
                                return 1;
                }
        }
        return 0;
}

int active_links_ss(axl_network *mysys, active_link *list_active_links)
{
        int i, j, k;
        int n = mysys->nagents;

        k = 0;
        for(i = 0; i < n; i++)
        {
                for(j = 0; j < n; j++)
                {
                        if(active_condition_ss(mysys, i, j))
                        {
                                list_active_links[k].source = i;
                                list_active_links[k].target = j;
                                k++;
                        }
                }
        }

        return 1;
}

int overlap(int f, double n, double m, double l)
{
	int r;
	int most_probable_r = 0;
	double distance = f*f;
	double distance_aux;

	for(r = 0; r <= f; r++)
	{
		if(((r >= 0.00) && (r >= (m+n+l-f)/2)) && ((r <= n)&&(r <= m)&&(r <= l)))
		{
			distance_aux = fabs((m-r)*(n-r)*(l-r) - r*(f-m-n-l+2*r)*(f-m-n-l+2*r));
			if(distance_aux < distance)
			{
				distance = distance_aux;
				most_probable_r = r;
			}
		}

	}
	
	return most_probable_r;
}

int evolution_similarity_stochastic(axl_network *mysys)
{
	int i, j, k;
	int step_n = 0;
	int number_active_links = 0;
	active_link *list_active_links;

	srand(mysys->seed);

	number_active_links = number_of_active_links_ss(mysys);
	if(number_active_links == 0)
		return 1;
	else
	{
		list_active_links = (active_link *)malloc(sizeof(active_link) * number_active_links);
		active_links_ss(mysys, list_active_links);
	}
			
	while(step_n < number_active_links)
	{
		k = rand() % number_active_links;
		i = list_active_links[k].source;
		j = list_active_links[k].target;

		if(active_condition_ss(mysys, i, j) == 1)
			increase_similarity_ss(mysys, i, j);

		step_n++;
	}

	free(list_active_links);

        mysys->seed = rand();

	return 1;
}

double up_rate(int f, double m, double n, double l)
{
	double t;
	int r = overlap(f,m,n,l);
	t = ((double)(m-r))/(f-n);

	if(t < EPSILON)
		t = 0.00;

	return t;
}

double down_rate(int f, double m, double n, double l)
{	
	double t;
	int r = overlap(f,m,n,l);
	t = ((double)(l-r))/(f-n);
	
	if(t < EPSILON)
		t = 0.00;

	return t;
}

int increase_similarity_ss(axl_network *mysys, int i, int j)
{
	int k;
	int n = mysys->nagents;
	double random;
	int r;
	double t1, t2;

	srand(mysys->seed);

	r = rand() % mysys->f;
	if(r < mysys->corr[i][j])
	{
		for(k = 0; k < n; k++)
		{
			if((k!=i) && (k!=j))
			{

				random = ((double)rand())/RAND_MAX;

				// Tasa de subida
				t1 = up_rate(mysys->f, mysys->corr[j][k], mysys->corr[i][j], mysys->corr[i][k]);
				// Tasa de bajada
				t2 = down_rate(mysys->f, mysys->corr[j][k], mysys->corr[i][j], mysys->corr[i][k]);

				// Sube con proba tanto
				if((random < t1) && (mysys->corr[i][k] < mysys->f - EPSILON))
				{
					mysys->corr[i][k] += 1.00;
					if(mysys->corr[i][k] > mysys->f)
						mysys->corr[i][k] = mysys->f;
				}


				// Baja con proba tanto 
				else if((random < (t1+t2)) && (random > t1) && (mysys->corr[i][k] > EPSILON))
				{
					mysys->corr[i][k] -= 1.00;
					if(mysys->corr[i][k] < 0.00)
						mysys->corr[i][k] = 0.00;
				}

				mysys->corr[k][i] = mysys->corr[i][k];
			}
		}

		mysys->corr[i][j] += 1.00;
		if(mysys->corr[i][j] > mysys->f)
			mysys->corr[i][j] = mysys->f;

		mysys->corr[j][i] = mysys->corr[i][j];
	}	

        mysys->seed = rand();

	return 1;
}

