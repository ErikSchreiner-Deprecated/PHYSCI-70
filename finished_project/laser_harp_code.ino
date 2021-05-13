#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE(); // to start MIDI

#include <AccelStepper.h>
const int stepPin = 33;  // connected to stepper driver
const int dirPin = 27;   // connected to stepper driver
AccelStepper stepper(1, stepPin, dirPin); // initialising accelstepper for a two wire board

const int waitTime = 30; // how long to stay at each position
const int numSegments = 8; // how many notes/beams
const int stepsPerSegment = 7; // steps between beam positions --> should be around 60/numSegments

unsigned long prevMillis; // for timer use
int n = 0; // for making the motor spin
int dir = 1; // for changing the motor direction
int laserPin = 13; // to connect and power on and off the laser
int sensorPin = A1; // to read analog light level

int averageLight;
int lightLevel;
int playLightLevel;


class Sound
{
    // for light sensor
    int val;
    int sensorPin;
    int playLightLevel;

    // for sending MIDI signals
    int note;
    int volume;
    int channel;
    int noteTime;
    int beam;

    unsigned long soundMillis = 0; // for timer use

  public:
    Sound(int sensorPin_input, int note_input, int volume_input, int channel_input, int noteTime_input, int beam_input)
    {
      // just assigning the varibles to the input values
      sensorPin = sensorPin_input;
      note = note_input;
      volume = volume_input;
      channel = channel_input;
      noteTime = noteTime_input;
      beam = beam_input;

      pinMode(sensorPin, INPUT); // telling the board the light sensor pin is for input
    }


    // the function that reads the light level, beam number "n", and plays the note
    // for the inputed amount of time.
    void Update()
    {
      unsigned long currentMillis = millis();

      val = analogRead(sensorPin);

      if ((val > playLightLevel) && (currentMillis - soundMillis >= noteTime) && (beam == n))
      {
        MIDI.sendNoteOn(note, volume, channel); // send MIDI data at inputed volume
        soundMillis = currentMillis;
      }
      else if (currentMillis - soundMillis >= noteTime)
      {
        MIDI.sendNoteOff(note, 0, channel); // turn off note
        soundMillis = currentMillis;
      }
    }
};

// creating instances of each beam
// recall takes form (sensorPin, note, volume/velocity (0-127), channel, time on, beam number)
Sound beamZero(sensorPin, 60, 127, 0, 1000, 0);
Sound beamOne(sensorPin, 62, 127, 1, 1000, 1);
Sound beamTwo(sensorPin, 64, 127, 2, 1000, 2);
Sound beamThree(sensorPin, 65, 127, 3, 1000, 3);
Sound beamFour(sensorPin, 67, 127, 4, 1000, 4);
Sound beamFive(sensorPin, 69, 127, 5, 1000, 5);
Sound beamSix(sensorPin, 71, 127, 6, 1000, 6);
Sound beamSeven(sensorPin, 72, 127, 7, 1000, 7);


void setup()
{
  pinMode(laserPin, OUTPUT); // set laser to be output
  stepper.setMaxSpeed(25000); // just settings a max speed
  stepper.setAcceleration(50000); // just setting an acceleration
  Serial.begin(57600); // open serial connection
}

void loop()
{

  if (millis() < 15000) {
    for (int i; i < 200; i++)
    {
      averageLight = averageLight + analogRead(sensorPin);
      delay(10);
    }
    lightLevel = averageLight / 200;
    int playLightLevel = lightLevel + (lightLevel / 2); // creating a threshold that must be broken to play a note

    //    Serial.print(lightLevel);  //for debugging
    //    Serial.print("\t");
    //    Serial.print(averageLight);
    //    Serial.print("\t");
    //    Serial.print(playLightLevel);
    //    Serial.print("\t");
    //    Serial.println(analogRead(sensorPin));
  }


  unsigned long currentMillis = millis();

  if (stepper.distanceToGo() == 0) { //when stepper is in position
    digitalWrite(laserPin, HIGH); // turn on laser
    if (currentMillis - prevMillis >= waitTime) {
      digitalWrite(laserPin, LOW);
      prevMillis = currentMillis;
      n = n + dir;
      stepper.moveTo(n * stepsPerSegment);
      stepper.setSpeed(3000 * dir); // this is needed to avoid accelerations
    }
    if (n == numSegments) {
      dir = -1;
    }
    if (n == 0) {
      dir = 1;
    }
  }
  if (stepper.distanceToGo() != 0) { // run stepper (the if condition is needed or else stepper just spins)
    stepper.runSpeed();
  }

  // check to see if note should play.
  beamZero.Update();
  beamOne.Update();
  beamTwo.Update();
  beamThree.Update();
  beamFour.Update();
  beamFive.Update();
  beamSix.Update();
  beamSeven.Update();

  // for debugging
  //  Serial.print(stepper.currentPosition());
  //  Serial.print("\t");
  //  Serial.print(stepper.distanceToGo());
  //  Serial.print("\t");
  //  Serial.print("n= ");
  //  Serial.print(n);
  //  Serial.print("\t");
  //  Serial.print("m= ");
  //  Serial.print(m);
  //  Serial.print("\t");
  //  Serial.print(currentMillis);
  //  Serial.print("\t");
  //  Serial.print(prevMillis);
  //  Serial.print("\t");
  //  Serial.print(currentMillis - prevMillis);
  //  Serial.print("\t");
  //  Serial.println();
}
