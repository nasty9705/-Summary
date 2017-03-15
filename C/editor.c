#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <unistd.h>

int set_tabwidth(char *str);
int print_pages(char **a,int p,int shir,int leng, int lin, int posl_j,int number, int flag);
int print_pages_no(char **a,int p,int shir,int leng, int lin,int posl_j,int number, int flag);
void print_range(char **a, int shir, int leg, int p, int posl_j,char *str);
char edit_string(char **a, int p, int stroka, int stolbes, char sym);
char** edit_string_n(char **a, int p, int stroka, int stolbes, char sym);
char** insert_symbol(char **a,int stroka, int stolbes, char sym, int p, int posl_j);
char** replace_substring(char **a, int p, char *str);
char** delete_range(char **a, int d1, int d2, int p);
char** delete_braces(char **a, int d1, int d2, int p);
char** p_read(char **a,int p, int posl_j,char *str);
char** open(char **a, int p, int posl_j,char *str);
FILE* p_write(char **a, FILE* f1,int p);
void help(void);
char* vvod(void);
char* vvod_stroky(char *str);
int check(char **mass,char *str);


int symbol;
struct winsize w;
struct termios old_attributes,new_attributes; 
char **mass;
int flag_new_string_n = 0;
int razn = 0;
int p_p,p_posl_j;
int j_perenos = -1;
FILE* f1;
char *filename1;
int flag_write = 0;
int arrg; // для аргументов
int r_vvod;
int flag_assotiaton = 1;

int perevod_v_canon(void);
int perevod_from_canon(void);


