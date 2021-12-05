
#include "ESPAsyncWebServer.h"
#include "esp_camera.h"


AsyncWebServer server_80(80);
AsyncWebServer server_82(82);
AsyncWebServer server_83(83);

extern String WiFiAddr;
extern int LEDIO;
extern void WheelDriver();

// port 80

void handleRoot(AsyncWebServerRequest *request) {
  AsyncResponseStream *response = request->beginResponseStream("text/html");
  response->print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0\">\n");
  response->print("<script>var xhttp = new XMLHttpRequest();</script>");
  response->print("<script>function getsend(arg) { xhttp.open('GET', arg +'?' + new Date().getTime(), true); xhttp.send() } </script>");
  response->print("<p align=center><IMG SRC='http://" + WiFiAddr + ":81/stream' style='width:300px'></p><br/><br/>");
  response->print("<p align=center> <button style=background-color:lightgrey;width:90px;height:80px onmousedown=getsend('go') onmouseup=getsend('stop') ontouchstart=getsend('go') ontouchend=getsend('stop') ><b>Forward</b></button> </p>");
  response->print("<p align=center>");
  response->print("<button style=background-color:lightgrey;width:90px;height:80px; onmousedown=getsend('left') onmouseup=getsend('stop') ontouchstart=getsend('left') ontouchend=getsend('stop')><b>Left</b></button>&nbsp;");
  response->print("<button style=background-color:indianred;width:90px;height:80px onmousedown=getsend('stop') onmouseup=getsend('stop')><b>Stop</b></button>&nbsp;");
  response->print("<button style=background-color:lightgrey;width:90px;height:80px onmousedown=getsend('right') onmouseup=getsend('stop') ontouchstart=getsend('right') ontouchend=getsend('stop')><b>Right</b></button>");
  response->print("</p>");
  response->print("<p align=center><button style=background-color:lightgrey;width:90px;height:80px onmousedown=getsend('back') onmouseup=getsend('stop') ontouchstart=getsend('back') ontouchend=getsend('stop') ><b>Backward</b></button></p>");
  response->print("<p align=center>");
  response->print("<button style=background-color:yellow;width:140px;height:40px onmousedown=getsend('ledon')><b>Light ON</b></button>");
  response->print("<button style=background-color:yellow;width:140px;height:40px onmousedown=getsend('ledoff')><b>Light OFF</b></button>");
  response->print("</p>");
  request->send(response);
}

void handleGo(AsyncWebServerRequest *request) {
  Serial.println("Request Go");
  request->send(200);
}

void handleBack(AsyncWebServerRequest *request) {
  Serial.println("Request Back");
  request->send(200);
}

void handleLeft(AsyncWebServerRequest *request) {
  Serial.println("Request Left");
  request->send(200);
}

void handleRight(AsyncWebServerRequest *request) {
  Serial.println("Request Right");
  request->send(200);
}

void handleStop(AsyncWebServerRequest *request) {
  Serial.println("Request Stop");
  request->send(200);
}

void handleLedOn(AsyncWebServerRequest *request) {
  Serial.println("Request LedOn");
  digitalWrite(LEDIO, HIGH);
  request->send(200);
}

void handleLedOff(AsyncWebServerRequest *request) {
  Serial.println("Request LedOff");
  digitalWrite(LEDIO, LOW);
  request->send(200);
}


// port 82

void handleTestConn(AsyncWebServerRequest *request) {
  Serial.println("Request TestConn");
  request->send(200);
}


void handleStatus(AsyncWebServerRequest *request) {

  sensor_t * s = esp_camera_sensor_get();
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->print("{");
  response->printf("\"framesize\":%u,", s->status.framesize);
  response->printf("\"quality\":%u,", s->status.quality);
  response->printf("\"brightness\":%d,", s->status.brightness);
  response->printf("\"contrast\":%d,", s->status.contrast);
  response->printf("\"saturation\":%d,", s->status.saturation);
  response->printf("\"special_effect\":%u,", s->status.special_effect);
  response->printf("\"wb_mode\":%u,", s->status.wb_mode);
  response->printf("\"awb\":%u,", s->status.awb);
  response->printf("\"awb_gain\":%u,", s->status.awb_gain);
  response->printf("\"aec\":%u,", s->status.aec);
  response->printf("\"aec2\":%u,", s->status.aec2);
  response->printf("\"ae_level\":%d,", s->status.ae_level);
  response->printf("\"aec_value\":%u,", s->status.aec_value);
  response->printf("\"agc\":%u,", s->status.agc);
  response->printf("\"agc_gain\":%u,", s->status.agc_gain);
  response->printf("\"gainceiling\":%u,", s->status.gainceiling);
  response->printf("\"bpc\":%u,", s->status.bpc);
  response->printf("\"wpc\":%u,", s->status.wpc);
  response->printf("\"raw_gma\":%u,", s->status.raw_gma);
  response->printf("\"lenc\":%u,", s->status.lenc);
  response->printf("\"hmirror\":%u,", s->status.hmirror);
  response->printf("\"dcw\":%u,", s->status.dcw);
  response->printf("\"colorbar\":%u", s->status.colorbar);
  response->print("}");
  request->send(response);
}


