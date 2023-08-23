// Déclaration des constantes pour les broches
const int ledPin = 3; // Changer en fonction de la broche que vous utilisez
const int timeUnit = 100;
int dotStartTime ;
int dotDelay= timeUnit;
int dotLedStatus = 0;

int dashStartTime ;
int dashDelay= timeUnit*3;

int breakStartTime ;
int breakDelay= timeUnit*3;

int megaBreakStartTime ;
int megaBreakDelay= timeUnit*5;

int pauseDelay = timeUnit;
int pauseStartTime;
enum State {
  STATE_IDLE,
  STATE_CHARA, //est en train d ecrire un chara
  STATE_CHARA_DOT, // est en train d'ecrire un dot
  STATE_CHARA_DASH, // est en train d'ecrire un dash
  STATE_CHARA_PAUSE, // pause entre deux dots ou dashs
  STATE_BREAK, //pause entre deux characteres
  STATE_MEGA_BREAK // pause entre la fin d'un mot et le recommencement
};
int index_chara = 0;
int index_code = 0;

State currentState = STATE_IDLE;
State charaState = STATE_IDLE;
State dotState = STATE_IDLE;
State pauseState = STATE_IDLE;
State breakState = STATE_IDLE;
State dashState = STATE_IDLE;
State megaBreakState = STATE_IDLE;
// Tableau de correspondance entre les lettres et leur code Morse (0: point, 1: trait)
const char* morseTable[26] = {
  ".-",   // A
  "-...", // B
  "-.-.", // C
  "-..",  // D
  ".",    // E
  "..-.", // F
  "--.",  // G
  "....", // H
  "..",   // I
  ".---", // J
  "-.-",  // K
  ".-..", // L
  "--",   // M
  "-.",   // N
  "---",  // O  
  ".--.", // P
  "--.-", // Q
  ".-.",  // R
  "...",  // S
  "-",    // T
  "..-",  // U
  "...-", // V
  ".--",  // W
  "-..-", // X
  "-.--", // Y
  "--.."  // Z
};

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // Configuration de la broche de la LED en sortie
}

void loop() {
  String word = "TIME"; // Mot à convertir en code Morse (changez-le si nécessaire)

  switch(currentState){
    case  STATE_IDLE:
        currentState = STATE_CHARA;
        break;
    case STATE_CHARA:
      if(index_chara < word.length()){
        chara(word[index_chara]);
      }else{
        index_chara = 0;
        currentState = STATE_MEGA_BREAK;

      }
      break;
    case STATE_BREAK:
        break_chara();
        break;
    case STATE_MEGA_BREAK:
        mega_break();
        break;

  }

}
void chara(char chara){
  String code = morseTable[chara - 'A'];
  Serial.print("charaState : ");
  Serial.println(charaState);
  switch (charaState){
    case STATE_IDLE:
      charaState = STATE_CHARA;
    case STATE_CHARA:
      if(index_code < code.length()){
        if (code[index_code] == '.'){
          charaState = STATE_CHARA_DOT;
        }
        if (code[index_code] == '-'){
          dash();
        }
      }else{
        index_chara ++;
        index_code = 0;
        currentState = STATE_BREAK;
      }
     
      break;
    case STATE_CHARA_DOT:
      dot();
      break;
    case STATE_CHARA_DASH:
      dash();
      break;
    case STATE_CHARA_PAUSE:
      chara_pause();
      break;

  }
   
   
}

void dot() {
  int dotActualTime = millis();
  if(dotState == STATE_CHARA_DOT){ // si on est déjà en train de faire le dot
    if(dotActualTime > dotStartTime + dotDelay ){
        Serial.println("Eteignage de la led");

        digitalWrite(ledPin, LOW);
        charaState = STATE_CHARA_PAUSE;
        dotState = STATE_IDLE;

        
        index_code ++; // on passe au charactere suivant , puisque cleui la est terminé
        
      }else{
        Serial.println("On laisse allumé la led");
      }
  }else{
    if(dotState != STATE_CHARA_DOT){ //si on débute le dot
        Serial.print("DOT : Allumage de la led pour ms :");
        Serial.println(dotDelay);
        digitalWrite(ledPin, HIGH);
        charaState = STATE_CHARA_DOT;
        dotState = STATE_CHARA_DOT;
        dotStartTime = millis();
      }
  }


}


void dash() {
  int dashActualTime = millis();
  if(dashState == STATE_CHARA_DASH){ // si on est déjà en train de faire le dot
    if(dashActualTime > dashStartTime + dashDelay ){
        Serial.println("Eteignage de la led");

        digitalWrite(ledPin, LOW);
        charaState = STATE_CHARA_PAUSE;
        dashState = STATE_IDLE;

       index_code ++; // on passe au charactere suivant , puisque cleui la est terminé
        
      }else{
        Serial.println("On laisse allumé la led");
      }
  }else{
    if(dashState != STATE_CHARA_DASH){ //si on débute le dot
        Serial.print("DASH : Allumage de la led pour ms : ");
        Serial.println(dashDelay);

        digitalWrite(ledPin, HIGH);
        charaState = STATE_CHARA_DASH;
        dashState = STATE_CHARA_DASH;
        dashStartTime = millis();
      }
  }


}
void chara_pause(){
  int pauseActualTime = millis();
  if(pauseState == STATE_CHARA_PAUSE){ // si on est déjà en train de faire la pause
    if(pauseActualTime > pauseStartTime + pauseDelay ){
        Serial.println("Fin de la pause");
        charaState = STATE_IDLE;
        pauseState = STATE_IDLE;
        
      }else{
        Serial.println("On continue la pause");
      }
  }else{
    if(pauseState != STATE_CHARA_PAUSE){ //si on débute la pause
        Serial.println("On débute la pause");
        digitalWrite(ledPin, LOW);
        charaState = STATE_CHARA_PAUSE;
        pauseState = STATE_CHARA_PAUSE;
        pauseStartTime = millis();
      }
  }
}
void break_chara(){
  int breakActualTime = millis();
  if(breakState == STATE_BREAK){ // si on est déjà en train de faire la pause
    if(breakActualTime > breakStartTime + breakDelay ){
        Serial.println("Fin du break");
        currentState = STATE_IDLE;
        breakState = STATE_IDLE;
        
      }else{
        Serial.println("On continue le break");
      }
  }else{
    if(breakState != STATE_BREAK){ //si on débute la pause
        Serial.println("On débute le break");
        digitalWrite(ledPin, LOW);
        currentState = STATE_BREAK;
        breakState = STATE_BREAK;
        breakStartTime = millis();
      }
  }
}

void mega_break(){
  int megaBreakActualTime = millis();
  if(megaBreakState == STATE_MEGA_BREAK){ // si on est déjà en train de faire la pause
    if(megaBreakActualTime > megaBreakStartTime + megaBreakDelay ){
        Serial.println("Fin du mega break");
        currentState = STATE_IDLE;
        megaBreakState = STATE_IDLE;
        
      }else{
        Serial.println("On continue le mega break");
      }
  }else{
    if(megaBreakState != STATE_BREAK){ //si on débute la pause
        Serial.println("On débute le mega break");
        digitalWrite(ledPin, LOW);
        currentState = STATE_MEGA_BREAK;
        megaBreakState = STATE_MEGA_BREAK;
        megaBreakStartTime = millis();
      }
  }
}