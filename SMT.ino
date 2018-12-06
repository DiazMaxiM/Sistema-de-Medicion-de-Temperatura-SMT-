#include <EtherCard.h>
#include <avr/wdt.h>  // include the library
#include <SimpleDHT.h>
int cantidadDeIntentos = 5;

SimpleDHT22 dht22;
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[700];
static uint32_t timer;
const char website[] PROGMEM = "10.1.103.6";

char buff[1];

static void my_callback (byte status, word off, word len) {
  Ethernet::buffer[off+700] = 0;
  String data =(const char*) Ethernet::buffer+off ;
  Serial.print(data);//.substring(data.indexOf("{"),data.length()));  
  if (data==""){
    wdt_reset();
  }
}

void setup () {
  Serial.begin(57600);
  Serial.println(F("\n[webClient]"));

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));
    
  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);
  
  if (!ether.dnsLookup(website))
    Serial.println("DNS failed");
        ether.parseIp(ether.hisip, "10.1.103.6");

  ether.printIp("SRV: ", ether.hisip);
  wdt_enable(WDTO_8S);  //Set up the watchdog timer. If it isn't reset every 2 seconds, the arduino will reset
}



char* string2char(String command){
  if(command.length()!=0){
    char *p = const_cast<char*>(command.c_str());
    return p;
    }
}



void loop () {
  ether.packetLoop(ether.packetReceive());
  if (millis() > timer) {
    timer = millis() + 60000;
    char gets[500]; 
    float temperature = 0;
    float humidity = 0;
    int err = SimpleDHTErrSuccess;
    int cantidadError=0;
    while ((err=dht22.read2(8, &temperature, &humidity, NULL))!= SimpleDHTErrSuccess && cantidadError<cantidadDeIntentos){
      delay(1000);
      cantidadError++;
    }
    String temp1 = String((float) temperature);
    temp1.replace(".",",");
    String hum1 = String((float) humidity);
    hum1.replace(".",",");
  
    temperature = 0;
    humidity = 0;
    err = SimpleDHTErrSuccess;
    cantidadError=0;
    while ((err=dht22.read2(9, &temperature, &humidity, NULL))!= SimpleDHTErrSuccess && cantidadError<cantidadDeIntentos){
      delay(1000);
      cantidadError++;
    }
    String temp2 = String((float) temperature);
    temp2.replace(".",",");
    String hum2 = String((float) humidity);
    hum2.replace(".",",");
    temperature = 0;
    humidity = 0;
    err = SimpleDHTErrSuccess;
    cantidadError=0;
  
    while ((err=dht22.read2(10, &temperature, &humidity, NULL))!= SimpleDHTErrSuccess && cantidadError<cantidadDeIntentos){
       delay(1000);    
     cantidadError++;
    }
    String temp3 = String((float) temperature);
    temp3.replace(".",",");
    String hum3 = String((float) humidity);
    hum3.replace(".",",");
    temperature = 0;
    humidity = 0;
    err = SimpleDHTErrSuccess;
    cantidadError=0;
    
    while ((err=dht22.read2(11, &temperature, &humidity, NULL))!= SimpleDHTErrSuccess && cantidadError<cantidadDeIntentos){
          delay(1000);

      cantidadError++;
    }
    String temp4 = String((float) temperature);
    temp4.replace(".",",");
    String hum4 = String((float) humidity);
    hum4.replace(".",",");
    Serial.println(temp1);
    Serial.println(hum1);
    Serial.println(temp2);
    Serial.println(hum2);
    Serial.println(temp3);
    Serial.println(hum3);
    Serial.println(temp4);
    Serial.println(hum4);
    sprintf(gets,"TEMP1=%s&HUM1=%s&TEMP2=%s&HUM2=%s&TEMP3=%s&HUM3=%s&TEMP4=%s&HUM4=%s",string2char(temp1),string2char(hum1),string2char(temp2),string2char(hum2),string2char(temp3),string2char(hum3),string2char(temp4),string2char(hum4)); 
    ether.browseUrl(PSTR("/sipfaweb/SMT/SMT1010WBG.PHP?case=guardar_lectura&" ),gets, website, my_callback); 
  }
}
