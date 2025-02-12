#include <ESP32Encoder.h>

#define CLK_1 14  // CLK ENCODER 1 (position MEASUREMENT)
#define DT_1 27   // DT ENCODER 1
#define CLK_2 26  // CLK ENCODER 2 (angle MEASUREMENT)
#define DT_2 25   // DT ENCODER 2
#define MOTOR_CLOCKWISE_PIN 12
#define MOTOR_COUNTERCLOCKWISE_PIN 32
#define ENCODER_ENABLE_PIN 33
#define MOTOR_ENABLE_PIN 13
#define ENCODER_PULSES_PER_REV 2048
#define SAMPLE_TIME 20
#define MAX_INPUT 3

typedef struct states_t {
  float position = 0;
  float angle = 0;
  float linear_speed = 0;
  float angular_speed = 0;
};


states_t states;

ESP32Encoder encoder_angle;
ESP32Encoder encoder_position;

float lqr_controller() {
  float K_1 = -17.7241, K_2 = -19.7739, K_3 = -13.3649, K_4 = -2.8352;

  float input = -(K_1 * states.position + K_2 * states.angle + K_3 * states.linear_speed + K_4 * states.angular_speed);

  return input;
}

void setup() {
  //encoders setup
  encoder_angle.attachHalfQuad(DT_2, CLK_2);
  encoder_angle.setCount(0);
  encoder_position.attachHalfQuad(DT_1, CLK_1);
  encoder_position.setCount(0);

  //motor setup
  pinMode(MOTOR_CLOCKWISE_PIN, OUTPUT);
  pinMode(MOTOR_COUNTERCLOCKWISE_PIN, OUTPUT);
  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(ENCODER_ENABLE_PIN, OUTPUT);

  digitalWrite(MOTOR_ENABLE_PIN, HIGH);
  digitalWrite(ENCODER_ENABLE_PIN, HIGH);
  analogWrite(MOTOR_CLOCKWISE_PIN, 0);
  analogWrite(MOTOR_COUNTERCLOCKWISE_PIN, 0);

  Serial.begin(115200);

  //test
  pinMode(2, OUTPUT);
}

float calc_angle_from_count(long encoder_angle_count) {
  if (encoder_angle_count < 0) return float(encoder_angle_count % ENCODER_PULSES_PER_REV + ENCODER_PULSES_PER_REV) * 2 * 3.141592 / ENCODER_PULSES_PER_REV - 3.141592;

  return float(encoder_angle_count % ENCODER_PULSES_PER_REV) * 2 * 3.141592 / ENCODER_PULSES_PER_REV - 3.141592;
}

void motor_write_pwm(float input) {

  digitalWrite(MOTOR_COUNTERCLOCKWISE_PIN, LOW);
  digitalWrite(MOTOR_CLOCKWISE_PIN, LOW);

  if (input == 0) {
    digitalWrite(MOTOR_ENABLE_PIN, LOW);
    return;
  }

  digitalWrite(MOTOR_ENABLE_PIN, HIGH);
  int input_pwm = (int)abs(input / 12 * 255); 
  
  Serial.print(input);
  Serial.print(" (V) ");
  Serial.print(input_pwm);
  Serial.println(" (PWM)");

  analogWrite((input < 0) ? MOTOR_CLOCKWISE_PIN : MOTOR_COUNTERCLOCKWISE_PIN, input_pwm);
}

void display(float input) {
  Serial.print("Angle: ");
  Serial.print(states.angle);
  Serial.print(" Posiion: ");
  Serial.print(states.position);
  Serial.print(" Angular Speed: ");
  Serial.print(states.angular_speed);
  Serial.print(" Linear Speed: ");
  Serial.print(states.linear_speed);
  Serial.print(" Entrada PWM: ");
  Serial.println(input);
}

float position_reference = 0;
float position_integral_error = 0;

void loop() {
  long encoder_angle_count = -encoder_angle.getCount();
  long encoder_position_count = encoder_position.getCount();
  static unsigned long time_elapsed = 0;
  static unsigned long last_reference_change = 0;
  static float input = 0;
  float dt = 0.001 * SAMPLE_TIME;

  static float position_error_sum = 0;

  if (millis() - time_elapsed > SAMPLE_TIME) {
    time_elapsed = millis();

    states.angular_speed = (calc_angle_from_count(encoder_angle_count) - states.angle) / dt;
    states.linear_speed = (float(encoder_position_count) * 0.00001953125 - states.position) / dt;
    states.position = float(encoder_position_count) * 0.00001953125;
    states.angle = calc_angle_from_count(encoder_angle_count);

    input = lqr_controller();

    if (input > MAX_INPUT) input = MAX_INPUT;
    if (input < -MAX_INPUT) input = -MAX_INPUT;

    if (abs(states.angle) > 0.6 || abs(states.position) > 0.25) input = 0;

    display(input);

    motor_write_pwm(input);
    
  }
}