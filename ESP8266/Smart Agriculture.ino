ng#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#define DHTPIN D2 //pin DATA konek ke pin 2 Arduino
#define DHTTYPE DHT11 //tipe sensor DHT11
#define FIREBASE_HOST "https://ml-agriculture-default-rtdb.firebaseio.com/Monitoring"
#define FIREBASE_AUTH "3YoTEK5oQnOQSdGUHtPzxfAvg3qanwXpZQ1JT3Ta"
#define WIFI_SSID "Dhea's hp"
#define WIFI_PASSWORD "12345678"
FirebaseData firebaseData;

DHT dht(DHTPIN, DHTTYPE); //set sensor + koneksi pin

const int SoilSensor = A0;
float humi, temp;//deklarasi variabel 
int pompa = D5;

void setup() {

Serial.begin(115200); //baud 9600
dht.begin();
pinMode(SoilSensor, INPUT);
pinMode(pompa, OUTPUT);
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  float humidity_1 = dht.readHumidity();
  //Pembacaan dalam format celcius (c)
  float celcius_1 = dht.readTemperature();
  if (isnan(humidity_1) || isnan(celcius_1)) { //jika tidak ada hasil
    Serial.println("DHT11 tidak terbaca... !");
    return;
  }
  float kelembabanTanah;
  int hasilPembacaan = analogRead(SoilSensor);
  kelembabanTanah = (100 - ((hasilPembacaan/1023.00)*100));
  Serial.print("Persentase Kelembaban Tanah = ");
  Serial.print(kelembabanTanah);
  Serial.println("%");
  if (Firebase.setFloat(firebaseData, "/Monitoring/Tanah", kelembabanTanah)){
      Serial.println("Tanah terkirim");
    } else{
      Serial.println("Tanah tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    }
  //pembacaan nilai pembacaan data kelembaban
//  Serial.print("Kelembaban: ");
//  Serial.print(humidity_1);
//  Serial.println(" %\t"); 
  //pembacaan nilai pembacaan data suhu
  Serial.print("Suhu : ");
  Serial.print(celcius_1); //format derajat celcius
  Serial.print("°"); //simbol derajat
  Serial.println("C / ");
   if (Firebase.setFloat(firebaseData, "/Monitoring/Suhu", celcius_1)){
      Serial.println("Suhu terkirim");
    } else{
      Serial.println("Suhu tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    }
  delay(500);
///////////////////////////////////////////////////////////////////////////
  if (Firebase.getString(firebaseData, "/Monitoring/Pompa"))
  {
   if (firebaseData.dataType() == "string"){
    String fireStatus = firebaseData.stringData();
  
  if (fireStatus == "1") {
    Serial.println("Pompa Nyala");
    digitalWrite(pompa, HIGH);
    String ON = "ON";
    Firebase.setString(firebaseData, "/Monitoring/pompa_state", ON);
  }
  else if (fireStatus == "0") {
    Serial.println("Pompa Mati");
    digitalWrite(pompa, LOW);
    String OFF = "OFF";
    Firebase.setString(firebaseData, "/Monitoring/pompa_state", OFF);
  }}}

   if (Firebase.getString(firebaseData, "/Monitoring/Tanaman"))
  {
   if (firebaseData.dataType() == "string"){
    String fireStatus = firebaseData.stringData();
  
  if (fireStatus == "1") {
    Serial.println("Xerofit");
    String Xerofit = "Xerofit";
    Firebase.setString(firebaseData, "/Monitoring/tanaman_state", Xerofit);
  }
  if (fireStatus == "2") {
    Serial.println("Tropofit");
    String Tropofit = "Tropofit";
    Firebase.setString(firebaseData, "/Monitoring/tanaman_state", Tropofit);
  }
  if (fireStatus == "3") {
    Serial.println("Saprofit");
    String Saprofit = "Saprofit";
    Firebase.setString(firebaseData, "/Monitoring/tanaman_state", Saprofit);
  }}}
  
//if (Firebase.getString(firebaseData, "/Monitoring/Pelembab"))
//  {
//   if (firebaseData.dataType() == "string"){
//    String fireStatus = firebaseData.stringData();
//  
//  if (fireStatus == "1") {
//    Serial.println("Pelembab Nyala");
//    digitalWrite(LED, HIGH);
//    String ON = "ON";
////    Firebase.setString(firebaseData, "/Hasil_Pembacaan/kondisi_LED", ON);
//  }
//  else if (fireStatus == "0") {
//    Serial.println("Pelembab Mati");
//    digitalWrite(LED, LOW);
//    String OFF = "OFF";
////    Firebase.setString(firebaseData, "/Hasil_Pembacaan/kondisi_LED", OFF);
//  }}}
}
