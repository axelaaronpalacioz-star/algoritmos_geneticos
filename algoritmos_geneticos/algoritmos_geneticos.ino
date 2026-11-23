const int POP_SIZE = 8;     // Tamaño de la población
const int GEN_LEN = 6;      // Longitud del cromosoma en bits (valores de 0 a 63)
long poblacion[POP_SIZE];   // Usamos 'long' para evitar desbordamiento con potencias a la 4
long aptitud[POP_SIZE];

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
  // 1. Inicializar población aleatoria (valores de 0 a 63)
  for(int i = 0; i < POP_SIZE; i++) {
    poblacion[i] = random(0, 64); 
  }
  
  Serial.println("--- INICIO: ALGORITMO GENETICO - MEJOR Velocista---");
}

void loop() {
  static int generacion = 1;
  long mejorAptitudActual = -999999;
  long mejorIndividuo = 0;

  // 2. Evaluar Función de Aptitud Polinómica para el 
  // x representa la frecuencia y técnica de brazada en el agua (0 a 63)
  for(int i = 0; i < POP_SIZE; i++) {
    long x = poblacion[i];
    
    // Función polinómica de grado 4 adaptada a la hidrodinámica del :
    // F(x) = -x^4 + 70x^3 - 1500x^2 + 12000x
    // Modela los valles de resistencia al agua y el pico máximo de velocidad eficiente.
    aptitud[i] = - (x * x * x * x) + 84 * (x * x * x) - 2628 * (x * x) + 35336 * x;
    
    // Evitar valores negativos irreales en la aptitud
    if(aptitud[i] < 0) {
      aptitud[i] = 0; 
    }
    
    if(aptitud[i] > mejorAptitudActual) {
      mejorAptitudActual = aptitud[i];
      mejorIndividuo = x;
    }
  }

  // Mostrar resultados de la generación actual en el Monitor Serie
  Serial.print("Generacion: ");
  Serial.print(generacion);
  Serial.print(" | Mejor X (Brazada Velocista): ");
  Serial.print(mejorIndividuo);
  Serial.print(" | Aptitud Maxima: ");
  Serial.println(mejorAptitudActual);

  // 3. Criterio de paro (ejemplo fijado a 15 generaciones)
  if(generacion >= 15) {
    Serial.println("--- PROCESO TERMINADO: MEJOR Velocista ENCONTRADO ---");
    while(true); // Detener ejecución del programa
  }

  // 4. Selección, Elitismo y Reproducción
  // Ordenamiento burbuja para dejar a los mejores individuos arriba (Élite)
  for(int i = 0; i < POP_SIZE - 1; i++) {
    for(int j = 0; j < POP_SIZE - i - 1; j++) {
      if(aptitud[j] < aptitud[j+1]) {
        // Intercambiar aptitud
        long tempApt = aptitud[j];
        aptitud[j] = aptitud[j+1];
        aptitud[j+1] = tempApt;
        // Intercambiar individuo
        long tempInd = poblacion[j];
        poblacion[j] = poblacion[j+1];
        poblacion[j+1] = tempInd;
      }
    }
  }

  // Reemplazar la mitad inferior con mutaciones de la élite y valores aleatorios
  for(int i = POP_SIZE / 2; i < POP_SIZE; i++) {
    if(i % 2 == 0) {
      // Mutación basada en el padre más apto de la élite
      poblacion[i] = poblacion[0] ^ random(1, 4); 
    } else {
      // Individuo aleatorio (exploración de nuevas zonas de la piscina)
      poblacion[i] = random(0, 64);
    }
  }

  generacion++;
  delay(2000);
}