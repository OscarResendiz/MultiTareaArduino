//---------------------------------------------------------Libreria que maneja la multitarea----------------------------------------------------------------
//---La clase CTarea representa una tarea y manda a llamar a una funcion que se le especifique.
class CTarea {
  int Tiempo;
  int Milisegundos;
private:
  void (*ptr_funct)(void);  //apuntador a mi funcion que voy a llamar
public:
  CTarea *sigiente;
  //constructor. se le pasa cada cuanto tiempo se tiene que ejecutar y el apuntador a la funcion que debe llamar
  //el primer parametro func es la funcion que se va a ejcutar
  //el segundo parametro es el tiempo que hay que esperar para que se ejecute la funcion. este tiempo es dado en milisegundos
public:
  CTarea(void (*func)(void), int milisegundos) {
    ptr_funct = func;
    Milisegundos = milisegundos;
    Tiempo = 0;  //se inicializa el tiempo a cero
    sigiente = NULL;
  }
  //esta funcion se llama cada ticks milisegundos por el planificador
public:
  void tick(int ticks) {
    //calculo el tiempo transcurrido
    Tiempo += ticks;
    //Serial.println("Tiempo: "+String(Tiempo));
    if (Tiempo >= Milisegundos) {
      //ya se cumplio el tiempode espera
      Tiempo = 0;  //reinicio el contador del tiempo
      //mando a llamar a la funcion
      ptr_funct();
    }
  }
};
//-------------ClaseMultiTarea esta clase es la que administra las tareas y las manda a llamar en secuencia para que todas compartan tiempo del procesador
class MultiTarea {
  // las tareas se almacenan en una pila para no estar limitado a un cierto numero fijo, por lo que puede crecer
private:
  CTarea *Pila = NULL;
  //--------funcion que agrega una tarea para ser ejecutada----------------------
  //el primer parametro es la funcion que se va a mandar a llamar
  //el segundo es el tiempo en milisegundos que hay que esperarpara que se eejcute la tarea
public:
  AgregaTarea(void (*func)(void), int milisegundos) {
    //creo la nueva tarea
    CTarea *tmp = new CTarea(func, milisegundos);
    if (Pila == NULL) {
      //la pila esta vacia
      Pila = tmp;
      return;
    }
    //recorro la pila hasta encontrar el ultimo
    CTarea *sig = Pila;
    while (sig->sigiente != NULL) {
      sig = sig->sigiente;
    }
    //agrego el siguiente
    sig->sigiente = tmp;
  }
  //esta funcion es la que ejecuta todas las tareas en un bucle infinito
public:
  Ejecuta() {
    CTarea *sig = Pila;
    int tinicial = millis();
    int tfinal = millis();
    int transcurrido = 0;
    while (true) {
      //inicia el conteo de tiempo que se tarda la tarea
      tinicial = millis();
      delay(1);
      //ejecuto la tarea
      sig->tick(transcurrido);
      sig = sig->sigiente;
      if (sig == NULL) {
        transcurrido = 0;
        sig = Pila;
      }
      //obtengo el tiempo final
      tfinal = millis();
      transcurrido += tfinal - tinicial;
    }
  }
};