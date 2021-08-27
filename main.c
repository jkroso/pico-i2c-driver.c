#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include <stdlib.h>
#include <stdio.h>

char wait_for_char() {
  while (true) {
    int c = getchar_timeout_us(0); // timeout > 0 will lock stdin for some reason
    if (c > -1) return (char)c;
    else sleep_us(10);
  }
}

uint8_t* buffer_bytes(uint8_t nb) {
  uint8_t *b = malloc(nb+1);
  b[nb] = 0;
  for (int i = 0; i < nb; i++) {
    b[i] = wait_for_char();
  }
  return b;
}

int main() {
  stdio_init_all();
  i2c_init(i2c_default, 100000); // SDA: pin 4, SCL: pin 5 on a Pico
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

  while (true) {
    char cmd = wait_for_char();

    // echo: e<byte>
    if (cmd == 'e') {
      putchar(wait_for_char());
      continue;
    }

    // write: w<addr><len><s|c><data...>
    if (cmd == 'w') {
      uint8_t addr = wait_for_char();
      uint8_t len = wait_for_char();
      bool nostop = wait_for_char() == 'c';
      uint8_t *data = buffer_bytes(len);
      int written = i2c_write_blocking(i2c_default, addr, data, len, nostop);
      putchar(written == len ? 0x0 : 0x1); // error reporting
      free(data);
      continue;
    }

    // read: r<addr><len><s|c>
    if (cmd == 'r') {
      uint8_t addr = wait_for_char();
      uint8_t len = wait_for_char();
      bool nostop = wait_for_char() == 'c';
      char data[len];
      int read = i2c_read_blocking(i2c_default, addr, data, len, nostop);
      putchar(read == len ? 0x0 : 0x1); // error reporting
      for (int j = 0; j < len; j++) putchar(data[j]);
      continue;
    }
  }
  return 0;
}