int main(int argc, char **argv)
{
	char *str;
    int i=0;
	int j=0;
	int p,t,lin=0;
	char **a;
	int c;
	FILE* f;
	//FILE* f1;
	FILE* mf;
	int posl_j;
	int flag = 0; //для set tabwidth
	int number;
	int flag_cut=0; //для set wrap
	int flag_save=0; //для exit и write
	int stroka,stolbes; //для edit string
	char sym; //для edit string 
	int flag_insert = 0; //для insert symbol
	int flag_new_string = 0; //для insert symbol и edit string
	int d1,d2; //для delete range
	int flag_delete_range = 0; //для delete range
	int flag_delete_braces = 0; //для delete braces
	int strok;
	//проверочки всякие

	/*if(argc<3)
	{
		printf("wrong program arguments\n");
		return 2;
	}*/

	f=fopen(argv[1],"r");
	if (f==NULL)
	{
		printf("error openning the file: \"%s\"\n",argv[1]);
		return 2;
	}

	/*f1=fopen(argv[2],"w+");
	if (f1==NULL)
	{
		printf("error openning the file: \"%s\"\n",argv[2]);
		return 2;
	}*/

	//массив с командами
	char **mass_new;
	mass_new=(char **)malloc(18*sizeof(char *));
	if (mass_new==NULL)
	{
		free(mass);
		printf("no memory\n");
		return -1;
	}

	mass=mass_new;
	mass[1]=(char *)malloc(13*sizeof(char));
	mass[1]="settabwidth\0";
	mass[2]=(char *)malloc(11*sizeof(char));
	mass[2]="printpages\0";
	mass[3]=(char *)malloc(12*sizeof(char));
	mass[3]="printrange\0";
	mass[4]=(char *)malloc(12*sizeof(char));
	mass[4]="setwrapyes\0";
	mass[5]=(char *)malloc(13*sizeof(char));
	mass[5]="insertafter\0";
	mass[6]=(char *)malloc(12*sizeof(char));
	mass[6]="editstring\0";
	mass[7]=(char *)malloc(14*sizeof(char));
	mass[7]="insertsymbol\0";
	mass[8]=(char *)malloc(18*sizeof(char));
	mass[8]="replacesubstring\0";
	mass[9]=(char *)malloc(13*sizeof(char));
	mass[9]="deleterange\0";
	mass[10]=(char *)malloc(14*sizeof(char));
	mass[10]="deletebraces\0";
	mass[11]=(char *)malloc(5*sizeof(char));
	mass[11]="exit\0";
	mass[12]=(char *)malloc(5*sizeof(char));
	mass[12]="read\0";
	mass[13]=(char *)malloc(5*sizeof(char));
	mass[13]="open\0";
	mass[14]=(char *)malloc(6*sizeof(char));
	mass[14]="write\0";
	mass[15]=(char *)malloc(8*sizeof(char));
	mass[15]="setname\0";
	mass[16]=(char *)malloc(5*sizeof(char));
	mass[16]="help\0";
	mass[17]=(char *)malloc(10*sizeof(char));
	mass[17]="setwrapno\0";
	mass[18]=(char *)malloc(9*sizeof(char));
	mass[18]="exitforce\0";


	//считывание текста из файла в **а
	p = 0;
	char **a_new1;
	a_new1=(char **)malloc(sizeof(char *));
	if (a_new1==NULL)
	{
		free(a);
		printf("no memory\n");
		return -1;
	}
	a=a_new1;

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
			posl_j=j;
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
		posl_j=j;
		p++;
	}
	fclose(f);

	//вычисление ширины и длины экрана

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	//printf("lines %d\n", w.ws_row);
	//printf("columns %d\n\n", w.ws_col);

	while(1)
	{
		lin=0;
		str=vvod();
		printf("%s\n",str);
		t=check(mass,str);


		if (t == 1)  //вызов set tabwidth
		{
			number = set_tabwidth(str);
			flag = 1;
			//printf("%d\n",number);
			//printf("%d\n",flag);
		}

		if (t == 2)   //вызов print_pages
		{
			if (flag_new_string == 1)
				p++;
			if (flag_new_string_n == 1)
				p++;
			if (flag_delete_range == 1)
				p = p -(d2-d1+1);
			if (flag_delete_braces == 1)
				p = p-razn;

			if (flag_cut == 0)
			{
				lin = print_pages(a,p,w.ws_col,w.ws_row,lin,posl_j,number,flag);
				perevod_v_canon();
				while((symbol = getchar()) != EOF)
   		 		{
   		 			printf("Был введён символ: код=%d начертание '%c'\n",symbol,(char)symbol);
   		 			//ctrl+d
   		 			if (symbol == '\004')
   		 				break;
   		 			if (symbol == 'q')
   		 				break;

	   		 		if ((lin == p) && (symbol == ' '))
   			 			printf("\nвесь текст выведен\n");

   			 		if ((symbol == ' ') && (lin != p))
   		 			{
   		 				perevod_from_canon();
   		 				lin = print_pages(a,p,w.ws_col,w.ws_row,lin,posl_j, number, flag);
   		 				perevod_v_canon();
   		 			}
   		 		}
   		 		perevod_from_canon();
			}

			if (flag_cut == 1)
			{

				lin = print_pages_no(a,p,w.ws_col,w.ws_row,lin,posl_j,number,flag);
				perevod_v_canon();
				while((symbol = getchar()) != EOF)
   		 		{
   		 			printf("Был введён символ: код=%d начертание '%c'\n",symbol,(char)symbol);
   		 			//ctrl+d
   		 			if (symbol == '\004')
   		 				break;

	   		 		if ((lin == p) && (symbol == ' '))
   			 			printf("\nвесь текст выведен\n");

   			 		if ((symbol == ' ') && (lin != p))
   		 			{
   		 				perevod_from_canon();
   		 				lin = print_pages_no(a,p,w.ws_col,w.ws_row,lin,posl_j, number, flag);
   		 				perevod_v_canon();
   		 			}
   		 		}
   		 		perevod_from_canon();
			}
			flag_new_string_n = 0;
			flag_new_string = 0;
			flag_delete_range = 0;
			flag_delete_braces = 0;
		}

		if (t == 3) //вызов print range
			print_range(a,w.ws_col,w.ws_row,p,posl_j,str);

		if (t == 4)
			flag_cut=0;

		if (t == 5)
		{
			printf("введите 1-ый аргумент [номер строки] : ");
			scanf("%d",&strok);
			printf("введите 2-ой аргумент [строку] : ");
		}

		if (t == 6) //edit string
		{
			int r;
			/*printf("введите 3-ий аргумент [символ]: ");
			sym = fgetc(stdin);
			printf("Введите 1-ый аргумент [номер строки]: ");
			scanf("%d",&stroka);
			printf("введите 2-ой аргумент [номер позиции]: ");
			scanf("%d",&stolbes);*/

			if (arrg != -1)
			{
				r = arrg;
				stroka = str[arrg] - '0';
				r++;
				while (str[r] != ',')
				{
					stroka = stroka*10;
					stroka = stroka + str[r] - '0';
					r++;
				}
				r++;
				if (str[r] != '\0')
				{
					stolbes = str[r] - '0';
					r++;
					while (str[r] != ',')
					{
						stolbes = stolbes*10;
						stolbes = stolbes + str[r] - '0';
						r++;
					}
				}
				r++;
				sym = str[r];
			}
			else
			{
				printf("ошибка: отсутствуют аргументы\n");
				break;
			}

			if ((stroka == 0) || (stolbes ==0))
			{
				printf("ошибка : указана конструкция вне диапазона2\n");
				break;
			}
			stroka--;
			stolbes--;

			if (sym != '\n')
			{
				//printf("обычный\n");
				a[stroka][stolbes] = edit_string(a,p,stroka,stolbes,sym);
			}
			if (sym == '\n')
			{
				flag_new_string = 1;
				a = edit_string_n(a,p,stroka,stolbes,sym);
			}
		}

		if (t == 7) //insert symbol
		{
			int r;
			flag_insert = 1;
			/*printf("введите 3-ий аргумент [символ]: ");
			sym = fgetc(stdin);
			if (sym == '\n')
				flag_new_string = 1;
			printf("Введите 1-ый аргумент [номер строки]: ");
			scanf("%d",&stroka);
			stroka--;
			printf("введите 2-ой аргумент [номер позиции]: ");
			scanf("%d",&stolbes);
			if (stolbes == 0)
				stolbes = 0;
			else
				stolbes--;*/

			if (arrg != -1)
			{
				r = arrg;
				stroka = str[arrg] - '0';
				r++;
				while (str[r] != ',')
				{
					stroka = stroka*10;
					stroka = stroka + str[r] - '0';
					r++;
				}
				r++;
				if (str[r] != '\0')
				{
					stolbes = str[r] - '0';
					r++;
					while (str[r] != ',')
					{
						stolbes = stolbes*10;
						stolbes = stolbes + str[r] - '0';
						r++;
					}
				}
				r++;
				sym = str[r];
			}
			else
			{
				printf("ошибка: отсутствуют аргументы\n");
				break;
			}

			stroka--;
			if (stolbes == 0)
				stolbes = 0;
			else
				stolbes--;
			if (sym == '\n')
				flag_new_string = 1;

			a = insert_symbol(a, stroka, stolbes, sym, p, posl_j);
		}

		if (t == 8)
			a = replace_substring(a,p,str);

		if (t ==  9)  //delete range
		{
			//printf("введите 1-ый аргумент [левая граница диапазона] : ");
			//scanf("%d",&d1);
			//printf("введите 2-ой аргумент, если его нет введите 0\n[правая граница диапазона] : ");
			//scanf("%d",&d2);
			int r;
			int flag1;

			if (arrg != -1)
			{
				r = arrg;
				d1 = str[arrg] - '0';
				r++;
				while ((str[r] != ',') && (str[r] != '\0'))
				{
					d1 = d1*10;
					d1 = d1 + str[r] - '0';
					r++;
				}
				if (str[r] == '\0')
				{
					d2 = 0;
					flag1 = 1;
				}

				if (flag1 == 0)
				{
					r++;
					if (str[r] != '\0')
					{
						d2 = str[r] - '0';
						r++;
						while (str[r] != '\0')
						{
							d2 = d2*10;
							d2 = d2 + str[r] - '0';
							r++;
						}
					}
				}
			}
			else
			{
				printf("ошибка: должны быть аргументы\n");
				break;
			}

			d1--;
			if (d2 == 0)
				d2 = p-1;
			else
				d2--;
			a = delete_range(a,d1,d2,p);
			flag_delete_range = 1;
		}

		if (t == 10) //delete braces
		{
			/*printf("введите 1-ый аргумент, если его нет введите 0\n[левая граница диапазона] : ");
			scanf("%d",&d1);
			printf("введите 2-ой аргумент, если его нет введите 0\n[правая граница диапазона] : ");
			scanf("%d",&d2);*/

			int r;
			int flag1;

			if (arrg != -1)
			{
				r = arrg;
				d1 = str[arrg] - '0';
				r++;
				while ((str[r] != ',') && (str[r] != '\0'))
				{
					d1 = d1*10;
					d1 = d1 + str[r] - '0';
					r++;
				}
				if (str[r] == '\0')
				{
					d2 = 0;
					flag1 = 1;
				}

				if (flag1 == 0)
				{
					r++;
					if (str[r] != '\0')
					{
						d2 = str[r] - '0';
						r++;
						while (str[r] != '\0')
						{
							d2 = d2*10;
							d2 = d2 + str[r] - '0';
							r++;
						}
					}
				}
			}
			else
			{
				d1 = 0;
				d2 = 0;
			}


			if (d1 == 0)
				d1 = 0;
			else d1--;
			if (d2 == 0)
				d2 = p-1;
			else d2--;
			a = delete_braces(a,d1,d2,p);
			flag_delete_braces = 1;
		}

		if (t == 12) //pread
		{
			a = p_read(a,p,posl_j,str);
			//printf("вышел из p_read\n");
			p = p_p;
			posl_j = p_posl_j;
		}

		if (t == 13) //open
		{
			a = open(a, p, posl_j,str);
			//printf("вышел из open\n");
			p = p_p;
			posl_j = p_posl_j;
		}

		if (t == 17) //set wrap no
		{
			flag_cut=1;
			//printf("flag_cut=%d\n",flag_cut);
		}

		if (t == 11) //pexit
		{
			if (flag_save == 0)
				printf("вы не сохранили файл\n");
			else
				break;
		}

		if (t == 18) //exit force
			break;

		if (t == 14) //pwrite
		{
			if (flag_assotiaton == 0)
			{
				printf("ассоциация с файлом была удалена, сохранить файл невозможно\nзадайте новую ассоциацию\n");
				return 0;
			}
			else
			{
				f1=fopen(argv[1],"w+");
				if (f1==NULL)
				{
					printf("error openning the file: \"%s\"\n",argv[1]);
					return 2;
				}
				p_write(a,f1,p);
				flag_save = 1;
			}
		}

		if (t == 15) //set name
		{
			int r;
			if (arrg != -1)
			{
				flag_assotiaton = 1;
				r = arrg +1;
				r_vvod = r;
				filename1 = vvod_stroky(str);
				f1 = fopen(filename1,"w+");
			}
			else 
			{
				flag_assotiaton = 0;
				printf("ассоциация с файлом удалена\n");
			}

			/*printf("введите имя файла: ");
			scanf("%s",filename1);*/
		}

		if (t==16)
			help();
	}


