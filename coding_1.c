#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void M_seq(int,int,int*);//訊號長度 ,errorpattern數

int main(){
/*input file*/
	FILE *ff ,*gg ;
	int a,x,i,j,k;
	char temp;
	int index, sum=0;
	/*開Sim.txt檔案*/
	ff = fopen("Sim.txt","r");
	fscanf(ff,"%d",&a);//a is message length
	fscanf(ff,"%*[+\n]",&temp);
	printf("truncation length a=%d\n",a);
	fflush(stdout);
	int *message = calloc(a,sizeof(int));
	M_seq(a,1,message); //mseq本身就是*型態

/*input data given by Sim.txt*/
//debug1
	printf("dubug1\n");
		// for(i=0; i<a; i++){
			// printf("message=%d\n",message[i]);
		// }
/*create empty state vector and it is initial state*/
/*get x1 x2 */
	int* x1 = calloc(a,sizeof(int));
	int* x2 = calloc(a,sizeof(int));
	int* state = calloc(6,sizeof(int));
	int* sms = calloc(6,sizeof(int));
	int* state_mapping_shift = calloc(6,sizeof(int));
	for(j=0;j<a;j++){
		x1[j]=(message[j]+(state[1]*(message[j-sms[1]]))+(state[2]*message[j-sms[2]])+(state[4]*message[j-sms[4]])+(state[5]*message[j-sms[5]]))%2;
		x2[j]=(message[j]+(state[0]*(message[j-sms[0]]))+(state[1]*message[j-sms[1]])+(state[2]*message[j-sms[2]])+(state[5]*message[j-sms[5]]))%2;
		for(i=0; i<a; i++){
			// printf("ddda=%d\n",x1[i]);
		}
		for(i=5;i>=1;i--){
			state[i]=state[i-1];
			// printf("state5~1=%d\n",state[i]);
		}
		// printf("state0=%d\n",state[0]);
		state[0]=message[j];
		for(x=0;x<6;x++){
			sms[x]=state[x]*(x+1); //state_mapping_shift
			// printf("sms0~5=%d\n",sms[x]);
		}
	}
	x=0;
/*judge what is the final state and decide appending how many zero-input for system*/
	int m=6 ;
	// while(!state[x] && x<6){
		// x=x+1;
	// }
	// m=6-x;
	// x=0;
	int* m_zero_vector = calloc(m,sizeof(int)); //m=0 maybe cause some problems****************
	int* m_x1 = calloc(m,sizeof(int));
	int* m_x2 = calloc(m,sizeof(int));
/*initaiate state machine and generate corresponding code*/
	if(m!=0){
		int* Conca_x=calloc(m+a,sizeof(int));
		for(i=0;i<a;i++){
			Conca_x[i]=message[i];
		}
		for(i=0;i<m;i++){
			Conca_x[a+i]=m_zero_vector[i];
		}
		for(j=0;j<m;j++){
			m_x1[j]=(Conca_x[j+a]+Conca_x[j+a-sms[1]]+Conca_x[j+a-sms[2]]+Conca_x[j+a-sms[4]]+Conca_x[j+a-sms[5]])%2;
			m_x2[j]=(Conca_x[j+a]+Conca_x[j+a-sms[0]]+Conca_x[j+a-sms[1]]+Conca_x[j+a-sms[2]]+Conca_x[j+a-sms[5]])%2;
			for(i=5;i>=1;i--){
				printf("******state5~1=%d\n",state[i]);
			}
			printf("******state0=%d\n",state[0]);
			for(i=5;i>=1;i--){
				state[i]=state[i-1];
				// printf("******state5~1=%d\n",state[i]);
			}
			// printf("******state0=%d\n",state[0]);
			state[0]=Conca_x[j+a];
			// printf(m_zero_vector("state0=%d\n",state[0]);
			for(x=0;x<6;x++){
				sms[x]=state[x]*(x+1); //state_mapping_shift
				// printf("sms0~5=%d\n",sms[x]);
			}
		}
	}
/*concatenating x1 x2 m_x1 m_x2*/
	int length_Gcode = 2*m+2*a ;
	int* Gcode = calloc(length_Gcode,sizeof(int));
	for(i=0;i<a;i++){
		Gcode[i+i]=x1[i];
		Gcode[i+i+1]=x2[i];
	}
	for(i=0;i<m;i++){
		Gcode[a+a+i+i] = m_x1[i];
		Gcode[a+a+i+i+1] = m_x2[i];
	}
/*mapping to BPSK*/
	for(i=0;i<length_Gcode;i++){
		Gcode[i] = (Gcode[i])? -1:1 ;
	}
/*write in file*/
	printf("\n");
	gg = fopen("x.txt","w"); 		
	for (int i=0;i<length_Gcode;i++){
		fprintf(gg," %d",Gcode[i]);
	}
		fprintf(gg," %%2*(%d+%d) elements\n",a,m);
	
	
}
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

