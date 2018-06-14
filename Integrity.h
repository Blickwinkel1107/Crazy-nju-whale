#pragma once

#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<graphics.h>
#include<math.h>
#include<time.h>

#pragma comment(lib, "Winmm.lib")	// 给游戏添加音乐要用到它

#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4
#define SPACE 5
#define Quantity 16
#define Q_Rect 12
#define Q_Bonus 4
#define ESC 27
#define SRC_W 1000
#define SRC_H 480
#define Interval_godmode 5000
#define Interval_talk 2000
#define Interval_notalk 2000
#define TALKNUM 22


//主角
typedef struct Whale
{
	IMAGE img;
	IMAGE imgx;
	int x; //矩形左上角x轴坐标
	int y; //矩形左上角y轴坐标
	int w; //矩形水平边像素长度
	int h; //矩形竖直边像素长度
	double v_x;
	double v_y;
	double a_x;
	double a_y;
	int st_x; //起始坐标
	int st_y;
	bool move;
	double t0;
}Whale;
//方形
typedef struct Rect
{
	IMAGE img;
	IMAGE imgx;
	int x; //矩形左上角x轴坐标
	int y; //矩形左上角y轴坐标
	int w; //矩形水平边像素长度
	int h; //矩形竖直边像素长度
	double v_x;
	double v_y;
	int st_x; //起始坐标
	int st_y;
	bool move;
	double t0;
}Rect;
//奖励
typedef struct Bonus
{
	IMAGE img;
	IMAGE imgx;
	int x; //矩形左上角x轴坐标
	int y; //矩形左上角y轴坐标
	int w; //矩形水平边像素长度
	int h; //矩形竖直边像素长度
	double v_x;
	double v_y;
	int st_x; //起始坐标
	int st_y;
	bool move;
	bool collide;
	double t0;
}Bonus;
//物体信息
typedef struct Data
{
	int x; //矩形左上角x轴坐标
	int y; //矩形左上角y轴坐标
	int w; //矩形水平边像素长度
	int h; //矩形竖直边像素长度
	double v_x;
	double v_y;
	int st_x; //起始坐标
	int st_y;
}Data;



//游戏变量
extern int Difficulty;
extern int dv;
extern int Interval;
extern double V_X;
extern int talknum;
extern bool isgodmode;
extern bool talking;
extern int bonus_score;
extern char str_talk[TALKNUM][128];
extern int life;
extern int score_max;
extern int score;

//系统变量
extern bool buttondown;
extern bool buttonup;
extern bool gameover;
extern bool exitgame;
extern IMAGE wImg[Quantity],wImgx[Quantity];
extern IMAGE bkimg;
extern int cmd;
extern bool spawn_big;
extern bool moving;
extern int cmd_exit;

//物体数组
extern Whale whale;
extern Rect rect[12];
extern Bonus bonus[4];

//时间变量
extern double dt;
extern int t0,t1;
extern int tmpt;
extern int hit;
extern int start1;
extern int godtime;

//文本信息
extern char str_score[128];
extern char str_interval[128];
extern char str_bonus[128];
extern char bgm[4][20];

//空中跳跃用到的变量
extern int times_jump,y0_jump;
extern double t0_jump,t1_jump,dt_jump;
extern bool jumping;
extern bool buttonspace;
extern int cmd_jump;

