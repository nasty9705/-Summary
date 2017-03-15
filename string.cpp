#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <fstream>
using namespace std;

class ExceptionCatcher
{
public:
	int sizeOfNonAllocatedMemory;
	ExceptionCatcher(int size){sizeOfNonAllocatedMemory = size;};	
};

class My_string
{
	private:
		char *body;
		size_t len;
		friend My_string operator +(const char *, const My_string); //строка+класс           
		friend bool operator ==(const My_string &, const My_string & ); //сравнение классов  
	public:
		void show_class();
		size_t length();
		My_string& operator=(const My_string& arg);
		My_string operator+(const char *arg)const;
		My_string operator+(const My_string &str)const;
		My_string(const char *); //конструктор класс:=строку     
		My_string(); //конструктор по умолчанию                 
		My_string(const My_string &); // конструктор класс:=класс   
		~My_string(); //деструктор по умолчанию                  
		My_string(const char *, int); //конструктор класс:=из файла          

};


My_string::My_string(const char *filename,int a)
{
	ifstream fin;
	fin.open(filename, ios::in);
	if (!fin)
	{
		cout << "не удается открыть файл\n";
		return;
	}

	char c;
	int i = 0;
	//char *str_temp = new char[1];
	char *str_temp;
	str_temp = (char*)malloc(sizeof(char));
	if (str_temp == NULL)
	{
		throw ExceptionCatcher(1);
	}

	while(!fin.eof())
	{
		//cout << "начало цикла: i="<<i<<"\n";
		fin.get(c);
		if (c == EOF)
			break;
		if (c == '\n')
			break;
		//str_temp[i] = c;
		str_temp = (char*)realloc(str_temp,(i+1)*sizeof(char));
		str_temp[i]=c;
		if (str_temp == NULL)
		{
			throw ExceptionCatcher((i+1)*sizeof(char));
		}
		i++;
		//cout << "строка: "<<str_temp<<"\n";
		//cout << "конец цикла i="<<i<<"\n\n\n";
	}
	//cout << "вышли из цикла i="<<i<<"\n";
	len = i;
	//cout <<str_temp<<endl;
	//body = new char[len];
	//char *body;
	body = (char*)malloc(len*sizeof(char));
	if (body == NULL)
	{
		throw ExceptionCatcher(len);
	}
	//strcat(body,str_temp);
	for (int i = 0; i<len; i++)
	{
		body[i] = str_temp[i];
	}
		//cout << "получена строка: ";
		//cout << body<<endl;
		//len = i-2;
		//cout<< "длина= ";
		//cout<<len<<endl;
	free(str_temp);

	fin.close();
}

My_string::My_string() //конструктор по умолчанию
{
	//cout << "555555\n";
	len=0;
	body=NULL;
}

My_string::My_string(const char *str) // конструктор класс := строку
{
	if (str == NULL)
	{
		//cout << "*********\n";
		body = NULL;
		len=0;
		//My_string();
	}
	else
	{
		len = strlen(str);
		//body = (char*)malloc((len+1)*sizeof(char));
		//body = new char[len+1];
		//char *body;
		body = (char*)malloc((len+1)*sizeof(char));
		if (body == NULL)
		{
			throw ExceptionCatcher(len+1);
		}
		/*if (body == NULL)
		{
				throw string::exeption();
		}*/
		//strcat(body,str);
		int i;
		for (i=0; i< len; i++)
		{
			body[i] = str[i];
		}
		body[i]='\0';
	}
	return;
}
	
My_string::~My_string() //деструктор по умолчанию
{
	/*if (body!=NULL)
	{
		free(body);
		body=NULL;
	}*/
	//cout<<"Destr\n"	; 
	if (body != NULL)
	{
		//delete[] body;
		free(body);
		body = NULL;
	}
}

