#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include <stdlib.h>
#include <stdio.h>

uint8_t* buffer_bytes(uint8_t nb) {
  uint8_t *b = malloc(nb+1);
  b[nb] = 0;
  for (int i = 0; i < nb; i++) {
    b[i] = getchar();
  }
  return b;
}

void write_command() {
  uint8_t addr = getchar();
  uint8_t len = getchar();
  bool nostop = getchar() == 'c';
  uint8_t *data = buffer_bytes(len);
  int written = i2c_write_blocking(i2c_default, addr, data, len, nostop);
  putchar((uint8_t)(written == len)); // error reporting
  free(data);
}

void read_command() {
  uint8_t addr = getchar();
  uint8_t len = getchar();
  bool nostop = getchar() == 'c';
  uint8_t *data = malloc(len);
  int read = i2c_read_blocking(i2c_default, addr, data, len, nostop);
  if (read == len) {
    putchar(0x01); // okay
    for (int i = 0; i < len; i++) putchar(data[i]);
  } else {
    putchar(0x00); // error
  }
  free(data);
}

int main() {
  stdio_init_all();
  i2c_init(i2c_default, 100000); // SDA: pin 4, SCL: pin 5 on a Pico
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

  while (true) {
    char cmd = getchar();
    // echo: e<byte>
    if (cmd == 'e') putchar(getchar());
    // write: w<addr><len><s|c><data...>
    if (cmd == 'w') write_command();
    // read: r<addr><len><s|c>
    if (cmd == 'r') read_command();
  }
  return 0;
}
