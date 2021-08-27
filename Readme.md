# pico-i2c-driver.c

This code turns a Raspberry Pi Pico into a USB slave and a I²C master. You send it commands over USB serial and it forwards them onto it's own I²C bus.

Pin Map:

```
GP4 <-> SDA
GP5 <-> SCL
3V3(Out) <-> VCC
Ground <-> Ground
```

# Commands

`e<byte>`: this is so you can test the USB connection. It just echoes a byte.

`w<addr><length><stop|continue><data...>`: write data to an I²C device

  1. `addr`: address byte of the I²C device you want to write to
  2. `length`: the number of bytes you will send it
  3. `stop`: a byte `0x73` or `0x63`
      1. `0x73` if you want to close the connection with the slave after this transaction
      2. `0x63` if you want to keep it open
  4. `data` any bytes you want to send to the slave. The number of bytes should match `length`

`r<addr><length><stop|continue>`: read data from an I²C device

  1. `addr`: address byte of the I²C device you want to read from
  2. `length`: the number of bytes you will read from the slave
  3. `stop` a byte `0x73` or `0x63`
      1. `0x73` if you want to close the connection with the slave after this transaction
      2. `0x63` if you want to keep it open
