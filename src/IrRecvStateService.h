#ifndef IrrecvStateService_h
#define IrrecvStateService_h

#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>
#include <FSPersistence.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#define IR_RECV_PIN 5

#define IR_RECV_SETTINGS_ENDPOINT_PATH "/rest/irRecvState"
#define IR_RECV_SETTINGS_SOCKET_PATH "/ws/irRecvState"

typedef std::function<void()> HandlerUpdateCallback;

class IrRecvState {
 public:
  String rawData{""};
  String runTime{""};

  static void read(IrRecvState& settings, JsonObject& root) {
    root["rawData"] = settings.rawData;
    root["runTime"] = settings.runTime;
  }

  static StateUpdateResult update(JsonObject& root, IrRecvState& irRecvState) {
    String newRawData = root["rawData"];
    String newRunTime = root["runTime"];
    if (!irRecvState.rawData.equals(newRawData) || !irRecvState.runTime.equals(newRunTime)) {
      irRecvState.rawData = newRawData;
      irRecvState.runTime = newRunTime;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }
};

class IrRecvStateService : public StatefulService<IrRecvState> {
 public:
  IrRecvStateService(AsyncWebServer* server, SecurityManager* securityManager, FS* fs);

  void begin();

  void recv(HandlerUpdateCallback updateCallback);

 private:
  HttpEndpoint<IrRecvState> _httpEndpoint;
  WebSocketTxRx<IrRecvState> _webSocket;
  FSPersistence<IrRecvState> _fsPersistence;

  void registerConfig();
  void onConfigUpdated();
};

#endif