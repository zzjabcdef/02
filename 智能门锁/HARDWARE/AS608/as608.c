#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "usart.h"
#include "as608.h"
#include "oled.h"
#include "key.h"

u32 AS608Addr = 0XFFFFFFFF; //Ĭ��
char str2[6] = {0};
extern unsigned char BMP1[];
extern __IO u8 PwdAlarmLimit;            //�����ж�����������ٴ���ʱ����

extern void DOOR_Open(int i);

__IO u32 box[100]={0};
//���ڷ���һ���ֽ�
static void MYUSART_SendData(u8 data)
{
  while((USART2->SR & 0X40) == 0);
  USART2->DR = data;
}
//���Ͱ�ͷ
static void SendHead(void)
{
  MYUSART_SendData(0xEF);
  MYUSART_SendData(0x01);
}
//���͵�ַ
static void SendAddr(void)
{
  MYUSART_SendData(AS608Addr >> 24);
  MYUSART_SendData(AS608Addr >> 16);
  MYUSART_SendData(AS608Addr >> 8);
  MYUSART_SendData(AS608Addr);
}
//���Ͱ���ʶ,
static void SendFlag(u8 flag)
{
  MYUSART_SendData(flag);
}
//���Ͱ�����
static void SendLength(int length)
{
  MYUSART_SendData(length >> 8);
  MYUSART_SendData(length);
}
//����ָ����
static void Sendcmd(u8 cmd)
{
  MYUSART_SendData(cmd);
}
//����У���
static void SendCheck(u16 check)
{
  MYUSART_SendData(check >> 8);
  MYUSART_SendData(check);
}
//�ж��жϽ��յ�������û��Ӧ���
//waittimeΪ�ȴ��жϽ������ݵ�ʱ�䣨��λ1ms��
//����ֵ�����ݰ��׵�ַ
static u8 *JudgeStr(u16 waittime)
{
  char *data;
  u8 str[8];
  str[0] = 0xef;
  str[1] = 0x01;
  str[2] = AS608Addr >> 24;
  str[3] = AS608Addr >> 16;
  str[4] = AS608Addr >> 8;
  str[5] = AS608Addr;
  str[6] = 0x07;
  str[7] = '\0';
  USART2_RX_STA = 0;
  while(--waittime)
  {
    delay_ms(1);
    if(USART2_RX_STA & 0X8000) //���յ�һ������
    {
      USART2_RX_STA = 0;
      data = strstr((const char*)USART2_RX_BUF, (const char*)str);
      if(data)
        return (u8*)data;
    }
  }
  return 0;
}
//¼��ͼ�� PS_GetImage
//����:̽����ָ��̽�⵽��¼��ָ��ͼ�����ImageBuffer��
//ģ�鷵��ȷ����
u8 PS_GetImage(void)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x03);
  Sendcmd(0x01);
  temp =  0x01 + 0x03 + 0x01;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//�������� PS_GenChar
//����:��ImageBuffer�е�ԭʼͼ������ָ�������ļ�����CharBuffer1��CharBuffer2
//����:BufferID --> charBuffer1:0x01	charBuffer1:0x02
//ģ�鷵��ȷ����
u8 PS_GenChar(u8 BufferID)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x04);
  Sendcmd(0x02);
  MYUSART_SendData(BufferID);
  temp = 0x01 + 0x04 + 0x02 + BufferID;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//��ȷ�ȶ���öָ������ PS_Match
//����:��ȷ�ȶ�CharBuffer1 ��CharBuffer2 �е������ļ�
//ģ�鷵��ȷ����
u8 PS_Match(void)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x03);
  Sendcmd(0x03);
  temp = 0x01 + 0x03 + 0x03;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//����ָ�� PS_Search
//����:��CharBuffer1��CharBuffer2�е������ļ����������򲿷�ָ�ƿ�.�����������򷵻�ҳ�롣
//����:  BufferID @ref CharBuffer1	CharBuffer2
//˵��:  ģ�鷵��ȷ���֣�ҳ�루����ָ��ģ�壩
u8 PS_Search(u8 BufferID, u16 StartPage, u16 PageNum, SearchResult *p)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x08);
  Sendcmd(0x04);
  MYUSART_SendData(BufferID);
  MYUSART_SendData(StartPage >> 8);
  MYUSART_SendData(StartPage);
  MYUSART_SendData(PageNum >> 8);
  MYUSART_SendData(PageNum);
  temp = 0x01 + 0x08 + 0x04 + BufferID
         + (StartPage >> 8) + (u8)StartPage
         + (PageNum >> 8) + (u8)PageNum;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    p->pageID   = (data[10] << 8) + data[11];
    p->mathscore = (data[12] << 8) + data[13];
  }
  else
    ensure = 0xff;
  return ensure;
}
//�ϲ�����������ģ�壩PS_RegModel
//����:��CharBuffer1��CharBuffer2�е������ļ��ϲ����� ģ��,�������CharBuffer1��CharBuffer2
//˵��:  ģ�鷵��ȷ����
u8 PS_RegModel(void)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x03);
  Sendcmd(0x05);
  temp = 0x01 + 0x03 + 0x05;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//����ģ�� PS_StoreChar
