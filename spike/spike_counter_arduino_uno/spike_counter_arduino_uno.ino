bool lastA = false;
bool lastB = false;
bool result = false;
bool lastResult = false;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);

}
int entradas = 0;
int saidas = 0;
bool messure = false;
bool dir = false;
void show(){
  Serial.println("PERSON COUNTER");
  Serial.print("IN ");
  Serial.println(entradas);
  Serial.print("OUT ");
  Serial.println(saidas);
  Serial.print("TOTAL ");
  Serial.println(entradas-saidas);
  }
void loop() {
  static long go = millis();
  bool a = !digitalRead(2);
  bool b = !digitalRead(3);
  if(a && !b){
    
    messure = true;
    dir = true;
    }else  if(!a && b){
      messure = true;
      dir = false;
    
    }
  if(a && b){
    Serial.println("EM FRENTE AO SENSOR");
  }if(!a && !b && messure){
    if(!dir){
     entradas++;
     show();
      }
      else{
       saidas++;
       show();
        }
    messure = false;
    delay(500);
    }

  
}
