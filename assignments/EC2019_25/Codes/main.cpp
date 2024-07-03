int d=1, q1=0;
int q2=0, clk = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void disp(int clk, int q2){
  digitalWrite(2, clk);
  digitalWrite(3, q2);
}

void loop() {
  // put your main code here, to run repeatedly:
  clk =!clk;
  d = !q1 && !q2;
  q2 = q1;
  q1 = d;
  disp(clk,q2);
  delay(1000);
  clk =!clk;
  d = !q1 && !q2;
  disp(clk,q2);
  delay(1000);
}