//����:�� CharBuffer1 �� CharBuffer2 �е�ģ���ļ��浽 PageID ��flash���ݿ�λ�á�
//����:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID��ָ�ƿ�λ�úţ�
//˵��:  ģ�鷵��ȷ����
u8 PS_StoreChar(u8 BufferID, u16 PageID)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x06);
  Sendcmd(0x06);
  MYUSART_SendData(BufferID);
  MYUSART_SendData(PageID >> 8);
  MYUSART_SendData(PageID);
  temp = 0x01 + 0x06 + 0x06 + BufferID
         + (PageID >> 8) + (u8)PageID;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//ɾ��ģ�� PS_DeletChar
//����:  ɾ��flash���ݿ���ָ��ID�ſ�ʼ��N��ָ��ģ��
//����:  PageID(ָ�ƿ�ģ���)��Nɾ����ģ�������
//˵��:  ģ�鷵��ȷ����
u8 PS_DeletChar(u16 PageID, u16 N)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x07);
  Sendcmd(0x0C);
  MYUSART_SendData(PageID >> 8);
  MYUSART_SendData(PageID);
  MYUSART_SendData(N >> 8);
  MYUSART_SendData(N);
  temp = 0x01 + 0x07 + 0x0C
         + (PageID >> 8) + (u8)PageID
         + (N >> 8) + (u8)N;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//���ָ�ƿ� PS_Empty
//����:  ɾ��flash���ݿ�������ָ��ģ��
//����:  ��
//˵��:  ģ�鷵��ȷ����
u8 PS_Empty(void)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x03);
  Sendcmd(0x0D);
  temp = 0x01 + 0x03 + 0x0D;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//дϵͳ�Ĵ��� PS_WriteReg
//����:  дģ��Ĵ���
//����:  �Ĵ������RegNum:4\5\6
//˵��:  ģ�鷵��ȷ����
u8 PS_WriteReg(u8 RegNum, u8 DATA)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x05);
  Sendcmd(0x0E);
  MYUSART_SendData(RegNum);
  MYUSART_SendData(DATA);
  temp = RegNum + DATA + 0x01 + 0x05 + 0x0E;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  if(ensure == 0)
    printf("\r\nSet the parameters successfully!");
  else
    EnsureMessage(ensure);
  return ensure;
}
//��ϵͳ�������� PS_ReadSysPara
//����:  ��ȡģ��Ļ��������������ʣ�����С��)
//����:  ��
//˵��:  ģ�鷵��ȷ���� + ����������16bytes��
u8 PS_ReadSysPara(SysPara *p)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x03);
  Sendcmd(0x0F);
  temp = 0x01 + 0x03 + 0x0F;
  SendCheck(temp);
  data = JudgeStr(1000);
  if(data)
  {
    ensure = data[9];
    p->PS_max = (data[14] << 8) + data[15];
    p->PS_level = data[17];
    p->PS_addr = (data[18] << 24) + (data[19] << 16) + (data[20] << 8) + data[21];
    p->PS_size = data[23];
    p->PS_N = data[25];
  }
  else
     ensure = 0xff;
  if(ensure == 0x00)
  {
    printf("\r\nMaximum fingerprint capacity of module=%d", p->PS_max);
    printf("\r\nContrast grade=%d", p->PS_level);
    printf("\r\nAddress=%x", p->PS_addr);
    printf("\r\nBaud rate=%d", p->PS_N * 9600);
  }
  else
    EnsureMessage(ensure);
  return ensure;
}
//����ģ���ַ PS_SetAddr
//����:  ����ģ���ַ
//����:  PS_addr
//˵��:  ģ�鷵��ȷ����
u8 PS_SetAddr(u32 PS_addr)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x07);
  Sendcmd(0x15);
  MYUSART_SendData(PS_addr >> 24);
  MYUSART_SendData(PS_addr >> 16);
  MYUSART_SendData(PS_addr >> 8);
  MYUSART_SendData(PS_addr);
  temp = 0x01 + 0x07 + 0x15
         + (u8)(PS_addr >> 24) + (u8)(PS_addr >> 16)
         + (u8)(PS_addr >> 8) + (u8)PS_addr;
  SendCheck(temp);
  AS608Addr = PS_addr; //������ָ�������ַ
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  AS608Addr = PS_addr;
  if(ensure == 0x00)
    printf("\r\nAddress set successfully!");
  else
    EnsureMessage(ensure);
  return ensure;
}
//���ܣ� ģ���ڲ�Ϊ�û�������256bytes��FLASH�ռ����ڴ��û����±�,
//	�ü��±��߼��ϱ��ֳ� 16 ��ҳ��
//����:  NotePageNum(0~15),Byte32(Ҫд�����ݣ�32���ֽ�)
//˵��:  ģ�鷵��ȷ����
u8 PS_WriteNotepad(u8 NotePageNum, u8 *Byte32)
{
  u16 temp;
  u8  ensure, i;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(36);
  Sendcmd(0x18);
  MYUSART_SendData(NotePageNum);
  for(i = 0; i < 32; i++)
  {
    MYUSART_SendData(Byte32[i]);
    temp += Byte32[i];
  }
  temp = 0x01 + 36 + 0x18 + NotePageNum + temp;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}
