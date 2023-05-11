#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)
void M_seq(int,int,int*);//訊號長度 ,errorpattern數
float ran1(int*);
int main(){
/*input file*/
	FILE *ff ,*gg ;
	int x,i,j,k;
	char c_temp;
	int i_temp=0;
	int index, sum=0;
	int count=0 ;
	int trun;
	float SNR;
	int Seed;
	int HorS_decision;
	int decode_bits;
	/*開Sim.txt檔案*/
	ff = fopen("Sim.txt","r");
	fscanf(ff,"%d",&decode_bits);//decode_bits is message length
	fscanf(ff,"%*[^\n]",&c_temp);
	fscanf(ff,"%d",&trun);
	fscanf(ff,"%*[^\n]",&c_temp);
	fscanf(ff,"%f",&SNR);
	fscanf(ff,"%*[^\n]",&c_temp);
	fscanf(ff,"%d",&Seed);
	fscanf(ff,"%*[^\n]",&c_temp);
	fscanf(ff,"%d",&HorS_decision);
	printf("SNR= %f\n",SNR);
	printf("the number of bits =%d\n",decode_bits);
	int *message = calloc(decode_bits,sizeof(int));
	M_seq(decode_bits,1,message); //mseq本身就是*型態

/*input data given by Sim.txt*/
//debug1
	// printf("dubug1\n");
		// for(i=0; i<decode_bits; i++){
			// printf("message=%d\n",message[i]);
		// }
/*create empty state vector and it is initial state*/
/*get x1 x2 */
	int* x1 = calloc(decode_bits,sizeof(int));
	int* x2 = calloc(decode_bits,sizeof(int));
	int* state = calloc(6,sizeof(int));
	int* sms = calloc(6,sizeof(int));
	int* state_mapping_shift = calloc(6,sizeof(int));
	 //x1=1+   d2+d3+d5+d6
    //x2=1+d1+d2+d3+   d6
	for(j=0;j<decode_bits;j++){
		x1[j]=message[j]^state[1]^state[2]^state[4]^state[5];
		x2[j]=message[j]^state[0]^state[1]^state[2]^state[5];
		for(i=0; i<decode_bits; i++){
			// printf("ddda=%d\n",x1[i]);
		}
		for(i=5;i>=1;i--){
			state[i]=state[i-1];
			// printf("state5~1=%d\n",state[i]);
		}
		// printf("state0=%d\n",state[0]);
		state[0]=message[j];
	}
	x=0;
	int m=6 ;
	int length_Gcode;
	int* m_zero_vector = calloc(m,sizeof(int)); //m=0 maybe cause some problems****************
	float* Gcode = calloc(m+m+decode_bits+decode_bits,sizeof(float));
	float* G_Ncode = calloc(m+m+decode_bits+decode_bits,sizeof(float));
	int* m_x1 = realloc(x1,sizeof(int)*(m+decode_bits));
	int* m_x2 = realloc(x2,sizeof(int)*(m+decode_bits));
	x1 = m_x1 ;
	x2 = m_x2 ;
/*initaiate state machine and generate corresponding code*/
	for(j=decode_bits;j<decode_bits+m;j++){
		x1[j]=m_zero_vector[j-decode_bits]^state[1]^state[2]^state[4]^state[5];
		x2[j]=m_zero_vector[j-decode_bits]^state[0]^state[1]^state[2]^state[5];
		for(i=5;i>=1;i--){
			state[i]=state[i-1];
			// printf("state5~1=%d\n",state[i]);
		}
		// printf("state0=%d\n",state[0]);
		state[0]=m_zero_vector[j-decode_bits];
	}
/*concatention*/
	for(i=0;i<m+decode_bits;i++){
		Gcode[2*i] = x1[i] ;
		Gcode[2*i+1] = x2[i] ;
	}
	length_Gcode=m+m+decode_bits+decode_bits;
/*mapping to BPSK*/
	for(i=0;i<m+m+decode_bits+decode_bits;i++){
		Gcode[i] = (Gcode[i])? -1:1 ;
	}
	
/*generate noise*/
	float *result_n1,*result_n2 ;
	float sd1_pow2,sd1;
	float s;
	float r1,r2;
	int *idum;
	idum = &Seed;
	
	result_n1 = malloc(sizeof(float));
    result_n2 = malloc(sizeof(float));
    sd1_pow2 = pow(pow(10,0.1),-1);
    sd1 = sqrt(sd1_pow2);
	//add noise to message
    for (i=0;i<decode_bits+m;i++){
        do{
          r1=ran1(idum);
          r2=ran1(idum);
          r1 = 2*r1-1;
          r2 = 2*r2-1;
          s = pow(r1,2)+pow(r2,2);
      } while(s>=1.0);
    *result_n1 = sd1*r1*sqrt(-2*log(s)/s);
    *result_n2 = sd1*r2*sqrt(-2*log(s)/s);
	
/*add noise*/
	G_Ncode[i+i] = Gcode[i+i] + *result_n1 ;
	G_Ncode[i+i+1] = Gcode[i+i+1] + *result_n2 ;
    
    // printf("result_n1= %.6f\n",*result_n1);
    // printf("result_n2= %.6f\n",*result_n2);
	// printf("Gcode[%d]= %.4f\n",2*i,Gcode[2*i]);
	// printf("Gcode[%d]= %.4f\n",2*i+1,Gcode[2*i+1]);

    }//for loop end
/*hard or soft decision*/
	if(!HorS_decision){
		for(i=0;i<m+m+decode_bits+decode_bits;i++){
			G_Ncode[i]=G_Ncode[i]>0?1:-1;
			printf("G_Ncode[%d]= %.4f\n",i,G_Ncode[i]);
		}
	}
/* write in file*/
	printf("\n");
	gg = fopen("x.txt","w"); 		
	for (int i=0;i<m+m+decode_bits+decode_bits;i++){
		fprintf(gg," %.0f",G_Ncode[i]);
	}
		fprintf(gg," %%2*(%d+%d) elements\n",decode_bits,m);

/*generate 64 path for initial condition*/
	int **path = calloc(64,sizeof(int*));
	for(i=0;i<64;i++){
		path[i] = calloc(32,sizeof(int));
	}
	// for (int i=0;i<64;i++){
		// for (int j=0;j<32;j++){
			// printf("%d ",path[i][j]);
		// }
		// printf("\n");
	// }
	
}//main end 	
	

