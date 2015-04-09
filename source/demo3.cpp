#define WIN32_LEAN_AND_MEAN  
#define INITGUID

//INCLUDES
#include <windows.h>
#include <windowsx.h> 
#include <mmsystem.h>
#include <objbase.h>
#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <time.h>
//DirectX stuff
#include <ddraw.h>
#include <dinput.h>
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
//Library stuff
#include "T3DLIB1.H"
#include "T3DLIB2.H"
#include "T3DLIB3.H"

//DEFINES
#define WINDOW_CLASS_NAME "WINCLASS1"
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define SCREEN_BPP      8
#define BITMAP_ID            0x4D42
#define MAX_COLORS_PALETTE   256
#define RAND_RANGE(a,b) ((a)+(rand()%((b)-(a)+1)))

#define MAX_STARS		256

//Las etapas del juego
//asteroides, naves y ambos
#define PHASE_I			0
#define PHASE_II		1
#define PHASE_III		2
#define PHASE_IV        3

#define BONUS			0
#define UP				1
#define HEART			2
#define TNT				3
#define MISSIL_UP		4

#define SHIP_FIGHTING	0
#define SHIP_READY		1
#define SHIP_EXPLODING	2

#define BOSS_FIGHTING   0
#define BOSS_EXPLODING  1
#define BOSS_DEAD		2

#define KABOOM_ASTEROID	0
#define KABOOM_SHIP		1
#define MAX_KABOOM		20 //10 de cada tipo
#define KABOOM_OFF		0
#define KABOOM_ON		1

#define MAX_PLASMA		50
#define PLASMA_OFF		0
#define PLASMA_ON		1

#define MAX_ROCKET		10
#define ROCKET_OFF		0
#define ROCKET_ON		1

#define MAX_ASTEROID	12  //3 de cada tipo
#define ASTEROID_OFF	0
#define ASTEROID_ON		1
#define ASTEROID_1		0
#define ASTEROID_2		1
#define ASTEROID_3		2
#define ASTEROID_4		3

#define MAX_ENEMY		20 //2 de cada tipo
#define ENEMY_OFF		0
#define ENEMY_ON		1
#define ENEMY_1			0
#define ENEMY_2			1
#define ENEMY_3			2
#define ENEMY_4			3
#define ENEMY_5			4
#define ENEMY_6			5
#define ENEMY_7			6
#define ENEMY_8			7
#define ENEMY_9			8
#define ENEMY_10		9

#define PARTICLE_STATE_DEAD               0
#define PARTICLE_STATE_ALIVE              1

#define PARTICLE_TYPE_FLICKER             0
#define PARTICLE_TYPE_FADE                1 

#define PARTICLE_COLOR_RED                0
#define PARTICLE_COLOR_GREEN              1
#define PARTICLE_COLOR_BLUE               2
#define PARTICLE_COLOR_WHITE              3

#define MAX_PARTICLES                     128

#define COLOR_RED_START                   13//32
#define COLOR_RED_END                     23//47
#define COLOR_GREEN_START                 32//96
#define COLOR_GREEN_END                   42//111
#define COLOR_BLUE_START                  144
#define COLOR_BLUE_END                    159
#define COLOR_WHITE_START                 237//16
#define COLOR_WHITE_END                   247//31

//contiene una estrella
typedef struct STAR_TYP{
	int x,y;
	int color;
	int plane;
}STAR;

//contiene una particula
typedef struct PARTICLE_TYP{
	int state;
    int type;
    int x,y;
    int xv,yv;
    int curr_color;
    int start_color;
    int end_color;
    int counter;
    int max_count;
} PARTICLE, *PARTICLE_PTR;

//Globales
HWND      main_window_handle = NULL;
HINSTANCE main_instance      = NULL;

//Campo de estrellas
STAR star_field[MAX_STARS];
//Particulas
PARTICLE particles[MAX_PARTICLES];

//Bitmap de inicio
BOB inicio;

//BOBS
BOB ship;
BOB boss[3];
BOB plasma[MAX_PLASMA];
BOB plasma_enemy[MAX_PLASMA];
BOB rocket[MAX_ROCKET];
BOB rocket_enemy[MAX_ROCKET];
BOB a1,a2,a3,a4;  //4 tipos de asteroides
BOB asteroid[MAX_ASTEROID]; //este arreglo contiene de los 4 tipos
BOB e1,e2,e3,e4,e5,e6,e7,e8,e9,e10; //naves enemigas
BOB enemy[MAX_ENEMY]; //este arreglo contiene de los 10 tipos
BOB k1,k2; //2 tipos de explosion
BOB kaboom[MAX_KABOOM];
BOB menu;
BOB numeros[10]; //10 digitos para el score
BOB health_bar,missiles_bar,lives_bar;
BOB ready,over,pause,completed;
BOB bonus,up,heart,tnt,missil_up; //para los pickups
BOB messages[8]; //los mensajes

//secuencias de animacion de la nave
int ship_normal[] = {0,1,2};
int ship_blink[]  = {0,1,2,3,4,5};

int level=1;

//banderas para saber que item ha tomado 
int heart_taken=0;
int missil_taken=0;
int bonus_taken=0;
int tnt_taken=0;
int up_taken=0;

int plasma_timer=0;
int plasma_timer_enemy=0;
int rocket_timer_enemy=0;
int rocket_flag=1;
int pause_flag=0;
int score=0;
int vida=100;
int misiles=50;
int player=2;

//Variables utiles para los jefes
int boss_final_pos = 1;
int final_pos;
int boss_go_down=1;
int boss_go_up=0;
int boss_move=0;

int phase=PHASE_I;	//para saber en que etapa del juego estamos
int start_phaseIV=1;
int count_asteroid=0;	//para saber cuantos asteroides se han creado
int count_enemy=0;	//para saber cuantas naves se han creado

int game_init=1;
int game_paused=0;
int game_over=0;
int game_ready=0;
int level_completed=0;

//Variables para la musica y sonidos
int sPlasma, sMissil, sExp_ship, sExp_enemy, sExp_boss;
int sCompleted,sPickUp;
int mLevel1, mLevel2, mBoss1, mBoss2, mInit;

time_t t_start,t_finish;
double t_result;

//Prototipos
int Game_Init(void *parms=NULL, int num_parms = 0);
int Game_Shutdown(void *parms=NULL, int num_parms = 0);
int Game_Main(void *parms=NULL,  int num_parms = 0);
int Game_Reset(void);

void Init_Stars(void);
void Draw_Stars(void);
void Move_Stars(void);

void Move_Boss(int);
void Draw_Boss(int);

void Move_Ship(void);
void Draw_Ship(void);

void Create_Plasma(void);
void Delete_Plasma(void);
void Move_Plasma(void);
void Draw_Plasma(void);
int Fire_Plasma(void);

void Create_Plasma_Enemy(void);
void Delete_Plasma_Enemy(void);
void Move_Plasma_Enemy(void);
void Draw_Plasma_Enemy(void);
int Fire_Plasma_Enemy(BOB*);

void Create_Rocket(void);
void Delete_Rocket(void);
void Move_Rocket(void);
void Draw_Rocket(void);
int Fire_Rocket(void);

void Create_Rocket_Enemy(void);
void Delete_Rocket_Enemy(void);
void Move_Rocket_Enemy(void);
void Draw_Rocket_Enemy(void);
int Fire_Rocket_Enemy(int,int,int,int);

void Create_Asteroid(void);
void Delete_Asteroid(void);
void Move_Asteroid(void);
void Draw_Asteroid(void);
int New_Asteroid(void);

void Create_Enemy(void);
void Delete_Enemy(void);
void Move_Enemy(void);
void Draw_Enemy(void);
int New_Enemy(void);

void Create_Kaboom(void);
void Delete_Kaboom(void);
void Draw_Kaboom(void);
int New_Kaboom(int,int,int);

void Init_Reset_Particles(void);
void Draw_Particles(void);
void Move_Particles(void);
void Start_Particle(int,int,int,int,int,int,int);
void Start_Particle_Explosion(int,int,int,int,int,int,int,int);
void Reset_Particles(void);

void Random_Pickup(BOB);
void New_Pickup(int,int,int);
void Move_Pickup(void);
void Draw_Pickup(void);

void Create_Tables(void);
void Score(void);
void Health_Bar(void);
void Missiles_Bar(void);
void Lives_Bar(void);
void Explode_All(void);
void Draw_Message(void);

LRESULT CALLBACK WindowProc(HWND hwnd,UINT msg,
							WPARAM wparam,LPARAM lparam)
{
	PAINTSTRUCT		ps;
	HDC				hdc;

	switch(msg)
	{	
		case WM_CREATE: 
        {
			return(0);
		} 
		break;
   
		case WM_PAINT: 
		{
	   	    hdc = BeginPaint(hwnd,&ps);	 
			EndPaint(hwnd,&ps);
			return(0);
   		}
		break;

		case WM_DESTROY: 
		{
			PostQuitMessage(0);
			return(0);
		}
		break;

		default:break;
    }//end switch

	return (DefWindowProc(hwnd, msg, wparam, lparam));
}//end WinProc

int WINAPI WinMain(	HINSTANCE hinstance,HINSTANCE hprevinstance,
					LPSTR lpcmdline,int ncmdshow)
{
	WNDCLASSEX winclass;
	HWND	   hwnd;
	MSG		   msg;
	//HDC        hdc;

	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC | 
	                          CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW); 
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

	main_instance = hinstance;

	if (!RegisterClassEx(&winclass))
		return(0);
	
	if (!(hwnd = CreateWindowEx(NULL,WINDOW_CLASS_NAME,
								"Demo",WS_POPUP | WS_VISIBLE,
					 			0,0,
								SCREEN_WIDTH,SCREEN_HEIGHT,
								NULL,NULL,hinstance,NULL)))
		return(0);

	main_window_handle = hwnd;

	Game_Init();

	while(TRUE)
	{
    	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
	       if (msg.message == WM_QUIT)
		       break;
	
		   TranslateMessage(&msg);
		   DispatchMessage(&msg);
		}//end if
		
		Game_Main();

	}//end while
	
	Game_Shutdown();

	return(msg.wParam);
}//end WinMain

void Score(void)
{
	int i,j;
	int aux;

	aux=score;

	for(i=100000,j=0; i>0; i/=10,j++)
	{
		switch(aux/i){
		case 0:
			Set_Pos_BOB(&numeros[0],19+(12*j),SCREEN_HEIGHT-50+11);
			Draw_BOB(&numeros[0],lpddsback);
			break;
		case 1:
			Set_Pos_BOB(&numeros[1],19+(12*j),SCREEN_HEIGHT-50+11);
			Draw_BOB(&numeros[1],lpddsback);
			break;
		case 2:
			Set_Pos_BOB(&numeros[2],19+(12*j),SCREEN_HEIGHT-50+11);
			Draw_BOB(&numeros[2],lpddsback);
			break;
		case 3:
			Set_Pos_BOB(&numeros[3],19+(12*j),SCREEN_HEIGHT-50+11);
			Draw_BOB(&numeros[3],lpddsback);
			break;
		case 4:
			Set_Pos_BOB(&numeros[4],19+(12*j),SCREEN_HEIGHT-50+11);
			Draw_BOB(&numeros[4],lpddsback);
			break;
		case 5:
			Set_Pos_BOB(&numeros[5],19+(12*j),SCREEN_HEIGHT-50+11);
			Draw_BOB(&numeros[5],lpddsback);
			break;
		case 6:
			Set_Pos_BOB(&numeros[6],19+(12*j),SCREEN_HEIGHT-50+11);
			Draw_BOB(&numeros[6],lpddsback);
			break;
		case 7:
			Set_Pos_BOB(&numeros[7],19+(12*j),SCREEN_HEIGHT-50+11);
			Draw_BOB(&numeros[7],lpddsback);
			break;
		case 8:
			Set_Pos_BOB(&numeros[8],19+(12*j),SCREEN_HEIGHT-50+11);
			Draw_BOB(&numeros[8],lpddsback);
			break;
		case 9:
			Set_Pos_BOB(&numeros[9],19+(12*j),SCREEN_HEIGHT-50+11);
			Draw_BOB(&numeros[9],lpddsback);
			break;
		default:
			break;
		}//switch

		aux=aux%i;
	}//for
}