//������PS_ReadNotepad
//���ܣ�  ��ȡFLASH�û�����128bytes����
//����:  NotePageNum(0~15)
//˵��:  ģ�鷵��ȷ����+�û���Ϣ
u8 PS_ReadNotepad(u8 NotePageNum, u8 *Byte32)
{
  u16 temp;
  u8  ensure, i;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x04);
  Sendcmd(0x19);
  MYUSART_SendData(NotePageNum);
  temp = 0x01 + 0x04 + 0x19 + NotePageNum;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    for(i = 0; i < 32; i++)
    {
      Byte32[i] = data[10 + i];
    }
  }
  else
    ensure = 0xff;
  return ensure;
}
//��������PS_HighSpeedSearch
//���ܣ��� CharBuffer1��CharBuffer2�е������ļ��������������򲿷�ָ�ƿ⡣
//		  �����������򷵻�ҳ��,��ָ����ڵ�ȷ������ָ�ƿ��� ���ҵ�¼ʱ����
//		  �ܺõ�ָ�ƣ���ܿ�������������
//����:  BufferID�� StartPage(��ʼҳ)��PageNum��ҳ����
//˵��:  ģ�鷵��ȷ����+ҳ�루����ָ��ģ�壩
u8 PS_HighSpeedSearch(u8 BufferID, u16 StartPage, u16 PageNum, SearchResult *p)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x08);
  Sendcmd(0x1b);
  MYUSART_SendData(BufferID);
  MYUSART_SendData(StartPage >> 8);
  MYUSART_SendData(StartPage);
  MYUSART_SendData(PageNum >> 8);
  MYUSART_SendData(PageNum);
  temp = 0x01 + 0x08 + 0x1b + BufferID
         + (StartPage >> 8) + (u8)StartPage
         + (PageNum >> 8) + (u8)PageNum;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    p->pageID 	= (data[10] << 8) + data[11];
    p->mathscore = (data[12] << 8) + data[13];
  }
  else
    ensure = 0xff;
  return ensure;
}
//����Чģ����� PS_ValidTempleteNum
//���ܣ�����Чģ�����
//����: ��
//˵��: ģ�鷵��ȷ����+��Чģ�����ValidN
u8 PS_ValidTempleteNum(u16 *ValidN)
{
  u16 temp;
  u8  ensure;
  u8  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);//�������ʶ
  SendLength(0x03);
  Sendcmd(0x1d);
  temp = 0x01 + 0x03 + 0x1d;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    *ValidN = (data[10] << 8) + data[11];
  }
  else
    ensure = 0xff;

  if(ensure == 0x00)
  {
    printf("\r\nNumber of effective fingerprints=%d", (data[10] << 8) + data[11]);
  }
  else
    EnsureMessage(ensure);
  return ensure;
}
//��AS608���� PS_HandShake
//����: PS_Addr��ַָ��
//˵��: ģ�鷵�µ�ַ����ȷ��ַ��
u8 PS_HandShake(u32 *PS_Addr)
{
  SendHead();
  SendAddr();
  MYUSART_SendData(0X01);
  MYUSART_SendData(0X00);
  MYUSART_SendData(0X00);
  delay_ms(200);
  if(USART2_RX_STA & 0X8000) //���յ�����
  {
    if(//�ж��ǲ���ģ�鷵�ص�Ӧ���
      USART2_RX_BUF[0] == 0XEF
      && USART2_RX_BUF[1] == 0X01
      && USART2_RX_BUF[6] == 0X07
    )
    {
      *PS_Addr = (USART2_RX_BUF[2] << 24) + (USART2_RX_BUF[3] << 16)
                 + (USART2_RX_BUF[4] << 8) + (USART2_RX_BUF[5]);
      USART2_RX_STA = 0;
      return 0;
    }
    USART2_RX_STA = 0;
  }
  return 1;
}
//ģ��Ӧ���ȷ������Ϣ����
//���ܣ�����ȷ���������Ϣ������Ϣ
//����: ensure
void EnsureMessage(u8 ensure)
{
  switch(ensure)
  {
    case  0x00:
			OLED_ShowString(56, 3, "OK",16);
			break;
		case  0x01:
			OLED_ShowCHinese(0,3,4);	       //��
			OLED_ShowCHinese(16,3,5);	       //��
			OLED_ShowCHinese(32,3,6);	       //��
			OLED_ShowCHinese(48,3,7);	       //��
			OLED_ShowCHinese(64,3,8);	       //��
			OLED_ShowCHinese(80,3,9);	       //��
			OLED_ShowCHinese(96,3,10);	     //��			
			break;
		case  0x02:
			OLED_ShowCHinese(0,3,11);	       //��
			OLED_ShowCHinese(16,3,12);	     //��
			OLED_ShowCHinese(32,3,13);	     //��
			OLED_ShowCHinese(48,3,14);	     //��
			OLED_ShowCHinese(64,3,15);	     //û
			OLED_ShowCHinese(80,3,16);	     //��
			OLED_ShowCHinese(96,3,17);	     //��
			OLED_ShowCHinese(112,3,18);	     //ָ
			break;
		case  0x03:
			OLED_ShowCHinese(0,3,19);	    //��˳��¼��ָ��ͼ��ʧ��   
			OLED_ShowCHinese(16,3,20);	    
			OLED_ShowCHinese(32,3,21);	     
			OLED_ShowCHinese(48,3,22);	     
			OLED_ShowCHinese(64,3,23);	     
			OLED_ShowCHinese(80,3,24);	     
			OLED_ShowCHinese(96,3,25);	     
			OLED_ShowCHinese(112,3,26);	     
			break;
		case  0x04:
			OLED_ShowCHinese(0,3,21);	    //��˳��ָ��̫�ɻ�̫��  
			OLED_ShowCHinese(16,3,22);	    
			OLED_ShowCHinese(32,3,29);	     
			OLED_ShowCHinese(48,3,27);	     
			OLED_ShowCHinese(64,3,28);	     
			OLED_ShowCHinese(80,3,29);	     
			OLED_ShowCHinese(96,3,30);	          
			break;
		case  0x05:
			OLED_ShowCHinese(0,3,21);	    //��˳��ָ��̫ʪ��̫��  
			OLED_ShowCHinese(16,3,22);	    
			OLED_ShowCHinese(32,3,29);	     
			OLED_ShowCHinese(48,3,27);	     
			OLED_ShowCHinese(64,3,31);	     
			OLED_ShowCHinese(80,3,29);	     
			OLED_ShowCHinese(96,3,32);	       
			break;
		case  0x06:
			OLED_ShowCHinese(0,3,21);	   //��˳��ָ��ͼ��̫��  
			OLED_ShowCHinese(16,3,22);	    
			OLED_ShowCHinese(32,3,23);	     
			OLED_ShowCHinese(48,3,24);	     	     
			OLED_ShowCHinese(64,3,29);	     
			OLED_ShowCHinese(80,3,33);	  
			break;
		case  0x07:
			OLED_ShowCHinese(0,3,21);	   //��˳��ָ��������̫��  
			OLED_ShowCHinese(16,3,22);	    
			OLED_ShowCHinese(32,3,34);	     
			OLED_ShowCHinese(48,3,35);	     	     
			OLED_ShowCHinese(64,3,36);	     
			OLED_ShowCHinese(80,3,29);	
			OLED_ShowCHinese(96,3,37);
			break;
		case  0x08:
			OLED_ShowCHinese(0,3,21);	   //��˳��ָ�Ʋ�ƥ��  
			OLED_ShowCHinese(16,3,22);	    
			OLED_ShowCHinese(32,3,38);	     
			OLED_ShowCHinese(48,3,39);	     	     
			OLED_ShowCHinese(64,3,40);	
			break;
		case  0x09:
			OLED_ShowCHinese(0,3,15);	     //û��������ָ��
			OLED_ShowCHinese(16,3,16);	  
			OLED_ShowCHinese(32,3,41);	
			OLED_ShowCHinese(48,3,42);	
			OLED_ShowCHinese(64,3,43);	
			OLED_ShowCHinese(80,3,21);	   
			OLED_ShowCHinese(96,3,22);	    
			break;
		case  0x0a:
			OLED_ShowCHinese(0,3,34);	 //  �����ϲ�ʧ��  
			OLED_ShowCHinese(16,3,35);	
			OLED_ShowCHinese(32,3,44);	     
			OLED_ShowCHinese(48,3,45);
			OLED_ShowCHinese(64,3,25);	     
			OLED_ShowCHinese(80,3,26);	 
			break;
		case  0x0b:
			OLED_ShowCHinese(0,3,46);	   //��˳�򣺵�ַ��ų�����Χ  
			OLED_ShowCHinese(16,3,47);	    
			OLED_ShowCHinese(32,3,48);	     
			OLED_ShowCHinese(48,3,49);	     	     
			OLED_ShowCHinese(64,3,50);	     
			OLED_ShowCHinese(80,3,51);	
			OLED_ShowCHinese(96,3,52);
			OLED_ShowCHinese(112,3,53);
			break;
		case  0x10:
			OLED_ShowCHinese(16,3,54);	   //��˳��ɾ��ģ��ʧ��  
			OLED_ShowCHinese(32,3,55);	    
			OLED_ShowCHinese(48,3,56);	     
			OLED_ShowCHinese(64,3,57);	     	     
			OLED_ShowCHinese(80,3,25);	     
			OLED_ShowCHinese(96,3,26);
			break;
		case  0x11:
			OLED_ShowCHinese(0,3,58);	     
			OLED_ShowCHinese(16,3,59);
			OLED_ShowCHinese(32,3,21);	   //��˳�����ָ�ƿ�ʧ�� 
			OLED_ShowCHinese(48,3,22);
			OLED_ShowCHinese(64,3,60);
			OLED_ShowCHinese(80,3,25);	     
			OLED_ShowCHinese(96,3,26);
			break;
		case  0x15:
			OLED_ShowCHinese(0,3,61);	     
			OLED_ShowCHinese(16,3,62);
			OLED_ShowCHinese(32,3,63);	   //��˳�򣺻�����������Чͼ 
			OLED_ShowCHinese(48,3,64);
			OLED_ShowCHinese(64,3,65);
			OLED_ShowCHinese(80,3,16);	     
			OLED_ShowCHinese(96,3,66);
			OLED_ShowCHinese(112,3,23);
			break;
		case  0x18:
			OLED_ShowCHinese(0,3,67);	     //��дFLASH����
			OLED_ShowCHinese(16,3,68);
			OLED_ShowString(32, 3, "FLASH",16);
			OLED_ShowCHinese(72,3,51);
			OLED_ShowCHinese(88,3,9);	
			break;
		case  0x19:
			OLED_ShowCHinese(16,3,69);
			OLED_ShowCHinese(32,3,70);	   //��˳��δ�������
			OLED_ShowCHinese(48,3,71);
			OLED_ShowCHinese(64,3,9);
			OLED_ShowCHinese(80,3,10);	  
			break;
		case  0x1a:
			OLED_ShowCHinese(16,3,65);
			OLED_ShowCHinese(32,3,66);	   //��˳����Ч�Ĵ�����
			OLED_ShowCHinese(48,3,72);
			OLED_ShowCHinese(64,3,73);
			OLED_ShowCHinese(80,3,74);	 
			OLED_ShowCHinese(96,3,75);
			break;
		case  0x1b:
			OLED_ShowCHinese(0,3,72);	     
			OLED_ShowCHinese(16,3,73);
			OLED_ShowCHinese(32,3,74);	   //��˳�򣺼Ĵ������ݴ��� 
			OLED_ShowCHinese(48,3,64);
			OLED_ShowCHinese(64,3,76);
			OLED_ShowCHinese(80,3,9);	     
			OLED_ShowCHinese(96,3,10);
			break;
		case  0x1c:
			OLED_ShowCHinese(0,3,77);	     
			OLED_ShowCHinese(16,3,78);
			OLED_ShowCHinese(32,3,79);	   //��˳�򣺼��±�ҳ����� 
			OLED_ShowCHinese(48,3,80);
			OLED_ShowCHinese(64,3,81);
			OLED_ShowCHinese(80,3,9);	     
			OLED_ShowCHinese(96,3,10);
			break;
		case  0x1f:
			OLED_ShowCHinese(32,3,21);	   //��˳��ָ�ƿ��� 
			OLED_ShowCHinese(48,3,22);
			OLED_ShowCHinese(64,3,60);
			OLED_ShowCHinese(80,3,82);	 
			break;
		case  0x20:
			OLED_ShowCHinese(32,3,46);	   //��˳�򣺵�ַ���� 
			OLED_ShowCHinese(48,3,47);
			OLED_ShowCHinese(64,3,9);
			OLED_ShowCHinese(80,3,10);	 
			break;
		default :     
			OLED_ShowCHinese(8,3,83);
			OLED_ShowCHinese(24,3,84);	   //����ȷ�������� 
			OLED_ShowCHinese(40,3,85);
			OLED_ShowCHinese(56,3,86);
			OLED_ShowCHinese(72,3,81);
			OLED_ShowCHinese(88,3,16);	     
			OLED_ShowCHinese(104,3,10);
			break;
  }
  return;
}