void M_seq(int messagelength,int c,int *mseq){ //input input output
	int u[6]={1,0,0,0,0,0},temp,i;
	temp = messagelength*c;
	// mseq=calloc(temp,sizeof(int));  //這個不用打 不然函數會出錯
	for(i=0; i<6; i++){
		mseq[i]=u[i];
	}
	for(i=6; i<temp; i++){
		mseq[i]=mseq[i-5]^mseq[i-6];
	}
}
float ran1(int *idum){ //return decode_bits float value not *float
    int j;
    long k;
    static long iy=0; //the static declaration will remain the value when the process ran out of the function.
    static long iv[NTAB];
    float temp;
 
    if (*idum <= 0 || !iy) {
        if (-(*idum) < 1) *idum=1;
        else *idum = -(*idum);
        for (j=NTAB+7;j>=0;j--) {
            k=(*idum)/IQ;
            *idum=IA*(*idum-k*IQ)-IR*k;
            if (*idum < 0) *idum += IM;
            if (j < NTAB) iv[j] = *idum;
        }
        iy=iv[0];
    }
    k=(*idum)/IQ;
    *idum=IA*(*idum-k*IQ)-IR*k;
    if (*idum < 0) *idum += IM;
    j=iy/NDIV;
    iy=iv[j];
    iv[j] = *idum;
    if ((temp=AM*iy) > RNMX) return RNMX;
    else return temp;
}
