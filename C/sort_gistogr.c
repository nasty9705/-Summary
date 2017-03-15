#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <unistd.h>


#define length 256
#define lgth 20

void sort(char **a, int n);
int stringcmp(char *str1, char *str2);
void gistogr(char **a, int n, int M);

int main(int argc,char **argv)
{
	int i=0;
	int j=0;
	int p,M; /*M-shirina*/
	char **a;
	int c;
	FILE* f;
	FILE* f1;
	int err;

	if(argc<3)
	{
		printf("wrong program arguments\n");
		return 2;
	}

	f=fopen(argv[1],"r");
	if (f==NULL)
	{
		printf("error openning the file: \"%s\"\n",argv[1]);
		return 2;
	}

	f1=fopen(argv[2],"w");
	if (f1==NULL)
	{
		printf("error openning the file: \"%s\"\n",argv[2]);
		return 2;
	}

	/*
	 * Number of strings is initialized by 0
	 */
	p=0;
	char **a_new1;
	a_new1=(char **)malloc(sizeof(char *));
	if (a_new1==NULL)
	{
		free(a);
		printf("no memory\n");
		return -1;
	}
	a=a_new1;

	/*int c_next;*/
	while(1)
	{
		c=fgetc(f);
		if (c==EOF)
		{
			if (feof(f)!=0)
				break;
			else
			{
				printf("error reading file\n");
				break;
			}
		}
		/*if (c==0xFF)
		{
			if ((c_next=getc(f))==0x00)
				break;
		}*/
		if (j==0)
		{
			char *a_new;	
			a_new=(char *)malloc(sizeof(char));
			if (a_new==NULL)
			{
				free(a[p]);
				printf("no memory\n");
				return -1;
			}
			a[p]=a_new;
		}
		else
		{
			char *a_new;
			a_new=(char *)realloc(a[p],(j+1)*sizeof(char));
			if (a_new==NULL)
			{
				free(a[p]);
				printf("no memory\n");
				return -1;
			}
			a[p]=a_new;
		}
		a[p][j]=c;
		j++;
		if (c=='\n')
		{
			char **a_new;
			a_new=(char **)realloc(a,(p+2)*sizeof(char *));
			if (a_new==NULL)
			{
				free(a);
				printf("no memory\n");
				return -1;
			}
			a=a_new;
			p++;
			j=0;
		}
	}

	if (j!=0)
	{
		char *a_new;
		a_new=(char *)realloc(a[p],(j+1)*sizeof(char));
		if (a_new==NULL)
		{
			free(a[p]);
			printf("no memory\n");
			return -1;
		}
		a[p]=a_new;
		a[p][j]='\n';
		p++;
	}

	fclose(f);
	sort(a, p);
	int test=0;
	for (i=0;i<p;i++)
	{
		j=0;
		while (a[i][j]!='\n')
		{
			if (a[i][j]==0x00)
				test=1;
		j++;
		}
	}
	if (test==0)
	{
		for(i=0;i<p;i++)
		{
			j=0;
			while (a[i][j]!='\n')
			{
				fputc(a[i][j],f1);
				j++;
			}
			fputc('\n',f1);
		}
	}
	else
	{
		for(i=0;i<p-1;i++)
		{
			j=0;
			while (a[i][j]!='\n')
			{
				fputc(a[i][j],f1);
				j++;
			}
			fputc('\n',f1);
		}
		j=0;
		while (a[p-1][j]!='\n')
		{
			fputc(a[p-1][j],f1);
			j++;
		}
	}

	fclose(f1);
	
	
{	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
/*	printf("lines %d\n", w.ws_row);*/
	printf("\n");
	printf("columns %d\n\n", w.ws_col);

/*	printf("enter the colums of screen\n");
	scanf("%d",&M);*/

	gistogr(a,p,w.ws_col);
}

	for(i=0;i<p;i++)
	{
		if(a[i]!=NULL)
		{
			free(a[i]);
			a[i]=NULL;
		}
	}

	if (a!=NULL)
	{
		free(a);
		a=NULL;
	}

	return 0;
}

int stringcmp(char *str1, char *str2)
{
	int i=0;
	while ((str1[i]!='\n')&&(str2[i]!='\n'))
	{
		if (str1[i]>str2[i])
			return 1;
		if (str1[i]<str2[i])
			return -1;
		i++;
	}
	if ((str1[i]=='\n')&&(str2[i]=='\n'))
		return 0;
	if (str1[i]=='\n')
		return -1;
	
	return 1;
}

void sort(char **a,int n)
{
	int i;
	int j=0;
	char *c;
	for (i=0;i<n-1;i++)
	for (j=i+1;j<n;j++)
	{
		if (stringcmp(a[i],a[j])==1)
		{
			c=a[i];
			a[i]=a[j];
			a[j]=c;
		}
	}
}

void gistogr(char **a,int n,int M)
{
	int S[length];
	double rate[length];
	int i=0;
	int max1=0;
	int j,x;
	/*float k;*/
	int kolvo=0;

	for (i=0;i<length;i++)
		S[i]=0;
	for (i=0;i<n;i++)
	{
		j=0;
		while(a[i][j]!='\n')
		{
			S[a[i][j]]++;
			kolvo++;
			/*if (S[a[i][j]]>max)
				max=S[a[i][j]];*/
			j++;
		}
	}
	
	S[10]=n;
	/*if (S[10]>max)
		max=S[10];*/
	for (i=0;i<length;i++)
	{
		if (S[i]>max1)
			max1=S[i];
	}

	printf("max: %i\n",max1);

	kolvo=kolvo+n;
	printf("kolvo simvolov: %i\n",kolvo);

	for (i=0;i<length;i++)
	{
		if (S[i]!=0)
			rate[i]=(double)S[i]/max1;
		if (S[i]==0)
			rate[i]=0;
	}

	if (max1==0)
		return;

	M=M-12;
	/*k=(float)M/max;*/

	for(i=0;i<length;i++)
	{
		if (S[i]>0)
		{
			if ((i>=33)&&(i<=127))
				printf("   %c|%6d|",(char)i,S[i]);
			else 
				printf("#%03d|%6d|",i,S[i]);

			/*x=S[i]*k;*/
			x=M*rate[i];
			/*if (x==0)
				x=1;*/
			for(j=0;j<x;j++)
				printf("*");
			printf("\n");
		}
	}
}