void handleControl(AsyncWebServerRequest *request) {
  sensor_t * s = esp_camera_sensor_get();
  int res = 0;
  int cnt = 0;

  AsyncWebParameter *p = NULL;
  if (request->hasParam("quality", true)) {  //Check if POST (but not File) parameter exists
    p = request->getParam("quality", true);
    res = s->set_quality(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("contrast", true)) {
    p = request->getParam("contrast", true);
    res = s->set_contrast(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("brightness", true)) {
    p = request->getParam("brightness", true);
    res = s->set_brightness(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("saturation", true)) {
    p = request->getParam("saturation", true);
    res = s->set_saturation(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("gainceiling", true)) {
    p = request->getParam("gainceiling", true);
    res = s->set_gainceiling(s, (gainceiling_t)atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("colorbar", true)) {
    p = request->getParam("colorbar", true);
    res = s->set_colorbar(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("awb", true)) {
    p = request->getParam("awb", true);
    res = s->set_whitebal(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("agc", true)) {
    p = request->getParam("agc", true);
    res = s->set_gain_ctrl(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("aec", true)) {
    p = request->getParam("aec", true);
    res = s->set_exposure_ctrl(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("hmirror", true)) {
    p = request->getParam("hmirror", true);
    res = s->set_hmirror(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("vflip", true)) {
    p = request->getParam("vflip", true);
    res = s->set_vflip(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("awb_gain", true)) {
    p = request->getParam("awb_gain", true);
    res = s->set_awb_gain(s, atoi(p->value().c_str()));
    cnt ++;
  }


  if (request->hasParam("agc_gain", true)) {
    p = request->getParam("agc_gain", true);
    res = s->set_agc_gain(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("aec_value", true)) {
    p = request->getParam("aec_value", true);
    res = s->set_aec_value(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("aec2", true)) {
    p = request->getParam("aec2", true);
    res = s->set_aec2(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("dcw", true)) {
    p = request->getParam("dcw", true);
    res = s->set_dcw(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("bpc", true)) {
    p = request->getParam("bpc", true);
    res = s->set_bpc(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("wpc", true)) {
    p = request->getParam("wpc", true);
    res = s->set_wpc(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("raw_gma", true)) {
    p = request->getParam("raw_gma", true);
    res = s->set_raw_gma(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("lenc", true)) {
    p = request->getParam("aec2", true);
    res = s->set_lenc(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("special_effect", true)) {
    p = request->getParam("special_effect", true);
    res = s->set_special_effect(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("wb_mode", true)) {
    p = request->getParam("wb_mode", true);
    res = s->set_wb_mode(s, atoi(p->value().c_str()));
    cnt ++;
  }

  if (request->hasParam("ae_level", true)) {
    p = request->getParam("ae_level", true);
    res = s->set_ae_level(s, atoi(p->value().c_str()));
    cnt ++;
  }


  if (res || cnt == 0) {
    Serial.printf("Camera control failed");
    request->send(500);
  }
  request->send(200);
}


void handleCapture(AsyncWebServerRequest *request) {
  Serial.println("Request Capture");
  int64_t fr_start = esp_timer_get_time();

  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.printf("Camera capture failed");
    request->send(500);
  }
  int fb_len = fb->len;
  AsyncResponseStream *response = request->beginResponseStream("image/jpeg");
  response->addHeader("Content-Disposition", "inline; filename=capture.jpg");
  for (int i = 0; i < fb_len; i ++) {
    response->print(((const char *)fb->buf)[i]);
  }
  request->send(response);

  esp_camera_fb_return(fb);
  int64_t fr_end = esp_timer_get_time();
  Serial.printf("JPG: %uB %ums", (uint32_t)(fb_len), (uint32_t)((fr_end - fr_start) / 1000));
}

// port 83
// receive Map:
// LF|RF|LB|RB_Direction: 1 (Forward)|0 (Back)
// LF|RF|LB|RB_Speed: [0-100] (PWM signal)
void handleAppMoveCmd(AsyncWebServerRequest *request) {
  Serial.println("Request AppMoveCmd");
  request->send(200);
}

void startWebServer() {
  server_80.on("/", HTTP_GET, handleRoot);

  server_80.on("/go", HTTP_GET, handleGo);
  server_80.on("/back", HTTP_GET, handleBack);
  server_80.on("/left", HTTP_GET, handleLeft);
  server_80.on("/right", HTTP_GET, handleRight);

  server_80.on("/ledon", HTTP_GET, handleLedOn);
  server_80.on("/ledoff", HTTP_GET, handleLedOff);



  server_82.on("/test", HTTP_GET, handleTestConn);
  server_82.on("/capture", HTTP_GET, handleCapture);
  server_82.on("/status", HTTP_GET, handleStatus);
  server_82.on("/control", HTTP_POST, handleControl);



  server_83.on("/move", HTTP_POST, handleAppMoveCmd);

  server_80.begin(); //启动服务器
  Serial.println("\nWeb server on port 80 started");

  server_82.begin();
  Serial.println("Web server on port 82 started");

  server_83.begin();
  Serial.println("Web server on port 83 started");

}

void WheelDriver() {

}
