#include "errors.hpp"
#include <stdio.h>

znam_ERROR::znam_ERROR()
{};

delenie_na_0_ERROR::delenie_na_0_ERROR()
{};

memory_allocated_ERROR::memory_allocated_ERROR(unsigned long int a):size(a)
{};

types_ERROR::types_ERROR()
{};

matrix_razmer_ERROR::matrix_razmer_ERROR()
{};

vector_razmer_ERROR::vector_razmer_ERROR()
{};