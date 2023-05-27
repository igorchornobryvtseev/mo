#pragma once

#include "CliMo.h"
#include "MoEthernet.h"

class MoSyncE : public Mo
{
public:
    static const T_MoType MO_TYPE = MO_SYNCE;

    static void BuildScript(MyOutStream* dstP);
    static void Create( );
    static void DownloadConfig( );

    RetStatus DoPerform(ActionType performType, void* dataP) override;
    void      DoHandleShow(ActionType cmdType) override;
    void      DoHandleSet(ActionType cmdType) override;

    ValueEthernetPort ethPort;

    DICTIONARY_2(PipeModeStatus, E_AdminState::Enable, "enabled", E_AdminState::Disable, "disabled");
    ValueDictT<PipeModeStatus, static_cast<uint32_t>(E_AdminState::Disable)> adminState;

private:
    RetStatus   CheckConstraints( );
};
