#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*------ G(D) = (1+D^2+D^3+D^5+D^6  1+D+D^2+D^3+D^6) -----*/
int main()
{
	int i, j, l, g,size;
	int *u, *x1, *x2, *x;
	int *state;

	
	FILE *fpr;
	
	fpr = fopen("Sim.txt", "r");
	fscanf(fpr, "%d", &l);
	printf("L = %d\n", l);
	fscanf(fpr, "%*[^\n]", &g);
	fclose(fpr);
	
	printf("\nu vector\n");
/*-----------Create u vector-----------*/  
	size = ((l+6)/64)+1;
	printf("\nsize = %d\n",size);
	u = malloc(sizeof(int)*64*size);
	u[0] = 1;
	for(int i = 1; i < 6; i++)
	{
		u[i] = 0;
	}
    for(int i = 6; i < 64*size; i++)
	{
		u[i] = u[i-6]^u[i-5];
	}
	for(i = 0; i < 64*size/8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			printf("%d ",u[j+8*i]);
		}
		printf("\n");
	}
	/*for(i = 0; i < 64; i++)
	{
		u[i] = 1;
		printf("%d", u[i]);
	}*/
	
/*-----------Create state regester-----------*/ 
	state = malloc(sizeof(int)*6);
/*-----------Create x1 vector-----------*/ 
	x1 = malloc(sizeof(int)*(l+6));
/*-----------Create x2 vector-----------*/ 
	x2 = malloc(sizeof(int)*(l+6));
/*-----------Create final x vector-----------*/ 
	x = malloc(sizeof(int)*(2*(l+6)));
/*-----------Initialize state regester-----------*/ 
	printf("initial state = ");
	for(i = 0; i < 6; i++)
	{
		state[i] = 0;
		printf("%d", state[i]);
	}
	printf("\n");
	
	for(i = 0; i < l; i++)
	{
		x1[i] = u[i] ^ state[1] ^ state[2] ^ state[4] ^ state[5];
		x2[i] = u[i] ^ state[0] ^ state[1] ^ state[2] ^ state[5];
		printf("x1 = %d, x2 = %d\n", x1[i], x2[i]);
		
		// updata state
		state[5] = state[4];
		state[4] = state[3];
		state[3] = state[2];
		state[2] = state[1];
		state[1] = state[0];
		state[0] = u[i];
		x[2*i] = x1[i]?-1:1;
		x[2*i+1] = x2[i]?-1:1;
		printf("x1 = %d, x2 = %d\n", x[2*i], x[2*i+1]);
		
		printf("state = ");
		for(j = 0; j < 6; j++)
		{
			printf("%d", state[j]);
		}
		printf("\n");
	}
	printf("\nEnding\n");
	for(i = 0; i < 6; i++)
	{
		x1[l+i] = 0 ^ state[1] ^ state[2] ^ state[4] ^ state[5];
		x2[l+i] = 0 ^ state[0] ^ state[1] ^ state[2] ^ state[5];
		printf("x1 = %d, x2 = %d\n", x1[l+i], x2[l+i]);
		
		// updata state
		state[5] = state[4];
		state[4] = state[3];
		state[3] = state[2];
		state[2] = state[1];
		state[1] = state[0];
		state[0] = 0;
		x[2*l+2*i] = x1[l+i]?-1:1;
		x[2*l+2*i+1] = x2[l+i]?-1:1;
		printf("x1 = %d, x2 = %d\n", x[2*l+2*i], x[2*l+2*i+1]);
		
		printf("state = ");
		for(j = 0; j < 6; j++)
		{
			printf("%d", state[j]);
		}
		printf("\n");
	}
	
	//開檔
    fpr=fopen("x.txt","w");
	for(i = 0; i < 2*(l + 6); i++)
	{
		printf("x[%d] = %d\n", i+1, x[i]);
		fprintf(fpr,"%d ", x[i]);
	}
	fprintf(fpr,"%%2*(%d+6) elements", l);
	
 //關檔
	fclose(fpr);
	
	return 0;
}