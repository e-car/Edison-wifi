#include <signal.h> // you must write down this line to resolve problem between WiFiSocket and Serial communication
#include <WiFi.h>


/* -------------------------------- Wifi Parameters  -------------------------------- */
char ssid[] = "BUFFALO-4C7A25"; // your network SSID (name), nakayama:506A 304HWa-84F1A0
char pass[] = "iebiu6ichxufg"; // your network password (use for WPA, or use as key for WEP), nakayama:12345678 11237204a
int keyIndex = 0; // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
int timeoutCount = 0;

WiFiClient lastClient;
WiFiServer server(9090); // 9090番ポートを指定
boolean alreadyConnected = false; // クライアントとの接続を確認
int timeOutConnect = 0;
boolean connectStatus;

void setup() {
  /* you must write down a following line */
  signal(SIGPIPE, SIG_IGN); // caution !! Please don't erase this line
  
  /*
   * serial port document
   * Serial : /dev/ttyGS0 in Edison(Linux OS), thus your arduino IDE serial monitor
   * Serial1 : /dev/ttyMFD1 in Edison(Linux OS), thus Pin 0(Rx) and 1(Tx) in your arduino breakout board for Edison
   * Serial2 : /dev/ttyMFD2 in Edison(Linux OS), thus a terminal when you login to J3 in arduino breakout board for Edison with microUSB-B   
  */
  Serial.begin(9600);  // 対PC通信用シリアルポートの設定 Serial
  delay(5000);         // XBeeの設定完了を待つ

  // set WiFi
  setWiFi();
}

void loop() {
  Serial.println("[[[ loop start ]]]");
  //　サーバー(Edison)として指定したポートにクライアント(Android)からのアクセスがあるか確認。あれば、接続する 
  WiFiClient client = server.available();
  Serial.print("client status : ");
  Serial.println(client);
  
  // クライアント(Android)が存在する場合
  if(client) {
    Serial.println("New Client");
    // クライアント(Android)とサーバー(Edison)
    // 処理に約5秒かかる  
    while (client.connected() > 0) {
      Serial.print("Connect Status : ");
      Serial.println(client.connected());  
      if (client.available()) {
        char revChar = client.read(); // read from TCP buffer
        //client.println(revChar);
        Serial.println(revChar);
        /***********************************************************************************/
        switch(revChar) {  
          case 'G':
            client.println("test EV86 car data");
            break;
          case 'S':
            // close the connection:
            client.flush();
            client.stop();
            Serial.println("client disonnected");
            break;
          case 'C':
            client.println("Yahoo!!!!!");
            break;
          default:
            client.println("Error");
            break;
        }
        /***********************************************************************************/
      }
      delay(1);
    }
  }
}


