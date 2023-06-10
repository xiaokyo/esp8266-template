import { FC } from "react";

import { Input } from "@mui/material";

import { WEB_SOCKET_ROOT } from "../api/endpoints";
import {
  BlockFormControlLabel,
  FormLoader,
  MessageBox,
  SectionContent,
} from "../components";
import { updateValue, useWs } from "../utils";

import { IrRecvState } from "./types";

export const IR_RECV_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "irRecvState";

const IrRecvStateWebSocketForm: FC = () => {
  const { connected, updateData, data } = useWs<IrRecvState>(
    IR_RECV_SETTINGS_WEBSOCKET_URL
  );

  const updateFormValue = updateValue(updateData);

  const content = () => {
    if (!connected || !data) {
      return <FormLoader message="Connecting to WebSocket…" />;
    }
    return (
      <>
        <MessageBox
          level="info"
          message="The switch below Recv the IR via the WebSocket. It will automatically update whenever the IR state changes."
          my={2}
        />
        <BlockFormControlLabel
          control={
            <Input
              placeholder="Empty"
              name="rawData"
              value={data.rawData}
              onChange={updateFormValue}
            />
          }
          label="RawData"
        />
      </>
    );
  };

  return (
    <SectionContent title="WebSocket irRecv" titleGutter>
      {content()}
    </SectionContent>
  );
};

export default IrRecvStateWebSocketForm;
