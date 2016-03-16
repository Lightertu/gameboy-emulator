#ifndef KEY_PRESSES_GB_H_
#define KEY_PRESSES_GB_H_


class KEYS {
public:

  KEYS();
  virtual~KEYS();

  static void initialize_keys();
  static void reset_keys();
  static void write_keys(uint8_t value);
  static uint8_t read_keys();
  
  static uint8_t key_up;
  static uint8_t key_down;
  static uint8_t key_left;
  static uint8_t key_right;

  static uint8_t key_start;
  static uint8_t key_select;
  static uint8_t key_a;
  static uint8_t key_b;

  static uint8_t keys[2];

  static uint8_t column;

};

#endif
