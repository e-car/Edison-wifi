#include <signal.h> // you must write down this line to resolve problem between WiFiSocket and Serial communication
#include <WiFi.h>


/* -------------------------------- Wifi Parameters  -------------------------------- */
char ssid[] = "BUFFALO-4C7A25"; // your network SSID (name), nakayama:506A 304HWa-84F1A0
char pass[] = "iebiu6ichxufg"; // your network password (use for WPA, or use as key for WEP), nakayama:12345678 11237204a
int keyIndex = 0; // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiClient lastClient;
WiFiServer server(9090); // 9090番ポートを指定
int socketTimeCount = 0;
int socketTimeOut = 20;
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
  Serial.print("Client Status : ");
  Serial.println(client);

  // クライアント(Android)が存在する場合
  if (client) { 
    Serial.println("New Client");
    socketTimeCount = 0;
    
    // クライアント(Android)とサーバー(Edison)
    // 処理に約5秒かかる
    while ((connectStatus = client.connected())) { // 注意!! client.connected()関数にはバグあり!
      Serial.print("Socket TimeCount : ");
      Serial.println(socketTimeCount);
      Serial.print("Connect Status : ");
      Serial.println(connectStatus);
      
      if (client.available() > 0) {
        socketTimeCount = 0;
        char revChar = client.read(); // read from TCP buffer
        Serial.print("Get [");
        Serial.print(revChar);
        Serial.print("] -> Send  : ");
        // クライアントからのリクエストの受け取りとその処理
        /***********************************************************************************/
        switch(revChar) {  
          case 'G':
          // センサーデータ取得 from XBee
            Serial.println("Sensor Data by XBee");
            /*****************************************************************/
            Serial.println("test");
            client.println("test");
            /*****************************************************************/
            break;
          
          case 'T':
            Serial.println("test EV86 car data");
            client.println("test EV86 car data");
            break;
            
          case 'S':
            // close the connection:
            Serial.println("Stop Signal & Close Socket");
            client.flush();
            client.stop();
            Serial.println("client disonnected");
            break;
          
          default:
            Serial.println("Error : Request from Client is invalid");
            client.println("Error : Request from Client is invalid");
            break;
        }
        /***********************************************************************************/
      } else {
        socketTimeCount++;
      }
      
      // 一定時間クライアントから応答がなければ、サーバー側からSocketを切る * client.connected()のバグ対策
      if (socketTimeCount > socketTimeOut) {
        Serial.println("Socket TimeOut. close Socket from this server");
        client.flush();
        client.stop();
        break;
      }
    }
  delay(500);
  }
}


