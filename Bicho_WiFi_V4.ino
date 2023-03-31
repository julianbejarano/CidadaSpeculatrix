
//Creator: JulianBejarano
// Project: Anima CicadaSpeculatrix

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    2

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 12

int NumRedes;

//int LED = 1; //Define blinking LED pin
int minBrillo = 1;

int Brightness= 150;

int SegEspera= 50;
int SegEncendido= 10;
int maxRedes = 100;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Anima Project</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
 
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 2.0rem;}

    table, th, td {

      border: 1px solid black;
      border-collapse: collapse;
      text-align: center;
      padding: 10px;
      }

    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    
  </style>
</head>
<body>
  <h2>Anima Project</h2>
   <h2>Cicada Speculatrix</h2>
  <p>By Julian Bejarano</p>
  <br><a href=/resultados>Ver Redes</a>
  %TEXTPLACEHOLDER%
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEXTPLACEHOLDER"){
  return Mensaje;
  }
  return String();
}
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void scan (){
  //Serial.println(var);
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  NumRedes = n;

    Mensaje = "";

    if (n == 0) {
      Mensaje = "<p>" + String(n) + " ,no networks found</p> ";

    } 
    else {
      Mensaje= Mensaje+ "<h1>Redes 2G encontradas: "+ String (n) +"</h1>";
    
      Mensaje = Mensaje+ "<br>";
      Mensaje = Mensaje+ "<table style=\"width:80%\" >";
      for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        Mensaje = Mensaje+ "<tr><td>"+String(i + 1);
        Mensaje = Mensaje+ "</td>";
        Mensaje = Mensaje+ "<td>" +String(WiFi.SSID(i));
        Mensaje = Mensaje+ " </td>";
        Mensaje = Mensaje+ "<td>"+ String(WiFi.RSSI(i));
        Mensaje = Mensaje+ "</td></tr>";
        }
    }
     Mensaje = Mensaje+ "</table>";
   
}
 
void setup(){
   // pinMode(LED, OUTPUT);
    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    strip.setBrightness(minBrillo); // Set BRIGHTNESS (max = 255)

    Serial.begin(115200);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("Anima-CicadaSpeculatrix", "Aeiou123");
    IPAddress myIP = WiFi.softAPIP();
    server.begin();
    /*en esta función se carga el forms de donde sacamos dos datos*/
    //  digitalWrite(LED, LOW);
   // inicio();
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  
}

void showMensaje(){
    server.on("/resultados", HTTP_GET, [](AsyncWebServerRequest *request){   
    request->send(200, "text/html", Mensaje);
  });
}
  
void loop(){  
  scan();
  showMensaje();
  setColor();
  delay( SegEncendido *1000 );
  cleanColor();
  delay( SegEspera *1000);
}
