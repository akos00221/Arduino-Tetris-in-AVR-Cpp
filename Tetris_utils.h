#ifndef TETRIS_UTILS_H
#define TETRIS_UTILS_H

#include <avr/io.h>
#include <stdint.h>
#include "Tetromino.h"

enum control_registers {
  DECODE_MODE = 0x09,
  INTENSITY,
  SCAN_LIMIT,
  SHUTDOWN,
  DISPLAY_TEST = 0x0F
};

enum game_states {
  START_SCREEN,
  GAMEPLAY,
  GAME_OVER
};

class Tetris_utils
{
  private:
    unsigned long int prev_move_r = 0;
    unsigned long int prev_move_l = 0;
    unsigned long int prev_rotate_r = 0;
    unsigned long int prev_rotate_l = 0;
    unsigned long int threshold = 150;

  public:
    void clearAllScreens();
    void screenInit();
    void sendCommandAll(uint8_t address, uint8_t data);
    void sendCommandAll(uint8_t data[]);
    uint8_t *convertData(uint8_t data[]);
    Tetromino get_shape();
    void assignToGame(uint8_t game[], Tetromino shape);
    void clearFromGame(uint8_t game[], Tetromino shape);
    bool collisionCheck(uint8_t game[], Tetromino shape);
    void checkForFullRows(uint8_t game[], uint16_t *score);
    void buttonPresses(Tetromino *shape, bool *update, unsigned long int timer0_counter, uint8_t game[]);
    bool menuButtonPress(unsigned long int timer0_counter);
};

#endif // TETRIS_UTILS_H