void Health_Bar()
{
	int i,j;

	for(i=10,j=0; i<=100; i+=10,j++)
	{
		if(vida/i > 0)
		{
			Set_Pos_BOB(&health_bar,116+(health_bar.width*j)+2*j,
									SCREEN_HEIGHT-50+10);
			Draw_BOB(&health_bar,lpddsback);
		}
	}
}

void Missiles_Bar()
{
	int i,j;

	for(i=5,j=0; i<=50; i+=5,j++)
	{
		if(misiles/i > 0)
		{
			Set_Pos_BOB(&missiles_bar,221+(missiles_bar.width*j)+2*j,
									 SCREEN_HEIGHT-50+10);
			Draw_BOB(&missiles_bar,lpddsback);
		}
	}
}

void Lives_Bar()
{
	int i,j;
	int aux;

	aux=player;
	Draw_BOB(&lives_bar,lpddsback);

	for(i=10,j=0; i>0; i/=10,j++)
	{
		switch(aux/i){
		case 0:
			Set_Pos_BOB(&numeros[0],
						lives_bar.width+(j*numeros[0].width),10);
			Draw_BOB(&numeros[0],lpddsback);
			break;
		case 1:
			Set_Pos_BOB(&numeros[1],
						lives_bar.width+(j*numeros[0].width),10);
			Draw_BOB(&numeros[1],lpddsback);
			break;
		case 2:
			Set_Pos_BOB(&numeros[2],
						lives_bar.width+(j*numeros[0].width),10);
			Draw_BOB(&numeros[2],lpddsback);
			break;
		case 3:
			Set_Pos_BOB(&numeros[3],
						lives_bar.width+(j*numeros[0].width),10);
			Draw_BOB(&numeros[3],lpddsback);
			break;
		case 4:
			Set_Pos_BOB(&numeros[4],
						lives_bar.width+(j*numeros[0].width),10);
			Draw_BOB(&numeros[4],lpddsback);
			break;
		case 5:
			Set_Pos_BOB(&numeros[5],
						lives_bar.width+(j*numeros[0].width),10);
			Draw_BOB(&numeros[5],lpddsback);
			break;
		case 6:
			Set_Pos_BOB(&numeros[6],
						lives_bar.width+(j*numeros[0].width),10);
			Draw_BOB(&numeros[6],lpddsback);
			break;
		case 7:
			Set_Pos_BOB(&numeros[7],
						lives_bar.width+(j*numeros[0].width),10);
			Draw_BOB(&numeros[7],lpddsback);
			break;
		case 8:
			Set_Pos_BOB(&numeros[8],
						lives_bar.width+(j*numeros[0].width),10);
			Draw_BOB(&numeros[8],lpddsback);
			break;
		case 9:
			Set_Pos_BOB(&numeros[9],
						lives_bar.width+(j*numeros[0].width),10);
			Draw_BOB(&numeros[9],lpddsback);
			break;
		default:
			break;
		}//switch

		aux=aux%i;
	}//for
}

void Create_Tables(void)
{
	int ang = 0;
	
	for (ang=0; ang <360; ang++)
	{
		float fang = PI*(ang*22.5)/180;
		cos_look[ang] = -cos(fang+PI/2);
		sin_look[ang] = -sin(fang+PI/2);
	}
}

void Explode_All()
{
	int i;

	for(i=0; i<MAX_ASTEROID; i++)
	{
		if(asteroid[i].state==ASTEROID_ON)
		{
			score+=10;
			asteroid[i].state=ASTEROID_OFF;
			New_Kaboom(asteroid[i].width/2+asteroid[i].x,
					   asteroid[i].height/2+asteroid[i].y,
					   KABOOM_ASTEROID);
		}
	}
}

void Draw_Message()
{
	t_finish=time(0);
	t_result=difftime(t_finish,t_start);

	if(game_ready)
	{
		Draw_BOB(&messages[0],lpddsback);
		return;
	}
	if(heart_taken)
	{
		if(t_result<=5)		
			Draw_BOB(&messages[1],lpddsback);
		else
			heart_taken=0;
		return;
	}
	if(missil_taken)
	{
		if(t_result<=5)
			Draw_BOB(&messages[2],lpddsback);
		else
			missil_taken=0;
		return;
	}
	if(bonus_taken)
	{
		if(t_result<=5)
			Draw_BOB(&messages[3],lpddsback);
		else
			bonus_taken=0;
		return;
	}
	if(tnt_taken)
	{
		if(t_result<=5)
			Draw_BOB(&messages[4],lpddsback);
		else
			tnt_taken=0;
		return;
	}
	if(up_taken)
	{
		if(t_result<=5)
			Draw_BOB(&messages[5],lpddsback);
		else
			up_taken=0;
		return;
	}
	if(vida <=30)
	{
		Draw_BOB(&messages[7],lpddsback);
		return;
	}
	Draw_BOB(&messages[6],lpddsback);
}

void New_Pickup(int type,int x,int y)
{
	switch(type){
	case BONUS:
		if(bonus.state==BOB_STATE_DEAD)
		{
			bonus.state=BOB_STATE_ALIVE;
			Set_Pos_BOB(&bonus,x,y);
		}
		break;
	case UP:
		if(up.state==BOB_STATE_DEAD)
		{
			up.state=BOB_STATE_ALIVE;
			Set_Pos_BOB(&up,x,y);
		}
		break;
	case HEART:
		if(heart.state==BOB_STATE_DEAD)
		{
			heart.state=BOB_STATE_ALIVE;
			Set_Pos_BOB(&heart,x,y);
		}
		break;
	case TNT:
		if(tnt.state==BOB_STATE_DEAD)
		{
			tnt.state=BOB_STATE_ALIVE;
			Set_Pos_BOB(&tnt,x,y);
		}
		break;
	case MISSIL_UP:
		if(missil_up.state==BOB_STATE_DEAD)
		{
			missil_up.state=BOB_STATE_ALIVE;
			Set_Pos_BOB(&missil_up,x,y);
		}
		break;
	default:
		break;
	}//switch
}

void Move_Pickup()
{
	if(game_paused)
		return;

	if(bonus.state==BOB_STATE_ALIVE)
	{
		if((bonus.y+=2)>SCREEN_HEIGHT)
			bonus.state=BOB_STATE_DEAD;
		if(Collision_BOBS(&bonus,&ship))
		{
			t_start=time(0);
			bonus_taken=1;
			bonus.state=BOB_STATE_DEAD;
			score+=1000;
			DSound_Play(sPickUp);
		}
	}
	if(up.state==BOB_STATE_ALIVE)
	{
		if((up.y+=2)>SCREEN_HEIGHT)
			up.state=BOB_STATE_DEAD;
		if(Collision_BOBS(&up,&ship))
		{
			t_start=time(0);
			up_taken=1;
			up.state=BOB_STATE_DEAD;
			player++;
			DSound_Play(sPickUp);
		}
	}
	if(heart.state==BOB_STATE_ALIVE)
	{
		if((heart.y+=2)>SCREEN_HEIGHT)
			heart.state=BOB_STATE_DEAD;
		if(Collision_BOBS(&heart,&ship))
		{
			t_start=time(0);
			heart_taken=1;
			heart.state=BOB_STATE_DEAD;
			vida+=50;
			if(vida>100)
				vida=100;
			DSound_Play(sPickUp);
		}
	}
	if(tnt.state==BOB_STATE_ALIVE)
	{
		if((tnt.y+=2)>SCREEN_HEIGHT)
			tnt.state=BOB_STATE_DEAD;
		if(Collision_BOBS(&tnt,&ship))
		{
			t_start=time(0);
			tnt_taken=1;
			tnt.state=BOB_STATE_DEAD;
			Explode_All();
			DSound_Play(sExp_enemy);
		}
	}
	if(missil_up.state==BOB_STATE_ALIVE)
	{
		if((missil_up.y+=2)>SCREEN_HEIGHT)
			missil_up.state=BOB_STATE_DEAD;
		if(Collision_BOBS(&missil_up,&ship))
		{
			t_start=time(0);
			missil_taken=1;
			missil_up.state=BOB_STATE_DEAD;
			if((misiles+=25)>50)
				misiles=50;
			DSound_Play(sPickUp);
		}
	}
}

void Draw_Pickup()
{
	if(bonus.state==BOB_STATE_ALIVE)
		Draw_BOB(&bonus,lpddsback);
	if(up.state==BOB_STATE_ALIVE)
		Draw_BOB(&up,lpddsback);
	if(heart.state==BOB_STATE_ALIVE)
		Draw_BOB(&heart,lpddsback);
	if(tnt.state==BOB_STATE_ALIVE)
		Draw_BOB(&tnt,lpddsback);
	if(missil_up.state==BOB_STATE_ALIVE)
		Draw_BOB(&missil_up,lpddsback);
}

void Random_Pickup(BOB b)
{
	if((rand()%5) == 3)
	{
		switch(rand()%5){
		case BONUS:
			New_Pickup(BONUS,(b.width/2)+b.x,
							 (b.height/2)+b.y);
			break;
		case UP:
			if((rand()%3) == 2)
			New_Pickup(UP,(b.width/2)+b.x,
						  (b.height/2)+b.y);
			else
			New_Pickup(BONUS,(b.width/2)+b.x,
							 (b.height/2)+b.y);
			break;
		case HEART:
			New_Pickup(HEART,(b.width/2)+b.x,
							 (b.height/2)+b.y);
			break;
		case TNT:
			New_Pickup(TNT,(b.width/2)+b.x,
						   (b.height/2)+b.y);
			break;
		case MISSIL_UP:
			New_Pickup(MISSIL_UP,(b.width/2)+b.x,
							     (b.height/2)+b.y);
			break;
		default:
			break;
		}//switch
	}//if
}

void Init_Reset_Particles(void)
{
	for (int index=0; index<MAX_PARTICLES; index++)
    {
		particles[index].state = PARTICLE_STATE_DEAD;
	    particles[index].type  = PARTICLE_TYPE_FADE;
		particles[index].x     = 0;
		particles[index].y     = 0;
		particles[index].xv    = 0;
		particles[index].yv    = 0;
		particles[index].start_color = 0;
		particles[index].end_color   = 0;
	    particles[index].curr_color  = 0;
		particles[index].counter     = 0;
		particles[index].max_count   = 0;
    }

}

