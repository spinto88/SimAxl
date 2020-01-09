#include "check_triangle_inequality.h"

int check_triangle_inequality(axl_network mysys)
{
	int i,j,k;
	int n = mysys.nagents;

	for(i = 0; i < n; i++)
	{
		for(j = i+1; j < n; j++)
		{
			for(k = j+1; k < n; k++)
			{
				if((mysys.corr[i][j]+mysys.f) < mysys.corr[i][k] + mysys.corr[j][k])
					return 0;
				if((mysys.corr[i][j]+fabs(mysys.corr[i][k] - mysys.corr[j][k])) > mysys.f)
					return 0;

			}
		}

	}
	return 1;
}