//��ʾȷ���������Ϣ
void ShowErrMessage(u8 ensure)
{
  EnsureMessage(ensure);
}


//¼ָ��  ����������ʱ���
void Add_FR(void)
{
  u8 ensure=1;
  u8 ID_NUM = 0;
	u8 processnum = 0;
	OLED_Clear();
	for(int i = 0; i < 5; i++)
{
	if(box[i]==0)	
{  
	box[i]=1;
	while(1)
  {
		
    switch (processnum)
    {
			case 0:  //��һ��
				OLED_ShowCHinese(32,3,99);	   //��˳���밴ָ�� 
				OLED_ShowCHinese(48,3,112);
				OLED_ShowCHinese(64,3,21);
				OLED_ShowCHinese(80,3,22);	 
				ensure = PS_GetImage();//����:̽����ָ��̽�⵽��¼��ָ��ͼ�����ImageBuffer��
				if(ensure == 0x00)
				{
					ensure = PS_GenChar(CharBuffer1); //��������
					if(ensure == 0x00)
					{
						OLED_Clear();  //����
						OLED_ShowCHinese(32,3,21);	   //��˳��ָ������ 
						OLED_ShowCHinese(48,3,22);
						OLED_ShowCHinese(64,3,113);
						OLED_ShowCHinese(80,3,114);
						delay_ms(500); //���ܳ���798
						processnum = 1; //�����ڶ���
					}
					else
					{
						OLED_Clear();  //����
						ShowErrMessage(ensure);//��ʾȷ���������Ϣ
						//��������ʧ�ܣ�����̽��ָ��
						processnum = 0;
						delay_ms(500); //���ܳ���798
						OLED_Clear();  //����
					}
						
				}
				else
				{
					OLED_Clear();  //����
					//û��̽�⵽��ָ������̽��
					processnum = 0;
					ShowErrMessage(ensure);//��ʾȷ���������Ϣ
					delay_ms(500); //���ܳ���798
					OLED_Clear();  //����
				}
				break;

			case 1:
				OLED_Clear();  //����
				OLED_ShowCHinese(24,3,99);	   //��˳�����ٰ�һ��
				OLED_ShowCHinese(40,3,115);
				OLED_ShowCHinese(56,3,112);
				OLED_ShowCHinese(72,3,116);
				OLED_ShowCHinese(88,3,117);
				delay_ms(500); //���ܳ���798
				ensure = PS_GetImage();//����:̽����ָ��̽�⵽��¼��ָ��ͼ�����ImageBuffer��
				if(ensure == 0x00)
				{
					ensure = PS_GenChar(CharBuffer2); //��������
					if(ensure == 0x00)
					{
						OLED_Clear();  //����
						OLED_ShowCHinese(32,3,21);	   //��˳��ָ������ 
						OLED_ShowCHinese(48,3,22);
						OLED_ShowCHinese(64,3,113);
						OLED_ShowCHinese(80,3,114);
						delay_ms(500); //���ܳ���798
						OLED_Clear();  //����
						processnum = 2; //����������
					}
					else
					{
						OLED_Clear();  //����
						ShowErrMessage(ensure);//��ʾȷ���������Ϣ
						//��������ʧ�ܣ�����̽��ָ��
						processnum = 0;
						delay_ms(500); //���ܳ���798
						OLED_Clear();  //����
					}
						
				}
				else
				{
					//û��̽�⵽��ָ������̽��
					processnum = 1; //�����ص��ڶ���
					OLED_Clear();  //����
					ShowErrMessage(ensure);//��ʾȷ���������Ϣ
					delay_ms(500); //���ܳ���798
					OLED_Clear();  //����
				}
				break;

			case 2:
				OLED_Clear();  //����
				OLED_ShowCHinese(16,3,118);	   //��˳�򣺶Ա�����ָ�� 
				OLED_ShowCHinese(32,3,119);
				OLED_ShowCHinese(48,3,120);
				OLED_ShowCHinese(64,3,117);
				OLED_ShowCHinese(80,3,21);
				OLED_ShowCHinese(96,3,11);
				delay_ms(500); //���ܳ���798
				//��ȷ�ȶ���öָ������ PS_Match
				//����:��ȷ�ȶ�CharBuffer1 ��CharBuffer2 �е������ļ�
				ensure = PS_Match();
				if(ensure == 0x00)
				{
					OLED_Clear();  //����
					OLED_ShowCHinese(32,3,118);	   //��˳�򣺶Աȳɹ� 
					OLED_ShowCHinese(48,3,119);
					OLED_ShowCHinese(64,3,95);
					OLED_ShowCHinese(80,3,96);
					delay_ms(500); //���ܳ���798
					processnum = 3; //�������Ĳ�
				}
				else
				{
					OLED_Clear();  //����
					OLED_ShowCHinese(32,3,118);	   //��˳�򣺶Ա�ʧ�� 
					OLED_ShowCHinese(48,3,119);
					OLED_ShowCHinese(64,3,25);
					OLED_ShowCHinese(80,3,26);
					delay_ms(500); //���ܳ���798
					OLED_Clear();  //����
					ShowErrMessage(ensure);
					delay_ms(500); //���ܳ���798
					OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
					
					return;
//					processnum = 0; //���ص�һ��
				}
				break;

			case 3:
				OLED_Clear();  //����
				OLED_ShowCHinese(16,3,121);	   //��˳������ָ��ģ�� 
				OLED_ShowCHinese(32,3,95);	    	     	     
				OLED_ShowCHinese(48,3,21);	     
				OLED_ShowCHinese(64,3,22);
				OLED_ShowCHinese(80,3,56);	     
				OLED_ShowCHinese(96,3,57);
				delay_ms(500); //���ܳ���798
				//�ϲ�����������ģ�壩PS_RegModel
				//����:��CharBuffer1��CharBuffer2�е������ļ��ϲ����� ģ��,�������CharBuffer1��CharBuffer2
				ensure = PS_RegModel();
				if(ensure == 0x00)
				{
					OLED_Clear();  //����
					OLED_ShowCHinese(0,3,121);	   //��˳������ָ��ģ��ɹ� 
					OLED_ShowCHinese(16,3,95);	    	     	     
					OLED_ShowCHinese(32,3,21);	     
					OLED_ShowCHinese(48,3,22);
					OLED_ShowCHinese(64,3,56);	     
					OLED_ShowCHinese(80,3,57);
					OLED_ShowCHinese(96,3,95);
					OLED_ShowCHinese(112,3,96);
//					delay_ms(500); //���ܳ���798
					processnum = 4; //�������岽
				}
				else
				{
					//�ص���һ��
					OLED_Clear();  //����
					processnum = 0;
					ShowErrMessage(ensure);
					delay_ms(500);
					OLED_Clear();  //����
				}
				break;

			case 4:
				OLED_Clear();  //����

				while(1)
				{
					for(int i = 0; i < 5; i++)
					{
						if(box[i]==0)
						{
							box[i]=1;
							ensure = PS_StoreChar(CharBuffer2, i); //����ģ��
							if(ensure == 0x00)
							{
								DOOR_Open(i);
								OLED_Clear();
								OLED_ShowCHinese(16,3,19);	   //��˳��¼��ָ�Ƴɹ� 
								OLED_ShowCHinese(32,3,20);	    	     	     
								OLED_ShowCHinese(48,3,21);	     
								OLED_ShowCHinese(64,3,22);
								OLED_ShowCHinese(80,3,95);	     
								OLED_ShowCHinese(96,3,96);
								delay_ms(500);
								box[ID_NUM] = 1;
								OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
								KeyState = ' ';
								return ;
							}
							else
							{
								OLED_Clear();
								processnum = 0; //���ص�һ��
								ShowErrMessage(ensure);
								delay_ms(500);
								OLED_Clear();
							}
							return;
						}
						
					}
					// ��������
					
					OLED_Clear();
					OLED_ShowCHinese(32,2,129);	   
					OLED_ShowCHinese(48,2,130);
					OLED_ShowCHinese(64,2,131);
					OLED_ShowCHinese(80,2,132);
					delay_ms(500);
					delay_ms(500);
					
					OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
					KeyState = ' ';
					return;
					
				}
				
				
		}
		
  }
}
}
			//��������

			OLED_ShowCHinese(32,2,129);	   
			OLED_ShowCHinese(48,2,130);
			OLED_ShowCHinese(64,2,131);
			OLED_ShowCHinese(80,2,132);

			delay_ms(500);
			delay_ms(500);
			
			OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
			KeyState = ' ';
			return;
}

