#include "Tetris_utils.h"
#include <stdlib.h>

void Tetris_utils::sendCommandAll(uint8_t address, uint8_t data) {
  PORTB &= ~(1 << 2);
  for (uint8_t screen = 0; screen < 4; screen++) {
    SPDR = address;
    while (!(SPSR & (1 << 7))) {}
    SPDR = data;
    while (!(SPSR & (1 << 7))) {}
  }
  PORTB |= (1 << 2);
}

void Tetris_utils::clearAllScreens() {
  for (uint8_t row = 1; row <= 8; ++row) {
    sendCommandAll(row, 0);
  }
}

void Tetris_utils::screenInit() {
  sendCommandAll(SHUTDOWN, 1);
  sendCommandAll(DISPLAY_TEST, 0);
  sendCommandAll(DECODE_MODE, 0);
  sendCommandAll(INTENSITY, 0);
  sendCommandAll(SCAN_LIMIT, 7);
}

void Tetris_utils::sendCommandAll(uint8_t data[]) {
  for (int i = 1; i <= 8; ++i) {
    PORTB &= ~(1 << 2);
    for (uint8_t screen = 0; screen < 4; screen++) {
      SPDR = i;
      while (!(SPSR & (1 << 7))) {}
      SPDR = data[screen * 8 + i - 1];
      while (!(SPSR & (1 << 7))) {}
    }
    PORTB |= (1 << 2);
  }
}

uint8_t* Tetris_utils::convertData(uint8_t data[]) {
  static uint8_t data_copy[DATA_SIZE];
  for (uint8_t screen = 0; screen < 4; ++screen) {
    for (uint8_t digit = 0; digit < 8; ++digit) {
      uint8_t num = 0;
      for (uint8_t row = 0; row < 8; row++) {
        uint8_t bit = (data[screen * 8 + row] >> digit) & 1;
        num += (bit << (7 - row));
      }
      data_copy[screen * 8 + (digit)] = num;
    }
  }
  return data_copy;
}

Tetromino Tetris_utils::get_shape() {
  uint8_t shape_ind = rand() % 7;
  switch (shape_ind) {
    case 0:
      return I_Tetromino(2, 0);
    case 1:
      return O_Tetromino(2, 0);
    case 2:
      return L_Tetromino(2, 0);
    case 3:
      return Z_Tetromino(2, 0);
    case 4:
      return J_Tetromino(2, 0);
    case 5:
      return S_Tetromino(2, 0);
    default:
      return T_Tetromino(2, 0);
  }
}

void Tetris_utils::assignToGame(uint8_t game[], Tetromino shape) {
  uint8_t j = 0;
  for (uint8_t i = shape.pos_y; i < shape.pos_y + 4; ++i)
  {
    if (i < 32)
    {
      if (shape.pos_x >= 0)
      {
        game[i] |= (shape.state[j] << shape.pos_x);
      }
      else
      {
        game[i] |= (shape.state[j] >> (-shape.pos_x));
      }
    }
    j++;
  }
}

void Tetris_utils::clearFromGame(uint8_t game[], Tetromino shape) {
  uint8_t j = 0;
  for (uint8_t i = shape.pos_y; i < shape.pos_y + 4; ++i)
  {
    if (i < 32)
    {
      if (shape.pos_x >= 0)
      {
        game[i] &= ~(shape.state[j] << shape.pos_x);
      }
      else
      {
        game[i] &= ~(shape.state[j] >> (-shape.pos_x));
      }
    }
    j++;
  }
}

bool Tetris_utils::collisionCheck(uint8_t game[], Tetromino shape) {
  uint8_t j = 0;
  for (uint8_t i = shape.pos_y; i < shape.pos_y + 4; ++i)
  {
    if (i < 32)
    {
      if (shape.pos_x >= 0)
      {
        if (((shape.state[j] << shape.pos_x) & game[i]) > 0)
        {
          return true;
        }
      }
      else
      {
        if (((shape.state[j] >> (-shape.pos_x)) & game[i]) > 0)
        {
          return true;
        }
      }
    }
    j++;
  }
  return false;
}

void Tetris_utils::checkForFullRows(uint8_t game[], uint16_t *score) {
  for (int8_t i = DATA_SIZE - 1; i >= 0; i--)
  {
    if (game[i] == 255)
    {
      for (int8_t j = i; j > 0; j--)
      {
        game[j] = game[j - 1];
      }
      game[0] = 0;
      *score += 1;
      i++;
    }
  }
}

void Tetris_utils::buttonPresses(Tetromino *shape, bool *update, unsigned long int timer0_counter, uint8_t game[]) {
  if ((PINB & (1 << 4)) == 0 && (timer0_counter - prev_move_r) > threshold)
  {
    shape->move_right(game);
    prev_move_r = timer0_counter;
    *update = true;
  }
  if ((PIND & (1 << 7)) == 0 && (timer0_counter - prev_move_l > threshold))
  {
    shape->move_left(game);
    prev_move_l = timer0_counter;
    *update = true;
  }
  if ((PIND & (1 << 4)) == 0 && (timer0_counter - prev_rotate_r > threshold))
  {
    shape->rotate_right(game);
    prev_rotate_r = timer0_counter;
    *update = true;
  }
  if ((PINB & (1 << 0)) == 0 && (timer0_counter - prev_rotate_l > threshold))
  {
    shape->rotate_left(game);
    prev_rotate_l = timer0_counter;
    *update = true;
  }
}

bool Tetris_utils::menuButtonPress(unsigned long int timer0_counter)
{
  if ((PINB & (1 << 4)) == 0 && (timer0_counter - prev_move_r) > threshold)
  {
    prev_move_r = timer0_counter;
    return true;
  }
  if ((PIND & (1 << 7)) == 0 && (timer0_counter - prev_move_l > threshold))
  {
    prev_move_l = timer0_counter;
    return true;
  }
  if ((PIND & (1 << 4)) == 0 && (timer0_counter - prev_rotate_r > threshold))
  {
    prev_rotate_r = timer0_counter;
    return true;
  }
  if ((PINB & (1 << 0)) == 0 && (timer0_counter - prev_rotate_l > threshold))
  {
    prev_rotate_l = timer0_counter;
    return true;
  }
  return false;
}