//освобождение памяти из массива с текстом
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



int print_pages(char **a,int p,int shir,int leng, int lin,int posl_j,int number, int flag)
{
	int tab=0;   //1-строка не влазиет, есть перенос
	int next=0;  //1-есть следующая страница
	int j,i,h;
	int kolvo=0; //колтчество перенесенных строк
	int k=0;     //колво строк на экране(i+kolvo)
	int m;
	int flag_ostatok_str = 0;

	i = lin;

	while ( (k != leng-1) && (i != p))
	{
		j=0;
		m=0;
		while (a[i][j] != '\n')
		{
			if (j == 0)
				printf(">:");

			if (m >= shir-2)
			{
				tab=1;
				kolvo++;
				m=0;
				}


				if (tab == 0)
				{
					//printf("%c",a[i][j]);
					if (a[i][j] == '	')
					{
						if (flag == 1)
							for (h=0;h<number;h++)
								printf(" ");
						else printf("%c",a[i][j]);
					}
					else
						printf("%c",a[i][j]);
				}
				else
				{	
					tab=0;
					k++;
					printf("\n");

					//printf("%c",a[i][j]);
					if (a[i][j] == '	')
					{
						if (flag == 1)
							for (h=0;h<number;h++)
								printf(" ");
						else printf("%c",a[i][j]);
					}
					else
						printf("%c",a[i][j]);


					//k=i+kolvo;
					if (k == leng-1)
					{
						if (j != posl_j)
							j_perenos = j+1; //перенос есть
						else
							j_perenos = -1; //переноса нет
						return i+1;
					}
				}
			j++;
			m++;
		}
		printf("\n");
		i++;
		k++;
	}

	if (k != leng-1)
	{
		if (flag_ostatok_str == 1)
			m = 1;
		else 
			m = 0;
		for (m=m; m<leng-k-1; m++)
			printf("\n");
	}

	return i;
}

