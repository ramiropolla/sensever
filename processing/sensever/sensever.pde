import processing.serial.*;
import processing.video.*;

int TESTANDO = 0;
String PORTA_SERIAL = "/dev/ttyUSB0";

PFont f;

Serial serial;
int LIMIAR = 192;
int COUNT = 3;

int tempo_reset = 3 * 60 * 1000; /* 3 minutos */
int N_FILMES = 13;

Movie[] filmes = new Movie[N_FILMES];
String[] arquivos = {
    "alguemmeolhandoveg.mov",   /* 4.004 */
    "cha.mov",                  /* 11.1145, > 11.111 */
    "emvezdabrisa.mov",         /* 2.922 */
    "estounoonibus.mov",        /* 3.008 */
    "estouvoando.mov",          /* 1.525 */
    "gargantafechando.mov",     /* 3.026 */
    "lagartixa.mov",            /* 4.05 */
    "ligooarcondicionado.mov",  /* 7.8411665, > 7.84 */
    "marentrando.mov",          /* 5.7140417, > 5.712 */
    "mechascaem.mov",           /* 4.2556667, > 4.25 */
    "pulopredio.mov",           /* 8.527262, > 8.52 */
    "vejoumavelha.mov",         /* 3.274 */
    "vejoumfuracao.mov",        /* 5.4825 */
};
float[] duracoes = {
   4.004, 11.111, 2.922, 3.008, 1.525, 3.026,
   4.05 ,   7.84, 5.712, 4.25 , 8.52 , 3.274,
   5.4825,
};

boolean[] usados = new boolean[N_FILMES];
int[] historico = new int[N_FILMES];
int historico_idx = 0;

boolean play_tudo = false;
boolean playing = false;
int play_idx = 0;

/* reset timer */
int t0;

/* caminho */
int maior_x = 0;
int count = 0;
boolean[] usado_t = new boolean[18];
int cur_t = 0;
int N_TABLADOS = 17;

void reiniciar() {
    maior_x = 0;
    count = 0;
    cur_t = 0;
    play_tudo = false;
    playing = false;
    play_idx = 0;
    historico_idx = 0;
    for (int i = 0; i < N_FILMES; i++) {
        historico[i] = -1;
        usados[i] = false;
    }
    for (int i = 0; i < 18; i++)
        usado_t[i] = false;
    t0 = millis();
}

void setup() {
    size(640, 480);
    ((javax.swing.JFrame) frame).getContentPane().setBackground(new java.awt.Color(0,0,0));

    f = createFont("Arial",16,true);

    println(Serial.list());
    serial = new Serial(this, PORTA_SERIAL, 19200);
//    serial = new Serial(this, PORTA_SERIAL, 4800);

    for (int i = 0; i < N_FILMES; i++)
        filmes[i] = new Movie(this, arquivos[i]);

    reiniciar();
}

void mouseClicked() {
  if (!playing)
    proximo_video();
}

void proximo_video() {
  do {
    play_idx = (int) random(N_FILMES);
  } while (usados[play_idx]);

  usados[play_idx] = true;
  historico[historico_idx] = play_idx;
  historico_idx++;

  playing = true;
  filmes[play_idx].play();
  t0 = millis();
}

