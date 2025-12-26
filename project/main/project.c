#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "portmacro.h"
#include "soc/gpio_num.h"
#include <stdint.h>
#include <stdio.h>
#include "esp_rom_sys.h"
#include "hd44780.h"
#include "esp_timer.h"

#define LCD_RS 21
#define LCD_E  22
#define LCD_D4 18
#define LCD_D5 19
#define LCD_D6 23
#define LCD_D7 17

#define TRIG_PIN 26
#define ECHO_PIN 25

float get_distance_cm()
{
  gpio_set_level(TRIG_PIN, 0);
  esp_rom_delay_us(2);

  gpio_set_level(TRIG_PIN, 1);
  esp_rom_delay_us(10);
  gpio_set_level(TRIG_PIN, 0);

  int64_t timeout = esp_timer_get_time() + 60000; // 60 ms
  while (gpio_get_level(ECHO_PIN) == 0) {
      if (esp_timer_get_time() > timeout) return -1;
  }

  int64_t start = esp_timer_get_time();

  timeout = esp_timer_get_time() + 60000; // 60 ms
  while (gpio_get_level(ECHO_PIN) == 1) {
      if (esp_timer_get_time() > timeout) return -1;
  }

  int64_t end = esp_timer_get_time();
  int64_t duration = end - start;

  float distance_cm = duration / 58.0f;
  return distance_cm;
}

void app_main(void)
{
  hd44780_t lcd = {
      .pins = {
          .rs = LCD_RS,
          .e  = LCD_E,
          .d4 = LCD_D4,
          .d5 = LCD_D5,
          .d6 = LCD_D6,
          .d7 = LCD_D7,
          .bl = HD44780_NOT_USED
      },
      .lines = 2,
      .font  = HD44780_FONT_5X8,
      .backlight = false,
      .write_cb = NULL
  };

  if (hd44780_init(&lcd) != ESP_OK) {
      printf("LCD init failed!\n");
      return;
  }

  hd44780_clear(&lcd);

  gpio_set_direction(TRIG_PIN, GPIO_MODE_OUTPUT);
  gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT);

  char str[30];

  bool timeout = false;

  while (1)
  {
    float distance = get_distance_cm();
    if (distance < 0) {
      if(!timeout)
      {
        hd44780_clear(&lcd);
        hd44780_puts(&lcd, "Timeout, < 0");
        timeout = true;
      }
    } else {
        timeout = true;
        hd44780_clear(&lcd);
        snprintf(str, sizeof(str), "Distance: %d", (int)distance);
        hd44780_puts(&lcd, str);
    }
    vTaskDelay(pdMS_TO_TICKS(200)); // 5 pomiarów na sekundę
  }
}