int print_pages_no(char **a,int p,int shir,int leng, int lin,int posl_j,int number, int flag)
{
	int tab=0;   //1-строка не влазиет, есть перенос
	int j,i,h;
	int kolvo=0; //колтчество перенесенных строк
	int k=0;     //колво строк на экране(i+kolvo)
	int m;

	i = lin;

	while ( (k != leng-1) && (i != p))
	{
		j=0;

		while ((j != shir-2) && (a[i][j] != '\n'))
		{
			if (j == 0)
				printf(">:");

			//printf("%c",a[i][j]);
			if (a[i][j] == '	')
			{
				if (flag == 1)
					for (h=0;h<number;h++)
						printf(" ");
				else printf("%c",a[i][j]);
			}
			else
				printf("%c",a[i][j]);

			j++;
		}
		printf("\n");
		i++;
		k++;

		if (k == leng-1)
			return i;
		if (i == p)
			return i;
	}
	
	printf("\n");
	i++;
	k++;
	

	if (k != leng-1)
		for (m=0; m<leng-k-1; m++)
			printf("\n");

	return i;
}

int perevod_v_canon(void)
{
    //int symbol;
	//struct termios old_attributes,new_attributes;    

    if(!isatty(0))
    {
        printf("Похоже вы перенаправили ввод не из терминала.\n");
        printf("В таких условиях работа программы бессмысленна.\n");
        return 1;  
    }
    
    tcgetattr(0,&old_attributes);
    // копируем атрибуты, чтобы потом восстановить исходное поведение терминала.
  
    memcpy(&new_attributes,&old_attributes,sizeof(struct termios));
    // Теперь буковки набираемые не будут печататься.

    new_attributes.c_lflag &= ~ECHO;
    //  не нужно будет нажимать <enter> для ввода символа.ctrl+d тоже перестанет восприниматься.

    new_attributes.c_lflag &= ~ICANON;
	//Выставляем, чтобы терминал считывал с клавиатуры по одному символу, 
     
    new_attributes.c_cc[VMIN] = 1;
 	new_attributes.c_cc[VTIME] = 0;


    tcsetattr(0,TCSANOW,&new_attributes);
}

int perevod_from_canon(void)
{
    tcsetattr(0,TCSANOW,&old_attributes);
    //printf("\nНастройки терминала изменены обратно\n");
}

int check(char **mass,char *str)
{
	//printf("зашел в check\n");
	int i;
	int t;
	int test=0;
	int flag_ravn = 1;
	for(i=1;i<19;i++)
	{
		//printf("зашел в for\n");
		//printf("mass[%d]: %s\n",i,mass[i]);
		//printf("str: %s\n",str);
		int j = 0;
		int kl = 0;
		while (mass[i][j] != '\0')
		{
			j++;
			kl++;
		}
		for (j = 0; j <= kl-1; j++)
		{
			if (mass[i][j] == str[j])
			{
				flag_ravn = 1;
			}
			else
			{
				flag_ravn = 0;
				break;
			}
		}

		if (flag_ravn == 1)
		{
			test = 1;
			t = i;
			printf("такая команда есть\n");
			if (str[kl] != '\0')
				arrg = kl;
			else arrg = -1;
			break;
		}
	}
	if (test==0)
		printf("неверная команда\n");
	return t;
}

char* vvod(void)
{
	//printf("зашел в vvod\n");
	int i=0;
	int j=0;
	char c;
	char *str;
	printf("editor: ");
	str=(char *)malloc(sizeof(char));
	/*if (str==NULL)
	{
		free(str);
		printf("no memory\n");
		return 0;
	}
	str[i]=s[j];*/

	while((c=fgetc(stdin))!=EOF)
	{
		//printf("зашел в while\n");
		if ((c!=' ') && (c!='\n'))
		{
			//printf("зашел в if\n");
			//printf("c: %c\n",c);

			char *s;
			s=(char *)realloc(str,(j+2)*sizeof(char));
			if (s==NULL)
			{
				free(str);
				printf("no memory\n");
				return 0;
			}
			str[i]=s[j];
			j++;
			str[i]=c;
			i++;
			//printf("str[%d]: %c\n",i,str[i]);
		}
	}
	//printf("вышел из while\n");
	str[i]='\0';
	//printf("введенная строка: %s\n",str);
	return str;
}

char* vvod_stroky(char *str)
{
	//printf("зашел в ввод\n");
	int i=0;
	int j=0;
	char c;
	char *str_tmp;
	int r;
	r = r_vvod;
	//printf("r_vvod=%d\n",r_vvod);

	str_tmp=(char *)malloc(sizeof(char));
	//printf("выделили память\n");

	//printf("str[r]=%c\n",str[r]);
	
	while (str[r] != '"')
	{
			c = str[r];
			str_tmp[i] = c;
			str_tmp = (char *)realloc(str_tmp,(i+1)*sizeof(char));
			i++;
			r++;
	}	

	str_tmp[i]='\0';
	r_vvod = r;
	//printf("\nвведенная строка: %s\n",str_tmp);
	return str_tmp;
}


int set_tabwidth(char *str)
{
	int number=8;
	int j;
	//printf("введите аргументы функции [если их нет, напишите n]: ");
	//scanf("%d", &number);
	if (arrg != -1)
	{
		j = arrg;
		number = str[arrg] - '0';
		j++;
		while (str[j] != '\0')
		{
			number = number*10;
			number = number + str[j] - '0';
			j++;
		}
	}
	
	if (number == 'n')
		number = 8;
	return number;
}

