#include "stm32f4xx.h"
#include "boctok_types.h"
#include "stm32f4xx_gpio.h"
/*

#define GPIO_IN_PUD         0x08
#define GPIO_IN_FLOAT       0x04
#define GPIO_ANALOG         0x00

#define GPIO_OUT_PP_2MHZ    0x02
#define GPIO_OUT_OD_2MHZ    0x06
#define GPIO_AF_PP_2MHZ     0x0A
#define GPIO_AF_OD_2MHZ     0x0E

#define GPIO_OUT_PP_10MHZ   0x01
#define GPIO_OUT_OD_10MHZ   0x05
#define GPIO_AF_PP_10MHZ    0x09
#define GPIO_AF_OD_10MHZ    0x0D

#define GPIO_OUT_PP_50MHz   0x03
#define GPIO_OUT_OD_50MHz   0x07
#define GPIO_AF_PP_50MHZ    0x0B
#define GPIO_AF_OD_50MHZ    0x0F
*/



void GPIO_configure(GPIO_TypeDef * port, uint32_t pin, uint32_t setup)
{
    if(pin < 8)
    {
        /**
        CRL
        pins 0..7
        clear old values first, even after reset there is 0x04!
        */
        /*
        port->CRL &= (uint32_t) ~(0x0F << (pin * 4));
        port->CRL |= (uint32_t)(setup << (pin * 4));
        */
    }
    else
    {
        /**
        CRH
        pins 8..15
        clear old values first, even after reset there is 0x04!
        */
        /*
        port->CRH &= (uint32_t) ~(0x0F << ((pin - 8) * 4));
        port->CRH |= (uint32_t)(setup << ((pin - 8) * 4));
        */
    }
}


/**
  * @brief  Selects the GPIO pin used as EXTI Line.
  * @param  EXTI_PortSourceGPIOx : selects the GPIO port to be used as source for
  *          EXTI lines where x can be (A..K) for STM32F42xxx/43xxx devices, (A..I)
  *          for STM32F405xx/407xx and STM32F415xx/417xx devices or (A, B, C, D and H)
  *          for STM32401xx devices.
  *
  * @param  EXTI_PinSourcex: specifies the EXTI line to be configured.
  *           This parameter can be EXTI_PinSourcex where x can be (0..15, except
  *           for EXTI_PortSourceGPIOI x can be (0..11) for STM32F405xx/407xx
  *           and STM32F405xx/407xx devices and for EXTI_PortSourceGPIOK x can
  *           be (0..7) for STM32F42xxx/43xxx devices.
  *
  * @retval None
  */

void SYSCFG_map_EXTI(uint32_t line, uint32_t port)
{
    if(line < 4)
    {
        SYSCFG->EXTICR[0] &= (uint32_t) ~(0x0F << (line * 4));
        SYSCFG->EXTICR[0] |= (uint32_t)(port << (line * 4));
    }
    else if(line < 8)
    {
        SYSCFG->EXTICR[1] &= (uint32_t) ~(0x0F << ((line -4) * 4));
        SYSCFG->EXTICR[1] |= (uint32_t)(port << ((line -4) * 4));
    }
    else if(line < 12)
    {
        SYSCFG->EXTICR[2] &= (uint32_t) ~(0x0F << ((line -8) * 4));
        SYSCFG->EXTICR[2] |= (uint32_t)(port << ((line -8) * 4));
    }
    else if(line < 16)
    {
        SYSCFG->EXTICR[3] &= (uint32_t) ~(0x0F << ((line -12) * 4));
        SYSCFG->EXTICR[3] |= (uint32_t)(port << ((line -12) * 4));
    }
}


/**
helper functions
*/
inline void gpio_set_pin_high(GPIO_TypeDef * Port, VU32 Pin)
{
    //on
    Port->BSRR= (U32) (1<< Pin);
}

inline void gpio_set_pin_low(GPIO_TypeDef * Port, VU32 Pin)
{
    //off
    Port->BSRR= (U32) (1<< (Pin + 16));
}




/**
convenient output pin modification
*/
void gpio_set_pin(GPIO_TypeDef * Port, VU32 Pin, output_pin_t Level)
{
    //GPIO port pin range 0..15!
    if(Pin > 15)
    {
        return;
        //TODO log warning
    }

    if(Level == ON)
    {
        gpio_set_pin_high(Port, Pin);
    }
    else if(Level == TOGGLE)
    {
        if(Port->ODR & (U32) (1<< Pin))
        {
            // OFF
            gpio_set_pin_low(Port, Pin);
        }
        else
        {
            //on
            gpio_set_pin_high(Port, Pin);
        }
    }
    else
    {
        gpio_set_pin_low(Port, Pin);
    }
}

