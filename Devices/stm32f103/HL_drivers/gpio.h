/**
 * @file gpio.h
 * @brief General Purpose Input Output - High Level driver
 * @author filipembedded
 */


#ifndef GPIO_H
#define GPIO_H

#include "util.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f103xb.h"


/***************************** Defines ****************************************/
#define GPIO_MAX_NUM_INSTANCES 5

#define assert_instance(instance) assert(instance == GPIO_HL_INSTANCE_PORT_A || \
                                         instance == GPIO_HL_INSTANCE_PORT_B || \
                                         instance == GPIO_HL_INSTANCE_PORT_C || \
                                         instance == GPIO_HL_INSTANCE_PORT_D || \
                                         instance == GPIO_HL_INSTANCE_PORT_E)


#define assert_pin(pin) assert(pin == GPIO_HL_PIN_0 || \
                               pin == GPIO_HL_PIN_1 || \
                               pin == GPIO_HL_PIN_2 || \
                               pin == GPIO_HL_PIN_3 || \
                               pin == GPIO_HL_PIN_4 || \
                               pin == GPIO_HL_PIN_5 || \
                               pin == GPIO_HL_PIN_6 || \
                               pin == GPIO_HL_PIN_7 || \
                               pin == GPIO_HL_PIN_8 || \
                               pin == GPIO_HL_PIN_9 || \
                               pin == GPIO_HL_PIN_10 || \
                               pin == GPIO_HL_PIN_11 || \
                               pin == GPIO_HL_PIN_12 || \
                               pin == GPIO_HL_PIN_13 || \
                               pin == GPIO_HL_PIN_14 || \
                               pin == GPIO_HL_PIN_15 || \
                               pin == GPIO_HL_PIN_ALL)

#define assert_edgestate(edgeState) assert((edgeState == GPIO_HL_EDGESTATE_FALLING) || \
                                (edgeState == GPIO_HL_EDGESTATE_NONE) || \
                                (edgeState == GPIO_HL_EDGESTATE_RISING) || \
                                (edgeState == GPIO_HL_EDGESTATE_RISING_FALLING))


/************************** Typedefs ******************************************/
/**
 * @brief GPIO Pin
 */
typedef enum {
    GPIO_HL_PIN_0 = LL_GPIO_PIN_0,
    GPIO_HL_PIN_1 = LL_GPIO_PIN_1,
    GPIO_HL_PIN_2 = LL_GPIO_PIN_2,
    GPIO_HL_PIN_3 = LL_GPIO_PIN_3,
    GPIO_HL_PIN_4 = LL_GPIO_PIN_4,
    GPIO_HL_PIN_5 = LL_GPIO_PIN_5,
    GPIO_HL_PIN_6 = LL_GPIO_PIN_6,
    GPIO_HL_PIN_7 = LL_GPIO_PIN_7,
    GPIO_HL_PIN_8 = LL_GPIO_PIN_8,
    GPIO_HL_PIN_9 = LL_GPIO_PIN_9,
    GPIO_HL_PIN_10 = LL_GPIO_PIN_10,
    GPIO_HL_PIN_11 = LL_GPIO_PIN_11,
    GPIO_HL_PIN_12 = LL_GPIO_PIN_12,
    GPIO_HL_PIN_13 = LL_GPIO_PIN_13,
    GPIO_HL_PIN_14 = LL_GPIO_PIN_14,
    GPIO_HL_PIN_15 = LL_GPIO_PIN_15,
    GPIO_HL_PIN_ALL = LL_GPIO_PIN_ALL,
} gpio_hl_pin_t;

/**
 * @brief GPIO Pin Mode
 */
typedef enum {
    GPIO_HL_MODE_ANALOG    = LL_GPIO_MODE_ANALOG,
    GPIO_HL_MODE_FLOATING  = LL_GPIO_MODE_FLOATING,
    GPIO_HL_MODE_INPUT     = LL_GPIO_MODE_INPUT,
    GPIO_HL_MODE_OUTPUT    = LL_GPIO_MODE_OUTPUT,
    GPIO_HL_MODE_ALTERNATE = LL_GPIO_MODE_ALTERNATE,
} gpio_hl_mode_t;

/**
 * @brief GPIO Pin Speed
 */
typedef enum {
    GPIO_HL_SPEED_10MHz = LL_GPIO_MODE_OUTPUT_10MHz,
    GPIO_HL_SPEED_2MHz  = LL_GPIO_MODE_OUTPUT_2MHz,
    GPIO_HL_SPEED_50MHz = LL_GPIO_MODE_OUTPUT_50MHz,
} gpio_hl_speed_t;

/**
 * @brief GPIO Output Type
 */