SysPara AS608Para;//ָ��ģ��AS608����
//ˢָ��
void press_FR(void)
{
  SearchResult seach;
  u8 ensure;
  char str[20];
	OLED_Clear();
	while(1)
	{
		ensure = PS_GetImage();
		if(ensure == 0x00) //��ȡͼ��ɹ�
	{
		ensure = PS_GenChar(CharBuffer1);
		if(ensure == 0x00) //���������ɹ�
		{
			ensure = PS_HighSpeedSearch(CharBuffer1, 0, 99, &seach);
			if(ensure == 0x00) //�����ɹ�
			{
				if(box[seach.pageID]==1)
				{
					box[seach.pageID]=0;
				OLED_Clear();
				OLED_ShowCHinese(16,2,21);	   //��˳��ָ����֤�ɹ� 
				OLED_ShowCHinese(32,2,22);	    	     	     
				OLED_ShowCHinese(48,2,124);	     
				OLED_ShowCHinese(64,2,125);
				OLED_ShowCHinese(80,2,95);	     
				OLED_ShowCHinese(96,2,96);
				sprintf(str, "ID:%d score:%d" ,seach.pageID, seach.mathscore);
				OLED_ShowString(0, 4, (u8*)str,16);
				delay_ms(500);
				delay_ms(500);
				OLED_Clear(); 	//����
				OLED_ShowCHinese(32,2,128);	   //��˳�����Ѵ� 
				OLED_ShowCHinese(48,2,87);
				OLED_ShowCHinese(64,2,88);
				OLED_ShowCHinese(80,2,89);
				OLED_ShowCHinese(32,4,90);	   //��˳�򣺻�ӭ���� 
				OLED_ShowCHinese(48,4,91);
				OLED_ShowCHinese(64,4,92);
				OLED_ShowCHinese(80,4,93);	    
//				GPIO_SetBits(GPIOF, GPIO_Pin_11); //�ſ�
//				GPIO_SetBits(GPIOF, GPIO_Pin_8); //��������
//				delay_ms(200); //���ܳ���798
//				GPIO_ResetBits(GPIOF, GPIO_Pin_8); //��������
//				for(int i = 0 ; i<=8 ; i++)
//				{
//					delay_ms(500); //���ܳ���798
//				}
//				GPIO_ResetBits(GPIOF, GPIO_Pin_11); //�Ź�
//				PwdAlarmLimit=0; //��λ����ֹ���󱨾�
				DOOR_Open(seach.pageID);
				OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
				//ɾ��ָ��
				ensure = PS_DeletChar(seach.mathscore, 1); //ɾ������ָ��
				if(ensure == 0)
				{
					OLED_Clear();
					OLED_ShowCHinese(16,3,54);	     
					OLED_ShowCHinese(32,3,55);
					OLED_ShowCHinese(48,3,21);	   //��˳��ɾ��ָ�Ƴɹ� 
					OLED_ShowCHinese(64,3,22);
					OLED_ShowCHinese(80,3,95);	     
					OLED_ShowCHinese(96,3,96);
					delay_ms(500);
					OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
				}
				return;
			}
				else
				{
					OLED_Clear();
				OLED_ShowCHinese(16,3,21);	   //��˳��ָ����֤ʧ�� 
				OLED_ShowCHinese(32,3,22);	    	     	     
				OLED_ShowCHinese(48,3,124);	     
				OLED_ShowCHinese(64,3,125);
				OLED_ShowCHinese(80,3,25);	     
				OLED_ShowCHinese(96,3,26);
				
				PwdAlarmLimit++;
				if(PwdAlarmLimit==8) //����������������8�ξͱ���
				{
					for(int i=0;i<25;i++)
					{
						GPIO_SetBits(GPIOF, GPIO_Pin_8); //��������
						delay_ms(50); //���ܳ���798
						GPIO_ResetBits(GPIOF, GPIO_Pin_8); //��������
						delay_ms(50); //���ܳ���798
					}
					PwdAlarmLimit=0; //��λ����ֹ���󱨾�
				}
				delay_ms(500);
				OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
				return;
				}
			}
			else
			{
				OLED_Clear();
				OLED_ShowCHinese(16,3,21);	   //��˳��ָ����֤ʧ�� 
				OLED_ShowCHinese(32,3,22);	    	     	     
				OLED_ShowCHinese(48,3,124);	     
				OLED_ShowCHinese(64,3,125);
				OLED_ShowCHinese(80,3,25);	     
				OLED_ShowCHinese(96,3,26);
				
				PwdAlarmLimit++;
				if(PwdAlarmLimit==8) //����������������8�ξͱ���
				{
					GPIO_ResetBits(GPIOF,GPIO_Pin_9);
					for(int i=0;i<25;i++)
					{
						GPIO_SetBits(GPIOF, GPIO_Pin_8); //��������
						delay_ms(50); //���ܳ���798
						GPIO_ResetBits(GPIOF, GPIO_Pin_8); //��������
						delay_ms(50); //���ܳ���798
					}
					GPIO_SetBits(GPIOF,GPIO_Pin_9);
					PwdAlarmLimit=0; //��λ����ֹ���󱨾�
				}
				delay_ms(500);
				OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
				return;

			}
		}
	}
	else
	{
		
		OLED_ShowCHinese(32,3,99);	   //��˳���밴ָ�� 
		OLED_ShowCHinese(48,3,112);
		OLED_ShowCHinese(64,3,21);
		OLED_ShowCHinese(80,3,22);	 
	}
}
}

