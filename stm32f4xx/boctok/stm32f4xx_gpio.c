#include "stm32f4xx.h"
#include "boctok_types.h"
#include "stm32f4xx_gpio.h"


void GPIO_configure(GPIO_TypeDef * Port, U32 Pin, U32 Mode, U32 OutputType, U32 Speed, U32 Pud)
{
    //GPIO port pin range 0..15!
    if(Pin > 15)
    {
        return;
        //TODO log warning
    }

    //Mode
    Port->MODER &= (U32) ~(0x03 << (Pin * 2));
    Port->MODER |= (U32) (Mode << (Pin * 2));

    //OutputType
    Port->OTYPER &= (U32) ~(0x01 << Pin);
    Port->OTYPER |= (U32) (OutputType << Pin);

    //Speed
    Port->OSPEEDR &= (U32) ~(0x03 << (Pin * 2));
    Port->OSPEEDR |= (U32) (Speed << (Pin * 2));

    //Pullup Pulldown
    Port->PUPDR &= (U32) ~(0x03 << (Pin * 2));
    Port->PUPDR |= (U32) (Pud << (Pin * 2));

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

void SYSCFG_map_EXTI(U32 line, U32 port)
{
    if(line < 4)
    {
        SYSCFG->EXTICR[0] &= (U32) ~(0x0F << (line * 4));
        SYSCFG->EXTICR[0] |= (U32)(port << (line * 4));
    }
    else if(line < 8)
    {
        SYSCFG->EXTICR[1] &= (U32) ~(0x0F << ((line -4) * 4));
        SYSCFG->EXTICR[1] |= (U32)(port << ((line -4) * 4));
    }
    else if(line < 12)
    {
        SYSCFG->EXTICR[2] &= (U32) ~(0x0F << ((line -8) * 4));
        SYSCFG->EXTICR[2] |= (U32)(port << ((line -8) * 4));
    }
    else if(line < 16)
    {
        SYSCFG->EXTICR[3] &= (U32) ~(0x0F << ((line -12) * 4));
        SYSCFG->EXTICR[3] |= (U32)(port << ((line -12) * 4));
    }
}


/**
helper functions
*/
void gpio_set_pin_high(GPIO_TypeDef * Port, VU32 Pin)
{
    //on
    Port->BSRR= (U32) (1<< Pin);
}

void gpio_set_pin_low(GPIO_TypeDef * Port, VU32 Pin)
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


/**
  * @brief  Changes the AF mapping of the specified pin.
  * @param  Port
  * @param  Pin: specifies the pin for the Alternate function where x can be (0..15).
  * @param  Function: sets the AF number (0..15)

  * @retval None
  */
void GPIO_SetAF(GPIO_TypeDef* Port, U16 Pin, U8 Function)
{
    //GPIO port pin range 0..15!
    if(Pin > 15)
    {
        return;
        //TODO log warning
    }

    //AF range 0..15!
    if(Function > 15)
    {
        return;
        //TODO log warning
    }


    if(Pin < 8)
    {
        Port->AFR[0] &= (U32) ~(0x0F << (Pin * 4));
        Port->AFR[0] |= (U32)(Function << (Pin * 4));
    }
    else
    {
        Port->AFR[1] &= (U32) ~(0x0F << ((Pin -8) * 4));
        Port->AFR[1] |= (U32)(Function << ((Pin -8) * 4));
    }

}