void Start_Particle(int type, int color, int count, int x, int y, int xv, int yv)
{
	int pindex = -1;

	for (int index=0; index < MAX_PARTICLES; index++)
		if (particles[index].state == PARTICLE_STATE_DEAD)
		{
			pindex = index; 
			break;
		}

	if (pindex==-1)
		return;

	particles[pindex].state = PARTICLE_STATE_ALIVE;
	particles[pindex].type  = type;
	particles[pindex].x     = x;
	particles[pindex].y     = y;
	particles[pindex].xv    = xv;
	particles[pindex].yv    = yv;
	particles[pindex].counter     = 0;
	particles[pindex].max_count   = count;     

	switch(color){
	case PARTICLE_COLOR_RED:
		particles[pindex].start_color = COLOR_RED_START;
        particles[pindex].end_color   = COLOR_RED_END;
        break;
    case PARTICLE_COLOR_GREEN:
        particles[pindex].start_color = COLOR_GREEN_START;
        particles[pindex].end_color   = COLOR_GREEN_END;
		break;
	case PARTICLE_COLOR_BLUE:
        particles[pindex].start_color = COLOR_BLUE_START;
        particles[pindex].end_color   = COLOR_BLUE_END;
		break;
    case PARTICLE_COLOR_WHITE:
        particles[pindex].start_color = COLOR_WHITE_START;
        particles[pindex].end_color   = COLOR_WHITE_END;
        break;
	default:
		break;
    }//switch

	if (type == PARTICLE_TYPE_FLICKER)
	{
		particles[index].curr_color=RAND_RANGE(particles[index].start_color, particles[index].end_color);
	}
	else
	{
	   particles[index].curr_color=particles[index].start_color;
	}
}

void Start_Particle_Explosion(int type, int color, int count, 
                              int x, int y, int xv, int yv, int num_particles)
{
	while(--num_particles >=0)
	{
	    int ang = rand()%360;
	    float vel = (float)2+rand()%4;
	    Start_Particle(type,color,count,
                   x+RAND_RANGE(-10,10),y+RAND_RANGE(-10,10), 
                   (int)(xv+cos_look[ang]*vel),(int)(yv+sin_look[ang]*vel));
    }
}

void Draw_Particles(void)
{
	DDraw_Lock_Back_Surface();

	for(int index=0; index<MAX_PARTICLES; index++)
    {
	    if (particles[index].state==PARTICLE_STATE_ALIVE)
		{
			int x = particles[index].x;  //+ (SCREEN_WIDTH/2);
			int y = particles[index].y;  //+ (SCREEN_HEIGHT/2);

		    if (x >= SCREEN_WIDTH || x < 0 || y >= SCREEN_HEIGHT || y < 0)
				continue;

			Draw_Pixel(x,y,particles[index].curr_color, back_buffer, back_lpitch);
		}
	}

	DDraw_Unlock_Back_Surface(); 
}

void Move_Particles(void)
{
	for (int index=0; index<MAX_PARTICLES; index++)
    {
		if (particles[index].state == PARTICLE_STATE_ALIVE)
		{	
			particles[index].x+=particles[index].xv;
			particles[index].y+=particles[index].yv;

	       if (particles[index].type==PARTICLE_TYPE_FLICKER)
		   {			
				particles[index].curr_color = RAND_RANGE(particles[index].start_color, particles[index].end_color);

				if (++particles[index].counter >= particles[index].max_count)
				{
					particles[index].state = PARTICLE_STATE_DEAD;
				}
			}
			else
			{
				if (++particles[index].counter >= particles[index].max_count)
				{
					particles[index].counter = 0;
					if (++particles[index].curr_color>particles[index].end_color)
					{
						particles[index].state = PARTICLE_STATE_DEAD;  
					}
				}
			} // end else
		} // end if 
    } // end for index
} // end Move_Particles

/*----------------------------------------------------------
	Init_Stars: Inicializa el campo de estrellas
/----------------------------------------------------------*/
void Init_Stars()
{
	int index;

	for(index=0; index<MAX_STARS; index++)
	{
		switch(rand()%3){
		case 0:
			star_field[index].color = rand()%255;
			star_field[index].plane = 0;
			break;
		case 1:
			star_field[index].color = rand()%245;
			star_field[index].plane = 1;
			break;
		case 2:
			star_field[index].color = rand()%235;
			star_field[index].plane = 2;
			break;
		default:
			break;
		}//switch

		star_field[index].x = rand()%SCREEN_WIDTH;
		star_field[index].y = rand()%SCREEN_HEIGHT;
	}//for
}

/*----------------------------------------------------------
						 Move_Stars
/----------------------------------------------------------*/
void Move_Stars()
{
	int i;
	int sy;

	for(i=0; i<MAX_STARS; i++)
	{
		sy=star_field[i].y;

		switch(star_field[i].plane){
		case 0:
			sy+=3;
			break;
		case 1:
			sy+=2;
			break;
		case 2:
			sy+=1;
			break;
		default:
			break;
		}//switch

		if(sy>=SCREEN_HEIGHT)
			sy=sy-SCREEN_HEIGHT;
		
		star_field[i].y=sy;
	}//for
}

/*----------------------------------------------------------
						 Draw_Stars
/----------------------------------------------------------*/
void Draw_Stars()
{
	int i;

	DDraw_Lock_Back_Surface();
	
	for(i=0; i<MAX_STARS; i++)
		Draw_Pixel(star_field[i].x,star_field[i].y,star_field[i].color,back_buffer,back_lpitch);

	DDraw_Unlock_Back_Surface();
}


void Move_Boss(int i)
{
	int mx,mbx;
	int px;
	int fire_rocket;
	int fire_plasma;
	static int b_aux=1;

	if(game_paused || ship.state == SHIP_READY || 
		boss[i].state == BOSS_EXPLODING ||
		boss[i].state == BOSS_DEAD)
		return; 

	mx = ship.width/2 + ship.x;
	mbx = boss[i].width/2 + boss[i].x;

	switch(boss_move){
	case 0:
		if(boss[i].y++ == 0)
			boss_move=1;
		break;

	case 1:
		//Follow it
		//con un rango de error de 2 pixels
		if(mx-2 > mbx)
			boss[i].x+=2;
		else
		if(mx+2 < mbx)
			boss[i].x-=2;
	
		boss_move = rand()%50+1;
		if(boss_move > 3)
			boss_move = 1;

		break;

	case 2:
		//Random Position
		//Ya llego a la posicion final?
		if(boss_final_pos)
		{
			//calcula la nueva posicion final
			final_pos = rand()%SCREEN_WIDTH;
			boss_final_pos=0;
		}

		if(boss[i].x<final_pos)
		{
			if(boss[i].x++ > SCREEN_WIDTH - boss[i].width)
			{
				//no llego a la posicion final pero llego al limite
				//en el eje x
				boss[i].x--;
				boss_final_pos=1;
				boss_move = rand()%50+1;
				if(boss_move > 3)
					boss_move = 1;
			}
		}
		else
		if(boss[i].x>final_pos)
		{
			boss[i].x--;
		}
		else
		if(boss[i].x == final_pos)
		{
			boss_final_pos=1;
			boss_move = rand()%50+1;
			if(boss_move > 3)
				boss_move = 1;
		}

		break;

	case 3:
		//Crush Attack

		if(level==2 && b_aux)
		{
			b_aux=0;
			if(boss[i].x < SCREEN_WIDTH/2)
				boss[i].xv=6;
			else
				boss[i].xv=-6;
		}

		if(boss_go_down)
		{
			boss[i].x+=boss[i].xv;
			if(boss[i].x > SCREEN_WIDTH-boss[i].width)
				boss[i].x-=boss[i].xv;
			if(boss[i].x <= 0)
				boss[i].x-=boss[i].xv;

			boss[i].y+=6;
			if(boss[i].y > (SCREEN_HEIGHT - boss[i].height - menu.height))
			{
				boss[i].y-=6;
				boss_go_down=0;
				boss_go_up=1;
			}
		}
		else
		if(boss_go_up)
		{

			boss[i].x+=boss[i].xv;
			if(boss[i].x > SCREEN_WIDTH-boss[i].width)
				boss[i].x-=boss[i].xv;
			if(boss[i].x <= 0)
				boss[i].x-=boss[i].xv;

			boss[i].y-=3;

			if(boss[i].y < 0)
			{
				b_aux=1;
				boss[i].y+=3;
				boss_go_down=1;
				boss_go_up=0;
				boss_move = rand()%50+1;
				if(boss_move > 3)
					boss_move = 1;
			}
		}

		break;

	default:
		break;
	}//switch

	if(level==1)
	{
		fire_rocket = rand()%20;
		fire_plasma = rand()%20;
	}
	else
	{
		fire_rocket = rand()%11;
		fire_plasma = rand()%11;
	}

	if(fire_rocket == 5)
	{
		if(level==1)
			Fire_Rocket_Enemy(21+boss[i].x,
							  115+boss[i].y,
							  2,112);
		else
		if(i==1)
			Fire_Rocket_Enemy(boss[i].width/2+boss[i].x,
							  boss[i].height+boss[i].y,
							  1,0);
	}

	if(fire_plasma == 10)
		if(level==1 || (level==2 && i==2))
			Fire_Plasma_Enemy(&boss[i]);

	//revisa si hay colisiones
	if(ship.state==SHIP_FIGHTING)
	{
		if(Collision_BOBS(&boss[i],&ship))
		{
			vida=vida-10;
			if(vida < 0)
			{
				DSound_Play(sExp_ship);
				t_start=time(0);
				ship.state=SHIP_EXPLODING;
				vida=100;
				misiles=50;
				if(player-- <= 0)
					game_over=1;
			}
		}//collision
	}//If ship
}

void Draw_Boss(int i)
{
	if(boss[i].state == BOSS_FIGHTING)
	{
		//Animate_BOB(&boss[i]);
		Draw_BOB(&boss[i],lpddsback);
	}
	else
	if(boss[i].state == BOSS_EXPLODING)
	{
		New_Kaboom((rand()%boss[i].width)+boss[i].x,
				   (rand()%boss[i].height)+boss[i].y,
				   KABOOM_SHIP);
		t_finish=time(0);
		t_result=difftime(t_finish,t_start);
		if(t_result >= 5)
		{
			if(level==1)
			{
				ship.state=SHIP_READY;
				level_completed=1;
				DMusic_Stop(mBoss1);
				DSound_Stop_All_Sounds();
				DSound_Play(sCompleted);
			}
			else
				boss[i].state=BOSS_DEAD;
		}
	}
	else
	if(level==2 && boss[1].state==BOSS_DEAD && 
		boss[2].state==BOSS_DEAD)
	{
		ship.state=SHIP_READY;
		level_completed=1;
		DMusic_Stop(mBoss2);
		DSound_Stop_All_Sounds();
		DSound_Play(sCompleted);
	}
}

