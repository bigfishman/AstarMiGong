#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include <windowsx.h>
#include <stdlib.h>
#include "resource.h"
#include "MainDlg.h"
#pragma comment(lib, "WINMM.LIB")

#include<stdio.h> 
#include<stdlib.h> 
#include<math.h>
int deep=0;//表示深度
int deep_Flag;//表示深度标志
 
struct mark //定义迷宫内点的坐标类型 
{ 
	int x; 
	int y; 
}; 
struct mark start,end; //start,end入口和出口的坐标 

   

	
struct Element //栈元素， 
{ 
		double f;//估价函数
	double g;//代价
	double h;//启发函数
	int x,y; //x行,y列 
	//int d; //d下一步的方向 
}; 

	Element elem,e; 

typedef struct LStack //链栈 
{ 

	Element elem; 
	struct LStack *next; 
}*PLStack; 

	PLStack Open,Close; 
    PLStack S1,S2;
/*************栈函数****************/ 
int InitStack(PLStack &S)
{
	S=NULL;
	return 1;
}


int InitStack1(PLStack &S)//构造空栈 
{ 
	//S=NULL;
	S=(PLStack)malloc(sizeof(LStack));
	S->elem.g=0;
	S->elem.h=sqrt(((end.x-start.x)*(end.x-start.x)+(end.y-start.y)*(end.y-start.y)));
	S->elem.f=S->elem.h;
	S->elem.x=start.x;
	S->elem.y=start.y;

    S->next=NULL;
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
	p=(PLStack)malloc(sizeof(LStack)); 
	p->elem=e; 
	p->next=S; 
	S=p; 
	return 1; 
} 

int Push1(PLStack &S, Element e)//压入新数据元素 
{ 
	//float g0,f0,h0;
	PLStack p; 
	PLStack q,t;

	p=(PLStack)malloc(sizeof(LStack));
	p->elem.g=deep;//表示深度
//	p->elem.h=sqrt(((end.x-e.x)*(end.x-e.x)+(end.y-e.y)*(end.y-e.y))*1.0);
	p->elem.h=abs(end.x-e.x)+abs(end.y-e.y);
	p->elem.f=p->elem.h+p->elem.g;
//	p->elem=e; 
	p->elem.x=e.x;
	p->elem.y=e.y;
	p->next=NULL;

	if(NULL==S||p->elem.f<=S->elem.f)
	{
		p->next=S;
		S=p;
		//return 1;
	}
	else
	{
		//if(p->elem.f<S->elem.f)
	//	{
	////		p->next=S;
	//		S=p;
	//	}
	//	else{
			q=S;
			t=q->next;
			while(t!=NULL)
			{
				//t=q;
			//	q=q->next;
				if(p->elem.f>t->elem.f)
				{
					q=t;
					t=t->next;
				}
				else
					break;
			}
			p->next=t;
			q->next=p;
	//	}
	}
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
		free(p); 
		return 1; 
	} 
	else 
		return 0; 
} 
int maze[18][25];
int maze1[18][25]={
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,1,1,1,1,0,0,1,0,1,0,0,1,1,0,1,1,1,0,1,1,1,0,1},
			{1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,0,1,0,1,0,1,1,1,0,1},
			{1,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0,1},
			{1,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,1,0,1,0,1},
			{1,1,0,1,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,1},
			{1,0,0,1,1,0,0,1,0,1,1,1,0,1,1,1,0,1,0,0,0,0,1,1,1},
			{1,1,0,0,1,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,1,1,0,1},
			{1,0,1,1,1,1,0,1,0,1,1,0,1,0,0,1,0,1,0,1,0,1,1,0,1},
			{1,1,0,1,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,0,0,0,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,1},
			{1,0,0,0,1,1,1,1,0,1,0,0,1,1,0,1,0,1,0,1,0,0,1,0,1},
			{1,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0,1},
			{1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,0,0,0,1,0,1},
			{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
		};
