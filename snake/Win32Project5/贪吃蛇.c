#include <windows.h>
#include "resource.h"
#include<time.h>
LRESULT CALLBACK myWNDPROC(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam);
typedef  struct  jj
{
	int x;
	int y;
	struct jj*pnext;
	struct jj*plast;
}snake,apple;
enum  direction {up,down,lift,right};
snake*phead=NULL;
snake*pmark=NULL;
snake*pend=NULL;
enum direction fx=up;
apple  ap={5,5,NULL,NULL};
void  showbackground(HDC hdc);
void  addnode(int x,int y);
void showsnake(HDC hdc);
void move();
void showapple(HDC hdc);
BOOL ifapple();
void newapple();
BOOL wall();
BOOL  eatself();
BOOL bflag=TRUE;
HBITMAP hBitmap_Background;
HBITMAP hBitmap_apple;
HBITMAP hBitmap_snakebody;
HBITMAP hBitmap_snakehead_up;
HBITMAP hBitmap_snakehead_down;
HBITMAP hBitmap_snakehead_lift;
HBITMAP hBitmap_snakehead_right;



int CALLBACK WinMain( 
 HINSTANCE hInstance,
 HINSTANCE hPrevInstance,
 LPSTR lpCmdLine,
 int nCmdShow)
{
	snake*p=NULL;
	WNDCLASSEX ex;
	HWND hwnd;
	MSG msg;
	srand((unsigned int )time(NULL));
hBitmap_Background=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP1 ));
hBitmap_apple=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP2 ));
hBitmap_snakebody=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP3 ));
hBitmap_snakehead_up=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP5 ));
hBitmap_snakehead_down=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP7 ));
hBitmap_snakehead_lift=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP6 ));
hBitmap_snakehead_right=LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP4 ));
	//1.设计窗口
	addnode(3,3);
	addnode(3,4);
	addnode(3,5);
	addnode(3,6);
	ex.style=CS_HREDRAW|CS_VREDRAW;
	ex.cbSize=sizeof(ex);
	ex.cbClsExtra=0;
	ex.cbWndExtra=0;
	ex.hInstance=hInstance;
	ex.hIcon=NULL;
	ex.hCursor=NULL;
	ex.hbrBackground=CreateSolidBrush(RGB(35,130,109));
	ex.hIconSm=NULL;
	ex.lpfnWndProc=&myWNDPROC;
	ex.lpszMenuName=NULL;
	ex.lpszClassName="aaa";

	//2.注册
	 RegisterClassEx(&ex);

	//3.创建
	hwnd=CreateWindow(ex.lpszClassName,"贪吃蛇",WS_OVERLAPPEDWINDOW,50,50,615,638,NULL,NULL,hInstance,NULL) ;  

	//4.显示
	ShowWindow(hwnd, SW_SHOW);
	SetTimer(hwnd,1,300,NULL);
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
LRESULT CALLBACK myWNDPROC(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)
{
 HDC hdc;
 PAINTSTRUCT ps;
	switch(Message)
	{
	case WM_CLOSE:
		
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:  //按键消息
		if(bflag==TRUE)
			{
		switch(wparam)
		{
			
		case VK_UP:
			if(fx!=down)
			fx=up;
		 
			break;
		case VK_DOWN:
			if(fx!=up)
			fx=down;
			 
			break;
		case VK_LEFT:
			if(fx!=right)
			fx=lift;
			break;
		case VK_RIGHT:
			if(fx!=lift)
			fx=right;
				break;
				
		}
		bflag=FALSE;
		}
		hdc=GetDC(hwnd);
		showbackground(hdc);
		showsnake(hdc);
		showapple(hdc);
		ReleaseDC(hwnd,hdc);
	
		break;
	case WM_PAINT://重绘消息
	hdc=BeginPaint(hwnd,&ps);
	showbackground(hdc);
	showsnake(hdc);
	showapple(hdc);
	EndPaint(hwnd,&ps);
	break;
	case WM_TIMER://计时器
		hdc=GetDC(hwnd);
		move();
		if(wall()||eatself())
		{
			KillTimer(hwnd,1);
MessageBox(hwnd,"游戏结束","提示",MB_OK);

		}
		// 判断是否吃到苹果
		if(ifapple())
		{
			addnode(-10,-10);
			newapple();
		}

		//
		showbackground(hdc);
		showsnake(hdc);
		showapple(hdc);
		ReleaseDC(hwnd,hdc);
			bflag=TRUE;
		break;
	}
return DefWindowProc(hwnd,Message,wparam,lparam);
}
void  showbackground(HDC hdc)
{
	 HDC  hdcmen;
	//创建兼容性dc
	 hdcmen=CreateCompatibleDC(hdc);
	 //给创建的dc选择一张位图
	 SelectObject(hdcmen,hBitmap_Background);
	 //传输
	 BitBlt(hdc,0,0,600,600,hdcmen,0,0,SRCCOPY);
	 //删除兼容性dc
	DeleteDC(hdcmen);
	
}
void  addnode(int x,int y)
{
snake*ptemp=(snake*)malloc(sizeof(snake));
ptemp->x=x;
ptemp->y=y;
ptemp->plast=NULL;
ptemp->pnext=NULL;
if(NULL==phead)
{
phead=ptemp;
}
else
{
	pend->pnext=ptemp;
	ptemp->plast=pend;
}
pend=ptemp;
}
void showsnake(HDC hdc)
	
