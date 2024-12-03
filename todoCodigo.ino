#include <LiquidCrystal_I2C.h> 
#include <DHT11.h>
#define TiraLed 7
#define LDR A1
#define PIN_RELE 8  // Pin del relé conectado a la bomba

DHT11 dht11(2);
LiquidCrystal_I2C lcd(0x27, 16, 2); 
int minimo=0; 
int maximo=0;
void setup() {
    lcd.init(); 
    lcd.backlight(); 
    Serial.begin(9600);
    pinMode(TiraLed, OUTPUT);
    pinMode(LDR , INPUT);
    pinMode(PIN_RELE, OUTPUT);       // Configurar el pin del relé como salida
    digitalWrite(PIN_RELE, LOW);    // Asegurarse de que la bomba está apagada
    Serial.begin(9600);
    configurarLDR();

}
void loop() {
    int temperature = dht11.readTemperature();
    if (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");
    } else {
        Serial.println(DHT11::getErrorString(temperature));
    }
    int humidity = dht11.readHumidity();
    if (humidity != DHT11::ERROR_CHECKSUM && humidity != DHT11::ERROR_TIMEOUT) {
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(humidity));
    }
    
    int ldr = analogRead(LDR);
    int luz = map(ldr, 1, 310, 0, 100);
    Serial.print("LDR: ");
    Serial.print(ldr);
    Serial.print("-");
    Serial.print( luz );
    Serial.println("%");
     
    if (luz < 50){
      digitalWrite(TiraLed, LOW);
    }
    else{
      digitalWrite(TiraLed, HIGH);
    }

    if (humidity < 50.0 || humidity > 70.0) {
      Serial.println("Humedad fuera de rango, activando bomba...");
      digitalWrite(PIN_RELE, HIGH);  // Activar la bomba
      delay(15000);                  // Mantener encendida 15 segundos
    }
    else{
      Serial.println("Humedad en rango, desactivando bomba...");
      digitalWrite(PIN_RELE, LOW);  // Activar la bomba
      }

  delay(1000);  //
    lcd.setCursor(0, 0); 
    lcd.print("Humedad: "); 
    lcd.print(humidity);
    lcd.print("%"); 
    lcd.setCursor(0, 1); 
    lcd.print("Temp.: "); 
    lcd.print(temperature); 
    lcd.print("*C");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("LDR: "); 
    lcd.print(ldr);
    lcd.print("-");
    lcd.print(luz);
    lcd.print("%");
    delay(1000);
}
void configurarLDR(){
  Serial.println("Vamos a realizar la lectura de la temperatura ambiental");
  Serial.println("Tapa el sensor con el dedo para saber la luz minima ambiental");
  minimo= analogRead(LDR);
  delay(3000);
  Serial.print("Valor minimo: ");
  Serial.println(minimo);
  delay(1000);
  Serial.println("Saca el dedo para saber la temperatura maxima ambiental");
  maximo= analogRead(LDR);
  delay(3000);
  Serial.print("Valor maximo: ");
  Serial.println(maximo);
  delay(1000);
}
