#ifndef __HP2055_H
#define __HP2055_H

#define rst( x ) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, x );
#define rs( x ) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, x );
#define cs( x ) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, x );

#define LCD_WIDTH              16
#define LCD_HEIGHT             2

void lcd_send_cmd(uint8_t cmd) {
  uint8_t buf[1] = { cmd };
  rs(0); // low for command
  cs(0);
  HAL_SPI_Transmit(&hspi1, buf, 1, 1000); // see spi.c for config spi
  cs(1);
}

void lcd_send_data(uint8_t data) {
  uint8_t buf[1] = { data };
  rs(1); // high for data
  cs(0);
  HAL_SPI_Transmit(&hspi1, buf, 1, 1000); // see spi.c for config spi
  cs(1);
}

void lcd_init() {
  rst(0); // low for reset
  rs(0); // low for command, high for data
  cs(1); // low before send
  HAL_Delay(20);
  rst(1);
  HAL_Delay(20);

  lcd_send_cmd(0b00111000); // 8 bit, 2 lines, 8px font, extend instruction set
  HAL_Delay(1);
  lcd_send_cmd(0b00111001); // repeat
  HAL_Delay(1);
  lcd_send_cmd(0b00010000); // bias 1/5, 122fps
  HAL_Delay(1);
  lcd_send_cmd(0b01111010); // set contrast, four right bit is low bits of contrast
  HAL_Delay(1);
  lcd_send_cmd(0b01011110); // i dont know what is it, two right bits is high bits of contrast
  HAL_Delay(1);
  lcd_send_cmd(0b01101100); // see datasheet on "folower control"
  HAL_Delay(1);
  lcd_send_cmd(0b00001100); // on display, off cursor, off blink
  HAL_Delay(1);
  lcd_send_cmd(0b00000001); //clear display
}

void lcd_set_cursor(uint8_t x, uint8_t y) {
  if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT)) return;
  lcd_send_cmd( 0x80 | (y << 6) | x ); // 0x80 is register for cursor
}

void lcd_send_str(char *str) {
  while (*str) {
    lcd_send_data(*str); // *str is one char byte
    str++;
  }
}

#endif /* __HP2055_H */