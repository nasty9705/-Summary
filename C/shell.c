#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <fcntl.h>
#define N 50
#define DEL 20

int fonflag=-1;
char *bufin,*bufout;
int infile,outfile,conv,fr,fr1,s;

typedef struct _node 
{
	char *value;
	struct _node *next;
	struct _node *prev;
}node;

typedef struct _Dlist
{
	unsigned int size;
	node *head;
	node *tail;
}Dlist;

void push(Dlist *list,char *sl)
{
	//printf("0000000000000000000%s\n",sl);
	node *tmp=(node*)malloc(sizeof(node));
	
	//printf("===========================%d\n", outfile);
	//printf("==============+++++++++++++++%d\n",infile);
	if (*sl != '\0')
	{
	if (infile == 1)
	{//printf("88888888888%s\n",sl);
		bufin = strdup(sl);
		infile = 2;
	}
	else
	{
		if ((outfile == 1) || (outfile == 3))
		{   
			bufout = strdup(sl);
			//printf("%s\n",sl);
			outfile ++;
		}
		else
		{
			//printf("%s\n",sl);
			tmp->value=strdup(sl);
			tmp->next=NULL;
			tmp->prev=list->tail;
			if (list->tail)
			{
				list->tail->next=tmp;
			}
			list->tail=tmp;
	
			if(list->head == NULL)
			{
				list->head=tmp;
			}
			list->size++;
			
		}
	}
	}
}



int slova(char *slovo2,Dlist *tmp2)
{
	char *slovo1=slovo2,c;
	int slovo_size=0,flag=0,temp,counter=N;fonflag =-1;
	int forout=0;
	conv=0;
	fr =0;
	printf("> ");
	while( ((c=getchar()) != '\n') && (c != EOF) )
	{   
			
		if (c=='"')
		{ 
			if (flag) 
				flag=0;
			else
				flag=1;
		}
		else
		{   
			if ((c == '|') && (flag == 0))
			{	fr ++; 
				fr1++;
				conv=1;
			}
			if (((c == '>') || (c == '<')) && (flag == 0))
			{ 
				if (c == '<')
					infile = 1;
				if ((c == '>') && (outfile == 1) && (forout == 0))
				{
				   outfile +=2;	
				}
				else 
				{
				if (c == '>')
					outfile = 1;
				}
			}
			if ((c == '&') && (flag == 0))
			{ 
				fonflag=1;
			}
			else
			{
			if (((c == ' ') || (c=='<') || (c == '>') || (c == '|')) && (flag == 0))
			{   
				*slovo1='\0';
				slovo_size=0;
				if((c == '>'))
					outfile--;
				else if ((c == '<'))
					infile--;
				push(tmp2,slovo2);
				forout = 0;
				if ((c == '>'))
					outfile++;
				else 
				if (c == '<')
					infile++;
				slovo1=slovo2;
				if (c == '|')
				{
					*slovo1=c;
					slovo1++;
					*slovo1='\0';
					slovo_size=0;
					push(tmp2,slovo2);	
					forout = 0;
					slovo1=slovo2;
				}
			}
			else
			{	 	
			if (slovo_size == counter)
			{
				temp=slovo1-slovo2;
				counter +=DEL;
				slovo2=(char *)realloc(slovo2,counter*(sizeof(char)));
				slovo1=slovo2+temp;
			}		
			*slovo1=c;
			slovo1++;
			slovo_size++;
			if (outfile == 1)
				forout = 1;
			if (fonflag != -1)
				fonflag=2;
					
			}
			}
				
		} 
	}	
	*slovo1='\0';
	push(tmp2,slovo2);
	slovo1=slovo2;
	if (flag) 
		return 1;
	if((c == EOF) && (slovo_size == 0))
		return 2;
	return 0;
}