My_string::My_string (const My_string &other) //конструктор копирования класс:=классу
{
	if ((other.body) == NULL)
	{
		this->body = NULL;
		this->len = 0;
	}
	else
	{
		this->len = other.len;
		//this->body = (char*)malloc(len*sizeof(char));
		//this->body = new char[this->len+1];
		this->body = (char*)malloc((this->len+1)*sizeof(char));
		if (this->body == NULL)
		{
			throw ExceptionCatcher(this->len+1);
		}
		//other.body = (char*)realloc(other.body,(other.len+1)*sizeof(char));
		int i;
		for (i=0; i<(this->len) ; i++)
		{
			this->body[i] = other.body[i];
		}
		this->body[i]='\0';
	}
}

void My_string::show_class()//вывод класса
{
	//cout << "--------\n";
	if(!body)
	{
		cout << "body пустое\n";
		return;
	}
	if (body != NULL)
	{
		cout << body<<endl;
	}
	//cout<<"End of show\n";
}

My_string operator+(const char *op1, const My_string op2) //перегрузка строка+класс
{
	cout << "строка + класс\n";
	My_string a(op1);
	My_string temp;
	temp.body=(char*)malloc((a.len + op2.len+1)*sizeof(char));
	if (temp.body == NULL)
	{
		throw ExceptionCatcher((a.len + op2.len+1)*sizeof(char));
	}
	//strcpy(temp.body,a.body);
	for (int i = 0; i<(a.len+1); i++)
	{
		temp.body[i] = a.body[i];
	}
	strcat(temp.body,op2.body);
	temp.len = a.len + op2.len;
	return temp;
}

My_string My_string::operator+(const My_string &str)const //перегрузка класс+класс
{
	cout << "класс + класс\n";
	My_string temp;
	if (str.body == NULL)
	{
		temp.body = (char*)malloc((this->len+1)*sizeof(char));
		if (temp.body == NULL)
		{
			throw ExceptionCatcher((this->len+1)*sizeof(char));
		}
		strcpy(temp.body, this->body);
		temp.len = this->len;
		return temp;
	}
	if (this->body == NULL)
	{
		temp.body = (char*)malloc((str.len+1)*sizeof(char));
		if (temp.body == NULL)
		{
			throw ExceptionCatcher((str.len+1)*sizeof(char));
		}
		strcpy(temp.body, str.body);
		temp.len = str.len;
		return temp;
	}
	else
	{
		temp.body= (char*)malloc((this->len+str.len+1)*sizeof(char));
		if (temp.body == NULL)
		{
			throw ExceptionCatcher((this->len+str.len+1)*sizeof(char));
		}
		//cout << this->body<<endl;
		strcpy(temp.body,this->body);
		//cout << temp.body<<endl;
		//cout << str.body<< endl;
		strcat(temp.body,str.body);
		temp.len=str.len+this->len;
		//cout <<"********\n";
		//cout <<temp.body<<endl;
		return temp;
	}


	temp.body = (char*)malloc((this->len + str.len+1)*sizeof(char));
	if (temp.body == NULL)
	{
		throw ExceptionCatcher((this->len + str.len+1)*sizeof(char));
	}
	//strcpy(temp.body, this->body);
	for (int i = 0; i<(this->len+1); i++)
	{
		temp.body[i] = this->body[i];
	}
	strcat(temp.body, str.body);
	temp.len = this->len + str.len;
	//cout << "результат2 :";
	//temp.show_class();
	return temp;
}


My_string &My_string::operator=(const My_string& arg) //перегрузка =
{
	//this->body = new char[arg.len+1];
	this->~My_string();
	this->body = (char*)malloc((arg.len+1)*sizeof(char));
	if (this->body == NULL)
	{
		throw ExceptionCatcher(arg.len+1);
	}
	for (int i = 0; i<(arg.len+1); i++)
	{
		this->body[i] = arg.body[i];
	}
	//strcpy(this->body, arg.body);
	this->len = arg.len;
	return *this;
}

