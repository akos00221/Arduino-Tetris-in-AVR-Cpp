#include "Tetromino.h"

Tetromino::Tetromino(uint8_t p_x, uint8_t p_y, const RotationState *shape_states) : pos_x(p_x), pos_y(p_y)
{
  states = shape_states;
  for (uint8_t i = 0; i < 4; ++i)
  {
    state[i] = states[state_ind].representation[i];
  }
}

void Tetromino::rotate_left(uint8_t game[])
{
  uint8_t next = (state_ind + 3) % 4;
  if (states[next].offset_y + states[next].heigth + pos_y > 31)
  {
    return;
  }
  else if ((pos_x + 4 - states[next].offset_x - states[next].width) < 0)
  {
    return;
  }
  else if ((pos_x - states[next].offset_x) > 4)
  {
    return;
  }

  uint8_t j = 0;
  for (uint8_t i = pos_y; i < pos_y + 4; ++i)
  {
    if (pos_x >= 0)
    {
      if (((states[next].representation[j] << pos_x) & game[i]) > 0)
      {
        return;
      }
    }
    else
    {
      if (((states[next].representation[j] >> (-pos_x)) & game[i]) > 0)
      {
        return;
      }
    }

    j++;
  }

  state_ind = next;
  for (uint8_t i = 0; i < 4; ++i)
  {
    state[i] = states[state_ind].representation[i];
  }
}

void Tetromino::rotate_right(uint8_t game[])
{
  uint8_t next = (state_ind + 1) % 4;
  if (states[next].offset_y + states[next].heigth + pos_y > 31)
  {
    return;
  }
  else if ((pos_x + 4 - states[next].offset_x - states[next].width) < 0)
  { 
    return;
  }
  else if ((pos_x - states[next].offset_x) > 4)
  {
    return;
  }

  uint8_t j = 0;
  for (uint8_t i = pos_y; i < pos_y + 4; ++i)
  {
    if (pos_x >= 0)
    {
      if (((states[next].representation[j] << pos_x) & game[i]) > 0)
      {
        return;
      }
    }
    else
    {
      if (((states[next].representation[j] >> (-pos_x)) & game[i]) > 0)
      {
        return;
      }
    }

    j++;
  }

  state_ind = next;
  for (uint8_t i = 0; i < 4; ++i)
  {
    state[i] = states[state_ind].representation[i];
  }
}

void Tetromino::move_left(uint8_t game[])
{
  if (pos_x + 1 - states[state_ind].offset_x <= 4)
  {
    uint8_t j = 0;
    for (uint8_t i = pos_y; i < pos_y + 4; ++i)
    {
      if (pos_x + 1 >= 0)
      {
        if (((states[state_ind].representation[j] << (pos_x + 1)) & game[i]) > 0)
        {
          return;
        }
      }
      else
      {
        if (((states[state_ind].representation[j] >> -(pos_x + 1)) & game[i]) > 0)
        {
          return;
        }
      }
      j++;
    }
    pos_x++;
  }
}

void Tetromino::move_right(uint8_t game[])
{
  if (pos_x + 4 - states[state_ind].offset_x - states[state_ind].width > 0)
  {
    uint8_t j = 0;
    for (uint8_t i = pos_y; i < pos_y + 4; ++i)
    {
      if (pos_x - 1 >= 0)
      {
        if (((states[state_ind].representation[j] << (pos_x - 1)) & game[i]) > 0)
        {
          return;
        }
      }
      else
      {
        if (((states[state_ind].representation[j] >> -(pos_x - 1)) & game[i]) > 0)
        {
          return;
        }
      }

      j++;
    }
    pos_x--;
  }
}

bool Tetromino::move_down(uint8_t game[])
{
  if ((pos_y + states[state_ind].offset_y + states[state_ind].heigth) > 31)
  {
    return false;
  }
  uint8_t j = 0;
  for (uint8_t i = pos_y + 1; i < pos_y + 1 + 4; ++i)
  {
    if (pos_x >= 0)
    {
      if (((states[state_ind].representation[j] << pos_x) & game[i]) > 0)
      {
        return false;
      }
    }
    else
    {
      if (((states[state_ind].representation[j] >> (-pos_x)) & game[i]) > 0)
      {
        return false;
      }
    }
    j++;
  }
  pos_y++;
  return true;
}

const RotationState I_Tetromino::i_states[4] = {
  {0, 4, 1, 1, {0, 15, 0, 0}},
  {2, 1, 0, 4, {2, 2, 2, 2}},
  {0, 4, 2, 1, {0, 0, 15, 0}},
  {1, 1, 0, 4, {4, 4, 4, 4}}
};
I_Tetromino::I_Tetromino(uint8_t p_x, uint8_t p_y) : Tetromino(p_x, p_y, i_states) {}

const RotationState O_Tetromino::o_states[4] = {
  {1, 2, 1, 2, {0, 6, 6, 0}},
  {1, 2, 1, 2, {0, 6, 6, 0}},
  {1, 2, 1, 2, {0, 6, 6, 0}},
  {1, 2, 1, 2, {0, 6, 6, 0}}
};
O_Tetromino::O_Tetromino(uint8_t p_x, uint8_t p_y) : Tetromino(p_x, p_y, o_states) {}

const RotationState L_Tetromino::l_states[4] = {
  {1, 3, 0, 2, {1, 7, 0, 0}},
  {2, 2, 0, 3, {2, 2, 3, 0}},
  {1, 3, 1, 2, {0, 7, 4, 0}},
  {1, 2, 0, 3, {6, 2, 2, 0}}
};
L_Tetromino::L_Tetromino(uint8_t p_x, uint8_t p_y) : Tetromino(p_x, p_y, l_states) {}

const RotationState Z_Tetromino::z_states[4] = {
  {0, 3, 1, 2, {0, 12, 6, 0}},
  {1, 2, 0, 3, {2, 6, 4, 0}},
  {0, 3, 2, 2, {0, 0, 12, 6}},
  {1, 2, 0, 3, {4, 6, 2, 0}}
};
Z_Tetromino::Z_Tetromino(uint8_t p_x, uint8_t p_y) : Tetromino(p_x, p_y, z_states) {}

const RotationState J_Tetromino::j_states[4] = {
  {1, 3, 0, 2, {4, 7, 0, 0}},
  {1, 2, 0, 3, {6, 4, 4, 0}},
  {1, 3, 1, 2, {0, 7, 1, 0}},
  {1, 2, 1, 3, {0, 2, 2, 6}}
};
J_Tetromino::J_Tetromino(uint8_t p_x, uint8_t p_y) : Tetromino(p_x, p_y, j_states) {}

const RotationState S_Tetromino::s_states[4] = {
  {0, 3, 1, 2, {0, 6, 12, 0}},
  {0, 2, 0, 3, {8, 12, 4, 0}},
  {0, 3, 2, 2, {0, 0, 6, 12}},
  {1, 2, 0, 3, {2, 6, 4, 0}}
};
S_Tetromino::S_Tetromino(uint8_t p_x, uint8_t p_y) : Tetromino(p_x, p_y, s_states) {}

const RotationState T_Tetromino::t_states[4] = {
  {0, 3, 0, 2, {4, 14, 0, 0}},
  {1, 2, 0, 3, {4, 6, 4, 0}},
  {1, 3, 1, 2, {0, 7, 2, 0}},
  {1, 2, 0, 3, {2, 6, 2, 0}}
};
T_Tetromino::T_Tetromino(uint8_t p_x, uint8_t p_y) : Tetromino(p_x, p_y, t_states) {}
