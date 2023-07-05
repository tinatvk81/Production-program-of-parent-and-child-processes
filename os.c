

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
int a,b,c,d;

int sum_square(){
	int sum=0;
	for (int i = 1; i <= a; i++)
	{
		sum+=(i*i);
	}
	//printf("%d \t",sum);
	return sum;
}

int min(){
	int m=a;
	if(b<m){
		m=b;
	}
	if(c<m){
		m=c;
	}
	return m;
}
int fact(){
	int sum=0;
	int n=1;
	for(int i=1;i<=a;i++){
		n*=i;
		sum+=n;
	}
	//printf("fact");
	//printf(" %d\t",sum);
	return sum;
}
float variance() {
    float mean = (a + b + c) / 3.0;
    float var = ((a - mean)*(a - mean) + (b - mean)*(b - mean) + (c - mean)*(c - mean)) / 3.0;
    return var;
}
int main()
{
	scanf("%d %d %d %d",&a,&b,&c,&d);
	// make two process which run same
	// program after this instruction
	int pfd[2];
		if (pipe(pfd) < 0)
        	exit(1);

	pid_t ch1=fork(),ch11,ch12;
	if(ch1==0){
		ch11=fork();
		if(ch11==0){
			int n=sum_square();
			exit(n);
			
		}else if (ch11>0){
			ch12=fork();
			if (ch12==0){
				int n=fact();
				exit(n);
			}
		}
		//printf("hello");
		int status1,status2,count;
		waitpid(ch11,&status1,0);
		waitpid(ch12,&status2,0);
		int n1=WEXITSTATUS(status1);
		int n2=WEXITSTATUS(status2);
		printf("square:  %d\n",n1);//
		printf("fact:   %d\n",n2);
		printf("d:   %d\n",d);
		count=d+n1+n2;
		FILE *file = fopen("data.txt", "w");
		if (file == NULL) {
			printf("Error opening file.\n");
			return 1;
		}
		fprintf(file, "%d",count);
		fclose(file);
		exit(0);

	}
	pid_t ch2=fork();
	pid_t ch21,ch22;
	int fd[2];
	if (pipe(fd) < 0)
        exit(1);
	if(ch2==0){
		ch21=fork();
		if(ch21==0){
			float n=variance();
			write(fd[1],&n,sizeof(n));
			exit(0);
			
		}else if (ch21>0)
		{
			ch22=fork();
			if (ch22==0){
				float n=(a + b + c) / 3.0;
				close(fd[0]);
				write(fd[1],&n,sizeof(n));
				exit(0);

			}
			
		}
		waitpid(ch21,NULL,0);
		waitpid(ch22,NULL,0);
		float n1,n2;
		close(fd[1]);
		read(fd[0],&n1,sizeof(n1));
		read(fd[0],&n2,sizeof(n2));
		printf("variance:   %f\n",n2);
		printf("avrage:   %f\n",n1);
		float s= n1+n2;
		
		close(pfd[0]);
		write(pfd[1],&s,sizeof(s));

		exit(0);
	}
	int min1;
	pid_t ch3=vfork();
	if(ch3==0){
		min1=min();
		exit(0);

	}
	waitpid(ch1,NULL,0);
	waitpid(ch2,NULL,0);
	waitpid(ch3,NULL,0);
	FILE *file = fopen("data.txt", "r");
	if (file == NULL) {
		printf("Error opening file.\n");
		return 1;
	}
	int c1;
	float c2;
	fscanf(file,"%d",&c1);
	read(pfd[0],&c2,sizeof(c2));
	int c3=min1;
	printf("ch1: %d\n",c1);
	printf("ch2: %f\n",c2);
	printf("ch3: %d\n",c3);
	
	
	
	return 0;

}


