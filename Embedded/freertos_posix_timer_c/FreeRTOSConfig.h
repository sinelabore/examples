/* FreeRTOS settings for the microwave on the POSIX port (macOS, Linux). */
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_PREEMPTION                1
#define configUSE_IDLE_HOOK                 0
#define configUSE_TICK_HOOK                 0
#define configTICK_RATE_HZ                  1000    /* 1 ms: the service's TS_TICK_US */
#define configMINIMAL_STACK_SIZE            ( ( unsigned short ) PTHREAD_STACK_MIN )
#define configTOTAL_HEAP_SIZE               ( ( size_t ) ( 64 * 1024 ) )
#define configMAX_TASK_NAME_LEN             12
#define configUSE_16_BIT_TICKS              0
#define configMAX_PRIORITIES                5
#define configUSE_MUTEXES                   1
#define configSUPPORT_DYNAMIC_ALLOCATION    1
#define configSUPPORT_STATIC_ALLOCATION     0
#define configCHECK_FOR_STACK_OVERFLOW      0
#define configUSE_MALLOC_FAILED_HOOK        0

/* Software timers. The timer task has the highest priority: the timer
 * service (freertos_timer_service.c) relies on a timer command being
 * carried out before the call that sent it returns. */
#define configUSE_TIMERS                    1
#define configTIMER_TASK_PRIORITY           ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH            10
#define configTIMER_TASK_STACK_DEPTH        configMINIMAL_STACK_SIZE

#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskDelayUntil             1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_xTaskGetSchedulerState      1

/* A failed assertion prints where and ends the program (main.c). */
void vAssertCalled( const char * file, unsigned long line );
#define configASSERT( x )    if( ( x ) == 0 ) { vAssertCalled( __FILE__, __LINE__ ); }

#endif /* FREERTOS_CONFIG_H */