void print_range(char **a, int shir, int leg, int p, int posl_j,char *str)
{
	int arg1=0, arg2=p;
	int tab=0;   //1-строка не влазиет, есть перенос
	int j,i,h;
	int m;
	int r;
	int flag1 = 0;

	//printf("введите первый аргумент функции [если их нет, напишите 0]: ");
	//scanf("%d", &arg1);
	//printf("введите второй аргумент функции [если их нет, напишите 0]: ");
	//scanf("%d", &arg2);
	if (arrg != -1)
	{
		r = arrg;
		arg1 = str[arrg] - '0';
		r++;
		while ((str[r] != ',') && (str[r] != '\0'))
		{
			arg1 = arg1*10;
			arg1 = arg1 + str[r] - '0';
			r++;
		}
		if (str[r] == '\0')
		{
			arg2 = 0;
			flag1 = 1;
		}

		if (flag1 == 0)
		{
			r++;
			if (str[r] != '\0')
			{
				arg2 = str[r] - '0';
				r++;
				while (str[r] != '\0')
				{
					arg2 = arg2*10;
					arg2 = arg2 + str[r] - '0';
					r++;
				}
			}
		}
	}
	else
	{
		arg1 = 0;
		arg2 = 0;
	}

	if (arg1 == 0)
	{
		arg1 = 0;
	}
	else 
		arg1--;
	if (arg2 == 0)
	{
		arg2 = p;
	}

	printf("arg2=%d\n",arg2);

	i=arg1;
	while (i != arg2)
	{
		j=0;
		m=0;
		if ((i == p) && (j == posl_j))
			exit;
		while (a[i][j] != '\n')
		{
			if (j == 0)
				printf(">:");

			if (m >= shir-2)
			{
				tab=1;
				m=0;
				}


				if (tab == 0)
				{
					printf("%c",a[i][j]);
				}
				else
				{	
					tab=0;
					printf("\n");

					printf("%c",a[i][j]);
				}
			j++;
			m++;
		}
		printf("\n");
		i++;
	}
}

void help(void)
{
	printf("[editor: ] приглаешает к вводу команды.\n");
	printf("После ввода команды, нажмите ctrl+d.\n");
}

FILE* p_write(char **a, FILE* f1,int p)
{
	if (flag_write == 1)
	{
		f1 = fopen(filename1,"w+");
	}
	int i=0;
	int j=0;
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
	return f1;
}

char edit_string(char **a, int p, int stroka,int stolbes, char sym)
{
	int i,j,m,k;

	if (stroka >= p)
	{
		printf("ошибка : указана конструкция вне диапазона1\n");
		return 0;
	}

	//подсчет колтчества символов в строке = m
	i = stroka;
	j = 0;
	m = 0;
	while (a[i][j] != '\n')
	{
		m++;
		j++;
	}
	m++;

	if (stolbes > m)
	{
		printf("ошибка : указана конструкция вне диапазона3\n");
		return 0;
	}

	a[stroka][stolbes] = sym;

	return a[stroka][stolbes];
}

char** edit_string_n(char **a, int p, int stroka, int stolbes, char sym)
{
	int i,j,m,k;

	if (stroka >= p)
	{
		printf("ошибка : указана конструкция вне диапазона\n");
		return 0;
	}

	//подсчет колтчества символов в строке = m
	i = stroka;
	j = 0;
	m = 0;
	while (a[i][j] != '\n')
	{
		m++;
		j++;
	}
	m++;

	if (stolbes > m)
	{
		printf("ошибка : указана конструкция вне диапазона\n");
		return 0;
	}

		i=stroka;
		j=stolbes;

		a[i][j] = sym;

		//добавление 1 строки в конец
		a = (char **)realloc(a,(p+2)*sizeof(char *));
		a[p] = (char *)malloc(sizeof(char));
		a[p][0] = 'w';
		
		i=p-1;
		while (i != stroka)
		{
			j = 0;
			m = 0;
			//смотрим сколько в строке символов
			while(a[i][j] != '\n')
			{
				m++;
				j++;
			}

			//освобождаем следующую строку
			free(a[i+1]);
			//создаем следующую строку с нужным количеством символов
			a[i+1] = (char *)malloc(m*sizeof(char));

			j = 0;
			//копирование строк в следующую строку
			while (a[i][j] != '\n')
			{
				a[i+1][j] = a[i][j];
				j++;
			}
			a[i+1][j] = a[i][j];

			i--;
		}

		//копирование остатка строки на слудующую
		i = stroka;
		j = stolbes+1;
		m = 0;
		while(a[i][j] != '\n')
		{
				m++;
				j++;
		}
		//освобождаем следующую строку
		free(a[i+1]);
		//создаем следующую строку с нужным количеством символов
		a[i+1] = (char *)malloc(m*sizeof(char));

		j = stolbes+1;
		k = 0;
		//копирование строк в следующую строку
		while (a[i][j] != '\n')
		{
			a[i+1][k] = a[i][j];
			j++;
			k++;
		}
		a[i+1][k] = a[i][j];

		return a;
}

