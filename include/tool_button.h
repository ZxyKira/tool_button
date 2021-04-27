/* *****************************************************************************************
 *    File Name   :tool_button.h
 *    Create Date :2021-04-27
 *    Modufy Date :
 *    Information :
 */

#ifndef TOOL_BUTTON_VERSION
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/* *****************************************************************************************
 *    Include
 */ 
#include <stdint.h> 

#include "fw_io_pin.h"
#include "tool_timer_scheduler.h"

#include "version.h"

/* *****************************************************************************************
 *    Macro
 */ 
 
/*----------------------------------------
 *  TOOL_BUTTON_REQ_FW_IO_PIN_VERSION
 *----------------------------------------*/
#define TOOL_BUTTON_REQ_FW_IO_PIN_VERSION VERSION_DEFINEE(1,0,0)
#if VERSION_CHECK_COMPATIBLE(FW_IO_PIN_VERSION, TOOL_BUTTON_REQ_FW_IO_PIN_VERSION)
  #if VERSION_CHECK_COMPATIBLE(FW_IO_PIN_VERSION, TOOL_BUTTON_REQ_FW_IO_PIN_VERSION) == 2
    #error "FW_IO_PIN_VERSION under 1.0.x"
  #else
    #warning "FW_IO_PIN_VERSION revision under 1.0.0"
  #endif
#endif



/*----------------------------------------
 *  TOOL_BUTTON_REQ_TOOL_TIMER_SCHEDULER_VERSION
 *----------------------------------------*/
#define TOOL_BUTTON_REQ_TOOL_TIMER_SCHEDULER_VERSION VERSION_DEFINEE(1,0,0)
#if VERSION_CHECK_COMPATIBLE(TOOL_TIMER_SCHEDULER_VERSION, TOOL_BUTTON_REQ_TOOL_TIMER_SCHEDULER_VERSION)
  #if VERSION_CHECK_COMPATIBLE(TOOL_TIMER_SCHEDULER_VERSION, TOOL_BUTTON_REQ_TOOL_TIMER_SCHEDULER_VERSION) == 2
    #error "TOOL_TIMER_SCHEDULER_VERSION under 1.0.x"
  #else
    #warning "TOOL_TIMER_SCHEDULER_VERSION revision under 1.0.0"
  #endif
#endif



/*----------------------------------------
 *  TOOL_BUTTON_VERSION
 *----------------------------------------*/
#define TOOL_BUTTON_VERSION VERSION_DEFINE(1, 0, 0)



/* *****************************************************************************************
 *    Typedef List
 */ 
typedef struct _tool_button_handle_t tool_button_handle_t;



/* *****************************************************************************************
 *    Typedef Function
 */ 
typedef void (*tool_button_execute_t)(void* attachment, uint32_t mode);



/* *****************************************************************************************
 *    Struct/Union/Enum
 */ 

struct tool_button_api_t{
  bool     (*init)       (tool_button_handle_t* _this, tool_timer_scheduler_handle_t* timerScheduler, fw_io_pin_handle_t* pin);
	bool     (*isEnable)   (tool_button_handle_t* _this);
	bool     (*addMode)    (tool_button_handle_t* _this, uint32_t mode);
	bool     (*removeMode) (tool_button_handle_t* _this, uint32_t mode);
	bool     (*start)      (tool_button_handle_t* _this, uint32_t msBaseTime, uint32_t msTimeout);
	bool     (*stop)       (tool_button_handle_t* _this);
	bool     (*isStart)    (tool_button_handle_t* _this);
};


/* *****************************************************************************************
 *    Typedef Struct/Union/Enum
 */ 

/*----------------------------------------
 *  tool_button_handle_t
 *----------------------------------------*/
typedef struct _tool_button_handle_t{
	struct{
		uint32_t cache;
		uint32_t pressDownCount;
		uint32_t pressUpCount;
		uint16_t magnification;
	  uint8_t  modeMask;
		uint8_t  reserved;
	}handle;
	
	struct{
		uint32_t mode[8];
		uint32_t scanTime;
	  uint32_t baseTime;
		uint32_t timeout;
		void* attachment;
		tool_button_execute_t execute;
	}config;
	
	struct{
	  tool_timer_scheduler_handle_t* timerScheduler;
		fw_io_pin_handle_t* pin;
	}reference;
}tool_button_handle_t;



/* *****************************************************************************************
 *    Inline Function
 */ 

/* *****************************************************************************************
 *    Extern
 */ 



#ifdef __cplusplus
}
#endif //__cplusplus
#endif //TOOL_BUTTON_VERSION
/* *****************************************************************************************
 *  End of file
 */
