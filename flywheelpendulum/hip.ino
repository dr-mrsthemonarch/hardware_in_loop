#//include <math.h>
String stringRead, enable;
double sig;
unsigned long previousMillis = 0;
const long interval = 300;
unsigned long currentMillis = millis();

String getValue(String data, char separator, double index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

double u_0[1] = {0};
double y_0[1] = {0};
void update_nssm_0(double *u, double *y)
{
  y[0] = 0.5 / pow(M_E, 5 * pow(-11 + u[0], 2)) - 0.5 / pow(M_E, 5 * pow(-4 + u[0], 2));
}
double u_1[2] = {0, 0};
double y_1[3] = {0, 0, 0};
void update_nssm_1(double *u, double *y)
{
  static double x[3] = {0, 0, 0};
  double x1[3];
  y[0] = x[0];
  y[1] = x[1];
  y[2] = x[2];
  x1[0] = x[0] + 0.001 * x[1];
  x1[1] = x[1] + 0.001 * (405.91040606701074 * (-0.000039255383907286545 * x[2] + 0.0335 * u[0]) + 405.91040606701074 * (-0.00016799999999999996 * x[1] + 0.0920755 * u[1] * cos(x[0]) - 0.23969569622304654 * sin(x[0]) - 0.001484 * tanh(10.*x[1])));
  x1[2] = x[2] + 0.001 * (7005.539400442916 * (-0.000039255383907286545 * x[2] + 0.0335 * u[0]) - 405.91040606701074 * (-0.00016799999999999996 * x[1] + 0.0920755 * u[1] * cos(x[0]) - 0.23969569622304654 * sin(x[0]) - 0.001484 * tanh(10.*x[1])));
  x[0] = x1[0];
  x[1] = x1[1];
  x[2] = x1[2];
}
double u[2] = {0, 0};
double y[3] = {0, 0, 0};
void update_scm(double *u, double *y)
{
  u_0[0] = u[0];
  update_nssm_0(u_0, y_0);
  u_1[0] = u[1];
  u_1[0] = y_0[0];
  update_nssm_1(u_1, y_1);
  y[0] = y_1[0];
  y[1] = y_1[1];
  y[2] = y_1[2];
}
void setup()
{
  Serial.begin(115200);
}
void loop() {
  //============================ read serial for manual control
  if (Serial.available() > 0) {
    stringRead = Serial.readString();
  }
  enable = getValue(stringRead, ':', 0);
  sig = enable.toFloat();
  //============================ read serial for manual control


  double time = micros() / 1e6;
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    sig = 0;
  }
  u[0] = time;
  update_scm(u, y);
  Serial.println(y[0] * 180 / M_PI, 6);
  // Serial.print(",");
  // Serial.print(y[1], 6);
  // Serial.print(",");
  //  Serial.println(y[2], 6);
  delay(1);
}
