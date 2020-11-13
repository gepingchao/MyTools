#include "bee.h"
unsigned short G_Music_Speed = 100;
unsigned short G_Power = 10;

unsigned short G_Music_Base_Beat = 100;
#define music_magic_num	111

const unsigned short Tone[22] = {3817,3401,3030,2865,2551,2273,2024,1912,1703,1517,1433,1275,1336,1012,956,851,758,715,637,568,506,music_magic_num};//0ÐÝÖ¹·û

#define High(x)	(x+13)
#define Mid(x)	(x+6)
#define Low(x)	(x-1)
#define Music_Beat_Stop	(21)

#define M_S(X,Y)	{X,Y}

unsigned char music_1[]={	5+6,5,6,8,7,6,5,6,13,13,//Òôµ÷
								5,5,6,8,7,6,5,3,13,13,
								2,2,3,5,3,5,6,3,2,1,
								6,6,5,6,5,3,6,5,13,13,
								5,5,6,8,7,6,5,6,13,13,
								5,5,6,8,7,6,5,3,13,13,
								2,2,3,5,3,5,6,3,2,1,
								6,6,5,6,5,3,6,1,   
								13,8,9,10,10,9,8,10,9,8,6,
								13,6,8,9,9,8,6,9,8,6,5,
								13,2,3,5,5,3,5,5,6,8,7,6,
								6,10,9,9,8,6,5,6,8

};     

unsigned char time_1[] = {	2,4,2,2,2,2,2,8,4, 4, //Ê±¼ä
								2,4,2,2,2,2,2,8,4, 4,
								2,4,2,4,2,2,4,2,2,8,
								2,4,2,2,2,2,2,8,4 ,4,
								2,4,2,2,2,2,2,8,4, 4,
								2,4,2,2,2,2,2,8,4, 4,
								2,4,2,4,2,2,4,2,2,8,
								2,4,2,2,2,2,2,8,
								4, 2,2,2, 4, 2,2,2, 2,2,8,
								4, 2,2,2,4,2,2,2,2,2,8,
								4, 2,2,2,4,2,2,5,2,6,2,4,
								2,2 ,2,4,2,4,2,2,12

}; 

unsigned char music_2[]={5,5,6,5,8,7,7,
	5,5,6,5,9,8,
	5,5,12,10,8,
	7,6,6,
	11,11,10,8,9,8};

unsigned char time_2[]={2,2,4,4,4,4,4,
	2,2,4,4,4,4,
	4,4,4,4,4,
	8,8,8,
	2,2,4,4,4,8,8};
unsigned char music_3[] = 
{
Mid(5),Mid(5),Mid(6),Mid(5),High(1),Mid(7),Music_Beat_Stop,
Mid(5),Mid(5),Mid(6),Mid(5),High(2),High(1),
Mid(5),Mid(5),High(5),High(3),High(1),Mid(7),Mid(6),Music_Beat_Stop,
High(4),High(4),High(3),High(1),High(2),High(1),Mid(5),Mid(5),High(1),Music_Beat_Stop,0
};

unsigned char time_3[] =
{
2,2,4,4,4,4,4,2,2,4,4,4,8,2,2,4,4,4,4,12,4,2,2,4,4,4,8,2,2,8,4
};

unsigned char music_4[] = 
{
Mid(1),Mid(3),Mid(3),Mid(3),Mid(2),Mid(1),Mid(2),
Mid(1),Mid(2),Mid(3),Mid(3),Mid(2),Mid(1),Mid(1),Music_Beat_Stop,
Mid(5),Mid(6),Mid(1),Mid(1),Mid(1),Mid(1),Mid(1),Mid(1),Mid(2),Mid(2),
Low(5),Low(6),Mid(1),Mid(1),Mid(1),Mid(1),Mid(1),Mid(1),Mid(2),Mid(2),
Mid(1),Mid(3),Mid(3),Mid(3),Mid(5),Mid(6),Mid(2),
Mid(1),Mid(1),Mid(2),Mid(3),Mid(5),Music_Beat_Stop,
Low(5),Low(5),Low(6),Mid(1),Mid(1),Mid(1),Mid(3),Low(2),Low(1),Music_Beat_Stop,Mid(2),Mid(1),
};

