/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

/*
 *@Note
   独立词语音识别例程：
 
*/

#include "debug.h"
#include "VoiceRcg.h"
#include "Get_Data.h"
#include "lcd.h"


#define Valid_Thl 245

vr_dtg_para  dtg_para;
vr_act_seg   active_segment;
__attribute__((aligned(4))) vr_chara_para  chara_para;
volatile uint8_t g_data_ready=0;

//一首歌3个模板信息，三首歌一共9个模板
const char  *key_words[]={"song1_part_I" , "song1_part_II" , "song1_part_III",
                          "song2_part_I" , "song2_part_II" , "song2_part_III",
                          "song3_part_I" , "song3_part_II" , "song3_part_III"};
//歌曲名字
const char  *song_name[]={"<HAPPY BIRTHDAY>",
                          "<SEE YOU AGAIN>",
                          "<我和我的祖国>"};

#if USE_ES8388
//ES8388 have two channels,we only need one of them
__attribute__((aligned(4))) uint16_t V_Data[SampleDataLen*2];
#else
__attribute__((aligned(4))) uint16_t V_Data[SampleDataLen];
#endif


uint8_t SW1_DOWN;
uint8_t SW2_DOWN;
static uint8_t song_name_index=-1;


void voice_record(void);
uint8_t save_chara_para_mdl(uint16_t *v_dat, uint32_t addr);
uint8_t parameters_practice(void);
void voice_recongition(void);
void key_init(void);
void sw1_scan(void);
void sw2_scan(void);


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_Init();
	USART_Printf_Init(115200);
	voice_init();
	key_init();
	lcd_init();
	lcd_fill(0, 0, 239, 239, BLACK);
	while(1)
	{
	    lcd_set_color(BLACK, WHITE);
	    lcd_show_string(20, 10, 24, "SONG RECOGNITION");
	    lcd_show_string(20, 32, 16, "SW1 TO RECOGNITION");
	    lcd_show_string(20, 50, 16, "JOY_DOWN TO PRACTICE");
	    sw1_scan();
	    sw2_scan();
	    if(SW1_DOWN)//sw1按键按下
	    {
	        lcd_show_string(40, 175, 24, "recognizing...");
	        lcd_show_string(30, 200, 16, "sw2 to quit recognition");
	        lcd_fill(0, 80, 239, 160, BLACK);
	        voice_recongition();//进行识别
	        delay_ms(1000);
	        if(song_name_index==0||song_name_index==1||song_name_index==2)//标志着识别成功
	        {
	            SW1_DOWN=0;
	        }
	        if(SW2_DOWN)//sw2按键可以终止识别
	        {
	            SW1_DOWN=0;
	        }
	    }
	    else
	    {
	        lcd_fill(0, 170, 239, 239, BLACK);
	        delay_ms(1000);
        }
	    if(song_name_index==0)
        {
	        lcd_set_color(BLACK, RED);

	        lcd_show_string(20, 100, 24, "result:");
            lcd_show_string(20, 120, 24, song_name[song_name_index]);

        }
        else if(song_name_index==1)
        {
            lcd_set_color(BLACK, RED);

            lcd_show_string(20, 100, 24, "result:");
            lcd_show_string(20, 120, 24, song_name[song_name_index]);

        }
        else if(song_name_index==2)
        {
            lcd_set_color(BLACK, RED);
            lcd_show_string(20, 100, 24, "result:");
            lcd_show_string(20, 120, 24, "<");
            lcd_show_chinese(40, 120, 0, BLACK, RED, 24);
            lcd_show_chinese(65, 120, 1, BLACK, RED, 24);
            lcd_show_chinese(90, 120, 0, BLACK, RED, 24);
            lcd_show_chinese(115, 120, 2, BLACK, RED, 24);
            lcd_show_chinese(140, 120, 3, BLACK, RED, 24);
            lcd_show_chinese(165, 120, 4, BLACK, RED, 24);
            lcd_set_color(BLACK, RED);
            lcd_show_string(195, 120, 24, ">");
        }
	    //joy_down按下
	    if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2))
        {
            Delay_Ms(20);//消抖，防误触
            if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2))
            {
                printf("will practice parameters...\r\n");
                parameters_practice();//进入模板训练
            }
        }
	}
}


/*********************************************************************
 * @fn      voice_record
 *
 * @brief   record voice data
 *
 * @return  none
 */
void voice_record(void)
{
#if USE_ES8388
    I2S_Cmd(SPI2,ENABLE);
    Delay_Ms(SampleDelay); //ES8388 data is empty
    DMA_Rx_Init( DMA1_Channel4, (u32)&SPI2->DATAR, (u32)V_Data, (SampleDataLen*2) );
    DMA_Cmd( DMA1_Channel4, ENABLE );
    Delay_Ms(SampleNoiseTime); //sample noise
    printf("speaking...\r\n");
#else
    TIM_Cmd(TIM1, ENABLE);
    Delay_Ms(SampleNoiseTime);
    printf("speaking...\r\n");
#endif
}

/*********************************************************************
 * @fn      save_chara_para_mdl
 *
 * @brief   save mfcc characteristic parameters
 *
 * @param   v_dat-source data buffer
 *          addr-start address of parameter
 *
 * @return  error code
 */