/*----------------------------------------------------------
	Move_ship: mueve la nave
/----------------------------------------------------------*/
void Move_Ship()
{
	if(game_paused || game_ready || ship.state!=SHIP_FIGHTING)
		return;

	if(keyboard_state[DIK_LEFT])
	{
		if((ship.x-=6) < 0)
			ship.x+=6;
	}
	if(keyboard_state[DIK_RIGHT])
	{
		if((ship.x+=6)>SCREEN_WIDTH-ship.width)
			ship.x-=6;
	}
	if(keyboard_state[DIK_UP])
	{
		if((ship.y-=6)<0)
			ship.y+=6;
	}
	if(keyboard_state[DIK_DOWN])
	{
		if((ship.y+=6) > SCREEN_HEIGHT-ship.height-50)
			ship.y-=6;
	}
}

void Draw_Ship()
{
	if(ship.state==SHIP_FIGHTING || ship.state==SHIP_READY)
	{
		Animate_BOB(&ship);
		Draw_BOB(&ship,lpddsback);
	}
	else
	if(ship.state==SHIP_EXPLODING)
	{
		New_Kaboom((rand()%ship.width)+ship.x,
				   (rand()%ship.height)+ship.y,
				   KABOOM_SHIP);
		t_finish=time(0);
		t_result=difftime(t_finish,t_start);
		if(t_result >= 3)
		{
			ship.state=SHIP_READY;
			Set_Animation_BOB(&ship,1);
			game_ready=1;
			t_start=time(0);
			Set_Pos_BOB(&ship,SCREEN_WIDTH/2-ship.width/2,
							  SCREEN_HEIGHT-100-ship.height);
		}
	}//else if EXPLODING
}

/*----------------------------------------------------------
						Create_Plasma
/----------------------------------------------------------*/
void Create_Plasma()
{
	int index;

	Load_Bitmap_File(&bitmap8bit,"art\\b02.dat");
	Set_Palette(bitmap8bit.palette);

	Create_BOB(&plasma[0],0,0,12,22,1,
				BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
				DDSCAPS_SYSTEMMEMORY);

	Load_Frame_BOB(&plasma[0],&bitmap8bit,0,
						231,78,BITMAP_EXTRACT_MODE_ABS); 


	Unload_Bitmap_File(&bitmap8bit);
	plasma[0].state=PLASMA_OFF;

	for(index=1; index<MAX_PLASMA; index++)
		memcpy(&plasma[index],&plasma[0],sizeof(BOB));
}

/*----------------------------------------------------------
						Delete_Plasma
/----------------------------------------------------------*/
void Delete_Plasma()
{
	int index;
	for(index=0; index<MAX_PLASMA; index++)
		Destroy_BOB(&plasma[index]);
}

/*----------------------------------------------------------
						Move_Plasma
/----------------------------------------------------------*/
void Move_Plasma()
{
	int index,j,k;

	if(game_paused)
		return;

	for(index=0; index<MAX_PLASMA; index++)
	{
		if(plasma[index].state == PLASMA_ON)
		{
			plasma[index].y-=10;

			//revisa si hay colisiones con algun asteroide
			for(j=0; j<MAX_ASTEROID; j++)
			{
				//para que suceda el asteroide debe estar activo
				if(asteroid[j].state==ASTEROID_ON)
				{
					if(Collision_BOBS(&plasma[index],&asteroid[j]))
					{
						for(k=0;k<5;k++)
						{
							Start_Particle_Explosion(PARTICLE_TYPE_FLICKER,
								PARTICLE_COLOR_GREEN,40,
								asteroid[j].x+asteroid[j].width/2,
								asteroid[j].y+asteroid[j].height/2,
								RAND_RANGE(-1,1),RAND_RANGE(-1,1),1);
						}
						score++;
						plasma[index].state=PLASMA_OFF;
						if(asteroid[j].health-- <= 0)
						{
							Random_Pickup(asteroid[j]);
							score+=10;
							asteroid[j].state=ASTEROID_OFF;
							New_Kaboom(asteroid[j].width/2+asteroid[j].x,
									   asteroid[j].height/2+asteroid[j].y,KABOOM_ASTEROID);
							DSound_Play(sExp_enemy);
						}
					}
				}
			}

			//revisa si hay colisiones con alguna nave enemiga
			for(j=0; j<MAX_ENEMY; j++)
			{
				if(enemy[j].state==ENEMY_ON)
				{
					if(Collision_BOBS(&plasma[index],&enemy[j]))
					{
						for(k=0;k<5;k++)
						{
							Start_Particle_Explosion(PARTICLE_TYPE_FLICKER,
								PARTICLE_COLOR_GREEN,40,
								enemy[j].x+enemy[j].width/2,
								enemy[j].y+enemy[j].height/2,
								RAND_RANGE(-1,1),RAND_RANGE(-1,1),1);
						}
						score++;
						plasma[index].state=PLASMA_OFF;
						if(enemy[j].health-- <= 0)
						{
							Random_Pickup(enemy[j]);
							score+=10;
							enemy[j].state=ENEMY_OFF;
							New_Kaboom(enemy[j].width/2+enemy[j].x,
									   enemy[j].height/2+enemy[j].y,
									   KABOOM_SHIP);
							DSound_Play(sExp_enemy);
						}
					}
				}
			}

			//colisiones con el jefe
			if(phase == PHASE_IV)
			{
				for(j=0; j<3; j++)
				{
					if(boss[j].state == BOSS_FIGHTING)
					{
						if(Collision_BOBS(&boss[j],&plasma[index]))
						{
							for(k=0;k<5;k++)
							{
								Start_Particle_Explosion(PARTICLE_TYPE_FLICKER,
									PARTICLE_COLOR_GREEN,40,
									boss[j].x+boss[j].width/2,
									boss[j].y+boss[j].height/2,
									RAND_RANGE(-1,1),RAND_RANGE(-1,1),1);
							}
							score++;
							plasma[index].state=PLASMA_OFF;
							boss[j].health--;
							if(boss[j].health <= 0)
							{
								DSound_Play(sExp_boss);
								t_start=time(0);
								boss[j].state=BOSS_EXPLODING;
							}
						}
					}
				}
			}

			if(plasma[index].y<0)
				plasma[index].state=PLASMA_OFF;
		}
	}		
}

/*----------------------------------------------------------
						Draw_Plasma
/----------------------------------------------------------*/
void Draw_Plasma()
{
	int index;

	for(index=0; index<MAX_PLASMA; index++)
	{
		if(plasma[index].state == PLASMA_ON)
		{
			Draw_BOB(&plasma[index],lpddsback);
		}
	}
}

/*----------------------------------------------------------
						Fire_Plasma
/----------------------------------------------------------*/
int Fire_Plasma()
{
	int index;
	int count;
	int x1,y1,x2,y2;

	count=1;

	if(game_paused || game_ready)
		return 0;

	if(plasma_timer++ < 5)
		return 0;

	plasma_timer=0;

	//la posicion del disparador menos el tamaño de la bala
	x1=ship.x+18;
	y1=ship.y+20;
	x2=ship.x+67;
	y2=ship.y+20;

	for(index=0; index<MAX_PLASMA; index++)
	{
		if(plasma[index].state == PLASMA_OFF)
		{
			if(count==1)
			{
				plasma[index].state=PLASMA_ON;
				Set_Pos_BOB(&plasma[index],x1,y1);
				count++;
			}
			else
			if(count == 2)
			{
				plasma[index].state=PLASMA_ON;
				Set_Pos_BOB(&plasma[index],x2,y2);
				DSound_Play(sPlasma);
				return 1;
			}
		}
	}

	return 0;
}

/*----------------------------------------------------------
						Create_Plasma_Enemy
/----------------------------------------------------------*/
void Create_Plasma_Enemy()
{
	int index;

	Load_Bitmap_File(&bitmap8bit,"art\\b02.dat");
	Set_Palette(bitmap8bit.palette);

	Create_BOB(&plasma_enemy[0],0,0,12,13,1,
				BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
				DDSCAPS_SYSTEMMEMORY);

	Load_Frame_BOB(&plasma_enemy[0],&bitmap8bit,0,
						209,20,BITMAP_EXTRACT_MODE_ABS); 


	Unload_Bitmap_File(&bitmap8bit);
	plasma_enemy[0].state=PLASMA_OFF;

	for(index=1; index<MAX_PLASMA; index++)
		memcpy(&plasma_enemy[index],&plasma_enemy[0],sizeof(BOB));
}

/*----------------------------------------------------------
						Delete_Plasma_Enemy
/----------------------------------------------------------*/
void Delete_Plasma_Enemy()
{
	int index;
	for(index=0; index<MAX_PLASMA; index++)
		Destroy_BOB(&plasma_enemy[index]);
}

/*----------------------------------------------------------
						Move_Plasma_Enemy
/----------------------------------------------------------*/
void Move_Plasma_Enemy()
{
	int index,j,k;

	if(game_paused)
		return;

	for(index=0; index<MAX_PLASMA; index++)
	{
		if(plasma_enemy[index].state == PLASMA_ON)
		{
			plasma_enemy[index].y+=10;

			//revisa si hay colisiones
			if(ship.state==SHIP_FIGHTING)
			{
				if(Collision_BOBS(&plasma_enemy[index],&ship))
				{
					for(k=0;k<10;k++)
					{
						Start_Particle_Explosion(PARTICLE_TYPE_FLICKER,
							PARTICLE_COLOR_BLUE,40,
							ship.x+ship.width/2,
							ship.y+ship.height/2,
							RAND_RANGE(-1,1),RAND_RANGE(-1,1),1);
					}
					plasma_enemy[index].state=PLASMA_OFF;
					vida=vida-5;
					if(vida < 0)
					{
						DSound_Play(sExp_ship);
						t_start=time(0);
						ship.state=SHIP_EXPLODING;
						vida=100;
						misiles=50;
						if(player-- <= 0)
							game_over=1;
					}
				}
			}

			if(plasma_enemy[index].y>SCREEN_HEIGHT)
				plasma_enemy[index].state=PLASMA_OFF;
		}
	}		
}

/*----------------------------------------------------------
						Draw_Plasma_Enemy
/----------------------------------------------------------*/
void Draw_Plasma_Enemy()
{
	int index;

	for(index=0; index<MAX_PLASMA; index++)
	{
		if(plasma_enemy[index].state == PLASMA_ON)
		{
			Draw_BOB(&plasma_enemy[index],lpddsback);
		}
	}
}

/*----------------------------------------------------------
						Fire_Plasma_Enemy
/----------------------------------------------------------*/
int Fire_Plasma_Enemy(BOB *e)
{
	int index;
	int x1,y1;
	int b=0;

	if(plasma_timer_enemy++ < 5)
		return 0;

	plasma_timer_enemy=0;

	//Calculamos el centro del enemigo
	x1=e->width/2+e->x;
	y1=e->height+e->y;

	//Centramos la bala
	x1-=plasma_enemy[0].width/2;
	y1-=plasma_enemy[0].height;

	for(index=0; index<MAX_PLASMA; index++)
	{
		if(plasma_enemy[index].state == PLASMA_OFF)
		{
			plasma_enemy[index].state=PLASMA_ON;
			Set_Pos_BOB(&plasma_enemy[index],x1,y1);
			return 1;
		}
	}

	return 0;
}

