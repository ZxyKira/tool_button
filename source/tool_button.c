/* *****************************************************************************************
 *    File Name   :tool_button.c
 *    Create Date :2021-04-27
 *    Modufy Date :
 *    Information :
 */

/* *****************************************************************************************
 *    Include
 */ 
#include <string.h>
 
#include "tool_button.h"



/* *****************************************************************************************
 *    Macro
 */
#define ENABLE_FLAG 0x63FFA138




/* *****************************************************************************************
 *    Extern Function/Variable
 */

/* *****************************************************************************************
 *    Typedef List
 */ 

/* *****************************************************************************************
 *    Typedef Function
 */ 

/* *****************************************************************************************
 *    Struct/Union/Enum
 */ 

/* *****************************************************************************************
 *    Typedef Struct/Union/Enum
 */ 

/* *****************************************************************************************
 *    Public Variable
 */

/* *****************************************************************************************
 *    Private Variable
 */

/* *****************************************************************************************
 *    Private Function 
 */ 

/*----------------------------------------
 *  tool_button_reset_handle
 *----------------------------------------*/
static void inline tool_button_reset_handle(tool_button_handle_t* _this){
	memset(&_this->handle, 0x00, sizeof(_this->handle));
  _this->handle.magnification = _this->config.baseTime / _this->config.scanTime;
	_this->handle.modeMask = _this->config.modeEnable;
}



/*----------------------------------------
 *  tool_button_getModeLevel
 *----------------------------------------*/
static uint8_t tool_button_getModeLevel(uint32_t value){
  if(!value)
    return 0;	
	
  int bitCount = 0;
  int levelCount = 1;
  


  while(value){
    
    if((value & 0x00000001)){
      bitCount++;  
    }else{
    	
      if(bitCount)
        levelCount++;
        
      bitCount = 0;
    }
    
    value = (value >> 1);
  }
  
  return levelCount;
}



/*----------------------------------------
 *  tool_button_getModeBitCount
 *----------------------------------------*/
static uint8_t tool_button_getModeBitCount(uint32_t value, uint8_t level){
  int bitCount = 0;
  int pulseCount = 0;

  while(value){
    
    if((value & 0x00000001)){
      bitCount++;  
    }else{
      if(pulseCount == level)
        return bitCount;
      
      if(bitCount)
        pulseCount++;
        
      bitCount = 0;
    }
    
    value = (value >> 1);
  }
  
  if(pulseCount == level)
    return bitCount;
  else
    return 0;
}

/*----------------------------------------
 *  tool_button_valueCompair
 *----------------------------------------*/
static bool tool_button_valueCompair(tool_button_handle_t* _this, uint32_t value){
  uint32_t realValue = (value / _this->handle.magnification);
}


/*----------------------------------------
 *  tool_button_hasNextLevel
 *----------------------------------------*/
static bool tool_button_hasNextLevel(tool_button_handle_t* _this, uint8_t level){
  int i, j;
	
	for(i=0; i<8; i++){
	  if(_this->handle.modeMask & (1 << i)){
			if(tool_button_getModeBitCount(_this->config.mode[i], level))
				return true;
		}
	}
	
	return false;
}



/*----------------------------------------
 *  tool_button_timer_event
 *----------------------------------------*/
static void tool_button_timer_event(void* attachment){
  tool_button_handle_t* _this = attachment;
  
  if(_this->reference.pin->api->getValue(_this->reference.pin) == _this->config.highEnable){ //pressing
		_this->handle.pressCount++;
	}else{
	
	
	}
  
  bool result = tool_timer_scheduler_schedule(_this->reference.timerScheduler, tool_button_timer_event, _this, _this->config.scanTime);
  if(!result)
    _this->config.execute = 0;
}



/* *****************************************************************************************
 *    Public Function
 */
 
/*----------------------------------------
 *  tool_button_init
 *----------------------------------------*/
