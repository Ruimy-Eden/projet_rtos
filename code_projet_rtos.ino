//Mes biliothéques 
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Déclaration des ports
int pot = A0;
int but1 = 2;
int but2 = 3;

//Déclaration de la structure pour la tache 5
struct valeurCapteurs 
{
int analogique;
int numérique;
double tempsEnMillisecondes;
};

// Déclaration du semaphore
SemaphoreHandle_t xSerialSemaphore = NULL;

// Déclaration des queues
QueueHandle_t qpot;
QueueHandle_t qbouton;
QueueHandle_t qstructure;
QueueHandle_t qsensor;

void setup() 
{
  Serial.begin(9600); //Baud rate: 9600
  
  while (!Serial);

  if ( xSerialSemaphore == NULL )  
  {
    xSerialSemaphore = xSemaphoreCreateMutex();
    if ( ( xSerialSemaphore ) != NULL )
      xSemaphoreGive( ( xSerialSemaphore ) );
  }

  // Initialisation des ports
  pinMode(pot, INPUT);
  pinMode(bouton1, INPUT);
  pinMode(bouton2, INPUT);
  
   // Initialisation des queues
  qpot = xQueueCreate(4, sizeof(int));
  qbouton = xQueueCreate(4, sizeof(int));
  qstructure = xQueueCreate(4, sizeof(valeurCapteurs));
  qsensor = xQueueCreate(4, sizeof(valeurCapteurs));
 
  // Initialisation des tâches
  xTaskCreate(task1, "Récupére la valeur du potentiomètre", 100, NULL, 0, NULL);
  xTaskCreate(task2, "Lis la valeur des boutons", 100, NULL, 0, NULL);
  xTaskCreate(task3, "Initialise la structure et envoie de la structure remplie", 100, NULL, 0, NULL);
  xTaskCreate(task4, "Affiche la structure", 100, NULL, 0, NULL);
  xTaskCreate(task5, "Affiche la nouvelle structure en minutes", 100, NULL, 0, NULL);
}

void loop() 
{
  //écriture du main
}

//tache 1 : 
//Récupère une valeur analogique sur l’entrée A0 qui est branchée avec un potentiomètre puis
//l’envoie à la tâche 3 (valeur entre 0 et 1023)

void task1(void *pvparameters){
}

//tache 2 :
//Récupère une valeur numérique qui est la résultante de l'addition des deux valeurs des deux
//entrées numérique 3 et 4 qui sont branchées avec des boutons poussoirs en montage pull
//down, puis envoie cette valeur numérique à la tâche 3 (valeur entre 0 et 2)

void task2(void *pvparameters){
}

//tache 3 :
//Reçoit les deux valeurs des tâches 1 et 2 puis les mets dans une structure en ajoutant la
//valeur de la fonction millis().
//Une fois la structure remplie, cette dernière doit être envoyée à la tâche 4.

void task3(void *pvparameters){
}

//tache 4 :
//Cette tâche reçoit la valeur de la structure et utilise le port série pour l’afficher, ensuite envoie
//cette structure à la tâche 5.

void task4(void *pvparameters){
}

//tache 5 :
//Cette tâche doit transformer la valeur du temps dans la structure en minutes, ensuite elle doit
//afficher cette nouvelle structure à travers le port série.

void task5(void *pvparameters){
}