My_string My_string::operator+(const char *arg)const //перегрузка класс+стока
{
	My_string temp;
	cout << "класс+строка\n";

	if (arg == NULL)
	{
		return *this;
	}
	//cout << "класс: ";
	//this->show_class();
	My_string base(arg);
	//cout << "после преобразования строки в класс: ";
	//base.show_class();
	temp.body=(char*)malloc((base.len + this->len+1)*sizeof(char));
	if (temp.body == NULL)
	{
		throw ExceptionCatcher((base.len + this->len+1)*sizeof(char));
	}
	//strcpy(temp.body, this->body);
	for (int i = 0; i<(this->len+1); i++)
	{
		temp.body[i] = this->body[i];
	}
	strcat(temp.body, base.body);
	temp.len=base.len + this->len;
	//cout << "результат1 :";
	//temp.show_class();
	return temp;
}

bool operator == (const My_string &str, const My_string &str1) //перегрузка сравнене классов
{
	My_string base(str);
	My_string a(str1);
	if (base.len == a.len)
	{
		int t=0;
		int i;
		for (i=0; i < (base.len); i++)
		{
			if (base.body[i] != a.body[i])
			{
				return false;
			}
			else t=1;
			//return true;i
		}
		if (t == 1) 
			return true;
	}
	else
	{
		return false;
	}
}



int main()
{
	int sizeOfNonAllocatedMemory;

	try{
		//char *a = new char[70];
		//char *b = new char[70];
		char *a;
		char *b;
		a = (char*)malloc(70*sizeof(char));
		b = (char*)malloc(70*sizeof(char));

		cout << "введите строку a\n";
		cin.getline(a,68);
		cout << "введите строку b\n";
		cin.getline(b,68);


		My_string my_class_a(a);
		My_string my_class_b(b);
		My_string class_c;
		My_string p;

		cout << "строка а:";
		my_class_a.show_class();
		cout << "строка b:";
		my_class_b.show_class();
		cout << "\n";

		//класс = из файла
		FILE* f;
		char filename[50];
		cout << "введите название файла: ";
		cin >> filename;
		My_string wo(filename,1);
		cout << "сравнение класса с файлом\n";
		if (wo == my_class_a)
			cout << "равны\n";
		else cout << "не равны\n";
			cout << "\n";

		//сравнение классов
		cout << "сравнение классов\n";
		if (my_class_b == my_class_a)
		{
			cout << "равны\n";
		}
		else
		{
			cout << "не равны\n";
		}
		cout << "\n";

		//конструктор копирования      
		My_string poo(my_class_b);
		cout << "класс poo: ";
		poo.show_class();
		cout << "\n";

		//класс + строка             
		class_c = (my_class_a + b);
		cout << "результат: ";
		class_c.show_class();
		cout << "\n";

		//класс+класс                
		p = my_class_a + my_class_b;
		cout << "результат :";
		p.show_class();
		cout << "\n";

		//строка+класс                
		p = a + my_class_b;
		cout << "результат :";
		p.show_class();
		cout << "\n";

	    my_class_a = class_c = my_class_a + my_class_b;
   		cout << "<Double = >" << endl;
   	 	my_class_a.show_class();
   		class_c.show_class();


		cout<< "NULL strings:" <<endl;
    	const char *n_str = NULL;

	    My_string e(n_str);
		cout << "e : ";
		e.show_class();
		cout << "\n";
    	My_string e1(e);
    	cout << "el: ";
    	e1.show_class();
    	cout << "\n";

    
    	my_class_a = my_class_a + e1;
		cout << "my_class_a : ";
		my_class_a.show_class();
		cout << "\n";

    	my_class_a = e1 + my_class_a;
    	cout << "my_class_a : ";
    	my_class_a.show_class();
    	cout << "\n";

    	my_class_a = my_class_a + n_str;

    	const My_string s1("aaa"), s2("bbb");
    	my_class_a = s1+s2;
    	cout << "my_class_a : ";
    	my_class_a.show_class();
    	cout << "\n";

    	My_string qi(NULL);
    	cout << "qi: ";
    	qi.show_class();
    	free(b);
		free(a);

		return 0;
	}
	catch(ExceptionCatcher){
		printf("Cannot allocate %i bytes!\n", sizeOfNonAllocatedMemory);
	}

	
	/*p.~My_string();
	class_c.~My_string();
	my_class_b.~My_string();
	my_class_a.~My_string();*/

	//return 0;
}
