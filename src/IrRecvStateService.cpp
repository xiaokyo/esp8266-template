#include "IrRecvStateService.h"

IRrecv irrecv(IR_RECV_PIN);
decode_results results;
unsigned long lastRecvTime{0};

IrRecvStateService::IrRecvStateService(AsyncWebServer* server, SecurityManager* securityManager) :
    _httpEndpoint(IrRecvState::read,
                  IrRecvState::update,
                  this,
                  server,
                  IR_RECV_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _webSocket(IrRecvState::read,
               IrRecvState::update,
               this,
               server,
               IR_RECV_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED) {
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void IrRecvStateService::begin() {
  _state.rawData = {0};
  onConfigUpdated();
}

void IrRecvStateService::onConfigUpdated() {
  Serial.println("onConfigUpdated, _state.rawData => ");
  Serial.println(_state.rawData);
}

void IrRecvStateService::recv(HandlerUpdateCallback updateCallback) {
  // 每一秒将rawData 加1
  if (millis() - lastRecvTime > 1000) {
    lastRecvTime = millis();
    _state.rawData++;
    updateCallback();
  }

  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);  // 打印 raw data
    irrecv.resume();

    // 保存到state               // 继续等待下一次红外信号
    _state.rawData = results.value;
    updateCallback();
  }
}