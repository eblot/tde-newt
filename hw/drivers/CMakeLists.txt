#-----------------------------------------------------------------------------
# Newt drivers
#
# @file CMakeLists.txt
#-----------------------------------------------------------------------------

use_sdk_headers (core/hw/mcu/nordic/nrf52xxx
                 core/hw/bsp/${NEWT_BSP}
                 core/hw/drivers/crypto
                 core/hw/drivers/crypto/crypto_${NEWT_SOC}
                 core/hw/drivers/pwm
                 core/hw/drivers/pwm/pwm_${NEWT_SOC}
                 core/hw/drivers/rtt
                 core/hw/drivers/debounce
                 core/hw/drivers/trng
                 core/hw/drivers/trng/trng_${NEWT_SOC}
                 core/hw/drivers/adc
                 core/hw/drivers/adc/adc_${NEWT_SOC})

build_component_from (
  adc/adc_nrf52/src/adc_nrf52.c
  adc/src/adc.c
  crypto/crypto_nrf52/src/crypto_nrf52.c
  crypto/src/crypto.c
  crypto/src/mbedtls_aes_alt.c
  debounce/src/debounce.c
  pwm/pwm_nrf52/src/pwm_nrf52.c
  pwm/src/pwm.c
  rtt/src/SEGGER_RTT.c
  rtt/src/SEGGER_RTT_printf.c
  trng/src/trng.c
  trng/trng_nrf52/src/trng_nrf52.c
  uart/src/uart.c
  uart/uart_hal/src/uart_hal.c
)