char** insert_symbol(char **a,int stroka, int stolbes, char sym, int p, int posl_j)
{
	char c;
	int i,j, end_j;
	int k,m;

	if (sym != '\n')
	{
		i = stroka;
		j = 0;
	
		while(a[i][j] != '\n')
			j++;

		end_j = j;

		if (stolbes > end_j)
		{
			while (end_j != stolbes)
			{
				a[i] = (char *)realloc(a[i],(end_j+1)*sizeof(char));
				a[i][end_j]=' ';
				end_j++;
			}
			a[i][end_j] = '\n';
		}

		j = stolbes;
		while (a[i][j] != '\n') 
		{
			c = a[i][j];
			a[i][j] = sym;
			j++;
			sym = c;
		}
		c = a[i][j];   //c='\n'
		a[i][j] = sym;
		sym = c;		//sym = '\n'

		a[i] = (char *)realloc(a[i],(j+1)*sizeof(char));
		j++;
		a[i][j] = sym;	//a[i][j] = '\n'

		//for(j = 0; j <= end_j+1; j++)
		//{
		//	printf("a[%d][%d]=%c\n",stroka,j,a[stroka][j]);
		//}
		return a;
	}

	if (sym == '\n')
	{
		i=stroka;
		j=stolbes;

		c = a[i][j];
		a[i][j] = sym;

		//добавление 1 строки в конец
		a = (char **)realloc(a,(p+2)*sizeof(char *));
		a[p] = (char *)malloc(sizeof(char));
		a[p][0] = 'w';
		
		i=p-1;
		while (i != stroka)
		{
			j = 0;
			m = 0;
			//смотрим сколько в строке символов
			while(a[i][j] != '\n')
			{
				m++;
				j++;
			}

			//освобождаем следующую строку
			free(a[i+1]);
			//создаем следующую строку с нужным количеством символов
			a[i+1] = (char *)malloc(m*sizeof(char));

			j = 0;
			//копирование строк в следующую строку
			while (a[i][j] != '\n')
			{
				a[i+1][j] = a[i][j];
				j++;
			}
			a[i+1][j] = a[i][j];

			i--;
		}

		//копирование остатка строки на слудующую
		i = stroka;
		j = stolbes+1;
		m = 0;
		while(a[i][j] != '\n')
		{
				m++;
				j++;
		}
		//освобождаем следующую строку
		free(a[i+1]);
		//создаем следующую строку с нужным количеством символов
		a[i+1] = (char *)malloc((m+1)*sizeof(char));

		j = stolbes+1;
		k = 1;
		//копирование строк в следующую строку
		a[i+1][0] = c;
		while (a[i][j] != '\n')
		{
			a[i+1][k] = a[i][j];
			j++;
			k++;
		}
		a[i+1][k] = a[i][j];
	}

	return a;
}

