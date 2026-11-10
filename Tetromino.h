#ifndef TETROMINO_H
#define TETROMINO_H

#include <avr/io.h>
#include <stdint.h>

#define DATA_SIZE 32

struct RotationState
{
  uint8_t offset_x;
  uint8_t width;
  uint8_t offset_y;
  uint8_t heigth;
  uint8_t representation[4];
};

class Tetromino
{
  public:
    int8_t pos_x, pos_y;
    uint8_t state[4];
  protected:
    uint8_t state_ind = 0;
    const RotationState* states;
  public:
    Tetromino(uint8_t p_x, uint8_t p_y, const RotationState* states);
    void rotate_left(uint8_t game[]);
    void rotate_right(uint8_t game[]);
    void move_left(uint8_t game[]);
    void move_right(uint8_t game[]);
    bool move_down(uint8_t game[]);
};

class I_Tetromino : public Tetromino {
  private:
    static const RotationState i_states[4];
  public:
    I_Tetromino(uint8_t p_x, uint8_t pos_y);
};

class O_Tetromino : public Tetromino {
  private:
    static const RotationState o_states[4];
  public:
    O_Tetromino(uint8_t p_x, uint8_t pos_y);
};

class L_Tetromino : public Tetromino {
  private:
    static const RotationState l_states[4];
  public:
    L_Tetromino(uint8_t p_x, uint8_t pos_y);
};

class Z_Tetromino : public Tetromino {
  private:
    static const RotationState z_states[4];
  public:
    Z_Tetromino(uint8_t p_x, uint8_t pos_y);
};

class J_Tetromino : public Tetromino {
  private:
    static const RotationState j_states[4];
  public:
    J_Tetromino(uint8_t p_x, uint8_t pos_y);
};

class S_Tetromino : public Tetromino {
  private:
    static const RotationState s_states[4];
  public:
    S_Tetromino(uint8_t p_x, uint8_t pos_y);
};

class T_Tetromino : public Tetromino {
  private:
    static const RotationState t_states[4];
  public:
    T_Tetromino(uint8_t p_x, uint8_t pos_y);
};

#endif // TETROMINO_H
