import processing.serial.* ;

void setup()
{
  new Serial(this, Serial.list()[0], 115200);
}

void draw()
{
}

void serialEvent(Serial myPort)
{
  background(80);
  fill(180);
  text(myPort.read(), 20, 20);
}
