#include <SoftwareSerial.h>

//motor drive
const uint8_t _motor_one_pin_ = 2;
const uint8_t _motor_two_pin_ = 3;
const uint8_t _motor_three_pin_ = 4;
const uint8_t _motor_four_pin_ = 5;
const uint8_t _motor_five_pin_ = 6;
const uint8_t _motor_six_pin_ = 7;

//radom number
uint8_t _random_byte_ = 0;

//number of test
uint8_t _num_of_test_ = 2;   // 0 to 2 (3 test)
uint8_t _test_number_ = 1;


//check peripherals
const uint8_t _check_peripherals_ = 0xFF;

//dispay command
const uint8_t _take_temerature_test_ = 0x01;
const uint8_t _start_smell_test_ = 0x02;
const uint8_t _test_one_ = 0x03;
const uint8_t _test_one_pass_ = 0x04;
const uint8_t _test_one_fail_ = 0x05;
const uint8_t _test_two_ = 0x06;
const uint8_t _test_two_pass_ = 0x07;
const uint8_t _test_two_fail_ = 0x08;
const uint8_t _test_three_ = 0x09;
const uint8_t _test_three_pass_ = 0x0a;
const uint8_t _test_three_fail_ = 0x0b;
const uint8_t  _final_result_ = 0x0c;
const uint8_t _final_result_pass_ = 0x0d;
const uint8_t _final_result_fail_ = 0x0e;
const uint8_t  _check_display_ = 0x0F;
const uint8_t  _clear_display_ = 0x10;

//display Serial
uint8_t _display_RX_pin_ = A1;
uint8_t _display_TX_pin_ = A2;
SoftwareSerial display_Serial(_display_RX_pin_, _display_TX_pin_); // RX, TX
uint32_t display_baudrate = 115200 ;

//step serial
uint8_t _step_init_ = 0x01;
uint32_t _step_response_delay_ = 10000;
uint8_t _step_value_ = 0;
uint8_t _step_RX_pin_ = 9;
uint8_t _step_TX_pin_ = 8;
SoftwareSerial step_Serial(_step_RX_pin_, _step_TX_pin_); // RX, TX
uint32_t step_baudrate = 115200 ;

//temp serial
uint8_t _temp_RX_pin_ = 11;
uint8_t _temp_TX_pin_ = 10;
SoftwareSerial temp_Serial(_temp_RX_pin_, _temp_TX_pin_); // RX, TX
uint32_t temp_baudrate = 115200 ;

//debug serial
uint32_t debug_badrate = 115200 ;

bool _test_;
uint8_t _test_passed = 0;


void setup()
{
  pinMode(_motor_one_pin_, OUTPUT);
  pinMode(_motor_two_pin_, OUTPUT);
  pinMode(_motor_three_pin_, OUTPUT);
  pinMode(_motor_four_pin_, OUTPUT);
  pinMode(_motor_five_pin_, OUTPUT);
  pinMode(_motor_six_pin_, OUTPUT);
  Serial.begin(debug_badrate);
  step_Serial.begin(step_baudrate);
  display_Serial.begin(display_baudrate);
  temp_Serial.begin(temp_baudrate);
  randomSeed(analogRead(5));
  //check_peripherals();
  display_send(_take_temerature_test_);
  delay(500);
}

uint8_t check_peripherals()
{
  display_send(_check_peripherals_);
}

uint8_t display_test_ini()
{
  Serial.print("                 Test = ");
  Serial.println(_test_number_);
  if (_test_number_ == 1)
  {
    display_send(_test_one_);
  }
  else if (_test_number_ == 2)
  {
    display_send(_test_two_);
  }
  else if (_test_number_ == 3)
  {
    display_send(_test_three_);
  }
}