/*----------------------------------------------------------
						Create_Rocket
/----------------------------------------------------------*/
void Create_Rocket()
{
	int index;

	Load_Bitmap_File(&bitmap8bit,"art\\b02.dat");
	Set_Palette(bitmap8bit.palette);

	Create_BOB(&rocket[0],0,0,14,20,2,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);

	Load_Frame_BOB(&rocket[0],&bitmap8bit,0,
						6,20,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&rocket[0],&bitmap8bit,1,
						40,20,BITMAP_EXTRACT_MODE_ABS); 


	Unload_Bitmap_File(&bitmap8bit);
	Set_Anim_Speed_BOB(&rocket[0],8);
	rocket[0].state=ROCKET_OFF;

	for(index=1; index<MAX_ROCKET; index++)
		memcpy(&rocket[index],&rocket[0],sizeof(BOB));
}

/*----------------------------------------------------------
						Delete_Rocket
/----------------------------------------------------------*/
void Delete_Rocket()
{
	int index;
	for(index=0; index<MAX_ROCKET; index++)
		Destroy_BOB(&rocket[index]);
}

/*----------------------------------------------------------
						Move_Rocket
/----------------------------------------------------------*/
void Move_Rocket()
{
	int index,j,k;

	if(game_paused)
		return;

	for(index=0; index<MAX_ROCKET; index++)
	{
		if(rocket[index].state == ROCKET_ON)
		{
			rocket[index].y-=10;

			//revisa si hay colisiones con algun asteroide
			for(j=0; j<MAX_ASTEROID; j++)
			{
				//para que suceda el asteroide debe estar activo
				if(asteroid[j].state==ASTEROID_ON)
				{
					if(Collision_BOBS(&rocket[index],&asteroid[j]))
					{
						for(k=0;k<20;k++)
						{
							Start_Particle_Explosion(PARTICLE_TYPE_FLICKER,
								PARTICLE_COLOR_GREEN,40,
								asteroid[j].x+asteroid[j].width/2,
								asteroid[j].y+asteroid[j].height/2,
								RAND_RANGE(-1,1),RAND_RANGE(-1,1),1);
						}
						score+=5;
						rocket[index].state=ROCKET_OFF;
						if((asteroid[j].health-=3) <= 0)
						{
							Random_Pickup(asteroid[j]);
							score+=10;
							asteroid[j].state=ASTEROID_OFF;
							New_Kaboom(asteroid[j].width/2+asteroid[j].x,
									   asteroid[j].height/2+asteroid[j].y,
									   KABOOM_ASTEROID);
							DSound_Play(sExp_enemy);
						}
					}
				}
			}

			//revisa si hay colisiones con alguna nave enemiga
			for(j=0; j<MAX_ENEMY; j++)
			{
				if(enemy[j].state==ENEMY_ON)
				{
					if(Collision_BOBS(&rocket[index],&enemy[j]))
					{
						for(k=0;k<20;k++)
						{
							Start_Particle_Explosion(PARTICLE_TYPE_FLICKER,
								PARTICLE_COLOR_GREEN,40,
								enemy[j].x+enemy[j].width/2,
								enemy[j].y+enemy[j].height/2,
								RAND_RANGE(-1,1),RAND_RANGE(-1,1),1);
						}
						score+=5;
						rocket[index].state=ROCKET_OFF;
						if((enemy[j].health-=3) <= 0)
						{
							Random_Pickup(enemy[j]);
							score+=10;
							enemy[j].state=ENEMY_OFF;
							New_Kaboom(enemy[j].width/2+enemy[j].x,
									   enemy[j].height/2+enemy[j].y,
									   KABOOM_SHIP);
							DSound_Play(sExp_enemy);
						}
					}
				}
			}

			if(phase == PHASE_IV)
			{
				for(j=0; j<3; j++)
				{
					if(boss[j].state == BOSS_FIGHTING)
					{
						if(Collision_BOBS(&boss[j],&rocket[index]))
						{
							for(k=0;k<20;k++)
							{
								Start_Particle_Explosion(PARTICLE_TYPE_FLICKER,
									PARTICLE_COLOR_GREEN,40,
									boss[j].x+boss[j].width/2,
									boss[j].y+boss[j].height/2,
									RAND_RANGE(-1,1),RAND_RANGE(-1,1),1);
							}//for
							score+=5;
							rocket[index].state=ROCKET_OFF;
							boss[j].health-=5;
							if(boss[j].health <= 0)
							{
								DSound_Play(sExp_boss);
								t_start=time(0);
								boss[j].state=BOSS_EXPLODING;
							}
						}//if collision
					}//if boss
				}//for
			}
			if(rocket[index].y<0)
				rocket[index].state=ROCKET_OFF;
		}
	}		
}

/*----------------------------------------------------------
						Draw_Rocket
/----------------------------------------------------------*/
void Draw_Rocket()
{
	int index;

	for(index=0; index<MAX_ROCKET; index++)
	{
		if(rocket[index].state == ROCKET_ON)
		{
			Animate_BOB(&rocket[index]);
			Draw_BOB(&rocket[index],lpddsback);
		}
	}
}

/*----------------------------------------------------------
						Fire_Rocket
/----------------------------------------------------------*/
int Fire_Rocket()
{
	int index;
	int x,y;

	if(game_paused || game_ready)
		return 0;

	x=ship.x+40;
	y=ship.y-10;

	for(index=0; index<MAX_ROCKET; index++)
	{
		if(rocket[index].state == ROCKET_OFF)
		{
			misiles--;
			rocket[index].state=ROCKET_ON;
			Set_Pos_BOB(&rocket[index],x,y);
			DSound_Play(sMissil);
			return 1;
		}
	}

	return 0;
}

/*----------------------------------------------------------
						Create_Rocket_Enemy
/----------------------------------------------------------*/
void Create_Rocket_Enemy()
{
	int index;

	Load_Bitmap_File(&bitmap8bit,"art\\b02.dat");
	Set_Palette(bitmap8bit.palette);

	Create_BOB(&rocket_enemy[0],0,0,12,20,2,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);

	Load_Frame_BOB(&rocket_enemy[0],&bitmap8bit,0,
						275,17,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&rocket_enemy[0],&bitmap8bit,1,
						309,17,BITMAP_EXTRACT_MODE_ABS); 


	Unload_Bitmap_File(&bitmap8bit);
	Set_Anim_Speed_BOB(&rocket_enemy[0],8);
	rocket_enemy[0].state=ROCKET_OFF;

	for(index=1; index<MAX_ROCKET; index++)
		memcpy(&rocket_enemy[index],&rocket_enemy[0],sizeof(BOB));
}

/*----------------------------------------------------------
						Delete_Rocket_Enemy
/----------------------------------------------------------*/
void Delete_Rocket_Enemy()
{
	int index;
	for(index=0; index<MAX_ROCKET; index++)
		Destroy_BOB(&rocket_enemy[index]);
}

/*----------------------------------------------------------
						Move_Rocket_Enemy
/----------------------------------------------------------*/
void Move_Rocket_Enemy()
{
	int index,j,k;

	if(game_paused)
		return;

	for(index=0; index<MAX_ROCKET; index++)
	{
		if(rocket_enemy[index].state == ROCKET_ON)
		{
			rocket_enemy[index].y+=10;

			//revisa si hay colisiones
			if(ship.state==SHIP_FIGHTING)
			{
				if(Collision_BOBS(&rocket_enemy[index],&ship))
				{
					for(k=0;k<20;k++)
					{
						Start_Particle_Explosion(PARTICLE_TYPE_FLICKER,
							PARTICLE_COLOR_BLUE,40,
							ship.x+ship.width/2,
							ship.y+ship.height/2,
							RAND_RANGE(-1,1),RAND_RANGE(-1,1),1);
					}
					rocket_enemy[index].state=ROCKET_OFF;
					vida=vida-10;
					if(vida < 0)
					{
						DSound_Play(sExp_ship);
						t_start=time(0);
						ship.state=SHIP_EXPLODING;
						vida=100;
						misiles=50;
						if(player-- <= 0)
							game_over=1;
					}
				}//collision
			}//If ship
		}//If rocket

		if(rocket_enemy[index].y>SCREEN_HEIGHT)
			rocket_enemy[index].state=ROCKET_OFF;
		}//For
}

/*----------------------------------------------------------
						Draw_Rocket_Enemy
/----------------------------------------------------------*/
void Draw_Rocket_Enemy()
{
	int index;

	for(index=0; index<MAX_ROCKET; index++)
	{
		if(rocket_enemy[index].state == ROCKET_ON)
		{
			Animate_BOB(&rocket_enemy[index]);
			Draw_BOB(&rocket_enemy[index],lpddsback);
		}
	}
}

/*----------------------------------------------------------
						Fire_Rocket_Enemy
/----------------------------------------------------------*/
int Fire_Rocket_Enemy(int x,int y,int count,int dx)
{
	int i,j,k;

	k=1;

	if(rocket_timer_enemy++ < 5)
		return 0;

	rocket_timer_enemy=0;

	x-=rocket_enemy[0].width/2;
	y-=rocket_enemy[0].height/2;

	for(i=0; i<MAX_ROCKET; i++)
	{
		if(rocket_enemy[i].state == ROCKET_OFF)
		{
			rocket_enemy[i].state=ROCKET_ON;
			Set_Pos_BOB(&rocket_enemy[i],x,y);
			x+=dx;
			k++;
			if(k > count)
				return 1;
		}
	}

	return 0;
}

/*----------------------------------------------------------
						Create_Asteroid
/----------------------------------------------------------*/
void Create_Asteroid()
{
	int index;

	Load_Bitmap_File(&bitmap8bit,"art\\b08.dat");
	Set_Palette(bitmap8bit.palette);

	Create_BOB(&a1,0,0,34,36,1,
				BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&a1,&bitmap8bit,0,
						495,5,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&a2,0,0,50,57,1,
				BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&a2,&bitmap8bit,0,
						491,56,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&a3,0,0,30,33,1,
				BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&a3,&bitmap8bit,0,
						555,60,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&a4,0,0,30,33,1,
				BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&a4,&bitmap8bit,0,
						488,134,BITMAP_EXTRACT_MODE_ABS); 

	Unload_Bitmap_File(&bitmap8bit);

	a1.state=ASTEROID_OFF;
	a2.state=ASTEROID_OFF;
	a3.state=ASTEROID_OFF;
	a4.state=ASTEROID_OFF;

	a1.type=ASTEROID_1;
	a2.type=ASTEROID_2;
	a3.type=ASTEROID_3;
	a4.type=ASTEROID_4;

	a1.health=3;
	a2.health=5;
	a3.health=3;
	a4.health=4;

	for(index=0; index<MAX_ASTEROID; index++)
	{
		if(index<3)
			memcpy(&asteroid[index],&a1,sizeof(BOB));
		else
		if(index>=3 && index<6)
			memcpy(&asteroid[index],&a2,sizeof(BOB));
		else
		if(index>=6 && index <9)
			memcpy(&asteroid[index],&a3,sizeof(BOB));
		else
		if(index>=9 && index<12)
			memcpy(&asteroid[index],&a4,sizeof(BOB));
	}
}

/*----------------------------------------------------------
						Delete_Asteroid
/----------------------------------------------------------*/
void Delete_Asteroid()
{
	int index;
	for(index=0; index<MAX_ASTEROID; index++)
		Destroy_BOB(&asteroid[index]);

	Destroy_BOB(&a1);
	Destroy_BOB(&a2);
	Destroy_BOB(&a3);
	Destroy_BOB(&a4);
}