//ɾ��ָ��
void Del_FR(void)
{
  u8  ensure;
  u16 ID_NUM = 0;
	OLED_Clear(); 	//����
	OLED_ShowString(16,0,"A+B-*",16);
	OLED_ShowCHinese(56,0,97); //ȷ��
	OLED_ShowCHinese(88,0,98);
	OLED_ShowString(16,2, "#",16);
	OLED_ShowCHinese(24,2,110); //���
	OLED_ShowCHinese(40,2,111);
	OLED_ShowCHinese(56,2,21);	//ָ�ƿ�
	OLED_ShowCHinese(72,2,22);
	OLED_ShowCHinese(88,2,60);
	OLED_ShowString(16,4, "D",16);
	OLED_ShowCHinese(32,4,126); //�˳�
	OLED_ShowCHinese(48,4,127);
	int clearflag=1;
  while(1)
  {
		get_key(); //��ȡ����״̬
		if(KeyState !=' ' && KeyState == 'B')
		{
			if(clearflag)
			{
				OLED_Clear();
				clearflag=0;
			}
			if(ID_NUM > 0)
        ID_NUM--;
			// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
			KeyState = ' ';
		}
		else if(KeyState !=' ' && KeyState == 'A')
		{
			if(clearflag)
			{
				OLED_Clear();
				clearflag=0;
			}
			if(ID_NUM < 99)
        ID_NUM++;
			// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
			KeyState = ' ';
		}
		else if(KeyState !=' ' && KeyState == 'D')
		{
			// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
			KeyState = ' ';
			break;
			
		}
		else if(KeyState !=' ' && KeyState == '#')
		{
			clearflag=0;
			ensure = PS_Empty(); //���ָ�ƿ�
      if(ensure == 0)
      {
        OLED_Clear();
				OLED_ShowCHinese(0,3,58);	     
				OLED_ShowCHinese(16,3,59);
				OLED_ShowCHinese(32,3,21);	   //��˳�����ָ�ƿ�ɹ� 
				OLED_ShowCHinese(48,3,22);
				OLED_ShowCHinese(64,3,60);
				OLED_ShowCHinese(80,3,95);	     
				OLED_ShowCHinese(96,3,96);
				for(int i=0;i<2;i++)
				{
					delay_ms(500);
				}
				OLED_Clear();
      }
      else
			{
				OLED_Clear();
				ShowErrMessage(ensure);
				delay_ms(500);
				OLED_Clear();
			}
			// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
			KeyState = ' ';	
		}
    else if(KeyState !=' ' && KeyState == 'C')
		{
			clearflag=0;
			ensure = PS_DeletChar(ID_NUM, 1); //ɾ������ָ��
			if(ensure == 0)
			{
				OLED_Clear();
				OLED_ShowCHinese(16,3,54);	     
				OLED_ShowCHinese(32,3,55);
				OLED_ShowCHinese(48,3,21);	   //��˳��ɾ��ָ�Ƴɹ� 
				OLED_ShowCHinese(64,3,22);
				OLED_ShowCHinese(80,3,95);	     
				OLED_ShowCHinese(96,3,96);
				delay_ms(500);
				OLED_Clear();
			}
			else
			{
				OLED_Clear();
				ShowErrMessage(ensure);
				delay_ms(500);
				OLED_Clear();
			}
			// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
			KeyState = ' ';
		}
		if(clearflag==0)
		{
			OLED_ShowString(32, 3, "ID = ",16);
			OLED_ShowNum(72, 3, ID_NUM, 2, 16);
		}
    
  }
  
}
