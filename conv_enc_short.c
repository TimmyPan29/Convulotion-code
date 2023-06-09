#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int *seq(int L, int m){
    int *u;

    //seq
    u=(int*)malloc((L+m)*sizeof(int*));
    u[0]=1;
    u[1]=0;
    u[2]=0;
    u[3]=0;
    u[4]=0;
    u[5]=0;
    for(int i=0;i<L-6;i++){
        u[i+6]=u[i+1]^u[i];
    }
    for(int i=0;i<m;i++){
        u[L+i]=0;
    }
    

    //all zero
    /*
    for(int i=0;i<L+m;i++){
        u[i]=1;
    }
    for(int i=0;i<m;i++){
        u[L+m-1-i]=0;
    }
    */
    return u;
}


int main(){
    int L, x1, x2;
    int m=6;//number of registers
    int state=0;
    int MSB=1;
    int tempa=0;
    for(int i=0;i<m-1;i++){
        MSB<<=1;
    }
    FILE *fpr;
    fpr = fopen("Sim.txt","r");
    if(fpr==NULL){
        return 0;
    }
    fscanf(fpr,"%d",&L);
    fclose(fpr);
    
    int *u;
    printf("L=%d m=%d\n",L,m);
    u=seq(L,m);

    
    printf("u=");
    for(int i=0;i<(L+m);i++){
        printf("%d",u[i]);
    }
    printf("\n");
    printf("L+m= %d\n",L+m);

    FILE* fpw;
    fpw=fopen("x.txt","w");
    int MSB_temp=MSB;
    //x1=1+   d2+d3+d5+d6
    //x2=1+d1+d2+d3+   d6
    //d=state(32)=MSB d2=state(16)=MSB>>1...d3=8>>2 d4=4>>3 d5=2>>4 d6=1>>5
    for(int i=0;i<L+m;i++){ //i=0 will make first state '000000' generate output x1 x2 when input is 1 or 0
        x1=u[i]^(16&state?1:0)^(8&state?1:0)^(2&state?1:0)^(1&state?1:0);
        x2=u[i]^(32&state?1:0)^(16&state?1:0)^(8&state?1:0)^(1&state?1:0);
        
        printf("state=");
        for(int k=0;MSB_temp;MSB_temp>>=1){ //out of for loop when MSB_temp=1
            printf("%d",(state&MSB_temp)?1:0);
        }
        printf("\n");
        MSB_temp=MSB;
        state>>=1;
        state+=u[i]?MSB:0;
        //{0, 1}=>{-1, 1}
        x1=x1?-1:1;
        x2=x2?-1:1;
        fprintf(fpw,"%d %d ",x1,x2);
    }

    //save as x.txt

    
    fprintf(fpw,"\t%%2*(L+m) elements\n");

    
    
    fclose(fpw);
    
    
    return 0;
}