int maze2[18][25]={
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,1,1,1,1,0,0,1,0,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1},
			{1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,0,1,0,1,0,1,1,1,0,1},
			{1,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,0,0,1},
			{1,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,1,0,1,0,1},
			{1,1,0,1,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,1},
			{1,0,0,1,1,0,0,1,0,1,1,1,0,1,1,1,0,1,0,0,0,0,1,1,1},
			{1,1,0,0,1,1,0,1,1,1,0,1,0,0,0,1,0,1,0,1,1,1,1,0,1},
			{1,0,1,1,1,1,0,1,0,1,1,0,1,0,0,0,0,1,0,1,0,1,1,0,1},
			{1,1,0,1,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,0,0,0,1},
			{1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
			{1,0,0,0,1,1,1,1,0,1,0,0,1,1,0,1,0,1,0,1,0,0,1,0,1},
			{1,0,0,1,1,0,0,0,0,1,0,0,1,0,0,1,0,1,0,1,0,0,1,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1},
			{1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,0,0,0,1,0,1},
			{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
		};
int maze3[18][25]={
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,1},
			{1,1,0,1,1,1,0,0,1,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,1},
			{1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,1,0,1},
			{1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1},
			{1,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1,1,0,1,0,1,0,1},
			{1,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,1},
			{1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,1,0,1,0,0,0,0,1,1,1},
			{1,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,1,0,1,1,1,1,0,1},
			{1,0,1,1,1,1,0,1,0,1,1,0,1,0,0,1,0,1,0,0,0,1,1,0,1},
			{1,1,0,1,0,0,0,1,0,0,1,0,1,1,0,1,0,1,1,1,0,0,0,0,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,1,0,0,0,0,1},
			{1,0,0,0,0,1,1,1,0,1,0,0,0,0,0,1,0,1,0,1,0,0,1,0,1},
			{1,0,0,1,0,0,0,1,0,1,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1},
			{1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,0,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
		};
int maze4[18][25]={
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},

};
int add[4][2]={{1,0},{0,1},{-1,0},{0,-1}};//行增量和列增量 方向依次为东西南北 


int Flag=0;//标志  用来判断程序结束没有
int M_Flag=0;//标志，用来判断地图画好没有

int p_Lsize=32;//小图片的长度
int p_Hsize=31;//小图片的宽度



int MazePath(int maze[18][25]) 
{ 

	struct mark start,end; //start,end入口和出口的坐标 


	start.x=1;
	start.y=0;
	end.x=16;
	end.y=24;

	int i,j,d;
	int a,b; 





	InitStack(S1); 
	InitStack(S2); 

	maze[start.x][start.y]=2; //入口点作上标记 
	elem.x=start.x; 
	elem.y=start.y; 
	//elem.d=-1; //开始为-1 

	Push(S1,elem); 
	while(!StackEmpty(S1)) //栈不为空 有路径可走 
	{ 
		Pop(S1,elem); 
		i=elem.x; 
		j=elem.y; 
	//	d=elem.d+1; //下一个方向 
		while(d<4) //试探东南西北各个方向 
		{ 
		a=i+add[d][0]; 
		b=j+add[d][1]; 
		if(a==end.x && b==end.y && maze[a][b]==0) //如果到了出口 
		{ 
			elem.x=i; 
			elem.y=j; 
		//	elem.d=d; 
			Push(S1,elem); 
			elem.x=a; 
			elem.y=b; 
		//	elem.d=8; //方向输出为-1 判断是否到了出口 
			Push(S1,elem); 

			while(S1) //逆置序列 并输出迷宫路径序列 
			{ 
				Pop(S1,e); 
				Push(S2,e); 
			} 

		//	while(S2) 
		//	{ 
		//		Pop(S2,e); 
			//	printf("-->(%d,%d,%d)",e.x,e.y,e.d);

		//		maze[e.x][e.y] = 9;
		//	} 
			//	system("PAUSE"); 
			return 0;  
		} 

		if(maze[a][b]==0) //找到可以前进的非出口的点 
		{ 
			maze[a][b]=2; //标记走过此点 
			elem.x=i; 
			elem.y=j; 
		//	elem.d=d; 
			Push(S1,elem); //当前位置入栈 
			i=a; //下一点转化为当前点 
			j=b; 
			//d=-1; 
		} 
		//	d++; 
	} 
	} 
	printf("\n");
	return 0;
}

int Judge(Element e,PLStack Close)//用于查看新扩展的节点是否已经存在于close表中
{
	PLStack P;
	P=Close;
    while(Close!=NULL)
	{
		if((Close->elem.x==e.x)&&(Close->elem.y==e.y))//说明已经存在该节点
			return 0;
		Close=Close->next;

	}
	Close=P;
	return 1;
}

BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hWnd,WM_CLOSE, Main_OnClose);
	case WM_LBUTTONDOWN:

		
		break;
	default :
		break;
    }

    return FALSE;
}

BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	
    return TRUE;
}

void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
//	PlaySound("SystemStart",NULL,SND_ALIAS|SND_ASYNC);

   	 int i,j;
	// int line = 0;//用来记录行数
//	 int row=0;//用来记录列数
	 //FILE *Fp = NULL;  //用来打开文件
/*	 if((Fp = fopen("E:\\maze2.txt","r")) == NULL) 
	 {
		 MessageBox(NULL,TEXT("地图载入出错,注意是否存有有地图文件！"),TEXT("Error!"),0);
		 exit(0);
	 }
	  
	while (!feof(Fp))
	{
		for(row=0;row<25;row++)
			fscanf(Fp,"%d",&maze[line][row]);
		line++;
	  
	}
/*	int maze[18][25]={
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,1,1,1,1,0,0,1,0,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1},
			{1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,0,1,0,1,0,1,1,1,0,1},
			{1,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0,1},
			{1,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,1,0,1,0,1},
			{1,1,0,1,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,1},
			{1,0,0,1,1,0,0,1,0,1,1,1,0,1,1,1,0,1,0,0,0,0,1,1,1},
			{1,1,0,0,1,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,1,1,0,1},
			{1,0,1,1,1,1,0,1,0,1,1,0,1,0,0,1,0,1,0,1,0,1,1,0,1},
			{1,1,0,1,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,0,0,0,1},
			{1,1,0,0,0,0,0,0,0,1,1,0,0,1,0,1,0,1,0,1,0,0,0,0,1},
			{1,0,0,0,1,1,1,1,0,1,0,0,1,1,0,1,0,1,0,1,0,0,1,0,1},
			{1,0,0,1,1,0,0,0,0,1,0,0,1,0,0,1,0,1,0,1,0,0,1,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0,1},
			{1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,0,0,0,1,0,1},
			{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
		};*/
   
		HWND hmap,h1,h3,h5,h7;
	    HDC hm,h2,h4,h6,h8;
     	hmap = GetDlgItem(hwnd,IDC_MAP);
		hm = GetDC(hmap);
			h1 = GetDlgItem(hwnd,IDC_P1);
			h2 = GetDC(h1);
				h3 = GetDlgItem(hwnd,IDC_P2);
				h4 = GetDC(h3);
					h5 = GetDlgItem(hwnd,IDC_P3);
					h6 = GetDC(h5);
						h7 = GetDlgItem(hwnd,IDC_P4);
						h8 = GetDC(h7);

	if(IsDlgButtonChecked(hwnd,IDC_M1)>0)
	{
		//	 MessageBox(NULL,TEXT("h"),TEXT("hello"),0);
		/*
     	 if((Fp = fopen("E:\\maze0.txt","r")) == NULL) 
		 {
		 MessageBox(NULL,TEXT("地图载入出错,注意是否存有有地图文件！"),TEXT("Error!"),0);
		 exit(0);
		 }
		 */
		for(i=0;i<18;i++)
			for(j=0;j<25;j++)
				maze[i][j]=maze1[i][j];
	  
	}
	else if(IsDlgButtonChecked(hwnd,IDC_M2)>0)
	{
		for(i=0;i<18;i++)
			for(j=0;j<25;j++)
				maze[i][j]=maze2[i][j];
	  
	  

	}
    else if(IsDlgButtonChecked(hwnd,IDC_M3)>0)
	{
		for(i=0;i<18;i++)
			for(j=0;j<25;j++)
				maze[i][j]=maze3[i][j];
	  
	  

	}

	
		
    switch(id)
    {
        case IDC_GO:

			if(1==M_Flag)
			{
				MessageBox(NULL,TEXT("迷宫已经画好了!"),TEXT("迷宫已经画好!"),0);
				break;
			}
			if((0==IsDlgButtonChecked(hwnd,IDC_M1))&&(0==IsDlgButtonChecked(hwnd,IDC_M2))&&(0==IsDlgButtonChecked(hwnd,IDC_M3)))
			{
				MessageBox(NULL,TEXT("请选择迷宫！迷宫不能为空!"),TEXT("小心!"),0);
				break;
			}
			/*
				while (!feof(Fp))
				{
			for(row=0;row<25;row++)
				fscanf(Fp,"%d",&maze[line][row]);
			line++;
				}
				*/

			

				for(j=0;j<25;j++)
				{
                    BitBlt(hm,j*32,0,p_Lsize,p_Hsize,h6,0,0,SRCCOPY);
					Sleep(10);
				}
				for(i=0;i<18;i++)
				{
                    BitBlt(hm,24*32,i*32,p_Lsize,p_Hsize,h6,0,0,SRCCOPY);
					Sleep(10);
				}
				for(j=24;j>=0;j--)
				{
                    BitBlt(hm,j*32,17*32,p_Lsize,p_Hsize,h6,0,0,SRCCOPY);
					Sleep(10);
				}
				for(i=17;i>=0;i--)
				{
                    BitBlt(hm,0,i*32,p_Lsize,p_Hsize,h6,0,0,SRCCOPY);
					Sleep(10);
				}
				for(i=1;i<17;i++){
					for(j=1;j<24;j++){
						if(maze[i][j]==1)
						{
                           BitBlt(hm,j*32,i*32,p_Lsize,p_Hsize,h8,0,0,SRCCOPY);
						   Sleep(5);
						}
					/*	else
						{
							BitBlt(hm,j*32,i*32,p_Lsize,p_Hsize,h4,0,0,SRCCOPY);
							Sleep(50);
						}*/
					}
				}

                BitBlt(hm,0,32,p_Lsize,p_Hsize,h4,0,0,SRCCOPY); //画入口
				Sleep(100);
				BitBlt(hm,24*32,16*32,p_Lsize,p_Hsize,h4,0,0,SRCCOPY);//画出口
				Sleep(100);
			   /*    	MazePath(maze);
                      while(S2) 
					{ 
						Pop(S2,e);
		            
					
					
						BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h2,0,0,SRCCOPY);
					//	PlaySound("E:\\music\\walk.wav", NULL, SND_FILENAME | SND_ASYNC);

					//	SetTimer(hwnd,0,2000,NULL);
						Sleep(250);
						BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h4,0,0,SRCCOPY);
					 }*/
				M_Flag=1;
					  	
						Sleep(100);
						
			        	MessageBox(NULL,TEXT("地图已经画好，可以冒险了!"),TEXT("地图完成"),0);
          break;

		case IDC_GO1:
             
			if(0==M_Flag)
			{
				MessageBox(NULL,TEXT("地图还没有画好，请先画好地图!"),TEXT("请先画好地图!"),0);
				break;
			}
			Flag=0;



	
	//int add[4][2]={{1,0},{0,1},{-1,0},{0,-1}};//行增量和列增量 方向依次为东西南北 

	 start.x=1;
	start.y=0;
	end.x=16;
	end.y=24;

	InitStack(Open); //用于open 表
	InitStack(Close); //用于close表

	//maze[start.x][start.y]=2; //入口点作上标记 
	elem.x=start.x; 
	elem.y=start.y; 
	

	Push(Open,elem); //开始节点进入open表

	while(!StackEmpty(Open)&&Flag==0) //栈不为空 有路径可走 
	{ 
		Pop(Open,elem); //将open表中的当前元素提出来

	             	BitBlt(hm,32+(elem.y-1)*32,32+(elem.x-1)*32,33,33,h2,0,0,SRCCOPY);
					//	SetTimer(hwnd,0,2000,NULL);
						Sleep(100);
					BitBlt(hm,32+(elem.y-1)*32,32+(elem.x-1)*32,33,33,h4,0,0,SRCCOPY);

		Push(Close,elem);//将从open表取出来的元素放入close表
		//扩展该元素的所有子节点
		//这里特别的注意，将该元素的下面和右面当做子节点，所有的元素都这样
    	//判断右子结点和下子节点是不是可以行,可行的话就将该子节点入open表
		
			if(0==maze[elem.x][elem.y+1]&&0==Flag)
			{
				
				e.x=elem.x;
				e.y=elem.y+1;
			

				if(Judge(e,Close))
					Push(Open,e);
				if(e.x==end.x&&e.y==end.y)//判断该点是不是终点
				{
					Flag=1;//表示程序已经找到目标了
						Pop(Open,e);
					Push(Close,e);

					BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h2,0,0,SRCCOPY);
					//	SetTimer(hwnd,0,2000,NULL);
			 		Sleep(50);
					BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h4,0,0,SRCCOPY);

				
					Sleep(100);
							//	Sleep(1000);
					
    				MessageBox(NULL,TEXT("恭喜"),TEXT("恭喜小兔子"),0);
				}
			}
			if(0==maze[elem.x+1][elem.y]&&0==Flag)
			{
				e.x=elem.x+1;
				e.y=elem.y;
				if(Judge(e,Close))
					Push(Open,e);
				if(e.x==end.x&&e.y==end.y)
				{
					Flag=1;//表示程序已经找到目标了
					Pop(Open,e);
					Push(Close,e);
						BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h2,0,0,SRCCOPY);
					//	SetTimer(hwnd,0,2000,NULL);
			 		Sleep(50);
					BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h4,0,0,SRCCOPY);

		
					Sleep(100);
							//	Sleep(1000);
				
    				MessageBox(NULL,TEXT("恭喜"),TEXT("恭喜小兔子"),0);
				}
			}
		  
            if(0==maze[elem.x-1][elem.y]&&0==Flag)
			{
				e.x=elem.x-1;
				e.y=elem.y;
				if(Judge(e,Close))
					Push(Open,e);
				if(e.x==end.x&&e.y==end.y)
				{
					Flag=1;//表示程;序已经找到目标了
					Pop(Open,e);
					Push(Close,e);
						BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h2,0,0,SRCCOPY);
					//	SetTimer(hwnd,0,2000,NULL);
			 		Sleep(50);
					BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h4,0,0,SRCCOPY);
				
					Sleep(100);
							//	Sleep(1000);
				
    				MessageBox(NULL,TEXT("恭喜"),TEXT("恭喜小兔子"),0);
				}
			}
			if(0==maze[elem.x][elem.y-1]&&0==Flag)
			{
				e.x=elem.x;
				e.y=elem.y-1;
				if(Judge(e,Close))
					Push(Open,e);
				if(e.x==end.x&&e.y==end.y)
				{
					Flag=1;//表示程序已经找到目标了
						Pop(Open,e);
					Push(Close,e);
						BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h2,0,0,SRCCOPY);
					//	SetTimer(hwnd,0,2000,NULL);
			 		Sleep(50);
					BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h4,0,0,SRCCOPY);
					
					Sleep(100);
							//	Sleep(1000);
					
    				MessageBox(NULL,TEXT("恭喜"),TEXT("恭喜小兔子"),0);
				}
			}
	}

	if(0==Flag)
	{
    	
		Sleep(1000);
				//	Sleep(1000);
    	MessageBox(NULL,TEXT("迷宫走不出去了!"),TEXT("迷宫走不出去了!"),0);
	}

    break;
    case IDC_GO2:
		if(0==M_Flag)
			{
				MessageBox(NULL,TEXT("地图还没有画好，请先画好地图!"),TEXT("请先画好地图!"),0);
				break;
			}
			Flag=0;