char** replace_substring(char **a, int p, char *str)
{
	int d1,d2;
	char *str1;
	char *str2; char *str_copy;
	int test = 0; //строка str1 найдена в указанном диапазоне
	int stroka,i,j,ki,l;
	char **b;
	int m,k;
	int perehod = -1; //присутствует \n в строке, на которую надо заменить
	int special_symbol_1 =0, special_symbol_2 = 0;

	/*printf("введите 3-ий аргумент\n[строка, которую заменить] : ");
	str1 = vvod_stroky();
	printf("введите 4-ый аргумент\n[строка, на которую заменить] : ");
	str2 = vvod_stroky();
	printf("введите 1-ый аргумент\nесли его нет, введите 0\n[левая граница диапазона] : ");
	scanf("%d",&d1);
	printf("введите 2-oй аргумент\nесли его нет, введите 0\n[правая граница диапазона] : ");
	scanf("%d",&d2);*/

	int r;

	if (arrg != -1)
	{
		if (str[arrg] == '"')
		{
			d1 = 0;
			d2 = 0;
			r_vvod = arrg+1;
			str1 = vvod_stroky(str);
			r = r_vvod;
			r_vvod = r+3;
			str2 = vvod_stroky(str);
		}
		else
		{
			r = arrg;
			d1 = str[arrg] - '0';
			r++;
			while (str[r] != ',')
			{
				d1 = d1*10;
				d1 = d1 + str[r] - '0';
				r++;
			}
			r++;
			if (str[r] != '\0')
			{
				d2 = str[r] - '0';
				r++;
				while (str[r] != ',')
				{
					d2 = d2*10;
					d2 = d2 + str[r] - '0';
					r++;
				}
			}
			r_vvod = r +2;
			//printf("d1=%d\n",d1);
			//printf("d2=%d\n",d2);
			str1 = vvod_stroky(str);
			//printf("stroka1=%s\n",str1);
			r=r_vvod;
			r_vvod = r+3;
			str2 = vvod_stroky(str);
			//printf("stroka2=%s\n",str2);
		}
	}
	else
	{
		printf("ошибка: должны быть аргументы\n");
		return 0;
	}


	int kolvo_str_copy = 0;

	if (d1 == 0)
		d1 = 0;
	else
		d1--;
	if (d2 == 0)
		d2 = p-1;
	else
		d2--;

	if (strcmp(str1,"''") == 0)
	{
		special_symbol_1 = 1;
		test = 1;
		//printf("special_symbol_1=%d\n",special_symbol_1);
	}
	if (strcmp(str1,"$") == 0)
	{
		special_symbol_2 = 1;
		test = 1;
	}

	m = d2-d1+1;
	b = (char **)malloc(m*sizeof(char *));

	//копирование диапазона строк в массив b(на конце \0)
	for (i = d1; i <= d2; i++)
	{
		j = 0;
		m = 0;
		while(a[i][j] != '\n')
		{
			m++;
			j++;
		}
		b[i] = (char *)malloc((m+1)*sizeof(char));

		j =0;
		while (a[i][j] != '\n')
		{
			b[i][j] = a[i][j];
			j++;
		}
		b[i][j] = '\0';

		//printf("b[%d]=%s\n",i,b[i]);
		//printf("str1=%s\n",str1);

		if (strcmp(b[i],str1) == 0)
		{
			test = 1;
			stroka = i;
		}
	}

	if (test == 1)
	{
		//printf("строка найдена\n");
		if ((special_symbol_1 == 0) && (special_symbol_2 == 0))
		{
			free(a[stroka]);
			//printf("память очищена\n");
		}
			//вычисляем сколько символов в str2
			i = 0;
			m = 0;
			while (str2[i] != '\0')
			{
				if (str2[i] == '\n')
					perehod = i;
				m++;
				i++;
			}
			m++;
			//printf("колво символов в str2: %d\n",i);
		if ((special_symbol_1 == 0) && (special_symbol_2 == 0))
		{
			//выделяем память под новую строку
			a[stroka] = (char *)malloc(m*sizeof(char));
			//printf("память выделена в a[%d]\n",stroka);
		}

		if (perehod == -1)
		{
			if (special_symbol_1 == 1)
			{
				for (l = d1; l <= d2; l++)
				{
					//количество символов в str2 = m
					j = 0;
					while (b[l][j] != '\0')
					{
						j++;
						m++;
					}
					m++; //для \n

					free(a[l]);
					a[l] = (char *)malloc(m*sizeof(char));

					i = 0;
					j = 0;
					while (str2[i] != '\0')
					{
						a[l][j] = str2[i];
						j++;
						i++;
					}
					i = 0;
					while (b[l][i] != '\0')
					{
						a[l][j] = b[l][i];
						j++;
						i++;
					}
					a[l][j] = '\n';
				}
			}
			else 
			if (special_symbol_2 == 1)
			{
				for (l = d1; l <= d2; l++)
				{
					//колво символов в str2 = m
					j = 0;
					while (b[l][j] != '\0')
					{
						j++;
						m++;
					}
					m++; //для \n

					free(a[l]);
					a[l] = (char *)malloc(m*sizeof(char));

					j = 0;
					while (b[l][j] != '\0')
					{
						a[l][j] = b[l][j];
						j++;
					}
					i = 0;
					while (str2[i] != '\0')
					{
						a[l][j] = str2[i];
						j++;
						i++;
					}
					a[l][j] = '\n';
				}
			}
			else
			{
				i = 0;
				j = 0;
				while (str2[i] != '\0')
				{
					a[stroka][j] = str2[i];
					i++;
					j++;
				}
				a[stroka][j] = '\n';
			}
		}
		else
		{
			flag_new_string_n = 1;

			for (j = 0; j <= perehod; j++)
				a[stroka][j] = str2[j];

			//добавление 1 строки в конец
			a = (char **)realloc(a,(p+2)*sizeof(char *));
			a[p] = (char *)malloc(sizeof(char));
			a[p][0] = 'w';
		
			i=p-1;
			while (i != stroka)
			{
				j = 0;
				m = 0;
				//смотрим сколько в строке символов
				while(a[i][j] != '\n')
				{
					m++;
					j++;
				}

				//освобождаем следующую строку
				free(a[i+1]);
				//создаем следующую строку с нужным количеством символов
				a[i+1] = (char *)malloc(m*sizeof(char));

				j = 0;
				//копирование строк в следующую строку
				while (a[i][j] != '\n')
				{
					a[i+1][j] = a[i][j];
					j++;
				}
				a[i+1][j] = a[i][j];

				i--;
			}

			//копирование остатка строки на слудующую
			i = stroka;
			j = perehod+1;
			m = 0;
			while(str2[j] != '\0')
			{
				m++;
				j++;
			}
			//освобождаем следующую строку
			free(a[i+1]);
			//создаем следующую строку с нужным количеством символов
			a[i+1] = (char *)malloc((m+1)*sizeof(char));

			j = perehod+1;
			k = 0;
			//копирование строк в следующую строку
			while (str2[j] != '\0')
			{
				a[i+1][k] = str2[j];
				j++;
				k++;
			}
			a[i+1][k] = '\n';
		}
	}

	return a;
}

char** delete_range(char **a, int d1, int d2, int p)
{
	int r;
	int i,m,j,l;

	for (i = d1; i <= d2; i++)
		free(a[i]);

	l = d1;
	for (i = d2+1; i <= p-1; i++)
	{
		//считаем сколько символов в a[i]
		m = 0;
		j = 0;
		while(a[i][j] != '\n')
		{
			m++;
			j++;
		}
		m++; //для \n
		//освобождаем место для копируемой строки
		a[l] = (char *)malloc(sizeof(char));
		//копируем строку
		j = 0;
		while (a[i][j] != '\n')
		{
			a[l][j] = a[i][j];
			j++;
		}
		a[l][j] = '\n';
		l++;
		free(a[i]);
	}
	return a;
}