uint8_t save_chara_para_mdl(uint16_t *v_dat, uint32_t addr)
{
    environment_noise(v_dat,SampleNoiseLen,&dtg_para);
    active_segment_detect(v_dat, SampleDataLen, &active_segment, &dtg_para);
    if(active_segment.act_end== NULL ) return ERR_DETEC;
    calc_mfcc_chara_para(&active_segment,&chara_para,&dtg_para);
    if(chara_para.seg_cnt==0) return ERR_PARA;
    return save_mfcc_chara_para(&chara_para, addr);
}



/*********************************************************************
 * @fn      parameters_practice
 *
 * @brief   practice some key words and save characteristic parameters
 *
 * @return  error code
 */
uint8_t parameters_practice(void)
{
    uint8_t i=0;
    uint8_t retry_count=0;
    uint32_t addr=0;
    while(1)
    {
        printf("\r\npractice start...\r\n\r\n");
        //遍历每个模板，进行模板训练
        for (i = 0; i < kw_num; i++)
        {
            lcd_show_string(40, 215, 24, "practicing...");
            retry:
            printf("please speak:%s \r\n",key_words[i]);
            voice_record();//es8388录入
            //addr为即将录入的模板的flash地址
            addr=charamls_start_addr+(i/charaml_per_kw)*size_per_kw+(i%charaml_per_kw)*size_per_chara;
            printf("addr:%08x\r\n",addr);
            while(!g_data_ready) //进行VAD语音激活检测
            {

            }

            if(g_data_ready)//检测到即开始训练模板
            {

                g_data_ready=0;
                if(save_chara_para_mdl(V_Data,addr)==ERR_OK)//如果存放模板成功
                {
                    printf("\r\n %s practice success\r\n",key_words[i]);
                    retry_count++;
                }
                else
                {
                    printf("%s practice fail !!!\r\n",key_words[i]);
                    if(retry_count!=9)//录入失败会反复录入，知道录入成功
                    goto retry;
                    else break;
                }
            }
            Delay_Ms(1000);
            Delay_Ms(1000);
        }
        printf("practice end!!!\r\n");
        lcd_fill(40, 215, 239, 239, BLACK);
        lcd_show_string(40, 215, 24, "practice done!");
        break;
    }
    return 0;
}


/*********************************************************************
 * @fn      voice_recongition
 *
 * @brief   sample data and recognize
 *
 * @return  none
 */
void voice_recongition(void)
{
    uint8_t     i=0,keyword_min=10;
    uint32_t    chara_para_addr=0;
    uint32_t    match_dis=0,cur_dis=0,min_dis=0;
    vr_chara_para *chara_para_mdl;

    voice_record();
    while(!g_data_ready); //wait for data ready
    if(g_data_ready)
    {
        g_data_ready=0;
        environment_noise(V_Data,SampleNoiseLen,&dtg_para);
        active_segment_detect(V_Data, SampleDataLen, &active_segment, &dtg_para);
        if(active_segment.act_end== NULL )
        {
            match_dis=ERR_MATCH;
        }
        calc_mfcc_chara_para(&active_segment,&chara_para,&dtg_para);
        if(chara_para.seg_cnt==0)
        {
            match_dis=ERR_MATCH;
        }
        min_dis=ERR_MATCH;
        for(chara_para_addr=charamls_start_addr; chara_para_addr<charamls_end_addr; chara_para_addr+=size_per_chara)
        {
            chara_para_mdl=(vr_chara_para*)chara_para_addr;
            cur_dis=((chara_para_mdl->magic)==MAGIC_DATA)?calc_chara_para_match_dis(&chara_para,chara_para_mdl):ERR_MATCH;
            if(cur_dis<min_dis)
            {
               min_dis=cur_dis;
               keyword_min=i;
            }
            i++;
        }
        keyword_min = keyword_min/charaml_per_kw;
        if(min_dis>Valid_Thl) match_dis=ERR_MATCH;  //unreliable when it is more than Valid_Thl

        if(match_dis==ERR_MATCH)
        {
            match_dis=0;
            printf("recg fail!!!\r\n");
            lcd_set_color(BLACK, RED);
            lcd_show_string(50, 215, 24, "recg fail!");
        }
        else
        {
           printf("recg end,your speaking is: %s\r\n",key_words[keyword_min]);
           if(keyword_min<=2)
           {
               song_name_index=0;
           }
           else if (keyword_min>=3&&keyword_min<=5)
           {
               song_name_index=1;
           }
           else
           {
               song_name_index=2;
           }
        }
    }
}


/*********************************************************************
 * @fn      key_init
 *
 * @brief   key initial
 *
 * @return  none
 */
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE , ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;//PE4-SW1   PE5-SW2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOE, &GPIO_InitStructure );
}

void sw1_scan(void)
{
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4))
    {
        delay_ms(20);
    }
    else
    {
        delay_ms(20);
        SW1_DOWN=1;
        song_name_index=10;
    }
}

void sw2_scan(void)
{
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5))
    {
        delay_ms(20);
        SW2_DOWN=0;
    }
    else
    {
        delay_ms(20);
        SW2_DOWN=1;
    }
}