//	struct mark start,end; //start,end入口和出口的坐标 
	//int add[4][2]={{1,0},{0,1},{-1,0},{0,-1}};//行增量和列增量 方向依次为东西南北 

 start.x=1;
	start.y=0;
	end.x=16;
	end.y=24;

	InitStack(Open); //用于open 表
	InitStack(Close); //用于close表

	//maze[start.x][start.y]=2; //入口点作上标记 
	elem.x=start.x; 
	elem.y=start.y; 
	
	deep_Flag=0;
	Push1(Open,elem); //开始节点进入open表
	deep++;
	while(!StackEmpty(Open)&&Flag==0) //栈不为空 有路径可走 
	{ 
		Pop(Open,elem); //将open表中的当前元素提出来

	             	BitBlt(hm,32+(elem.y-1)*32,32+(elem.x-1)*32,33,33,h2,0,0,SRCCOPY);
					//	SetTimer(hwnd,0,2000,NULL);
						Sleep(100);
					BitBlt(hm,32+(elem.y-1)*32,32+(elem.x-1)*32,33,33,h4,0,0,SRCCOPY);

		Push(Close,elem);//将从open表取出来的元素放入close表
		//扩展该元素的所有子节点
		//这里特别的注意，将该元素的下面和右面当做子节点，所有的元素都这样
    	//判断右子结点和下子节点是不是可以行,可行的话就将该子节点入open表
			//Sleep(5000);
			if(0==maze[elem.x][elem.y+1]&&0==Flag)
			{
				
				deep_Flag++;
				e.x=elem.x;
				e.y=elem.y+1;
			

				if(Judge(e,Close)){
					Push1(Open,e);
				}
				else
					deep_Flag--;
				if(e.x==end.x&&e.y==end.y)//判断该点是不是终点
				{
					Flag=1;//表示程序已经找到目标了
					//	Pop(Open,e);
					Push(Close,e);

					BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h2,0,0,SRCCOPY);
					//	SetTimer(hwnd,0,2000,NULL);
			 		Sleep(50);
					BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h4,0,0,SRCCOPY);

				
					Sleep(100);
							//	Sleep(1000);
					
    				MessageBox(NULL,TEXT("恭喜"),TEXT("恭喜小兔子"),0);
				}
			}
			if(0==maze[elem.x+1][elem.y]&&0==Flag)
			{
				deep_Flag++;
				e.x=elem.x+1;
				e.y=elem.y;
				if(Judge(e,Close)){
					Push1(Open,e);
				}
				else
					deep_Flag--;
				if(e.x==end.x&&e.y==end.y)
				{
					Flag=1;//表示程序已经找到目标了
					//Pop(Open,e);
					Push(Close,e);
						BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h2,0,0,SRCCOPY);
					//	SetTimer(hwnd,0,2000,NULL);
			 		Sleep(50);
					BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h4,0,0,SRCCOPY);

		
					Sleep(100);
							//	Sleep(1000);
				
    				MessageBox(NULL,TEXT("恭喜"),TEXT("恭喜小兔子"),0);
				}
			}
		  
            if(0==maze[elem.x-1][elem.y]&&0==Flag)
			{
					deep_Flag++;
				e.x=elem.x-1;
				e.y=elem.y;
				if(Judge(e,Close)){
					Push1(Open,e);
				}
				else
					deep_Flag--;
				if(e.x==end.x&&e.y==end.y)
				{
					Flag=1;//表示程;序已经找到目标了
				//	Pop(Open,e);
					Push(Close,e);
						BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h2,0,0,SRCCOPY);
					//	SetTimer(hwnd,0,2000,NULL);
			 		Sleep(50);
					BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h4,0,0,SRCCOPY);
				
					Sleep(100);
							//	Sleep(1000);
				
    				MessageBox(NULL,TEXT("恭喜"),TEXT("恭喜小兔子"),0);
				}
			}
			if(0==maze[elem.x][elem.y-1]&&0==Flag)
			{
					deep_Flag++;
				e.x=elem.x;
				e.y=elem.y-1;
				if(Judge(e,Close)){
					Push1(Open,e);
				}
				else
					deep_Flag--;
				if(e.x==end.x&&e.y==end.y)
				{
					Flag=1;//表示程序已经找到目标了
					//	Pop(Open,e);
					Push(Close,e);
						BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h2,0,0,SRCCOPY);
					//	SetTimer(hwnd,0,2000,NULL);
			 		Sleep(50);
					BitBlt(hm,32+(e.y-1)*32,32+(e.x-1)*32,33,33,h4,0,0,SRCCOPY);
					
					Sleep(100);
							//	Sleep(1000);
					
    				MessageBox(NULL,TEXT("恭喜"),TEXT("恭喜小兔子"),0);
				}
			}
			if(0==deep_Flag)
				deep--;
			if(1==deep_Flag)
				deep++;
			deep_Flag=0;
	}

	if(0==Flag)
	{
    	
		Sleep(1000);
				//	Sleep(1000);
    	MessageBox(NULL,TEXT("迷宫走不出去了!"),TEXT("迷宫走不出去了!"),0);
	}

    break;

	case IDC_GO3:
		M_Flag=0;
		Flag=0;
		for(i=0;i<18;i++)
			for(j=0;j<25;j++)
				BitBlt(hm,j*32,i*32,p_Lsize,p_Hsize,h4,0,0,SRCCOPY);
		break;
//	case C2:
	//	MessageBox(NULL,TEXT("Hello"),TEXT("ceshi"),0);
	//	break;
        default:
	    	break;
    }
}

void Main_OnClose(HWND hwnd)
{
    EndDialog(hwnd, 0);
}