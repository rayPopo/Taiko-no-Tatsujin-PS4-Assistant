//#include <avr/pgmspace.h>

#define BUTTON_PRESS_TIME 35
#define ROLL_COUNT 1 // Don't be too greedy

//******************** Tips about the sheet file *******************
// BPM value format: "BXXXXXXXX\n"
// XXXXXXXX value can't have more than 15 digits including the decimal.
// For example, "B166.6667" is valid but "B166.66666666666667" is not.
// This is the sheet of 「紅」
char* sheetFile = "B156.33\n"
                  "x x x x xxx xxxxx o o o o  o  o x x xxxxx x xxxxx o o o o  o  xx"
                  "x o xxo x ox xo x o xxo x ox xo x o xxo x ox xo x o xxooooxxx xx"
                  "x o x o xxo xxo x o x o xxo xxo x o x o xxo xxo x o xxx o xxx x "
                  "x o xxo x ox xo x o xxo x ox xo x xxx x o ooo o x o xxo x ox xo "
                  "x o xxo x ox xo x o xxo x ox xo x xxx x o ooo o ooxxooxxooxxx   "
                  "x o xxo x o xxo x o xxo x ox xo x xxx x o ooo o X  X  O O   xxx "
                  "x o xxo x o xxo x o xxo x   o o o x x x o xxxxx "
                  "x x x x xxx xxxxx o o o o  o  o x xxxxx x xxxxx x o o o o  o    "
                  "x o xxo x ox xo x o xxo x ox xo x xxx x o ooo o x o xxo x ox xo "
                  "x o xxo x ox xo x o xxo x ox xo x xxx x o ooo o ooxxooxxooxxx   "
                  "x o xxo x o xxo x o xxo x ox xo x xxx x o ooo o X  X  O O   xxx "
                  "x o xxo x o xxo x o xxo x   o o o xxx x o xxx   "
                  "ooxxooxxooxxxxo xxooxxooxxooxxx ooxxooxxooxxxxo xxooxxooxxooxxx "
                  "ooxxooxxooxxxxo xxooxxooxxooxxx ooxxooxxooxxxxooo   o o x   xxx "
                  "xxxoooxxxoooxxxxx  xo xxx o x   "
                  "x xxx xxo  o  x xxx   x x  o  o x   xxx x  x  o x o xxooo xxo oo"
                  "x     x o  o  oox     xxoox ooxxxxooxxooxxxxooxxooxxxxooo x x x "
                  "x xxx xxx x x x x o   o x x x x xxx xxx x x x x x o   o x x o o "
                  "xxx xxx x x x x x o   o x x x x xxooxxooxxooxxooo  x  x o xxxxx "
                  "x o xxo x ox xo x o xxo xxoox o xxo x o xxo x o xxooxxooxxoox o "
                  "x xxo xxo xxo o x xxo xxooxxo o xxxxx x xxxxx x ooxxooxxooxxx   "
                  "x o xxo x ox xo x o xxo xxxxx o xxoox o xxoox o x xxxxo x xxo o "
                  "x xxo xxo xxo o x xxo xxooxxo o x xxx x x xxx   "
                  "x x x x xxx xxx o x x x xxx xxx o xxx x o o xxx o xxx x o x xxx "
                  "xxxoooxxxoooxxx xxxoooxxxoooxxo xxxoooxxxoooxxx O"
                  "\0";

float bpm;
int sheetCursor;
int bpmCursor;
bool started;
float sheetTime;
unsigned long startTime;

void setup() {
  pinMode(A0, OUTPUT); // A0 - left don
  pinMode(A1, OUTPUT); // A1 - right don
  pinMode(A2, OUTPUT); // A2 - left kat
  pinMode(A3, OUTPUT); // A3 - right kat
  pinMode(A4, INPUT); // A4 - start switch
  
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);

  digitalWrite(LED_BUILTIN, LOW);

  sheetCursor = 0;
  sheetTime = 0;
  started = false;

  Serial.begin (9600);
}

void loop() {
  if (analogRead(A4) >= 10 && !started) {
    started = true;
    startTime = micros();
  }
  if (!started) {
    return;
  }
  switch (sheetFile[sheetCursor]) {
    case 'B':
      bpmCursor = sheetCursor + 1;
      while (sheetFile[bpmCursor] != '\n') {
        bpmCursor++;
      }
      char bpmText[15];
      strncpy(bpmText, &sheetFile[sheetCursor + 1], bpmCursor - sheetCursor - 1);
      bpm = atof(bpmText);
      sheetCursor = bpmCursor + 1;
      return;

    //Empty
    case ' ': case '0':
      sheetTime += 1.5e7 / bpm;
      sheetCursor++;
      longMicroDelay (sheetTime - micros() + startTime);
      return;
      
    // light don
    case 'x': case '1':
      digitalWrite(A1, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
      delay (BUTTON_PRESS_TIME);
      digitalWrite(A1, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      sheetTime += 1.5e7 / bpm;
      sheetCursor++;
      longMicroDelay (sheetTime - micros() + startTime);
      return;

    // light kat
    case 'o': case '2':
      digitalWrite(A3, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
      delay (BUTTON_PRESS_TIME);
      digitalWrite(A3, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      sheetTime += 1.5e7 / bpm;
      sheetCursor++;
      longMicroDelay (sheetTime - micros() + startTime);
      return;

    // heavy don
    case 'X': case '3':
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
      delay (BUTTON_PRESS_TIME);
      digitalWrite(A0, HIGH);
      digitalWrite(A1, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      sheetTime += 1.5e7 / bpm;
      sheetCursor++;
      longMicroDelay (sheetTime - micros() + startTime);
      return;

    // heavy kat
    case 'O': case '4':
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
      delay (BUTTON_PRESS_TIME);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      sheetTime += 1.5e7 / bpm;
      sheetCursor++;
      longMicroDelay (sheetTime - micros() + startTime);
      return;

    // Roll
    case 'r': case 'R': case '5':
//      for (int i = 0; i < ROLL_COUNT; i++) {
//        digitalWrite(A1, LOW);
//        digitalWrite(LED_BUILTIN, HIGH);
//        delay (BUTTON_PRESS_TIME);
//        digitalWrite(A1, HIGH);
//        digitalWrite(LED_BUILTIN, LOW);
//        delay (BUTTON_PRESS_TIME);
//      }
      sheetTime += 1.5e7 / bpm;
      sheetCursor++;
      longMicroDelay (sheetTime - micros() + startTime);
      return;
  }
  
  if (sheetCursor > strlen(sheetFile)) {
    started = false;
    sheetCursor = 0;
  }
}

void longMicroDelay (float microTime) {
  delay (microTime / 1000);
  delayMicroseconds (microTime - floor(microTime / 1000) * 1000);
}
