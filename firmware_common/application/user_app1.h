/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/

/*ANTͨ����������ĳ���*/
#define ANT_CHANNEL_TYPE_USERAPP                  CHANNEL_TYPE_MASTER //�豸��MASTER
#define ANT_CHANNEL_USERAPP                       ANT_CHANNEL_0          //Ƶ��0  -  7
#define ANT_CHANNEL_PERIOD_LO_USERAPP             (u8)      0x00 // LO; 0x0001  -  0x7fff
#define ANT_CHANNEL_PERIOD_HI_USERAPP             (u8)      0x20 // HI; 0x0001  -  0x7fff
#define ANT_DEVICEID_LO_USERAPP                   (u8)      0x63 //�豸�����ֽ�
#define ANT_DEVICEID_HI_USERAPP                   (u8)      0x00 //�豸�����ֽ�
#define ANT_DEVICE_TYPE_USERAPP                   (u8)      1    // 1  -  255
#define ANT_TRANSMISSION_TYPE_USERAPP             (u8)      1 // 1-127; MSB�������
#define ANT_FREQUENCY_USERAPP                     (u8)       50 // 2400MHz + 0  -  99 MHz
#define ANT_TX_POWER_USERAPP                      RADIO_TX_POWER_4DBM //���tx����
//#define UserApp1SM_AntChannelAssign               (u8)

/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    
static void UserApp1SM_Error(void);         
static void UserApp1SM_AntChannelAssign(void);

#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/