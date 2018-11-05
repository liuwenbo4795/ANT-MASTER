/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */

extern u32                           G_u32AntApiCurrentMessageTimeStamp;                    
extern AntApplicationMessageType     G_eAntApiCurrentMessageClass;    
extern u8                            G_au8AntApiCurrentMessageBytes [ANT_APPLICATION_MESSAGE_BYTES];  
extern AntExtendedDataType           G_sAntApiCurrentMessageExtData;

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static AntAssignChannelInfoType  UserApp1_sChannelInfo;  /* ANT setup parameters */
static u8 UserApp1_au8MessageFail[] = "\n\r***ANT channel setup failed***\n\n\r";

//static u32 UserApp1_StateMachine;

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:g
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
    u8 au8WelcomeMessage[] = "ANT Master";
    

  /* Write a weclome message on the LCD */
#if EIE1
  /* Set a message up on the LCD. Delay is required to let the clear command send. */
  LCDCommand(LCD_CLEAR_CMD);
  for(u32 i = 0; i < 10000; i++);
  LCDMessage(LINE1_START_ADDR, au8WelcomeMessage);
#endif /* EIE1 */
  
#if 0 // untested for MPG2
  
#endif /* MPG2 */

 /* Configure ANT for this application  通道配置*/
  UserApp1_sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP;
  UserApp1_sChannelInfo.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP;
  UserApp1_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  UserApp1_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
 
  UserApp1_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  UserApp1_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  UserApp1_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  UserApp1_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  UserApp1_sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP;
  UserApp1_sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP;

  UserApp1_sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    UserApp1_sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
  
  /* Attempt to queue the ANT channel setup */
  if( AntAssignChannel(&UserApp1_sChannelInfo) )
  {
    UserApp1_u32Timeout   = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_AntChannelAssign;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    DebugPrintf(UserApp1_au8MessageFail);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
} /* end UserApp1Initialize() */
  

/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment*/
static void UserApp1SM_AntChannelAssign()
{
  if( AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    /* Channel assignment is successful, so open channel and
    proceed to Idle state  打开通达进入空闲状态*/
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Watch for time out */
  if(IsTimeUp(&UserApp1_u32Timeout, 3000))
  {
    DebugPrintf(UserApp1_au8MessageFail);
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
     
} /* end UserApp1SM_AntChannelAssign */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */


static void UserApp1SM_Idle(void)
{
    static u8 au8TestMessage [] = {0x5B,0,0,0,0xff,0,0,0};  //数组1,2,3,失败消息计数器；5,6,7成功消息计数器
     u8 au8DataContent[] = "xxxxxxxxxxxxxxxx";
     u8 au8DataContent2[] = "NanJing Tech University";
    /* Check all the buttons and update au8TestMessage according to the button state */ 
    //au8TestMessage[0] = 0x00;
    static u8 au8message[]={0x5a,0,0,0,0,0,0,0};
    
  //au8TestMessage[0] = 0x00;
  if( IsButtonPressed(BUTTON0) )
  {
    //au8TestMessage[0] = 0xff;
    LedToggle(WHITE);
    LedToggle(PURPLE);
  }
    //au8TestMessage[1] = 0x00;
  if( IsButtonPressed(BUTTON1) )
  {
    //au8TestMessage[1] = 0xff;
    LedToggle(BLUE);
    LedToggle(CYAN);
  }
   // au8TestMessage[2] = 0x00;
  if( IsButtonPressed(BUTTON2) )
  {
   // au8TestMessage[2] = 0xff;
    LedToggle(GREEN);
    LedToggle(YELLOW);
  }

 // au8TestMessage[3] = 0x00;
  if( IsButtonPressed(BUTTON3) )
  {
    //au8TestMessage[3] = 0xff;
    LedToggle(ORANGE);
    LedToggle(RED);
  }
  
    if(AntReadAppMessageBuffer())      /*如果至少有一条消息，则返回TRUE，进入if*/
  {
     /*来自ANT任务的新消息：检查它是什么*/
    if(G_eAntApiCurrentMessageClass == ANT_DATA)   //ANT_DATA型数据
    { 
      /*我们得到了一些数据,将其解析为au8DataContent*/
      for(u8 i = 0; i < ANT_DATA_BYTES; i++)
      {
        au8DataContent[2 * i]     = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] / 16);
        au8DataContent[2 * i + 1] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] % 16);
      }
      
      LCDMessage(LINE2_START_ADDR,au8DataContent);
      LCDMessage(LINE1_START_ADDR,au8DataContent2);
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      /* Update and queue the new message data ，更新并且排队新的消息队列*/
      /*计数正确消息*/
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8TestMessage);
     /*一个频道周期已过去：通常这是新数据应排队等待发送的时间*/
      /*计数错误消息*/
      if(G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX] == EVENT_TRANSFER_TX_FAILED )
      {
        au8TestMessage[3]++;
        if(au8TestMessage[3] == 0)
        {
          au8TestMessage[2]++;
          if(au8TestMessage[2] == 0)
          {
            au8TestMessage[1]++;
          }
        }
      } 
      AntQueueAcknowledgedMessage(ANT_CHANNEL_USERAPP, au8TestMessage);
    }/* end AntReadAppMessageBuffer（）*/
  } 

    
}/* end UserApp1SM_Idle() */
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
///*--------------------------------------------------------------------------------------------------------------------*/
///* End of File                                                                                                        */j