bool tool_button_init(tool_button_handle_t* _this, tool_timer_scheduler_handle_t* timerScheduler, fw_io_pin_handle_t* pin){
  if(!_this)
    return false;
  
  if(!timerScheduler)
    return false;
  
  if(!pin)
    return false;
  
  if(_this->enableFlag == ENABLE_FLAG)
    return false;
  
  memset(_this, 0x00, sizeof(tool_button_handle_t));
  
  _this->enableFlag = ENABLE_FLAG;
  _this->reference.pin = pin;
  _this->reference.timerScheduler = timerScheduler;
  _this->config.baseTime = 100;
  _this->config.timeout = 500;
  
  return true;
}



/*----------------------------------------
 *  tool_button_isEnable
 *----------------------------------------*/
bool tool_button_isEnable(tool_button_handle_t* _this){
  if(!_this)
    return false;
  
  if(_this->enableFlag != ENABLE_FLAG)
    return false;
  
  return true;
}



/*----------------------------------------
 *  tool_button_addMode
 *----------------------------------------*/
bool tool_button_addMode(tool_button_handle_t* _this, uint32_t mode){
  if(!tool_button_isEnable(_this))
    return false;
  
  if(!mode)
    return false;
  
  int i;
  
  for(i=0; i<8; i++){

    if(!_this->config.mode[i]){
      _this->config.mode[i] = mode;
			_this->config.modeEnable |= (1 << i);
      return true;
    }
  }
  
  return false;
}



/*----------------------------------------
 *  tool_button_removeMode
 *----------------------------------------*/
bool tool_button_removeMode(tool_button_handle_t* _this, uint32_t mode){
  if(!tool_button_isEnable(_this))
    return false;

  if(!mode)
    return false;
  
  int i;
  
  for(i=0; i<8; i++){
    
    if(_this->config.mode[i] == mode){
      _this->config.mode[i] = 0;
			_this->config.modeEnable &= ~(1 << i);
      return true;
    }
  }
  
  return false;
}



/*----------------------------------------
 *  tool_button_isStart
 *----------------------------------------*/
bool tool_button_isStart(tool_button_handle_t* _this){
  if(!tool_button_isEnable(_this))
    return false;
  
  if(_this->config.execute)
    return true;
  
  return false;
}



/*----------------------------------------
 *  tool_button_start
 *----------------------------------------*/
bool tool_button_start(tool_button_handle_t* _this, uint32_t msBaseTime, tool_button_execute_t execute, void* attachment, uint32_t msScanTime){
  if(tool_button_isStart(_this))
    return false;

  if(msScanTime > _this->config.baseTime)
    return false;
  
  _this->config.execute = execute;
  _this->config.attachment = attachment;
  _this->config.scanTime = msScanTime;

	tool_button_reset_handle(_this);
  
  bool result = tool_timer_scheduler_schedule(_this->reference.timerScheduler, tool_button_timer_event, _this, _this->config.scanTime);
  
  if(result){  //timer add taks successful
    return true;
    
  }else{    //timer add taks fail
    _this->config.execute = 0;
    return false;
  }
}



/*----------------------------------------
 *  tool_button_stop
 *----------------------------------------*/
bool tool_button_stop(tool_button_handle_t* _this){
  if(!tool_button_isStart(_this))
    return false;
  
  _this->config.execute = 0;
  
  return true;
}



/*----------------------------------------
 *  tool_button_setTimeout
 *----------------------------------------*/
bool tool_button_setTimeout(tool_button_handle_t* _this, uint32_t msTimeout){
  if(!tool_button_isEnable(_this))
    return false;
  
  _this->config.timeout = msTimeout;
  
  return true;
}



/*----------------------------------------
 *  tool_button_setBaseTime
 *----------------------------------------*/
bool tool_button_setBaseTime(tool_button_handle_t* _this, uint32_t msBaseTime){
  if(!tool_button_isEnable(_this))
    return false;
  
  _this->config.baseTime = msBaseTime;
  
  return true;
}



/*----------------------------------------
 *  tool_button_setBaseTime
 *----------------------------------------*/
bool tool_button_setHighEnable(tool_button_handle_t* _this, bool enable){
  if(!tool_button_isEnable(_this))
    return false;
  
  _this->config.highEnable = enable;
  
  return true;
}



/* *****************************************************************************************
 *    API Link
 */

/* *****************************************************************************************
 *    End of file
 */
 