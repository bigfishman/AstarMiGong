#include<stdio.h> 
#include<stdlib.h> 
#include<malloc.h>
#include"migong.h"
#include "stdafx.h"


struct mark //定义迷宫内点的坐标类型 
{ 
	int x; 
	int y; 
}; 

struct Element //栈元素 
{ 
	int x,y; //x行,y列 
	int d; //d下一步的方向 
}; 

typedef struct LStack //链栈 
{ 
	Element elem; 
	struct LStack *next; 
}*PLStack; 


/*************栈函数****************/ 

int InitStack(PLStack &S)//构造空栈 
{ 
	S=NULL; 
	return 1; 
} 

int StackEmpty(PLStack S)//判断栈是否为空 
{ 
	if(S==NULL) 
	return 1; 
	else 
	return 0; 
} 

int Push(PLStack &S, Element e)//压入新数据元素 
{ 
	PLStack p; 
//	p=(PLStack)malloc(sizeof(LStack)); 
	p->elem=e; 
	p->next=S; 
	S=p; 
	return 1; 
} 

int Pop(PLStack &S,Element &e) //栈顶元素出栈 
{ 
	PLStack p; 
	if(!StackEmpty(S)) 
	{ 
		e=S->elem; 
		p=S; 
		S=S->next; 
	//	free(p); 
		return 1; 
	} 
	else 
		return 0; 
} 


/***************求迷宫路径函数***********************/ 
void MazePath() 
{ 
	int i,j,d;
	int a,b; 
  /*迷宫初始化*/
	int maze[7][7]={
			{0,1,1,1,1,1,1},
			{0,0,0,1,0,1,1},
			{1,1,0,1,1,1,1},
			{0,1,0,0,0,0,1},
			{1,1,1,1,1,0,1},
			{1,1,0,0,1,0,0},
			{1,1,1,1,1,1,0}
		};

    struct mark start,end;//用于标记迷宫开始点和结束点
	start.x=0;
	start.x=0;
	end.y =6;
	end.y =6;

	int add[4][2]={{1,0},{0,1},{-1,0},{0,-1}};//行和列增量 依次为下右上左

	Element elem,e; 
	PLStack S1, S2; 
	InitStack(S1); 
	InitStack(S2); 

	maze[start.x][start.y]=2; //入口点作上标记 
	elem.x=start.x; 
	elem.y=start.y; 
	elem.d=-1; //开始为-1 

	Push(S1,elem); //入口进栈

	while(!StackEmpty(S1)) //栈不为空 有路径可走 
	{ 
		Pop(S1,elem); 
		i=elem.x; 
		j=elem.y; 
		d=elem.d+1; //下一个方向 
		while(d<4) //试探各个方向 
		{ 

			a=i+add[d][0]; 
			b=j+add[d][1]; 

			if(a==end.x && b==end.y && maze[a][b]==0) //如果到了出口 
			{ 
			elem.x=i; 
			elem.y=j; 
			elem.d=d; 

			Push(S1,elem); //进栈

			elem.x=a; 
			elem.y=b; 
			elem.d=8; //方向输出为-1 判断是否到了出口 

			Push(S1,elem); //最后一个元素进栈

				while(S1) //逆置序列 并输出迷宫路径序列 从s1中转到s2中
				{ 
					Pop(S1,e); 
					Push(S2,e);   
				} 
				while(S2) 
				{ 
					Pop(S2,e); 
				//	printf("-->(%d,%d,%d)",e.x,e.y,e.d);
					maze[e.x][e.y] = 9;
				} 
				return; //跳出两层循环
			} 
        	if(maze[a][b]==0) //找到可以前进的非出口的点 
			{ 
				maze[a][b]=2; //标记走过此点 
				elem.x=i; 
				elem.y=j; 
				elem.d=d; 
				Push(S1,elem); //当前位置入栈 
				i=a; //下一点转化为当前点 
	    		j=b; 
	    		d=-1; 
			} 
	    	d++; 
		} 
	} 
//	printf("没有找到可以走出此迷宫的路径\n"); 
} 