int main()
{	while(1){
		int i,pid,status,t,res,fd[2],fd1[2],ii=0;
		char *slovo3,*slovo2 = {"cd"},*slovo4={"exit"};
		node *node11,*node12;
		Dlist *tmp1 = (Dlist*)malloc(sizeof(Dlist));
		tmp1->size = 0;
		tmp1->head = tmp1->tail = NULL;
		slovo3 = (char*)malloc(N*sizeof(char));
	
		while((res=waitpid(-1,&status,WNOHANG))>0)
			printf("finished process with pid %d and status %d\n",res,status);
		
		t = slova(slovo3,tmp1);//printf("%d\n",t);
	
		if((fonflag == 1) && (tmp1->size == 0)) 
		{
			printf("error\n"); 
			continue;
		}
		if(t==2) 
		{
			printf("\n");
			free(tmp1);
			free(slovo3);
			return 2;
		}
		if (tmp1->size == 0)
		{  if ((outfile != 0) || (infile != 0))
			{
				printf("error,creatingfile without command\n");
				if (infile != 0)
				{
					if (bufin != NULL)
					{
						free(bufin);
					}
					
					infile =0;
				}
				if (outfile != 0)
				{
					if (bufout != NULL)	
					{
						free(bufout);
					}
					
					outfile =0;
				}
			}
			continue;
		}
	
		if ((!strcmp(tmp1->head->value,slovo4)) || (t == 2))
		{ 
			node11=tmp1->head;
			for (i = 1;i <= (tmp1->size);i++)
			{
				free(node11->value);
				node12 = node11;
				node11 = node11->next;
				free(node12);
			}		
			free(tmp1);
			free(slovo3);
			return 1;
		}
	
		char ** argv = (char **)malloc(((tmp1->size+1)*(sizeof(char*))));	
		if (t)
		{
			printf("\nError,unpaired quotes\n");
			return 0;
		}
		else
		{	//printf("-----------------------%d\n",tmp1->size);
			node11 = tmp1->head;
			for (ii = 1;ii <= (tmp1->size);ii++)
			{
				argv[ii-1] = node11->value;
				node11 = node11->next;
			}
		}	 
		argv[ii-1] = NULL;
		int save_in,argc;
		argc=tmp1->size;
		char **argv2,**argv1,**fordel;
		save_in=dup(0);
		argc++;
		if(*argv[tmp1->size-1] == '|')
		{	
			node11 = tmp1->head;
			for (i = 1;i <= (tmp1->size);i++)
			{ 
				free(node11->value);
				node12 = node11;
				node11 = node11->next;
				free(node12);
			}
			free(argv);
			free(tmp1);
			free(slovo3);	
			continue;
		} 
		int k,k1,maspids[argc];
		k=0;
		char **mymas=(char**)malloc((argc+1)*sizeof(char*));
		for(i=0;i<argc-1;i++)
		{
			*(mymas+i)=strdup(*(argv+i));

		}
		*(mymas+i+1)=(char*)malloc(2*sizeof(char));
		argv1=mymas;	
		fordel=mymas;
		int s1;
		char ccc= 'n';
		*(mymas+argc-1)=NULL;
		if (!strcmp(tmp1->head->value,slovo2))
		{
			if ((tmp1->size == 1) || (*argv[1] == '~'))
				argv[1]=getenv("HOME");
			chdir(argv[1]); 
		}
		else
		{
			if ((fonflag == 2) || ((fonflag == 1) && (tmp1->size == 0)))
			{
				printf("error\n");
				fonflag =0;
			}
			else
			{
			for (i=0;i<argc-1;i++)
			{
				if ((**(mymas+i) == '|') || (i==argc-2))
				{	fr--;
					if (i != argc-2)
						*(mymas+i)=NULL;			
					if (!(i==argc-1))
						argv2=(mymas+i+1);
					else
						argv2=(mymas+i);
						
						
					if (infile == 0)
					{ 
						bufin = &ccc;
					}
					if (outfile == 0)
					{
						bufout = &ccc;
					}
					if ((infile == 1)||(outfile==1))
					{
						printf("error (without file name)\n"); 
						exit(5);
					}
				
					if (!((strcmp(bufin,".")) && (strcmp(bufin,"..")) && (strcmp(bufout,".")) && (strcmp(bufout,".."))))
					{
						printf("ERROR\n");
						exit(6);
					}
					
				
					if (infile == 2)
					{ 
						if ((fd1[0]=open(bufin,O_RDONLY))==-1) 
							printf("error in file\n");
						dup2(fd1[0],0);
						close(fd1[0]);
						infile=0;
					}
				
					pipe(fd);
					pid=fork();
					if ((fonflag == -1)  || ((fonflag != -1) && (fr > 0)))
					{
						maspids[k]=pid;
						k++;
					}
					if (pid == 0)
					{	
						if (i+1 != argc-1)
						{
							dup2(fd[1],1);
							close(fd[1]);
							close(fd[0]);
						}
						else
						{ 	
						if (outfile == 2)
						{
							if((fd1[1]=open(bufout,O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
								printf("error in creating file\n");
							dup2(fd1[1],1);
							close(fd1[1]);
						}
						if (outfile == 4)
						{
							if((fd1[1]=open(bufout,O_WRONLY | O_CREAT | O_APPEND, 0666))==-1) 
								printf("error in creating file\n");
							dup2(fd1[1],1);
							close(fd1[1]);
						}
						else
						{
							dup2(1,1);}
							close(fd[1]);
							close(fd[0]);
						}
						execvp(*argv1,argv1);
						printf("\nError in exec\n");
						exit(11);
					}
					else 
					{
						argv1=argv2;
						dup2(fd[0],0);
						close(fd[1]);
						close(fd[0]);			
						if (pid == -1) 
							printf("Process can't be created");
						else
						if ((fonflag) == -1)
						{
						}
						else
						{
							if ((fr1 == 0) || (fr == 0) || (s == 0))
								printf("process with pid %d started \n",pid);
						}
					}
				}
			}
			dup2(save_in,0);
			close(save_in);
			for (k1=0;k1<=k;k1++)
			{
				waitpid(maspids[k1],&status,0);
			}
			}		
			if (infile != 0)
			{
				free(bufin);
				infile =0;
			}
			if (outfile != 0)
			{
				free(bufout);
				outfile =0;
			}
			for(i=0;i<=(tmp1->size)-1;i++)//
				free(*(fordel+i));//
			free(fordel);//
			node11 = tmp1->head;
			for (i = 1;i <= (tmp1->size);i++)
			{   
				free(node11->value);
				node12 = node11;
				node11 = node11->next;
				free(node12);
			}
			free(argv);
			free(tmp1);
			free(slovo3);
		}
	}
	return 0;
}
