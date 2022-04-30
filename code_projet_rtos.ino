//Mes biliothéques 
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <queue.h> // add the freeRTOS fonctions for queue

// Déclaration des ports
const int Analogpin = A0; //Potentiométre sur l'entrée A0
int pushbutton1 = 2; //Bouton poussoir1 
int pushbutton2 = 3; //Boutton poussoir2


//Déclaration de la structure pour la tache 5
struct valeurCapteurs 
{
  int analogique;
  int numerique;
  double tempsEnMillisecondes;
};

// Déclaration du semaphore
SemaphoreHandle_t xSerialSemaphore = NULL;

// Déclaration des queues
QueueHandle_t qpot; //analog reading of the potentiometer
QueueHandle_t qbouton; //digital sum of 2 push buttons
QueueHandle_t qstructure; //structure "valeurCapteurs" 
QueueHandle_t qsensor; //structure "valeurCapteurs" used by task 4 in order to send data

// Définition des tasks:
void TaskAnalogRead( void *pvparameters );
void TaskDigitalRead( void *pvparameters );
void TaskReceiveValue( void *pvparameters );
void TaskReceiveFromSerialPort (void *pvparameters);
void TaskTransformTimeData (void *pvparameters);

void setup() 
{
  Serial.begin(9600); //Baud rate: 9600
  
   while (!Serial); // wait for serial port to connect.

  if ( xSerialSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
  {
    xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xSerialSemaphore ) != NULL )
      xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }

   // Initialisation des queues
  qpot = xQueueCreate(5, sizeof(uint32_t));
  qbouton = xQueueCreate(5, sizeof(uint32_t));
  qstructure = xQueueCreate(5, sizeof(valeurCapteurs));
  qsensor = xQueueCreate(5, sizeof(valeurCapteurs));
  
  // Initialisation des ports
 pinMode(Analogpin, INPUT);
 pinMode(pushbutton1, INPUT);
 pinMode(pushbutton2, INPUT);
 
  // Initialisation des tâches
  xTaskCreate(task1, "Récupére la valeur du potentiomètre", 128, NULL, 1, NULL);
  xTaskCreate(task2, "Lis la valeur des boutons", 128, NULL, 1, NULL);
  xTaskCreate(task3, "Initialise la structure et envoie de la structure remplie", 128, NULL, 1, NULL);
  xTaskCreate(task4, "Affiche la structure", 1000, NULL, 1, NULL);
  xTaskCreate(task5, "Affiche la nouvelle structure en minutes", 1000, NULL, 1, NULL);
}

void loop() 
{
  //écriture du main
}

//tache 1 : 
//Récupère une valeur analogique sur l’entrée A0 qui est branchée avec un potentiomètre puis
//l’envoie à la tâche 3 (valeur entre 0 et 1023)

void task1(void *pvparameters)
{
   int AnalogValue; // value to send the task3
  
  while(1)
  {
    AnalogValue=analogRead(Analogpin);  // read the input on analog pin:
    xQueueSend(qpot,&AnalogValue,0); // send the value you read:
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}

//tache 2 :
//Récupère une valeur numérique qui est la résultante de l'addition des deux valeurs des deux
//entrées numérique 3 et 4 qui sont branchées avec des boutons poussoirs en montage pull
//down, puis envoie cette valeur numérique à la tâche 3 (valeur entre 0 et 2)

void task2(void *pvparameters)
{
   int databuttons;
    while(1){
    databuttons = digitalRead(pushbutton1) + digitalRead(pushbutton2);
    xQueueSend(qbouton, &databuttons, 0);

    vTaskDelay(100);
  }
}

//tache 3 :
//Reçoit les deux valeurs des tâches 1 et 2 puis les mets dans une structure en ajoutant la
//valeur de la fonction millis().
//Une fois la structure remplie, cette dernière doit être envoyée à la tâche 4.

void task3(void *pvparameters)
{
   int PTRead;
   int BTRead;
   valeurCapteurs s;
   while(1)
   {
     xQueueReceive(qpot, &PTRead, 0); //reçoit les  valeurs de la tâche 1
     xQueueReceive(qbouton, &BTRead, 0); //reçoit les  valeurs de la tâche2

     s.analogique = PTRead; //affecter la valeur de la tâche 1 à la structure 
     s.numerique = BTRead; //affecter la valeur de la tâche2 à la structure
     s.tempsEnMillisecondes = millis();

     xQueueSend(qstructure, &s, 0); //Attribuer la valeur sensor à la Queue qStruct

     vTaskDelay(100);
    }
}

//tache 4 :
//Cette tâche reçoit la valeur de la structure et utilise le port série pour l’afficher, ensuite envoie
//cette structure à la tâche 5.

void task4(void *pvparameters)
{
    valeurCapteurs s;
    while(1)
    {
    xQueueReceive(qstructure, &s, 0); //reçois la valeur dans la queue qStruct
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ) //On peut accéder à la ressource partagée
    {
      //Affiche le contenu de la structure
      Serial.print("La valeur de la structure est : {");
      Serial.print(s.analogique);
      Serial.print(" ; ");
      Serial.print(s.numerique);
      Serial.print("  ; ");
      Serial.print(s.tempsEnMillisecondes);
      Serial.print(" } \n");
      xSemaphoreGive( xSerialSemaphore ); // Libére le port série
    }
    xQueueSend(qsensor, &s, 0);

    vTaskDelay(100);
  }
}

//tache 5 :
//Cette tâche doit transformer la valeur du temps dans la structure en minutes, ensuite elle doit
//afficher cette nouvelle structure à travers le port série.

void task5(void *pvparameters)
{
   valeurCapteurs s;
  valeurCapteurs s2;
  while(1){
    xQueueReceive(qsensor, &s, 0); //reçoit les  valeurs de la tâche 4
    s2.analogique = s.analogique; //garde la meme valeur analogique 
    s2.numerique = s.numerique; //garde la meme valeur numérique 
    s2.tempsEnMillisecondes = s.tempsEnMillisecondes/60000; //conversion de la valeur du temps dans la structure en minutes
    
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ) // Vérifier si on peut accéder au port série
    {
      //Afficher le contenu de la nouvelle structure
      Serial.print("La valeur de la nouvelle structure est: { ");
      Serial.print(s2.analogique);
      Serial.print(" ; ");
      Serial.print(s2.numerique);
      Serial.print(" ; ");
      Serial.print(s2.tempsEnMillisecondes);
      Serial.print(" } \n");
      
      xSemaphoreGive( xSerialSemaphore ); //Libérer la sémaphore
    }

    vTaskDelay(100);
  }
}