/*----------------------------------------------------------
						Move_Asteroid
/----------------------------------------------------------*/
void Move_Asteroid()
{
	int index;
	int aux;

	if(game_paused)
		return;

	for(index=0; index<MAX_ASTEROID; index++)
	{
		if(asteroid[index].state == ASTEROID_ON)
		{
			asteroid[index].y+=asteroid[index].yv;
			
			if(Collision_BOBS(&asteroid[index],&ship) &&
				ship.state==SHIP_FIGHTING)
			{
				if(vida-- < 0)
				{
					DSound_Play(sExp_ship);
					t_start=time(0);
					ship.state=SHIP_EXPLODING;
					vida=100;
					misiles=50;
					if(player-- <= 0)
						game_over=1;
				}
			}

			if(asteroid[index].y>SCREEN_HEIGHT)
				asteroid[index].state=ASTEROID_OFF;
		}
	}		
}

/*----------------------------------------------------------
						Draw_Asteroid
/----------------------------------------------------------*/
void Draw_Asteroid()
{
	int index;

	for(index=0; index<MAX_ASTEROID; index++)
	{
		if(asteroid[index].state == ASTEROID_ON)
			Draw_BOB(&asteroid[index],lpddsback);
	}
}

/*----------------------------------------------------------
						New_Asteroid
/----------------------------------------------------------*/
int New_Asteroid()
{
	int index;
	int x,y;
	int random;

	if(game_paused || game_ready)
		return 0;

	//generamos un tipo de asteroide al azar  0..3
	random = rand()%4;
	
	x = rand()%SCREEN_WIDTH;
	y = -50;

	for(index=0; index<MAX_ASTEROID; index++)
	{
		if(asteroid[index].type == random)
		{
			switch(random){
			case 0:
				asteroid[index].health=3;
				break;
			case 1:
				asteroid[index].health=5;
				break;
			case 2:
				asteroid[index].health=3;
				break;
			case 3:
				asteroid[index].health=4;
				break;
			default:
				break;
			}

			if(asteroid[index].state == ASTEROID_OFF)
			{
				asteroid[index].state=ASTEROID_ON;
				asteroid[index].yv=rand()%4+2;
				Set_Pos_BOB(&asteroid[index],x,y);
				return 1;
			}
		}
	}

	return 0;
}

/*----------------------------------------------------------
						Create_Enemy
/----------------------------------------------------------*/
void Create_Enemy()
{
	int index;

	Load_Bitmap_File(&bitmap8bit,"art\\b06.dat");
	Set_Palette(bitmap8bit.palette);

	Create_BOB(&e1,0,0,42,33,3,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&e1,&bitmap8bit,0,
						18,23,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e1,&bitmap8bit,1,
						66,23,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e1,&bitmap8bit,2,
						114,23,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&e2,0,0,64,48,3,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&e2,&bitmap8bit,0,
						173,11,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e2,&bitmap8bit,1,
						247,11,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e2,&bitmap8bit,2,
						321,11,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&e3,0,0,44,36,3,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&e3,&bitmap8bit,0,
						401,24,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e3,&bitmap8bit,1,
						455,24,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e3,&bitmap8bit,2,
						509,24,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&e4,0,0,68,74,3,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&e4,&bitmap8bit,0,
						7,321,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e4,&bitmap8bit,1,
						87,321,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e4,&bitmap8bit,2,
						165,321,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&e5,0,0,58,46,3,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&e5,&bitmap8bit,0,
						10,61,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e5,&bitmap8bit,1,
						80,61,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e5,&bitmap8bit,2,
						150,61,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&e6,0,0,80,91,3,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&e6,&bitmap8bit,0,
						5,109,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e6,&bitmap8bit,1,
						93,109,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e6,&bitmap8bit,2,
						181,109,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&e7,0,0,76,60,3,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&e7,&bitmap8bit,0,
						3,200,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e7,&bitmap8bit,1,
						85,200,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e7,&bitmap8bit,2,
						167,200,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&e8,0,0,58,60,3,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&e8,&bitmap8bit,0,
						11,259,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e8,&bitmap8bit,1,
						89,259,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e8,&bitmap8bit,2,
						167,259,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&e9,0,0,92,64,3,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&e9,&bitmap8bit,0,
						263,73,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e9,&bitmap8bit,1,
						365,73,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e9,&bitmap8bit,2,
						467,73,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&e10,0,0,92,56,3,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&e10,&bitmap8bit,0,
						269,139,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e10,&bitmap8bit,1,
						365,139,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&e10,&bitmap8bit,2,
						461,139,BITMAP_EXTRACT_MODE_ABS); 

	Unload_Bitmap_File(&bitmap8bit);

	e1.state=ENEMY_OFF;
	e2.state=ENEMY_OFF;
	e3.state=ENEMY_OFF;
	e4.state=ENEMY_OFF;
	e5.state=ENEMY_OFF;
	e6.state=ENEMY_OFF;
	e7.state=ENEMY_OFF;
	e8.state=ENEMY_OFF;
	e9.state=ENEMY_OFF;
	e10.state=ENEMY_OFF;
	
	e1.type=ENEMY_1;
	e2.type=ENEMY_2;
	e3.type=ENEMY_3;
	e4.type=ENEMY_4;
	e5.type=ENEMY_5;
	e6.type=ENEMY_6;
	e7.type=ENEMY_7;
	e8.type=ENEMY_8;
	e9.type=ENEMY_9;
	e10.type=ENEMY_10;

	for(index=0; index<MAX_ENEMY; index++)
	{
		if(index<2)
			memcpy(&enemy[index],&e1,sizeof(BOB));
		else
		if(index>=2 && index<4)
			memcpy(&enemy[index],&e2,sizeof(BOB));
		else
		if(index>=4 && index <6)
			memcpy(&enemy[index],&e3,sizeof(BOB));
		else
		if(index>=6 && index<8)
			memcpy(&enemy[index],&e4,sizeof(BOB));
		else
		if(index>=8 && index<10)
			memcpy(&enemy[index],&e5,sizeof(BOB));
		else
		if(index>=10 && index<12)
			memcpy(&enemy[index],&e6,sizeof(BOB));
		else
		if(index>=12 && index<14)
			memcpy(&enemy[index],&e7,sizeof(BOB));
		else
		if(index>=14 && index<16)
			memcpy(&enemy[index],&e8,sizeof(BOB));
		else
		if(index>=16 && index<18)
			memcpy(&enemy[index],&e9,sizeof(BOB));
		else
		if(index>=18 && index<20)
			memcpy(&enemy[index],&e10,sizeof(BOB));

		enemy[index].health=7;
		Set_Anim_Speed_BOB(&enemy[index],2);
	}
}

/*----------------------------------------------------------
						Delete_Enemy
/----------------------------------------------------------*/
void Delete_Enemy()
{
	int index;
	for(index=0; index<MAX_ENEMY; index++)
		Destroy_BOB(&enemy[index]);

	Destroy_BOB(&e1);
	Destroy_BOB(&e2);
	Destroy_BOB(&e3);
	Destroy_BOB(&e4);
	Destroy_BOB(&e5);
	Destroy_BOB(&e6);
	Destroy_BOB(&e7);
	Destroy_BOB(&e8);
	Destroy_BOB(&e9);
	Destroy_BOB(&e10);
}

/*----------------------------------------------------------
						Move_Enemy
/----------------------------------------------------------*/
void Move_Enemy()
{
	int index;
	int aux;

	if(game_paused)
		return;

	for(index=0; index<MAX_ENEMY; index++)
	{
		if(enemy[index].state == ENEMY_ON)
		{
			enemy[index].y+=enemy[index].yv;
			if(rand()%10==5)
			{
				if(level==1)
					Fire_Plasma_Enemy(&enemy[index]);
				else
					Fire_Rocket_Enemy(enemy[index].width/2+enemy[index].x,
									  enemy[index].height+enemy[index].y,
									  1,0);
			}
		
			if(Collision_BOBS(&enemy[index],&ship) &&
				ship.state==SHIP_FIGHTING)
			{
				if(vida-- < 0)
				{
					DSound_Play(sExp_ship);
					t_start=time(0);
					ship.state=SHIP_EXPLODING;
					vida=100;
					misiles=50;
					if(player-- <= 0)
						game_over=1;
				}
			}

			if(enemy[index].y>SCREEN_HEIGHT)
				enemy[index].state=ENEMY_OFF;
		}
	}		
}

/*----------------------------------------------------------
						Draw_Enemy
/----------------------------------------------------------*/
void Draw_Enemy()
{
	int index;

	for(index=0; index<MAX_ENEMY; index++)
	{
		if(enemy[index].state == ENEMY_ON)
		{
			Animate_BOB(&enemy[index]);
			Draw_BOB(&enemy[index],lpddsback);
		}
	}
}

/*----------------------------------------------------------
						New_Enemy
/----------------------------------------------------------*/
int New_Enemy()
{
	int index;
	int x,y;
	int random;

	if(game_paused || game_ready)
		return 0;

	//generamos un tipo de enemigo al azar  0..9
	if(level==1)
		random = rand()%5;
	else
	if(level==2)
		random = rand()%5+5;
	
	x = rand()%SCREEN_WIDTH;
	y = -100;

	for(index=0; index<MAX_ENEMY; index++)
	{
		if(enemy[index].type == random)
		{
			if(enemy[index].state == ENEMY_OFF)
			{
				enemy[index].health=7;
				enemy[index].state=ENEMY_ON;
				enemy[index].yv=3;//rand()%4+2;
				Set_Pos_BOB(&enemy[index],x,y);
				return 1;
			}
		}
	}

	return 0;
}

void Create_Kaboom()
{
	int index;

	Load_Bitmap_File(&bitmap8bit,"art\\b02.dat");
	Set_Palette(bitmap8bit.palette);

	Create_BOB(&k1,0,0,50,50,5,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&k1,&bitmap8bit,0,
						274,281,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&k1,&bitmap8bit,1,
						324,278,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&k1,&bitmap8bit,2,
						371,274,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&k1,&bitmap8bit,3,
						424,271,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&k1,&bitmap8bit,4,
						488,271,BITMAP_EXTRACT_MODE_ABS); 

	Create_BOB(&k2,0,0,51,45,6,
				BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_FRAME,
				DDSCAPS_SYSTEMMEMORY);
	Load_Frame_BOB(&k2,&bitmap8bit,0,
						10,187,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&k2,&bitmap8bit,1,
						70,187,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&k2,&bitmap8bit,2,
						140,187,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&k2,&bitmap8bit,3,
						204,187,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&k2,&bitmap8bit,4,
						268,187,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&k2,&bitmap8bit,5,
						336,187,BITMAP_EXTRACT_MODE_ABS); 

	Unload_Bitmap_File(&bitmap8bit);

	Set_Anim_Speed_BOB(&k1,3);
	k1.state=KABOOM_OFF;
	k1.type=KABOOM_ASTEROID;
	Set_Anim_Speed_BOB(&k2,3);
	k2.state=KABOOM_OFF;
	k2.type=KABOOM_SHIP;
	
	for(index=0; index<MAX_KABOOM; index++)
	{
		//5 kabooms para rocas y 15 para naves
		if(index<5)
			memcpy(&kaboom[index],&k1,sizeof(BOB));
		else
		if(index>=5)
			memcpy(&kaboom[index],&k2,sizeof(BOB));
	}

}