{
	 HDC  hdcmen=CreateCompatibleDC(hdc);
	//显示蛇头
	switch (fx)
	{
	case up:
		   SelectObject(hdcmen,hBitmap_snakehead_up);
		  
		break;
	case down:
		 SelectObject(hdcmen,hBitmap_snakehead_down);
		break;
	case lift:
		 SelectObject(hdcmen,hBitmap_snakehead_lift);
		break;
	case right:
		 SelectObject(hdcmen,hBitmap_snakehead_right);
		break;
	default:
		break;
	}
	  BitBlt(hdc,phead->x*30,phead->y*30,30,30,hdcmen,0,0,SRCCOPY);
			

	// 显示蛇身
	  pmark=phead->pnext;
			 SelectObject(hdcmen,hBitmap_snakebody);
			while(pmark!=NULL)
			{
				 BitBlt(hdc,pmark->x*30,pmark->y*30,30,30,hdcmen,0,0,SRCCOPY);
				pmark=pmark->pnext;
			}
			DeleteDC(hdcmen);
}
void move()
{
	snake*pmark=pend;
	//移动蛇身
	while(pmark!=phead)
	{
		pmark->x=pmark->plast->x;
		pmark->y=pmark->plast->y;
pmark=pmark->plast;
	}
	//移动蛇头
switch (fx)
	{
	case up:
		   phead->y--;
		break;
	case down:
		phead->y++;
		break;
	case lift:
		
		phead->x--;
		break;
	case right:
		phead->x++;
		break;
	default:
		break;
	}
}
void showapple(HDC hdc)
{
	 HDC  hdcmen=CreateCompatibleDC(hdc);
	 SelectObject(hdcmen,hBitmap_apple);
	  BitBlt(hdc,ap.x*30,ap.y*30,30,30,hdcmen,0,0,SRCCOPY);
	  DeleteDC(hdcmen);

	}
BOOL ifapple()
{
if(phead->x==ap.x&&phead->y==ap.y)
{
	return TRUE;
}
return FALSE;
}
void newapple()
{
	int x;
	int y;
	int flag=1;
	snake*pmark=phead;
//随机生成坐标  1-18  xy
	while(1)
	{
		flag=2;
		x=rand()%18+1;
		y=rand()%18+1;

	//2苹果不能在蛇身上
		pmark=phead;
		while(pmark!=NULL)
		{
			if(x==pmark->x&&y==pmark->y)
			{flag=0;
				break;
			}
			pmark=pmark->pnext;
		}
		if(flag!=0)
		{
		ap.x=x;
		ap.y=y;
		break;
		}

	//遍历蛇身   去比较新生成的坐标
	}

	}
BOOL wall()
{
if(phead->x==0||phead->x==19||phead->y==0||phead->y==19)
{
	return TRUE;
}
return FALSE;
}
BOOL  eatself()
{
snake*pmark=phead->pnext;
while(pmark!=NULL)
{
	if(phead->x==pmark->x&&phead->y==pmark->y)
	{
return TRUE;
	}

	pmark=pmark->pnext;
}
return  FALSE;
}