unsigned char time_4[]=
{
4,4,2,4,2,2,3,
2,2,2,2,2,2,4,4,
2,4,2,2,3,2,2,2,2,2,4,
4,4,4,4,2,2,4,
2,2,2,2,4,4,4,2,
2,2,2,1,1,2,2,4,4,2,2,8,4
};

unsigned char music_5[][2] = {
M_S(Mid(1),2),M_S(Mid(2),1),M_S(Mid(3),1),M_S(Mid(3),2),M_S(Mid(3),2),M_S(Mid(2),2),M_S(Mid(1),1),M_S(Mid(2),1),M_S(Mid(2),3),
M_S(Mid(1),1),M_S(Mid(2),1),M_S(Mid(3),2),M_S(Mid(3),2),M_S(Mid(2),2),M_S(Mid(1),2),M_S(Mid(1),3),
M_S(Mid(5),1),M_S(Low(6),1),M_S(Low(5),4),M_S(Low(5),1),M_S(Mid(5),1),M_S(Mid(6),1),M_S(Mid(1),1),

};

//53532125µÍ2234233453 53532125µÍ


unsigned char Alarm_music_2[] = {
Mid(5),Mid(3),Mid(5),Mid(3),Mid(2),Mid(1),Mid(2),Low(5),
Mid(2),Mid(2),Mid(3),Mid(4),Mid(2),Mid(2),Mid(3),Mid(4),Mid(5),Mid(3),
Mid(5),Mid(3),Mid(5),Mid(3),Mid(2),Mid(1),Mid(2),Low(5),0,

};
unsigned char Alarm_Beat_2[] = {
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
};

unsigned char Alarm_music_1[] = {Mid(5),Low(3),High(5),Mid(3),0,};
unsigned char Alarm_Beat_1[] = {2,1,2,1};

S_Music G_Music;

void ChangeBee(unsigned short pulse,unsigned short period)//power , tone
{
	TIM_OC_InitTypeDef sConfigOC = {0};

	htim16.Instance = TIM16;
	htim16.Init.Prescaler = 71;
	htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim16.Init.Period = period;
	htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim16.Init.RepetitionCounter = 0;
	htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim16);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = pulse;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&htim16, &sConfigOC, TIM_CHANNEL_1);
	if(music_magic_num != period)
		{
			HAL_TIMEx_PWMN_Start(&htim16,TIM_CHANNEL_1);
		}
}

void PlayNextBeat(void)
{
	G_Music.Music_Step++;
	G_Music_Speed = *(G_Music.Beat+ G_Music.Music_Step) * G_Music_Base_Beat;
	ChangeBee(G_Power,Tone[*(G_Music.Music + G_Music.Music_Step)]);
	HAL_TIM_Base_Start_IT(&BEEP_COUNT_TIMER);
}

void play_next_beat_test(void)
{	
	G_Music.Music_Step++;
	ChangeBee(G_Power,Tone[music_5[G_Music.Music_Step][0]]);
	G_Music_Speed = music_5[G_Music.Music_Step][1] * G_Music_Base_Beat;
}

void StartPlay(P_S_Music music)
{
	G_Music.Beat = music->Beat;
	G_Music.Music= music->Music;
	G_Music.Time_Step= 0;
	G_Music.Music_Step= 0;
	
	G_Music.Music_Length = strlen(G_Music.Music);
	//G_Music.Music_Length = 23;
	G_Music_Speed = *(G_Music.Beat+ G_Music.Music_Step) * G_Music_Base_Beat;
	
	ChangeBee(G_Power,Tone[*(G_Music.Music + G_Music.Music_Step)]);
	HAL_TIM_Base_Start_IT(&BEEP_COUNT_TIMER);
}

void BeeCountTimeCallBack(void)
{
	G_Music.Time_Step++;
	if((G_Music.Time_Step) > G_Music_Speed)
		{
			G_Music.Time_Step = 0;
			G_Music.Music_Length --;
			PlayNextBeat();
			//play_next_beat_test();
		}
	if(0 == G_Music.Music_Length)
		{
			HAL_TIM_Base_Stop_IT(&BEEP_COUNT_TIMER);
			HAL_TIMEx_PWMN_Stop(&htim16,TIM_CHANNEL_1);
		}
}


void test_music(void)
{
	S_Music music_test = {0};
	music_test.Music = music_3;
	music_test.Beat= time_3;
	StartPlay(&music_test);
}