char** delete_braces(char **a, int d1, int d2, int p)
{
	int i,j,m,l;
	int k,ki = 0,end_k,end_ki = 0;
	char *a_copy;

	for(i = d1; i <= d2; i++)
	{
		j = 0;
		while (a[i][j] != '\n')
		{
			if (a[i][j] == '{')
			{
				k = j;
				ki = i;
			}
			if (a[i][j] == '{')
				break;
			j++;
		}
	}
	
	//i = d2;
	for(i = d1; i <= d2; i++)
	{
		j = 0;
		while (a[i][j] != '\n')
		{
			if (a[i][j] == '}')
			{
				end_k = j;
				end_ki = i;
			}
			j++;
		}
		//if (a[i][j] == '}')
		//	break;
		//i--;
	}
	razn = end_ki-ki;

	if (ki != end_ki)
	{
		//освобождаем память между скобками и сами скобки
		i = ki;
		j = 0;
		m = 0;
		while(a[i][j] != '{')
		{
			m++;
			j++;
		}

		a_copy = (char *)malloc((m+1)*sizeof(char));
		j = 0;
		while (a[i][j] != '{')
		{
			a_copy[j] = a[i][j];
			j++;
		}
		a_copy[j] = '\0';
		free(a[i]);
		a[i] = (char *)malloc(m*sizeof(char));
		j = 0;
		while (a_copy[j] != '\0')
		{
			a[i][j] = a_copy[j];
			j++;
		}
		//перенос остатка строки на ту строку
		i = end_ki;
		j = end_k+1; // символ после }
		while (a[i][j] != '\n')
		{
			a[ki] = (char *)realloc(a[ki],(k+1)*sizeof(char));
			a[ki][k] = a[i][j];
			k++;
			j++;
		}
		a[ki] = (char *)realloc(a[ki],(k+1)*sizeof(char));
		a[ki][k] = '\n';

		//копирование оставшихся строк
		l = ki+1;
		for (i = end_ki+1; i <= p-1; i++)
		{
			//считаем сколько символов в a[i]
			m = 0;
			j = 0;
			while(a[i][j] != '\n')
			{
				m++;
				j++;
			}
			m++; //для \n

			//освобождаем место a[l]
			free(a[l]);
			//выделяeм нужное количество символов для a[i]
			a[l] = (char *)malloc(m*sizeof(char));
			//копируем
			j = 0;
			while (a[i][j] != '\n')
			{
				a[l][j] = a[i][j];
				j++; 
			}
			a[l][j] = '\n';
			l++;
			free(a[i]);
		}
	}
	else
	{
		j = 0;
		m = 0;
		while(a[ki][j] != '{')
		{
			m++;
			j++;
		}
		a_copy = (char *)malloc(m*sizeof(char));
		j = 0;
		while(a[ki][j] != '{')
		{
			a_copy[j] = a[ki][j];
			j++;
		}
		l = j;
		j = end_k+1;
		while(a[ki][j] != '\n')
		{
			a_copy = (char *)realloc(a_copy,(m+1)*sizeof(char));
			a_copy[l] = a[ki][j];
			m++;
			j++;
			l++;
		}
		a_copy = (char *)realloc(a_copy,(m+1)*sizeof(char));
		a_copy[l] = '\n';
		free(a[ki]);
		a[ki] = (char *)malloc((m+1)*sizeof(char));
		j = 0;
		while(a_copy[j] != '\n')
		{
			a[ki][j] = a_copy[j];
			j++;
		}
		a[ki][j] = '\n';
	}
	free(a_copy);
	return a;
}

char** insert_after(char **a)
{
}

char** p_read(char **a,int p, int posl_j,char *str)
{
	int i = 0;
	int j = 0;
	int r;
	char c;
	char *filename;
	FILE *f3;
	/*printf("введите имя файла: ");
	scanf("%s",filename);*/
	if (arrg != -1)
	{
		r = arrg +1;
		r_vvod = r;
		filename = vvod_stroky(str);
	}
	else 
	{
		printf("ошибка:должны быть аргументы\n");
		return 0;
	}

	f3 = fopen(filename,"r");
	if (f3 == NULL)
	{
		printf("error openning file\n");
		return 0;
	}
	else
	{
		//printf("файл открыт\n");
		//освобождаем массив a
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

		//printf("память освобождена\n");
		//заполняем массив из файла f3
		p = 0;
		char **a_new1;
		a_new1=(char **)malloc(sizeof(char *));
		if (a_new1==NULL)
		{
			free(a);
			printf("no memory\n");
			return 0;
		}
		a=a_new1;

		while(1)
		{
			c=fgetc(f3);
			if (c==EOF)
			{
				if (feof(f3)!=0)
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
					return 0;
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
					return 0;
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
					return 0;
				}
				a=a_new;
				p++;
				posl_j=j;
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
				return 0;
			}
			a[p]=a_new;
			a[p][j]='\n';
			posl_j=j;
			p++;
		}
		fclose(f3);
		//printf("ок\n");
		p_p = p;
		p_posl_j = posl_j;
	}
	return a;
}

char** open(char **a, int p, int posl_j,char *str)
{
	int i = 0;
	int j = 0;
	char c;
	int r;
	//char filename1[25];
	FILE *f3;
	/*printf("введите имя файла: ");
	scanf("%s",filename1);*/
	if (arrg != -1)
	{
		r = arrg +1;
		r_vvod = r;
		filename1 = vvod_stroky(str);
	}
	else 
	{
		printf("ошибка:должны быть аргументы\n");
		return 0;
	}

	f3 = fopen(filename1,"r");
	flag_write = 1;
	if (f3 == NULL)
	{
		printf("error openning file\n");
		return 0;
	}
	else
	{
		//освобождаем массив a
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

		//заполняем массив из файла f3
		p = 0;
		char **a_new1;
		a_new1=(char **)malloc(sizeof(char *));
		if (a_new1==NULL)
		{
			free(a);
			printf("no memory\n");
			return 0;
		}
		a=a_new1;

		while(1)
		{
			c=fgetc(f3);
			if (c==EOF)
			{
				if (feof(f3)!=0)
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
					return 0;
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
					return 0;
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
					return 0;
				}
				a=a_new;
				p++;
				posl_j=j;
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
				return 0;
			}
			a[p]=a_new;
			a[p][j]='\n';
			posl_j=j;
			p++;
		}
	}
	f1 = f3;
	fclose(f3);
	//printf("ок\n");
	p_p = p;
	//printf("p_p=%d \n",p_p);
	p_posl_j = posl_j;
	//printf("p_posl_j=%d\n",posl_j);
	return a;
}