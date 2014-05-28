import processing.serial.*;

String PORTA_SERIAL = "/dev/ttyUSB0";

Serial serial;

int N_TABLADOS = 17;

void setup() {
    size(800, 600);
    ((javax.swing.JFrame) frame).getContentPane().setBackground(new java.awt.Color(0,0,0));

    println(Serial.list());
    serial = new Serial(this, PORTA_SERIAL, 19200);
}

int frame_num = 0;
void draw() {
  int i;

  int height_i = height / N_TABLADOS;

  for (i = 0; i < N_TABLADOS; i++) {
    char w = (char) i;
    w += '0';
    serial.write(w);
    int t0 = millis();
    while (serial.available() == 0 && millis()-t0 < 20);
    int r = serial.read();
    if (r == -1)
        r = 0;
    print("[" + i + "] " + r);
    stroke(255);
    line(frame_num, height - (height_i * i    ), frame_num, height - (height_i * i + r));
    stroke(0);
    line(frame_num, height - (height_i * i + r), frame_num, height - (height_i * (i+1)));
  }
  println("");

  frame_num++;
  if (frame_num > width)
      frame_num = 0;
}

