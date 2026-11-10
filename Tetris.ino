#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include "Tetromino.h"
#include "Tetris_utils.h"

Tetris_utils utils;

game_states state = START_SCREEN;
uint8_t game[DATA_SIZE] = {0};
uint8_t game_start[DATA_SIZE] = {
  0, (1 << 7) | (1 << 6) | (1 << 5) | (1 << 3), (1 << 7) | (1 << 4) | (1 << 3), (1 << 7) | (1 << 4) | (1 << 3),
  (1 << 7) | (1 << 6) | (1 << 5) | (1 << 3), (1 << 7) | (1 << 3), (1 << 7) | 15, 0,
  0, (1 << 6) | (1 << 5) | (1 << 3) | 1, (1 << 7) | (1 << 4) | (1 << 3) | 1, (1 << 7) | (1 << 4) | 15,
  (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 2) | (1 << 1), (1 << 7) | (1 << 4) | (1 << 2) | (1 << 1),
  (1 << 7) | (1 << 4) | (1 << 2) | (1 << 1), 0,
  0, (1 << 5) | (1 << 2), (1 << 4) | (1 << 3), (1 << 4) | (1 << 3), (1 << 5) | (1 << 2), 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};
uint8_t game_over_screen[DATA_SIZE] = {
  0, (1 << 6) | (1 << 5) | (1 << 2) | (1 << 1), (1 << 7) | (1 << 4) | (1 << 3) | 1, (1 << 7) | (1 << 3) | 1,
  (1 << 7) | (1 << 5) | (1 << 4) | 15, (1 << 7) | (1 << 4) | (1 << 3) | 1, (1 << 6) | (1 << 5) | (1 << 3) | 1, 0,
  0, (1 << 7) | (1 << 4) | 7, (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 2), 247,
  (1 << 7) | (1 << 4) | (1 << 2), (1 << 7) | (1 << 4) | (1 << 2), (1 << 7) | (1 << 4) | 7, 0,
  0, (1 << 6) | (1 << 5) | (1 << 3) | 1, (1 << 7) | (1 << 4) | (1 << 3) | 1, (1 << 7) | (1 << 4) | (1 << 3) | 1,
  (1 << 7) | (1 << 4) | (1 << 3) | 1, (1 << 7) | (1 << 4) | (1 << 3) | 1, (1 << 6) | (1 << 5) | (1 << 2) | (1 << 1), 0,
  0, (1 << 7) | (1 << 6) | (1 << 5) | 14, (1 << 7) | (1 << 3) | 1, (1 << 7) | (1 << 6) | (1 << 5) | (1 << 3) | 1,
  (1 << 7) | 15, (1 << 7) | (1 << 3) | (1 << 1), (1 << 7) | (1 << 6) | (1 << 5) | (1 << 3) | 1, 0
};

Tetromino shape = I_Tetromino(3, 0);
volatile uint8_t drop_piece = 0;
uint16_t score = 0;
const uint16_t levels[30] = {
  32768, 28672, 24576, 20480, 18432, 15360, 12288, 8192, 5120, 4096,
  3072, 3072, 3072, 2660, 2660, 2660, 2045, 2045, 2045, 1364,
  1364, 1364, 1364, 1364, 1364, 1364, 1364, 1364, 1364, 682
};
volatile unsigned long int timer0_counter = 0;
unsigned long int threshold = 150;
volatile bool game_end = false;

void setup() {
  srand(1);
  TIMSK0 |= (1 << OCIE0A);
  TCCR0A = (1 << WGM01);
  TCCR0B = (1 << CS00) | (1 << CS01);
  OCR0A = 249;
  TCCR1B = (1 << WGM12) | (1 << CS12);
  TCCR1A = 0;
  TIMSK1 |= (1 << OCIE1A);
  OCR1A = 32768;
  PORTD = (1 << 4) | (1 << 7);
  DDRB = (1 << 2) | (1 << 3) | (1 << 5);
  PORTB = (1 << 2) | (1 << 3) | (1 << 5) | 1 | (1 << 4);
  SPCR = 0;
  SPCR = (1 << 4) | (1 << 6);
  utils.screenInit();
  utils.clearAllScreens();
  sei();
}

ISR (TIMER0_COMPA_vect) {
  timer0_counter++;
}

ISR (TIMER1_COMPA_vect) {
  if (score > 29 * 5) {
    score = 0;
    game_end = true; 
  }
  OCR1A = levels[score / 5];
  drop_piece = 1;
}

void loop() {
  switch (state) {

    case START_SCREEN: {
      utils.sendCommandAll(utils.convertData(game_start));
      if (utils.menuButtonPress(timer0_counter)) {
        state = GAMEPLAY;
        utils.clearAllScreens();
      }
      break;
    }

    case GAMEPLAY: {
      if (game_end)
      {
        state = GAME_OVER;
        utils.clearAllScreens();
        break;
      }
      utils.clearFromGame(game, shape);

      bool update = false;
      utils.buttonPresses(&shape, &update, timer0_counter, game);

      if (drop_piece)
      {
        if (!shape.move_down(game))
        {
          utils.assignToGame(game, shape);
          utils.checkForFullRows(game, &score);
          shape = utils.get_shape();
          game_end = utils.collisionCheck(game, shape);
        }
        drop_piece = 0;
        update = 1;
      }
      utils.assignToGame(game, shape);

      if (update)
      {
        utils.sendCommandAll(utils.convertData(game));
      }
      break;
    }

    case GAME_OVER: {
      utils.sendCommandAll(utils.convertData(game_over_screen));
      if (utils.menuButtonPress(timer0_counter)) {
        state = GAMEPLAY;
        score = 0;
        memset(game, 0, sizeof(uint8_t) * DATA_SIZE);
        game_end = false;

        utils.clearAllScreens();
      }
      break;
    }
  }
}
