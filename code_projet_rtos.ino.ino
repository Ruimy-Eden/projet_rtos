void setup() {
  Serial.begin(9600); //Baud rate: 9600
}
void loop() {
  int sensorValue = analogRead(A0);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(voltage); // print out the value you read:
  delay(500);
}

//tache 1 : 
//Récupère une valeur analogique sur l’entrée A0 qui est branchée avec un potentiomètre puis
//l’envoie à la tâche 3 (valeur entre 0 et 1023)

//tache 2 :
//Récupère une valeur numérique qui est la résultante de l&#39;addition des deux valeurs des deux
//entrées numérique 3 et 4 qui sont branchées avec des boutons poussoirs en montage pull
//down, puis envoie cette valeur numérique à la tâche 3 (valeur entre 0 et 2)

//tache 3 :
//Reçoit les deux valeurs des tâches 1 et 2 puis les mets dans une structure en ajoutant la
//valeur de la fonction millis().
//Une fois la structure remplie, cette dernière doit être envoyée à la tâche 4.

struct valeurCapteurs 
{
int analogique;
int numérique;
double tempsEnMillisecondes;
};

//tache 4 :
//Cette tâche reçoit la valeur de la structure et utilise le port série pour l’afficher, ensuite envoie
//cette structure à la tâche 5.

//tache 5 :
//Cette tâche doit transformer la valeur du temps dans la structure en minutes, ensuite elle doit
//afficher cette nouvelle structure à travers le port série.
