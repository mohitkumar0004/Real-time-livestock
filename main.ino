#include <WiFi.h>
#include <DHT.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WiFiServer server(80);

#define MQ2 34
#define MQ3 35
#define MQ135 32
#define MQ137 33

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
    delay(500);

  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  while (!client.available())
    delay(1);

  client.readStringUntil('\r');

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  int mq2 = analogRead(MQ2);
  int mq3 = analogRead(MQ3);
  int mq135 = analogRead(MQ135);
  int mq137 = analogRead(MQ137);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();

  client.println("<html><body>");
  client.println("<h1>Agriculture Monitor</h1>");
  client.print("Temperature: "); client.print(t); client.println(" C<br>");
  client.print("Humidity: "); client.print(h); client.println(" %<br>");
  client.print("MQ-2: "); client.println(mq2);
  client.print("MQ-3: "); client.println(mq3);
  client.print("MQ-135: "); client.println(mq135);
  client.print("MQ-137: "); client.println(mq137);
  client.println("</body></html>");

  delay(100);
}