void Delete_Kaboom()
{
	int index;

	for(index=0; index<MAX_KABOOM; index++)
		Destroy_BOB(&kaboom[index]);
}

void Draw_Kaboom()
{
	int index; 

	for(index=0; index<MAX_KABOOM; index++)
	{
		if(kaboom[index].state==KABOOM_ON)
		{
			Animate_BOB(&kaboom[index]);
			Draw_BOB(&kaboom[index],lpddsback);
			switch(kaboom[index].type){
			case KABOOM_ASTEROID:
				if(kaboom[index].curr_frame==4)
				{
					kaboom[index].curr_frame=0;
					kaboom[index].state=KABOOM_OFF;
				}
				break;
			case KABOOM_SHIP:
				if(kaboom[index].curr_frame==5)
				{
					kaboom[index].curr_frame=0;
					kaboom[index].state=KABOOM_OFF;
				}
				break;
			}//end switch
		}//end if KABOOM_ON
	}//end for
}

int New_Kaboom(int x,int y,int type)
{
	int index;

	for(index=0; index<MAX_KABOOM; index++)
	{
		if(kaboom[index].state==KABOOM_OFF &&
		   kaboom[index].type==type)
		{
			kaboom[index].state=KABOOM_ON;
			kaboom[index].x=x;
			kaboom[index].y=y;
			return 1;
		}
	}

	return 0;
}

/*----------------------------------------------------------
						 Game_Reset
/----------------------------------------------------------*/
int Game_Reset()
{
	int index;

	Set_Pos_BOB(&ship,SCREEN_WIDTH/2-ship.width/2,
					  SCREEN_HEIGHT-100-ship.height);
	ship.state=SHIP_FIGHTING;
	boss[0].state=BOSS_FIGHTING;
	boss[1].state=BOSS_FIGHTING;
	boss[2].state=BOSS_FIGHTING;
	vida=100;
	player=3;
	misiles=50;
	score=0;
	plasma_timer=0;
	plasma_timer_enemy=0;
	rocket_timer_enemy=0;
	rocket_flag=1;
	pause_flag=0;
	game_over=0;
	game_paused=0;
	game_ready=0;
	game_init=1;

	bonus.state=BOB_STATE_DEAD;
	up.state=BOB_STATE_DEAD;
	heart.state=BOB_STATE_DEAD;
	tnt.state=BOB_STATE_DEAD;
	missil_up.state=BOB_STATE_DEAD;

	Set_Animation_BOB(&ship,0);

	//eliminamos cualquier asteroide, plasma, misil o explosion
	//que se este pintando...
	for(index=0; index<MAX_ASTEROID; index++)
		asteroid[index].state=ASTEROID_OFF;
	for(index=0; index<MAX_PLASMA; index++)
	{
		plasma[index].state=PLASMA_OFF;
		plasma_enemy[index].state=PLASMA_OFF;
	}
	for(index=0; index<MAX_ROCKET; index++)
		rocket[index].state=ROCKET_OFF;
	for(index=0; index<MAX_KABOOM; index++)
		kaboom[index].state=KABOOM_OFF;

	DMusic_Play(mInit);

	return 1;
}

