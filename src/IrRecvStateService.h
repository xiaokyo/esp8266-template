#ifndef IrrecvStateService_h
#define IrrecvStateService_h

#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#define IR_RECV_PIN 5

#define IR_RECV_SETTINGS_ENDPOINT_PATH "/rest/irRecvState"
#define IR_RECV_SETTINGS_SOCKET_PATH "/ws/irRecvState"

typedef std::function<void()> HandlerUpdateCallback;

class IrRecvState {
 public:
  int rawData{0};

  static void read(IrRecvState& settings, JsonObject& root) {
    root["rawData"] = settings.rawData;
  }

  static StateUpdateResult update(JsonObject& root, IrRecvState& irRecvState) {
    int newRawData = root["rawData"];
    if (irRecvState.rawData != newRawData) {
      irRecvState.rawData = newRawData;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }
};

class IrRecvStateService : public StatefulService<IrRecvState> {
 public:
  IrRecvStateService(AsyncWebServer* server, SecurityManager* securityManager);

  void begin();

  void recv(HandlerUpdateCallback updateCallback);

 private:
  HttpEndpoint<IrRecvState> _httpEndpoint;
  WebSocketTxRx<IrRecvState> _webSocket;

  void registerConfig();
  void onConfigUpdated();
};

#endif