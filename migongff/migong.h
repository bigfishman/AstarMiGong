#ifndef _H_MIGONG_H
#define _H_MIGONG_H
#include<stdio.h> 
#include<stdlib.h> 
#include<windows.h>
#include<malloc.h>


int InitStack(PLStack &S);

int StackEmpty(PLStack S);
int Push(PLStack &S, Element e);
int Pop(PLStack &S,Element &e);
void MazePath();
#endif