/*----------------------------------------------------------
						 Game_Init
/----------------------------------------------------------*/
int Game_Init(void *parms,  int num_parms)
{
	int index;

	DDraw_Init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
	DInput_Init();
	DInput_Init_Keyboard();
	Init_Stars();
	ShowCursor(FALSE);

	//Carga la imagen de inicio
	Load_Bitmap_File(&bitmap8bit,"art\\b01.dat");
	Set_Palette(bitmap8bit.palette);
	if (!Create_BOB(&inicio,0,0,640,480,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&inicio,&bitmap8bit,0,
						0,0,BITMAP_EXTRACT_MODE_ABS); 

	/******************************************************/
	//carga los jefes
	Load_Bitmap_File(&bitmap8bit,"art\\b08.dat");
	Set_Palette(bitmap8bit.palette);
	if (!Create_BOB(&boss[0],0,0,148,205,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&boss[0],&bitmap8bit,0,
						5,3,BITMAP_EXTRACT_MODE_ABS); 

	if (!Create_BOB(&boss[1],0,0,60,84,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&boss[1],&bitmap8bit,0,
						459,319,BITMAP_EXTRACT_MODE_ABS); 

	boss[0].health=1000;
	boss[0].state=BOSS_FIGHTING;
	boss[1].health=1000;
	boss[1].state=BOSS_FIGHTING;

	memcpy(&boss[2],&boss[1],sizeof(BOB));

	Set_Pos_BOB(&boss[0],SCREEN_WIDTH/2-boss[0].width/2,-boss[0].height);
	Set_Pos_BOB(&boss[1],SCREEN_WIDTH/4-boss[1].width/2,-boss[1].height);
	Set_Pos_BOB(&boss[2],3*SCREEN_WIDTH/4-boss[2].width/2,-boss[2].height);

	Unload_Bitmap_File(&bitmap8bit);

	/******************************************************/
	//Carga las naves
	Load_Bitmap_File(&bitmap8bit,"art\\b07.dat");
	Set_Palette(bitmap8bit.palette);

	if (!Create_BOB(&ship,0,0,96,75,6,
					BOB_ATTR_VISIBLE | BOB_ATTR_MULTI_ANIM,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&ship,&bitmap8bit,0,
						3,10,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&ship,&bitmap8bit,1,
						107,10,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&ship,&bitmap8bit,2,
						213,10,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&ship,&bitmap8bit,3,
						323,10,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&ship,&bitmap8bit,4,
						427,10,BITMAP_EXTRACT_MODE_ABS); 
	Load_Frame_BOB(&ship,&bitmap8bit,5,
						533,10,BITMAP_EXTRACT_MODE_ABS); 

	Unload_Bitmap_File(&bitmap8bit);

	Load_Animation_BOB(&ship,0,3,ship_normal);
	Load_Animation_BOB(&ship,1,6,ship_blink);

	Set_Anim_Speed_BOB(&ship,2);
	Set_Animation_BOB(&ship,1);

	Set_Pos_BOB(&ship,SCREEN_WIDTH/2-ship.width/2,
					  SCREEN_HEIGHT-100-ship.height);

	//la nave esta activa
	ship.state=SHIP_READY;

	/******************************************************/
	//Las imagenes Get Ready, Game Over, Pause y Completed
	Load_Bitmap_File(&bitmap8bit,"art\\b03.dat");

	if (!Create_BOB(&ready,0,0,273,52,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&ready,&bitmap8bit,0,
						4,10,BITMAP_EXTRACT_MODE_ABS); 

	if (!Create_BOB(&over,0,0,273,52,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&over,&bitmap8bit,0,
						4,86,BITMAP_EXTRACT_MODE_ABS); 

	if (!Create_BOB(&pause,0,0,150,52,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&pause,&bitmap8bit,0,
						4,150,BITMAP_EXTRACT_MODE_ABS); 

	if (!Create_BOB(&completed,0,0,505,55,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&completed,&bitmap8bit,0,
						4,216,BITMAP_EXTRACT_MODE_ABS); 

	Unload_Bitmap_File(&bitmap8bit);

	Set_Pos_BOB(&ready,180,200);
	Set_Pos_BOB(&over,180,200);
	Set_Pos_BOB(&pause,245,200);
	Set_Pos_BOB(&completed,67,200);

	/******************************************************/
	//El menu
	Load_Bitmap_File(&bitmap8bit,"art\\b04.dat");

	if (!Create_BOB(&menu,0,0,640,50,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&menu,&bitmap8bit,0,
						0,0,BITMAP_EXTRACT_MODE_ABS); 

	//Carga los mensajes
	if (!Create_BOB(&messages[0],0,0,152,14,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);
	Load_Frame_BOB(&messages[0],&bitmap8bit,0,
						10,182,BITMAP_EXTRACT_MODE_ABS); 

	if (!Create_BOB(&messages[1],0,0,250,14,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);
	Load_Frame_BOB(&messages[1],&bitmap8bit,0,
						10,197,BITMAP_EXTRACT_MODE_ABS); 

	if (!Create_BOB(&messages[2],0,0,271,14,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);
	Load_Frame_BOB(&messages[2],&bitmap8bit,0,
						10,213,BITMAP_EXTRACT_MODE_ABS); 

	if (!Create_BOB(&messages[3],0,0,265,14,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);
	Load_Frame_BOB(&messages[3],&bitmap8bit,0,
						10,227,BITMAP_EXTRACT_MODE_ABS); 

	if (!Create_BOB(&messages[4],0,0,209,14,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);
	Load_Frame_BOB(&messages[4],&bitmap8bit,0,
						10,242,BITMAP_EXTRACT_MODE_ABS); 

	if (!Create_BOB(&messages[5],0,0,151,14,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);
	Load_Frame_BOB(&messages[5],&bitmap8bit,0,
						10,257,BITMAP_EXTRACT_MODE_ABS); 

	if (!Create_BOB(&messages[6],0,0,121,14,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);
	Load_Frame_BOB(&messages[6],&bitmap8bit,0,
						10,273,BITMAP_EXTRACT_MODE_ABS); 

	if (!Create_BOB(&messages[7],0,0,233,14,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);
	Load_Frame_BOB(&messages[7],&bitmap8bit,0,
						10,289,BITMAP_EXTRACT_MODE_ABS); 

	/******************************************************/
	//carga la barra de vida y misiles
	if (!Create_BOB(&health_bar,0,0,7,13,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&health_bar,&bitmap8bit,0,
						13,66,BITMAP_EXTRACT_MODE_ABS); 
	//missiles bar
	if (!Create_BOB(&missiles_bar,0,0,7,13,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&missiles_bar,&bitmap8bit,0,
						13,82,BITMAP_EXTRACT_MODE_ABS); 

	//lives bar
	if (!Create_BOB(&lives_bar,0,0,50,28,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&lives_bar,&bitmap8bit,0,
						9,138,BITMAP_EXTRACT_MODE_ABS); 

	/******************************************************/
	//carga los diez digitos
	for(index=0; index<10; index++)
	{
		if (!Create_BOB(&numeros[index],0,0,12,11,1,
						BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
						DDSCAPS_SYSTEMMEMORY))
			return(0);

		Load_Frame_BOB(&numeros[index],&bitmap8bit,0,
						14+(18*index),112,BITMAP_EXTRACT_MODE_ABS); 
	}

	Unload_Bitmap_File(&bitmap8bit);

	Set_Pos_BOB(&menu,0,430);
	Set_Pos_BOB(&lives_bar,0,0);

	/******************************************************/
	//Carga los Pickups
	Load_Bitmap_File(&bitmap8bit,"art\\b05.dat");

	if (!Create_BOB(&bonus,0,0,18,23,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&bonus,&bitmap8bit,0,
						557,58,BITMAP_EXTRACT_MODE_ABS); 
	if (!Create_BOB(&up,0,0,38,16,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&up,&bitmap8bit,0,
						459,207,BITMAP_EXTRACT_MODE_ABS); 
	if (!Create_BOB(&heart,0,0,26,26,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&heart,&bitmap8bit,0,
						176,16,BITMAP_EXTRACT_MODE_ABS); 
	if (!Create_BOB(&tnt,0,0,15,28,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&tnt,&bitmap8bit,0,
						384,99,BITMAP_EXTRACT_MODE_ABS); 
	if (!Create_BOB(&missil_up,0,0,30,28,1,
					BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME,
					DDSCAPS_SYSTEMMEMORY))
		return(0);

	Load_Frame_BOB(&missil_up,&bitmap8bit,0,
						4,90,BITMAP_EXTRACT_MODE_ABS); 
	Unload_Bitmap_File(&bitmap8bit);

	for(index=0; index<8; index++)
		Set_Pos_BOB(&messages[index],menu.x+340,menu.y+15);

	bonus.state=BOB_STATE_DEAD;
	up.state=BOB_STATE_DEAD;
	heart.state=BOB_STATE_DEAD;
	tnt.state=BOB_STATE_DEAD;
	missil_up.state=BOB_STATE_DEAD;

	Create_Plasma();
	Create_Plasma_Enemy();
	Create_Rocket();
	Create_Rocket_Enemy();
	Create_Asteroid();
	Create_Enemy();
	Create_Kaboom();

	//definimos el area de recorte
	RECT screen_rect = {0,0,screen_width,screen_height}; 
	lpddclipper = DDraw_Attach_Clipper(lpddsback,1,&screen_rect);

	Init_Reset_Particles();

	//Inicializamos el Sonido y la Musica
	DSound_Init();
	sPlasma = DSound_Load_WAV("music\\s07.dat");
	sMissil = DSound_Load_WAV("music\\s05.dat");
	sPickUp = DSound_Load_WAV("music\\s06.dat");
	sExp_enemy = DSound_Load_WAV("music\\s03.dat");
	sExp_ship  = DSound_Load_WAV("music\\s02.dat");
	sExp_boss  = DSound_Load_WAV("music\\s04.dat");
	sCompleted = DSound_Load_WAV("music\\s01.dat");
	
	DMusic_Init();
	mLevel1 = DMusic_Load_MIDI("music\\m04.dat");
	mLevel2 = DMusic_Load_MIDI("music\\m05.dat");
	mBoss1  = DMusic_Load_MIDI("music\\m01.dat");
	mBoss2  = DMusic_Load_MIDI("music\\m02.dat");
	mInit   = DMusic_Load_MIDI("music\\m03.dat");

	DMusic_Play(mInit);

	return(1);
}//end Game_Init

/*----------------------------------------------------------
						 Game_Shutdown()
/----------------------------------------------------------*/
int Game_Shutdown(void *parms,  int num_parms)
{
	int i;

	DDraw_Shutdown();
	DInput_Release_Keyboard();
	DInput_Shutdown();

	Destroy_BOB(&inicio);
	Destroy_BOB(&boss[0]);
	Destroy_BOB(&boss[1]);
	Destroy_BOB(&boss[2]);
	Destroy_BOB(&ship);
	Destroy_BOB(&menu);
	Destroy_BOB(&health_bar);
	Destroy_BOB(&missiles_bar);
	Destroy_BOB(&lives_bar);
	Destroy_BOB(&ready);
	Destroy_BOB(&over);
	Destroy_BOB(&pause);
	Destroy_BOB(&completed);
	Destroy_BOB(&bonus);
	Destroy_BOB(&up);
	Destroy_BOB(&heart);
	Destroy_BOB(&tnt);
	Destroy_BOB(&missil_up);

	for(i=0; i<8; i++)
		Destroy_BOB(&messages[i]);

	Delete_Plasma();
	Delete_Plasma_Enemy();
	Delete_Rocket();
	Delete_Rocket_Enemy();
	Delete_Asteroid();
	Delete_Enemy();
	Delete_Kaboom();

	DSound_Stop_All_Sounds();
	DSound_Delete_All_Sounds();
	DSound_Shutdown();

	DMusic_Delete_All_MIDI();
	DMusic_Shutdown();

	return(1);
}//end Game_Shutdown

/*----------------------------------------------------------
						 Game_Main()
/----------------------------------------------------------*/
int Game_Main(void *parms, int num_parms)
{

	if(game_init)
	{
		DDraw_Fill_Surface(lpddsback, 0);
		Draw_BOB(&inicio,lpddsback);
		DDraw_Flip();
		DInput_Read_Keyboard();
		if(keyboard_state[DIK_RETURN])
		{
			game_init=0;
			game_ready=1;

			Save_Palette(bitmap8bit.palette);

			DDraw_Lock_Primary_Surface();
            back_lpitch = (int)(ddsd.lPitch);
            back_buffer = (UCHAR *)ddsd.lpSurface;
			Screen_Transitions(SCREEN_SWIPE_Y,back_buffer,back_lpitch);
			DDraw_Unlock_Primary_Surface();
			Set_Palette(bitmap8bit.palette);

			DDraw_Fill_Surface(lpddsback, 0);
			DMusic_Stop(mInit);
			//DMusic_Play_Loop(mLevel1,5,88,88);
			DMusic_Play(mLevel1);
			t_start=time(0);
		}
		else
		if(keyboard_state[DIK_ESCAPE])
			PostMessage(main_window_handle, WM_DESTROY,0,0);

		return 1;
	}

	DInput_Read_Keyboard();

	if(keyboard_state[DIK_P] & 0x80)
	{
		if(pause_flag)
		{
			pause_flag=0;
			if(game_paused)
				game_paused=0;
			else
				game_paused=1;
		}
	}
	else
		pause_flag=1;

	if(keyboard_state[DIK_ESCAPE])
		PostMessage(main_window_handle, WM_DESTROY,0,0);

	Start_Clock();
	DDraw_Fill_Surface(lpddsback, 0);

	if(keyboard_state[DIK_ESCAPE])
		PostMessage(main_window_handle, WM_DESTROY,0,0);

	Start_Clock();
	DDraw_Fill_Surface(lpddsback, 0);

	Move_Stars();
	Draw_Stars();

	if(keyboard_state[DIK_LCONTROL])
		Fire_Plasma();

	Move_Plasma();
	Draw_Plasma();

	if(keyboard_state[DIK_SPACE] & 0x80)
	{
		//Para disparar un nuevo misil la tecla debe de presionarse
		//nuevamente, no se puede hacer en "automatico"
		if(rocket_flag && misiles > 0)
		{
			rocket_flag=0;
			Fire_Rocket();
		}
	}
	else
		rocket_flag=1;

	Move_Rocket();
	Draw_Rocket();

	Move_Ship();
	Draw_Ship();

	if(phase == PHASE_I)
	{
		if(rand()%30 == 15)
		{
			New_Asteroid();
			if(count_asteroid++ > 50)
			{
				phase=PHASE_II;
				count_asteroid=0;
			}
		}

		Move_Asteroid();
		Draw_Asteroid();
	}
	else
	if(phase == PHASE_II)
	{
		if(rand()%100 == 50)
		{
			New_Enemy();
			if(count_enemy++ > 30)
			{
				phase=PHASE_III;
				count_enemy=0;
			}
		}

		Move_Asteroid();
		Draw_Asteroid();
		Move_Enemy();
		Draw_Enemy();
	}
	else
	if(phase == PHASE_III)
	{
		if(rand()%30 == 15)
			New_Asteroid();

		Move_Asteroid();
		Draw_Asteroid();

		if(rand()%100 == 50)
		{
			New_Enemy();
			if(count_enemy++ > 50)
			{
				phase=PHASE_IV;
				count_enemy=0;
			}
		}

		Move_Enemy();
		Draw_Enemy();
	}
	else
	if(phase==PHASE_IV)
	{
		if(start_phaseIV)
		{
			if(level==1)
			{
				DMusic_Stop(mLevel1);
				DMusic_Play_Loop(mBoss1,5,80,80);
			}
			else
			{
				DMusic_Stop(mLevel2);
				DMusic_Play_Loop(mBoss2,5,52,52);
			}
			start_phaseIV=0;
		}

		Move_Asteroid();
		Draw_Asteroid();
		Move_Enemy();
		Draw_Enemy();

		if(level==1)
		{
			Move_Boss(0);
			Draw_Boss(0);
		}
		else
		if(level==2)
		{
			Move_Boss(1);
			Draw_Boss(1);
			Move_Boss(2);
			Draw_Boss(2);
		}
	}

	Move_Plasma_Enemy();
	Draw_Plasma_Enemy();
	Move_Rocket_Enemy();
	Draw_Rocket_Enemy();

	Draw_Kaboom();

	Move_Pickup();
	Draw_Pickup();

	Draw_BOB(&menu,lpddsback);
	Score();
	Health_Bar();
	Missiles_Bar();
	Lives_Bar();

	if(game_paused)
		Draw_BOB(&pause,lpddsback);
	if(game_ready)
	{
		Draw_BOB(&ready,lpddsback);
		t_finish=time(0);
		t_result=difftime(t_finish,t_start);
		if(t_result >= 3)
		{
			game_ready=0;
			ship.state=SHIP_FIGHTING;
			Set_Animation_BOB(&ship,0);
		}
	}

	if(level_completed==1)
	{
		Draw_BOB(&completed,lpddsback);
		DDraw_Flip();
		Sleep(12000);
		if(level==1)
		{
			Save_Palette(bitmap8bit.palette);
			DDraw_Lock_Primary_Surface();
            back_lpitch = (int)(ddsd.lPitch);
            back_buffer = (UCHAR *)ddsd.lpSurface;
			Screen_Transitions(SCREEN_DISOLVE,back_buffer,back_lpitch);
			DDraw_Unlock_Primary_Surface();
			Set_Palette(bitmap8bit.palette);

			level=2;
			start_phaseIV=1;
			phase=PHASE_I;
			game_ready=1;
			vida=100;
			misiles=50;
			Set_Animation_BOB(&ship,1);
			game_ready=1;
			Set_Pos_BOB(&ship,SCREEN_WIDTH/2-ship.width/2,
							  SCREEN_HEIGHT-100-ship.height);
			DMusic_Play_Loop(mLevel2,6,105,105);
			t_start=time(0);
		}
		else
		{
			game_over=1;
		}

    	boss[0].state=BOSS_FIGHTING;
		boss[0].health=1000;
		ship.state=SHIP_READY;
		level_completed=0;
	}

	if(game_over && ship.state == SHIP_READY)
	{
		DMusic_Stop(mLevel1);
		DMusic_Stop(mBoss1);
		Save_Palette(bitmap8bit.palette);
		Screen_Transitions(SCREEN_WHITENESS,NULL,0);
		Set_Palette(bitmap8bit.palette);
		DDraw_Fill_Surface(lpddsback, 0);
		Draw_BOB(&over,lpddsback);
		DDraw_Flip();
		Sleep(5000);
		Save_Palette(bitmap8bit.palette);
		Screen_Transitions(SCREEN_DARKNESS,NULL,0);
		Set_Palette(bitmap8bit.palette);
		Game_Reset();
		DDraw_Fill_Surface(lpddsback, 0);
	}

	Draw_Message();

	Move_Particles();
	Draw_Particles();

	DDraw_Flip();
	Wait_Clock(30);

	return(1);
}//end Game_Main