int frame_num = 0;
void draw() {
  if (millis() - t0 > tempo_reset) {
    reiniciar();
    t0 = millis();
  }
  if (playing) {
    if (play_tudo)
      image(filmes[play_idx], 0, 0);
  } else {
      if (TESTANDO == 0)
          background(0);
  }

  if (filmes[play_idx].time() >= duracoes[play_idx]) {

    /* filme terminou */
    filmes[play_idx].stop();
    playing = false;

    if (play_tudo) {
      historico_idx++;
      play_idx = historico[historico_idx];
      if (play_idx == -1) {
        println("reiniciando");
        reiniciar();
      } else {
        filmes[play_idx].play();
        playing = true;
      }
    } else {
      if (usado_t[5]) {
        play_tudo = true;
        historico_idx = 0;
        play_idx = historico[historico_idx];
        filmes[play_idx].play();
        playing = true;
      }
    }
  }

  if (playing)
    return;

  int[] v = new int[6];
  int i;
  for (i = 0; i < 6; i++)
    v[i] = 0;
  int maior = 0;
  for (i = 0; i < 6 && vizinhos[cur_t][i] != 0; i++);
  int height_i = height / i;
  if (TESTANDO == 1) {
    print("cur " + cur_t + " ");
  }
  for (i = 0; i < 6 && vizinhos[cur_t][i] != 0; i++) {
    char w = (char) vizinhos[cur_t][i];
    w += '0';
    serial.write(w);
    int t0 = millis();
    while (serial.available() == 0 && millis()-t0 < 20);
    int r = serial.read();
    if (r == -1)
        r = 0;
    v[i] = r;
    if (TESTANDO == 1) {
        print("[" + w + "] " + r);
        stroke(255);
        line(frame_num, height - (height_i * i    ), frame_num, height - (height_i * i + v[i]));
        stroke(0);
        line(frame_num, height - (height_i * i + v[i]), frame_num, height - (height_i * (i+1)));
    }
  }
  if (TESTANDO == 1)
      println("");

  frame_num++;
  if (frame_num > width)
      frame_num = 0;
  while (i-- != 0) {
    if (v[maior] < v[i])
        maior = i;
  }
  if (v[maior] > LIMIAR) {
      println(maior + "=>" + vizinhos[cur_t][maior]);
      if (maior == maior_x) {
        count++;
      } else {
        maior_x = maior;
        count = 0;
      }
      if (count > COUNT) {
          if (TESTANDO == 1) {
              textFont(f,32);
              fill(255);
              print("vizinhos[" + cur_t + "][" + maior + "] = ");
          }
          cur_t = vizinhos[cur_t][maior];

          if (TESTANDO == 1) {
              height_i = height / i;
              println(cur_t);
              for (i = 0; i < 6 && vizinhos[cur_t][i] != 0; i++) {
                  print("[" + vizinhos[cur_t][i] + "] ");
              }
              println("");
              for (i = 0; i < 6 && vizinhos[cur_t][i] != 0; i++) {
                  text("" + vizinhos[cur_t][i], frame_num-32, height - (height_i * i));
              }
          }

          if (!usado_t[cur_t]) {
              proximo_video();
              usado_t[cur_t] = true;
          }
          maior_x = 0;
          count = 0;
      }
  }
}

/* faz o sketch rodar em tela cheia */
boolean sketchFullScreen() {
  if (TESTANDO == 0)
      return true;
  else
      return false;
}

/* evento para os filmes */
void movieEvent(Movie m) {
  m.read();
}

int[][] vizinhos = {
  { 1, 0, 0, 0, 0, 0 },
  { 2, 6, 10, 0, 0, 0 },
  { 1, 3, 6, 7, 10, 11 },
  { 2, 4, 7, 8, 11, 12 },
  { 3, 5, 8, 9, 12, 13 },
  { 4, 9, 13, 0, 0, 0 },
  { 1, 2, 7, 14, 0, 0 },
  { 2, 3, 6, 8, 14, 15 },
  { 3, 4, 7, 9, 15, 0 },
  { 4, 5, 8, 0, 0, 0 },
  { 1, 2, 11, 16, 0, 0 },
  { 2, 3, 10, 12, 16, 17 },
  { 3, 4, 11, 13, 17, 0 },
  { 4, 5, 12, 0, 0, 0 },
  { 6, 7, 15, 0, 0, 0 },
  { 7, 8, 14, 0, 0, 0 },
  { 10, 11, 17, 0, 0, 0 },
  { 11, 12, 16, 0, 0, 0 },
};

