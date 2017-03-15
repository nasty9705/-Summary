#include "errors.hpp"
#include "elements.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

Elements::Elements()
{
    array = NULL;
    size = 0;
}

Elements::Elements(const Elements &op)
{
    array = NULL;
    size = 0;
    
    for (unsigned long int i = 0; i < op.size; i++)
    {
        add_element(op.array[i].i, op.array[i].j, op.array[i].subject);
    }
}


Elements :: ~Elements()
{
    delete []array;
}


Elements Elements::operator=(const Elements &op)
{
    delete []array;
    size = 0;
    array = NULL;
    for (unsigned long int i = 0; i < op.size; i++)
    {
        add_element(op.array[i].i, op.array[i].j, op.array[i].subject);
    }
    return *this;
}


void Elements::add_element(unsigned long int i, unsigned long int j, const RatNum &op)
{
    for (unsigned long int k = 0; k < size; k++)
    {
        if ((array[k].i == i) && (array[k].j == j))
        {
            array[k].subject = op;
            return;
        }
    }
    
    element* tmp;
    tmp = new element[size + 1];
    if (tmp == NULL) memory_allocated_ERROR(size + 1);
    
    for (unsigned long int k = 0; k < size; k++)
    {
        tmp[k].i = array[k].i;
        tmp[k].j = array[k].j;
        tmp[k].subject = array[k].subject;
    }
    
    if (array != NULL)
        delete []array;
    array = tmp;
    size++;
    array[size - 1].i = i;
    array[size - 1].j = j;
    array[size - 1].subject = op;
}

void Elements::delete_element(unsigned long int i, unsigned long int j)
{
    unsigned long int k;
    if (array == NULL)
        return;
    
    for (k = 0; k < size; k++)
    {
        if ((array[k].i == i) && (array[k].j == j))
            break;
    }
    
    if ((size == 1) && (k == 0))
    {
        delete []array;
        array = NULL;
        size = 0;
    }
    
    Elements tmp;
    
    for (unsigned long int i = 0; i < size; i++)
    {
        if (i != k)
            tmp.add_element(array[i].i, array[i].j, array[i].subject);
    }
    
    *this = tmp;
}


RatNum Elements::get(unsigned long int i, unsigned long int j)const
{
    for (unsigned long int k = 0; k < size; k++)
    {
        if ((array[k].i == i) && (array[k].j == j))
	{
	//printf("i = %ld ",i);
	//printf("j = %ld ",j);
	//array[k].subject.print();
	//printf("\n");	
            return array[k].subject;
	}
    }
    return RatNum();
}