typedef enum {
    GPIO_HL_OUTTYPE_PUSHPULL  = LL_GPIO_OUTPUT_PUSHPULL,
    GPIO_HL_OUTTYPE_OPENDRAIN = LL_GPIO_OUTPUT_OPENDRAIN,
} gpio_hl_outtype_t;

/**
 * @brief GPIO Pull Type
 */
typedef enum {
    GPIO_HL_PULL_DOWN = LL_GPIO_PULL_DOWN,
    GPIO_HL_PULL_UP   = LL_GPIO_PULL_UP,
} gpio_hl_pull_t;

/**
 * @brief GPIO Pin Value
 */
typedef enum {
    GPIO_HL_RESET = 0,
    GPIO_HL_SET      ,
} gpio_hl_pinvalue_t;

/**
 * @brief GPIO Instance
 */
typedef enum {
    GPIO_HL_INSTANCE_PORT_A = 0,
    GPIO_HL_INSTANCE_PORT_B,
    GPIO_HL_INSTANCE_PORT_C,
    GPIO_HL_INSTANCE_PORT_D,
    GPIO_HL_INSTANCE_PORT_E,
} gpio_hl_instance_t;

/**
 * @brief GPIO Edge State
 */
typedef enum {
    GPIO_HL_EDGESTATE_NONE           = LL_EXTI_TRIGGER_NONE,
    GPIO_HL_EDGESTATE_RISING         = LL_EXTI_TRIGGER_RISING,
    GPIO_HL_EDGESTATE_FALLING        = LL_EXTI_TRIGGER_FALLING,
    GPIO_HL_EDGESTATE_RISING_FALLING = LL_EXTI_TRIGGER_RISING_FALLING,
} gpio_hl_edgestate_t;

/**
 * @brief GPIO Config Struct
 */
typedef struct {
    gpio_hl_mode_t     mode;
    gpio_hl_speed_t    speed;
    gpio_hl_outtype_t  outputType;
    gpio_hl_pull_t     pull;
} gpio_hl_cfg_t;


/*************************** User Callbacks ***********************************/
typedef void (*gpio_hl_callback_t)(void);


/***************************** Declarations ***********************************/

/**
 * @brief Initialize the GPIO with config struct
 * @param instance GPIO port, available ports: A,B,C,D,E
 * @param pin GPIO pin ::gpio_hl_pin_t
 * @retval ::hl_status_t
 */
hl_status_t HL_GPIO_Init(gpio_hl_instance_t instance,
                         gpio_hl_pin_t pin,
                         gpio_hl_cfg_t *cfg);


/**
 * @brief Toggle Pin
 * @param instance GPIO port, available ports: A,B,C,D,E
 * @param pin GPIO pin ::gpio_hl_pin_t
 * @return none
 */
void HL_GPIO_TogglePin(gpio_hl_instance_t instance, gpio_hl_pin_t pin);

/**
 * @brief Set pin value 
 * @param instance GPIO port, available ports: A,B,C,D,E
 * @param pin GPIO pin ::gpio_hl_pin_t
 * @param value Value to set ::gpio_hl_pinvalue_t
 */
void HL_GPIO_SetPinValue(gpio_hl_instance_t instance, 
                         gpio_hl_pin_t pin, 
                         gpio_hl_pinvalue_t value);

/**
 * @brief Get value from specified pin
 * @param instance GPIO port, available ports: A,B,C,D,E
 * @param pin GPIO pin ::gpio_hl_pin_t
 * @retval ::hl_state_t
 */
hl_state_t HL_GPIO_GetPinValue(gpio_hl_instance_t instance, gpio_hl_pin_t pin);

/**
 * @brief Enable external interrupt on specific pin with specific trigger type
 * @param instance GPIO port, available ports: A,B,C,D,E
 * @param pin GPIO pin ::gpio_hl_pin_t
 * @param edgeState Trigger signal edge state ::gpio_hl_edgestate_t
 * @retval hl_status_t
 */
hl_status_t HL_GPIO_EnableInterrupt(gpio_hl_instance_t instance,
                             gpio_hl_pin_t pin,
                             gpio_hl_edgestate_t edgeState);

/**
 * @brief Configure user callback
 * @param instance GPIO port, available ports: A,B,C,D,E
 * @param pin GPIO pin ::gpio_hl_pin_t
 * @param callback User Callback ::gpio_hl_callback_t
 * @retval ::hl_status_t
 * 
 * @note When defining user callback, it should be like gpio_hl_callback_t
 *       void userCallbackExample(void)
 *       {
 *          This executes when interrupt is triggered...  
 *       }
 *       Return type must be void, and callback should not have any parameters!
 */
hl_status_t HL_GPIO_ConfigureCallback(gpio_hl_instance_t instance, 
                                      gpio_hl_pin_t      pin,
                                      gpio_hl_callback_t callback);



#endif