uint8_t drive_motor()
{
  switch (_random_byte_)
  {
    case 1:
      digitalWrite(_motor_one_pin_, HIGH);
      delay(500);
      digitalWrite(_motor_one_pin_, LOW);
      break;
    case 2:
      digitalWrite(_motor_two_pin_, HIGH);
      delay(500);
      digitalWrite(_motor_two_pin_, LOW);
      break;
    case 3:
      digitalWrite(_motor_three_pin_, HIGH);
      delay(500);
      digitalWrite(_motor_three_pin_, LOW);
      break;
    case 4:
      digitalWrite(_motor_four_pin_, HIGH);
      delay(500);
      digitalWrite(_motor_four_pin_, LOW);
      break;
    case 5:
      digitalWrite(_motor_five_pin_, HIGH);
      delay(500);
      digitalWrite(_motor_five_pin_, LOW);
      break;
    case 6:
      digitalWrite(_motor_six_pin_, HIGH);
      delay(500);
      digitalWrite(_motor_six_pin_, LOW);
      break;
  }
}

uint8_t generate_radom_byte()
{
  uint8_t _old_random_byte_ = _random_byte_;
  _random_byte_ = random(1, 7);
  while ( _old_random_byte_ == _random_byte_)
  {
    _random_byte_ = random(1, 7);
  }
  Serial.print("                      Radom Number= ");
  Serial.println(_random_byte_);
}

uint8_t read_step_value()
{
  _step_value_ = 0;
  Serial.println("read_step_value");
  step_Serial.println(_step_init_);
  String _step_value_string_;
  uint32_t _start_time_ = millis();
  step_Serial.listen();
  while ((millis() - _start_time_) < _step_response_delay_)
  {
    if (step_Serial.available())
    {
      _step_value_string_ = (step_Serial.readString());
      _step_value_ = _step_value_string_.toInt();
      Serial.print("      Step value Rec = ");
      Serial.println(_step_value_);
      if (_step_value_ <= 6)
      {
        return;
      }
      if (_step_value_ == 7)
      {
        drive_motor();
      }

    }
  }
}

uint8_t compare_values()
{
  _test_ = false;
  Serial.println("compare_values");
  if (_random_byte_ == _step_value_)
  {
    _test_  = true;
    _test_passed ++;
  }
  else
  {
    _test_ = false;
  }
  Serial.print("Test result = ");
  Serial.println(_test_);
}

uint8_t display_command()
{
  if (_test_number_ == 1)
  {
    if (_test_)
    {
      display_send(_test_one_pass_);
    }
    if (!_test_)
    {
      display_send(_test_one_fail_);
    }
  }
  if (_test_number_ == 2)
  {
    if (_test_)
    {
      display_send(_test_two_pass_);
    }
    if (!_test_)
    {
      display_send(_test_two_fail_);
    }
  }
  if (_test_number_ == 3)
  {
    if (_test_)
    {
      display_send(_test_three_pass_);
    }
    if (!_test_)
    {
      display_send(_test_three_fail_);
    }
  }
}

uint8_t display_send(uint8_t _command_)
{
  display_Serial.listen();
  delay(250);
  display_Serial.println(_command_);
  delay(1000);
}

uint8_t final_result()
{
  display_send(_final_result_);
  delay(1000);
  if (_test_passed == 2)
  {
    display_send(_final_result_pass_);
  }
  else
  {
    display_send(_final_result_fail_);
  }
}

void loop()
{
  //Serial.println("Take TEMP");
//  temp_Serial.listen();
//  delay(250);
//  if (temp_Serial.available())
//  {
    //Serial.println(temp_Serial.read());
    display_send(_start_smell_test_);
    delay(1000);
    for (int i = 0; i <= _num_of_test_; i++)
    {
      delay(1000);
      display_test_ini();
      generate_radom_byte();
      drive_motor();
      read_step_value();
      compare_values();
      display_command();
      _test_number_ ++;
      _random_byte_ = 0;
      if (_test_passed == 2)
      {
        break;
      }
    }
    delay(1000);
    final_result();
    Serial.println("                      One test complete!");
    _test_number_ = 1;
    _test_passed = 0;
    //display_send(_clear_display_);
    display_send(_take_temerature_test_);
    //delay(5000);
  //}
  delay(1000);
}
