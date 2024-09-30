/**
 * @file gpio.h
 * @brief General Purpose Input Output - High Level driver
 * @author filipembedded
 */

#include "stm32f1xx_ll_gpio.h"


/************************** Typedefs ******************************************/
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
    GPIO_HL_PIN_ALL = LL_GPIO_PIN_ALL,
} gpio_hl_pin_t;

typedef enum {
    GPIO_HL_MODE_ANALOG    = LL_GPIO_MODE_ANALOG,
    GPIO_HL_MODE_FLOATING  = LL_GPIO_MODE_FLOATING,
    GPIO_HL_MODE_INPUT     = LL_GPIO_MODE_INPUT,
    GPIO_HL_MODE_OUTPUT    = LL_GPIO_MODE_OUTPUT,
    GPIO_HL_MODE_ALTERNATE = LL_GPIO_MODE_ALTERNATE,
} gpio_hl_mode_t;

typedef enum {
    GPIO_HL_SPEED_10MHz = LL_GPIO_MODE_OUTPUT_10MHz,
    GPIO_HL_SPEED_2MHz  = LL_GPIO_MODE_OUTPUT_2MHz,
    GPIO_HL_SPEED_50MHz = LL_GPIO_MODE_OUTPUT_50MHz,
} gpio_hl_speed_t;

typedef enum {
    GPIO_HL_OUTTYPE_PUSHPULL  = LL_GPIO_OUTPUT_PUSHPULL,
    GPIO_HL_OUTTYPE_OPENDRAIN = LL_GPIO_OUTPUT_OPENDRAIN,
} gpio_hl_outtype_t;

typedef enum {
    GPIO_HL_PULL_DOWN = LL_GPIO_PULL_DOWN,
    GPIO_HL_PULL_UP   = LL_GPIO_PULL_UP,
} gpio_hl_pull_t;

typedef struct {
    gpio_hl_pin_t      pin;
    gpio_hl_mode_t     mode;
    gpio_hl_speed_t    speed;
    gpio_hl_outtype_t  outputType;
    gpio_hl_pull_t     pull;
} gpio_hl_cfg_t;