//加载图片
void loadimgs()
{
	int i;
	loadimage(&bkimg,"res\\bkimgL.jpg");
	char name[Quantity][20]={"whale","comb","island1","island2","land","rock","shrimp",
		"squid","star1","star2","weed1","weed2","fish1","fish2","fish3","fish4"};
	for(i=0;i<Quantity;i++){
		char tmp[128];
		sprintf_s(tmp,"res\\%s.bmp",name[i]);
		loadimage(&wImg[i],tmp);
		sprintf_s(tmp,"res\\%sx.bmp",name[i]);
		loadimage(&wImgx[i],tmp);
	}
}
//加载音效
void loadsfx()
{
	mciSendString("open res\\easy.wma alias easy",NULL,0,NULL);
	mciSendString("open res\\inwater.wma alias inwater",NULL,0,NULL);
	mciSendString("open res\\outwater.wma alias outwater",NULL,0,NULL);
	mciSendString("open res\\medium.wma alias medium",NULL,0,NULL);
	mciSendString("open res\\hard.wma alias hard",NULL,0,NULL);
	mciSendString("open res\\insane.wma alias insane",NULL,0,NULL);
	mciSendString("open res\\gameover.wma alias gameover",NULL,0,NULL);
	mciSendString("open res\\cantmove.wma alias cantmove",NULL,0,NULL);
	mciSendString("open res\\move.wma alias move",NULL,0,NULL);
	mciSendString("open res\\100.wma alias 100",NULL,0,NULL);
	mciSendString("open res\\god.wma alias god",NULL,0,NULL);
	mciSendString("open res\\youareweak.wma alias youareweak",NULL,0,NULL);
	mciSendString("open res\\collide.wma alias collide",NULL,0,NULL);
	mciSendString("open res\\jump.wma alias jump",NULL,0,NULL);
	mciSendString("open res\\turn.wma alias turn",NULL,0,NULL);
}
void playbgm()
{
	char str[128];
	sprintf_s(str,"play %s from 0 repeat",bgm[Difficulty-1]);
	mciSendString(str,NULL,0,NULL);
}
void stopbgm()
{
	char str[128];
	sprintf_s(str,"stop %s",bgm[Difficulty-1]);
	mciSendString(str,NULL,0,NULL);
}
//字体设置
void settxt()
{
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight=30;                      // 设置字体高度为 30
	_tcscpy_s(f.lfFaceName,_T("微软雅黑"));    // 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	f.lfQuality=ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f);
}
//打印文本
void outtxt(bool showcoordinate=false)
{
	score=((t1-t0)/1000+bonus_score)*Difficulty*Difficulty;
	sprintf_s(str_score,"得分:%d 生命:%d 历史最高分:%d",score,life,score_max);
	outtextxy(0,0,str_score); //绘制位于左上角的分数
	if(showcoordinate){
		sprintf_s(str_interval,"系统间隔:%d HIT:%d 无敌间隔:%d",t1-start1,hit,t1-godtime);
		outtextxy(SRC_W-textwidth(str_interval),0,str_interval);
		int h=textheight(str_score);
		char str[Quantity][128]={'\0'};
		char rectname[Q_Rect][20]={"whale","comb","island1","island2","land","rock","shrimp",
			"squid","star1","star2","weed1","weed2"};
		char bonusname[Q_Bonus][20]={"fish1","fish2","fish3","fish4"};
		int i;
		for(i=0;i<Quantity;i++){
			if(i==0) sprintf_s(str[i],"%s x:%d y:%d",rectname[i],whale.x,whale.y);
			else if(i<Q_Rect) sprintf_s(str[i],"%s x:%d y:%d",rectname[i],rect[i].x,rect[i].y);
			else sprintf_s(str[i],"%s x:%d y:%d",bonusname[i-Q_Rect],bonus[i-Q_Rect].x,bonus[i-Q_Rect].y);
		}
		int h1=h;
		for(i=0;i<Quantity;i++){
			outtextxy(0,h1,str[i]);
			h1+=h;
		}
	}
}
//绘制物体
void put(Whale obj)
{
	int x=obj.x,y=obj.y;
	putimage(x,y,&obj.imgx,SRCAND); //绘制补码图
	putimage(x,y,&obj.img,SRCPAINT); //绘制原图
	if(talking){
		LOGFONT f;
		gettextstyle(&f);                     // 获取当前字体设置
		f.lfHeight=20;                      // 设置字体高度为 30
		_tcscpy_s(f.lfFaceName,_T("微软雅黑"));    // 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
		f.lfQuality=PROOF_QUALITY;    // 设置输出效果为抗锯齿  
		settextstyle(&f);
		outtextxy(obj.x,obj.y+whale.h,str_talk[talknum%TALKNUM]);
		settxt();
	}
	if(isgodmode){
		int length=100*(Interval_godmode-(t1-godtime))/Interval_godmode;
		LOGFONT f;
		gettextstyle(&f);                     // 获取当前字体设置
		f.lfHeight=20;                      // 设置字体高度为 30
		_tcscpy_s(f.lfFaceName,_T("微软雅黑"));    // 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
		f.lfQuality=PROOF_QUALITY;    // 设置输出效果为抗锯齿  
		settextstyle(&f);
		outtextxy(obj.x,obj.y-40,"无敌模式");
		setlinecolor(RED);
		line(obj.x,obj.y-15,obj.x+length,obj.y-15);
		settxt();
	}
}
void put(Rect obj)
{
	int x=obj.x,y=obj.y;
	putimage(x,y,&obj.imgx,SRCAND); //绘制补码图
	putimage(x,y,&obj.img,SRCPAINT); //绘制原图
}
void put(Bonus obj)
{
	int x=obj.x,y=obj.y;
	putimage(x,y,&obj.imgx,SRCAND); //绘制补码图
	putimage(x,y,&obj.img,SRCPAINT); //绘制原图
}
//碰撞检测
bool collision_judgment(Whale whale,Rect *rect)
{
	bool case1,case2;
	case1=(whale.x+whale.w)>rect->x && (whale.x-rect->w)<rect->x;
	case2=(whale.y+whale.h)>rect->y && (whale.y-rect->h)<rect->y;
	if(case1 && case2) return true;
	else return false;
}
bool collision_judgment(Whale whale,Bonus *rect)
{
	bool case1,case2;
	case1=(whale.x+whale.w)>rect->x && (whale.x-rect->w)<rect->x;
	case2=(whale.y+whale.h)>rect->y && (whale.y-rect->h)<rect->y;
	if(case1 && case2) return true;
	else return false;
}
//以bkimg刷新背景
void flush()
{
	putimage(0,0,&bkimg);
} //用bkimg刷新画面
//停止物体
void stopall()
{
	int i;
	for(i=0;i<Quantity;i++){
		if(i<Q_Rect) rect[i].move=false;
		else bonus[i-Q_Rect].move=false;
	}
}
//移动物体
void move(Rect *obj)
{
	if(obj->move==false){
		return ;
	}
	else{
		double t1=clock();
		double dt=(t1-obj->t0)/1000;
		int st_x=obj->st_x,st_y=obj->st_y;
		obj->x=(int)(st_x+obj->v_x*dt);
		//obj->y=(int)(st_y+obj->v_y*dt);
		if((obj->x)<-500){
			obj->move=false;
			obj->x=obj->st_x;
			obj->y=obj->st_y;
		}
		put(*obj);
		if(isgodmode==false && collision_judgment(whale,obj)){
			mciSendString("play collide from 0",NULL,0,NULL);
			gameover=true;
		}
	}
}
void move(Bonus *obj)
{
	if(obj->move==false){
		return ;
	}
	else if(obj->collide){
		outtextxy(obj->x,obj->y,str_bonus);
		obj->y-=1;
		if(fabs(obj->y-obj->st_y)>200){
			obj->move=false;
			obj->collide=false;
		}
		return ;
	}
	else{
		double t1=clock();
		double dt=(t1-obj->t0)/1000;
		int st_x=obj->st_x,st_y=obj->st_y;
		obj->x=(int)(st_x+obj->v_x*dt);
		obj->y=(int)(st_y+obj->v_y*dt);
		if((obj->x)<-500){
			obj->move=false;
			obj->x=obj->st_x;
			obj->y=obj->st_y;
		}
		put(*obj);
		if(collision_judgment(whale,obj)){
			int randnum=rand()%100;
			if(randnum<60){
				bonus_score+=100;
				obj->collide=true;
				sprintf_s(str_bonus,"+%d00分",Difficulty*Difficulty);
				mciSendString("play 100 from 0",NULL,0,NULL);
			}
			else{
				isgodmode=true;
				obj->collide=true;
				strcpy_s(str_bonus,"无敌模式");
				mciSendString("play god from 0",NULL,0,NULL);
			}
		}
	}
}
void moveall()
{
	int i;
	for(i=0;i<Quantity;i++){
		if(i<Q_Rect) move(&rect[i]);
		else move(&bonus[i-Q_Rect]);
	}
}
void startmove(Rect *obj)
{
	obj->move=true;
	obj->t0=clock();
}
void startmove(Bonus *obj)
{
	obj->move=true;
	obj->t0=clock();
}
//摸奖
void dice(bool spawnrect,bool spawnbonus){
	if(spawnrect){
		if(spawn_big==false){
			int randnum=rand()%(Q_Rect-1)+1;
			if(rect[randnum].move==false){
				if(randnum==1 || randnum==4) spawn_big=true;
				startmove(&rect[randnum]);
			}
			else{
				dice(true,false);
			}
		}
		else{
			int randnum;
			do{
				randnum=rand()%(Q_Rect-1)+1;
			} while(randnum==10);
			if(rect[randnum].move==false){
				if(randnum==1 || randnum==4) spawn_big=true;
				else spawn_big=false;
				startmove(&rect[randnum]);
			}
			else{
				dice(true,false);
			}
		}
	}
	else{
		int randnum=rand()%Q_Bonus;
		if(bonus[randnum].move==false){
			startmove(&bonus[randnum]);
		}
		else{
			dice(true,false);
		}
	}
}
//获取操作
void getcmd()
{
	cmd=0;
	//if(GetAsyncKeyState(65) & 0x8000) cmd=LEFT;
	//if(GetAsyncKeyState(68) & 0x8000) cmd=RIGHT;
	if(GetAsyncKeyState(87)&0x8000 || GetAsyncKeyState(VK_UP)&0x8000) buttonup=true;
	if(GetAsyncKeyState(83)&0x8000 || GetAsyncKeyState(VK_DOWN)&0x8000) buttondown=true;
	if(buttonup){
		if(!(GetAsyncKeyState(87)&0x8000) && !(GetAsyncKeyState(VK_UP)&0x8000)){
			buttonup=false;
			cmd=UP;
		}
	}
	if(buttondown){
		if(!(GetAsyncKeyState(83)&0x8000) && !(GetAsyncKeyState(VK_DOWN)&0x8000)){
			buttondown=false;
			cmd=DOWN;
		}
	}
	/*if(GetAsyncKeyState(VK_SPACE) & 0x8000){
	outtextxy(100,100,"游戏暂停");
	system("pause");
	}*/
	if(GetAsyncKeyState(27)&0x8000) cmd=ESC;
}
void getcmd_jump()
{
	cmd_jump=0;
	if(GetAsyncKeyState(VK_SPACE)&0x8000) buttonspace=true;
	if(!(GetAsyncKeyState(VK_SPACE)&0x8000) && buttonspace)
	{
		cmd_jump=SPACE;
		buttonspace=false;
	}
}
void jump()
{
	if(whale.y>175)
	{
		mciSendString("play inwater from 0",NULL,0,NULL);
		jumping=false;
		moving=false;
		times_jump=0;
	}
	t1_jump=clock();
	dt_jump=t1_jump-t0_jump;
	if(jumping == true) whale.y=(int)(y0_jump+whale.v_y*dt_jump+0.5*whale.a_y*dt_jump*dt_jump);
	else whale.y = 175;
}
void jump_main()
{
	if(jumping==true && times_jump>=2)
	{
		jump();
	}
	else if(jumping==true && times_jump<2)
	{
		if(cmd_jump==SPACE)
		{
			mciSendString("play jump from 0",NULL,0,NULL);
			t0_jump=clock();
			y0_jump=whale.y;
			jump();
			times_jump++;
		}
		else jump();
	}
	else if(cmd_jump==SPACE)
	{
		t0_jump=clock();
		y0_jump=175;
		jump();
		times_jump++;
		jumping=true;
		moving=true;
		mciSendString("play jump from 0",NULL,0,NULL);
		mciSendString("play outwater from 0",NULL,0,NULL);
	}
}
void getcmd_exit()
{
	cmd_exit=0;
	if(GetAsyncKeyState(87) & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000) buttonup=true;
	if(GetAsyncKeyState(83) & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000) buttondown=true;
	if(buttonup) {
		if(!(GetAsyncKeyState(87) & 0x8000) && !(GetAsyncKeyState(VK_UP)&0x8000)) {
			buttonup=false;
			cmd_exit=UP;
		}
	}
	if(buttondown) {
		if(!(GetAsyncKeyState(83) & 0x8000) && !(GetAsyncKeyState(VK_DOWN) & 0x8000)) {
			buttondown=false;
			cmd_exit=DOWN;
		}
	}
	if(GetAsyncKeyState(27) & 0x8000) cmd_exit=ESC;
}
void print_exit(int t)
{
	char str1[128];
	sprintf_s(str1,"按ESC键返回主界面,W或↑继续游戏,选择剩余时间:%ds",t);
	outtextxy(SRC_W/2-textwidth(str1)/2,SRC_H/2-textheight(str1)/2,str1);
}
//主界面
void change(int a,int b,IMAGE flower,IMAGE maskbitmap)
{
	putimage(a,b,&maskbitmap,SRCAND);
	putimage(a,b,&flower,SRCPAINT);
}
int inter_face(void)
{
	initgraph(640,480);
	MOUSEMSG m;
	IMAGE main1,main2,main3,main4,main5,main11,main11y,main12,main12y,main13,main13y;
	IMAGE main14,main21,main22,main23,main24,main31,main51,main52,main53,main54,main55;
	IMAGE main14y,main21y,main22y,main23y,main24y,main31y,main51y,main52y,main53y,main54y,main55y;
	loadimage(&main1,"res\\main1.bmp");//天空主背景 
	loadimage(&main2,"res\\main2.bmp");// 菜单背景 
	loadimage(&main3,"res\\main3.bmp");//人员介绍背景 
	loadimage(&main4,"res\\main4.bmp");//规则介绍背景 
	loadimage(&main5,"res\\main5.bmp");//难度选择背景 
	loadimage(&main11,"res\\main11.bmp");//游戏名称图像 
	loadimage(&main11y,"res\\main11y.bmp");//游戏名称掩码图
	loadimage(&main12,"res\\main12.bmp");//小蓝鲸图像 
	loadimage(&main12y,"res\\main12y.bmp");//小蓝鲸图像掩码图
	loadimage(&main13,"res\\main13.bmp");//气泡图像 
	loadimage(&main13y,"res\\main13y.bmp");//气泡图像掩码图
	loadimage(&main14,"res\\main14.bmp");//欢迎字体 
	loadimage(&main14y,"res\\main14y.bmp");//欢迎字体掩码图
	loadimage(&main21,"res\\main21.bmp");//菜单栏游戏介绍 
	loadimage(&main21y,"res\\main21y.bmp");//菜单栏游戏介绍掩码图
	loadimage(&main22,"res\\main22.bmp");//菜单栏团队介绍 
	loadimage(&main22y,"res\\main22y.bmp");//菜单栏团队介绍掩码图
	loadimage(&main23,"res\\main23.bmp");//菜单栏开始游戏 
	loadimage(&main23y,"res\\main23y.bmp");//菜单栏开始游戏掩码图
	loadimage(&main24,"res\\main24.bmp");//菜单栏退出游戏 
	loadimage(&main24y,"res\\main24y.bmp");//菜单栏退出游戏掩码图
	loadimage(&main31,"res\\main31.bmp");//back按钮 
	loadimage(&main31y,"res\\main31y.bmp");//back按钮掩码图
	loadimage(&main51,"res\\main51.bmp");//难度选择：脑残 
	loadimage(&main51y,"res\\main51y.bmp");//难度选择：脑残掩码图
	loadimage(&main52,"res\\main52.bmp");//难度选择：正常 
	loadimage(&main52y,"res\\main52y.bmp");//难度选择：正常掩码图
	loadimage(&main53,"res\\main53.bmp");//难度选择：不正常 
	loadimage(&main53y,"res\\main53y.bmp");//难度选择：不正常掩码图
	loadimage(&main54,"res\\main54.bmp");//难度选择：肯定过不了 
	loadimage(&main54y,"res\\main54y.bmp");//难度选择：肯定过不了掩码图
									  //游戏开始欢迎动画 ,背景音乐 
	mciSendString("open res\\intro.wma alias interfacemusic",NULL,0,NULL);
	mciSendString("open res\\turn.wma alias turn",NULL,0,NULL);
	mciSendString("setaudio turn volume to 999",NULL,0,NULL);
	mciSendString("play interfacemusic repeat",NULL,0,NULL);
	//起始页动画
	int i;
	BeginBatchDraw();
	for(i=1;i<=48;i++){
		char tmp[128];
		IMAGE bk;
		sprintf_s(tmp,"res\\%d.jpg",i+1);
		loadimage(&bk,tmp);
		putimage(0,0,&bk);
		FlushBatchDraw();
		Sleep(50);
	}
	Sleep(2000);
	//用于返回跳跃 
loop1:{
	putimage(0,0,&main2);
	while(true){
		m=GetMouseMsg();
		//选择游戏介绍 
		if(m.x>98&&m.x<213&&m.y>148&&m.y<191){
			change(75,140,main21,main21y);
			FlushBatchDraw();
			if(m.uMsg==WM_LBUTTONDOWN){
			mciSendString("play turn from 0",NULL,0,NULL);
			loop2:{
				putimage(0,0,&main4);
				FlushBatchDraw();
				m=GetMouseMsg();
				if(m.x>531&&m.x<623&&m.y>403&&m.y<468){
				loop7:{
					change(540,433,main31,main31y);
					FlushBatchDraw();
					if(m.uMsg==WM_LBUTTONDOWN){
						mciSendString("play turn from 0",NULL,0,NULL);
						goto loop1;
					}
					m=GetMouseMsg();
					if(m.x>531&&m.x<623&&m.y>403&&m.y<468){
						goto loop7;
					}
					else goto loop2;
				}
				}
				else goto loop2;
			}
			}
		}
		//选择团队介绍
		if(m.x>98&&m.x<213&&m.y>247&&m.y<289){
			change(75,239,main22,main22y);
			FlushBatchDraw();
			if(m.uMsg==WM_LBUTTONDOWN){
			mciSendString("play turn from 0",NULL,0,NULL);
			loop3:{
				putimage(0,0,&main3);
				FlushBatchDraw();
				m=GetMouseMsg();
				if(m.x>531&&m.x<623&&m.y>403&&m.y<468){
				loop6:{
					change(540,433,main31,main31y);
					FlushBatchDraw();
					if(m.uMsg==WM_LBUTTONDOWN){
						mciSendString("play turn from 0",NULL,0,NULL);
						goto loop1;
					}
					m=GetMouseMsg();
					if(m.x>531&&m.x<623&&m.y>403&&m.y<468){
						goto loop6;
					}
					else goto loop3;
				}
				}
				else goto loop3;
			}
			}
		}
		//选择开始游戏 
		if(m.x>98&&m.x<213&&m.y>345&&m.y<391){
			change(75,342,main23,main23y);
			FlushBatchDraw();
			if(m.uMsg==WM_LBUTTONDOWN){
			mciSendString("play turn from 0",NULL,0,NULL);
			loop4:{
				putimage(0,0,&main5);
				FlushBatchDraw();
				m=GetMouseMsg();
				if(m.x>551&&m.x<623&&m.y>433&&m.y<468){
				loop8:{
					change(540,433,main31,main31y);
					FlushBatchDraw();
					m=GetMouseMsg();
					if(m.uMsg==WM_LBUTTONDOWN){
						mciSendString("play turn from 0",NULL,0,NULL);
						goto loop1;
					}
					if(m.x>551&&m.x<623&&m.y>433&&m.y<468){
						goto loop8;}
					else goto loop4;
				}
				}
				//选择脑残模式，返回 1
				if(m.x>98&&m.x<213&&m.y>156&&m.y<192){
				loop9:{
					change(70,150,main51,main51y);
					FlushBatchDraw();
					m=GetMouseMsg();
					if(m.uMsg==WM_LBUTTONDOWN){
						mciSendString("stop interfacemusic",NULL,0,NULL);
						mciSendString("close interfacemusic",NULL,0,NULL);
						closegraph();
						return 1;
					}
					if(m.x>98&&m.x<213&&m.y>156&&m.y<192)
						goto loop9;
					else goto loop4;
				}
				}
				//选择正常模式，返回  2
				if(m.x>98&&m.x<213&&m.y>236&&m.y<274){
				loop10:{
					change(70,230,main52,main52y);
					FlushBatchDraw();
					m=GetMouseMsg();
					if(m.uMsg==WM_LBUTTONDOWN){
						mciSendString("stop interfacemusic",NULL,0,NULL);
						mciSendString("close interfacemusic",NULL,0,NULL);
						closegraph();
						return 2;
					}
					if(m.x>98&&m.x<213&&m.y>236&&m.y<274)
						goto loop10;
					else goto loop4;
				}
				}
				//选择不正常模式，返回3
				if(m.x>98&&m.x<213&&m.y>316&&m.y<354){
				loop11:{
					change(70,310,main53,main53y);
					FlushBatchDraw();
					m=GetMouseMsg();
					if(m.uMsg==WM_LBUTTONDOWN){
						mciSendString("stop interfacemusic",NULL,0,NULL);
						mciSendString("close interfacemusic",NULL,0,NULL);
						closegraph();
						return 3;
					}
					if(m.x>98&&m.x<213&&m.y>316&&m.y<354)
						goto loop11;
					else goto loop4;
				}
				}
				//选择肯定过不了模式，返回  3
				if(m.x>98&&m.x<213&&m.y>396&&m.y<436){
				loop12:{
					change(70,390,main54,main54y);
					FlushBatchDraw();
					m=GetMouseMsg();
					if(m.uMsg==WM_LBUTTONDOWN){
						mciSendString("stop interfacemusic",NULL,0,NULL);
						mciSendString("close interfacemusic",NULL,0,NULL);
						closegraph();
						return 4;
					}
					if(m.x>98&&m.x<213&&m.y>396&&m.y<436)
						goto loop12;
					else goto loop4;
				}

				}
				if((m.x>551&&m.x<623&&m.y>433&&m.y<468||m.x>98&&m.x<213&&m.y>156&&m.y<192&&m.uMsg==WM_LBUTTONDOWN||m.x>98&&m.x<213&&m.y>236&&m.y<274&&m.uMsg==WM_LBUTTONDOWN||m.x>98&&m.x<213&&m.y>316&&m.y<354&&m.uMsg==WM_LBUTTONDOWN||m.x>98&&m.x<213&&m.y>396&&m.y<436&&m.uMsg==WM_LBUTTONDOWN)==0)
					goto loop4;
			}
			}
		}
		//选择退出游戏
		if(m.x>480&&m.x<623&&m.y>410&&m.y<468){
		loop5:{
			change(475,418,main24,main24y);
			FlushBatchDraw();
			m=GetMouseMsg();
			if(m.uMsg==WM_LBUTTONDOWN){
				mciSendString("stop interfacemusic",NULL,0,NULL);
				mciSendString("close interfacemusic",NULL,0,NULL);
				closegraph();
				return -1;
			}
			if(m.x>480&&m.x<623&&m.y>410&&m.y<468)
				goto loop5;
			else goto loop1;
		}
		}
		if((m.x>98&&m.x<213&&m.y>148&&m.y<191||m.x>98&&m.x<213&&m.y>247&&m.y<289||m.x>98&&m.x<213&&m.y>345&&m.y<391||m.x>502&&m.x<623&&m.y>427&&m.y<468)==0){
			putimage(0,0,&main2);
			FlushBatchDraw();
		}
	}
	EndBatchDraw();
}
}
