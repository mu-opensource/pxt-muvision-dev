#include "pxt.h"
#include "MuVisionSensor3/MuVisionSensor.h"
#include "MuVisionSensor3/MuVisionSensor3_AT.h"

MuVisionSensor* MU[4]={nullptr};

enum mu_id_t {
    //% block="MU00"
    MU00=0,
    //% block="MU01"
    MU01,
    //% block="MU10"
    MU10,
    //% block="MU11"
    MU11
};

enum mu_status_t {
    //% block="enable"
    enable=1,
    //% block="disable"
    disable=0
};

enum mu_ls_t {
    //% block="proximity detect"
    kLsProximity=       1,
    //% block="ambient light detect"
    kLsAmbientLight=    2,
    //% block="gesture detect"
    kLsGesture=         8,
};

enum mu3at_mode_t {
    //%block="STA"
    kModeSTA,
    //%block="AP"
    kModeAP,
};

//% color="#ff6600" weight=50 icon="\uf110"
namespace MUVisionSensor {

    /**
     * Initialize MU.
     */
    //% blockId=mu_init block="init %id|interface %port"
    //% weight=100
    //% group="Settings"
    void begin(mu_id_t id, MuVsMode port) {
        if (!MU[id]) {
            MU[id] = new MuVisionSensor(0x60+id);
        }
        MuVisionSensor *mu = MU[id];
        if (port == kSerialMode) {
            mu->begin(&uBit.serial);
        } else if(port == kI2CMode) {
            mu->begin(&uBit.i2c);
        }
    }

    /**
     * Reset MU.
     */
    //% blockId=MU_reset block="%id|restore default settings"
    //% weight=100
    //% group="Settings"
    void reset(mu_id_t id){
        MuVisionSensor *mu = MU[id];
        while(mu->SensorSetDefault()!=MU_OK);
    }

    /**
     * MU vision begin.
     */
    //% blockId=MU_VisionBegin block="%id|%enable|algorithm%type"
    //% weight=50
    //% group="Settings"
    void VisionBegin(mu_id_t id, mu_status_t status, MuVsMessageVisionType type){
        MuVisionSensor *mu = MU[id];
        if (status) {
            while(mu->VisionBegin((MuVisionType)visionTypeEnumToMacro(type))!=MU_OK);
        } else {
            mu->VisionEnd((MuVisionType)visionTypeEnumToMacro(type));
        }
    }

    /**
     * set led color.
     * @param id MU id
     * @param led led type.
     * @param detected_color led color while sensor detected target.
     * @param undetected_color led color while sensor undetected target.
     * @param level led brightness, form 0(close) to 15
     */
    //% blockId=MU_set_led block="%id|LED %led|when detected %detected_color|when undetected %undetected_color"
    //% weight=200 inlineInputMode=inline
    //% group="Settings" advanced=true
    void set_led(mu_id_t id, MuVsLed led, MuVsLedColor detected_color, MuVsLedColor undetected_color) {
        if (detected_color == undetected_color) {
            while(MU[id]->LedSetMode(led, 1, 1) != MU_OK);
        } else {
            while(MU[id]->LedSetMode(led, 0, 0) != MU_OK);
        }
        while(MU[id]->LedSetColor(led, detected_color, undetected_color, 1) != MU_OK);
    }
    /**
     * set vision level.
     * @param id MU id
     * @param type vision type
     * @param level vision level
     */
    //% blockId=MU_set_level block="%id|algorithm%VISION_TYPE|Level%level"
    //% weight=96
    //% group="Settings" advanced=true
    void set_level(mu_id_t id, MuVsMessageVisionType type, MuVsVisionLevel level){
        while(MU[id]->VisionSetLevel((MuVisionType)visionTypeEnumToMacro(type), level) != MU_OK);
    }
    /**
     * set camera zoom.
     * @param id MU id
     * @param zoom zoom value.
     */
    //% blockId=MU_set_zoom block="%id|digital zoom%level"
    //% weight=95
    //% group="Settings" advanced=true
    void set_zoom(mu_id_t id, MuVsCameraZoom zoom) {
        while(MU[id]->CameraSetZoom(zoom) != MU_OK);
    }

    // /**
    //  * set UART baud rate.
    //  * @param id MU id
    //  * @param  UART baud rate.
    //  */
    // //% blockId=MU_set_baudrate block="%id|baudrate%baud"
    // //% weight=94 inlineInputMode=inline
    // //% group="Settings" advanced=true
    // //% deprecated=true
    // void set_baudrate(mu_id_t id, MuVsBaudrate baud){
    //     MuVisionSensor *mu = MU[id];
    //     while(MU[id]->UartSetBaudrate(MuVsBaudrate(baud)) != MU_OK);
    // }
    /**
     * set camera white balance.
     * @param id MU id
     * @param wb white balance type.
     */
    //% blockId=MU_set_awb block="%id|white balance%wb"
    //% weight=93
    //% group="Settings" advanced=true
    void set_WB(mu_id_t id, MuVsCameraWhiteBalance wb) {
        while(MU[id]->CameraSetAwb(wb) != MU_OK);
    }
    /**
     * set camera FPS.
     * @param id MU id
     * @param on FPS type.
     */
    //% blockId=MU3CameraSetFPS block="%id|high FPS mode$on"
    //% on.shadow="toggleOnOff" on.defl="true"
    //% group="Settings" advanced=true
    void onOff(mu_id_t id, bool on) {
        while(MU[id]->CameraSetFPS(MuVsCameraFPS(on)) != MU_OK);
    }
    /**
     * Enable/Disable light sensor function,gesture detect can not used with other functions.
     * @param id MU id
     * @param status enable/disable function
     * @param[in] ls_type Function type
     */
    //% blockId=MU3LsBegin block="%id|light sensor|%status|%ls_type"
    //% group="Light Sensor"
    void LsBegin(mu_id_t id, mu_status_t status, mu_ls_t ls_type) {
        if (status == enable) {
            MU[id]->LsBegin(ls_type);
        } else {
            MU[id]->LsEnd(ls_type);
        }
    }
    /**
     * Light snesor set sensitivity.
     * @param id MU id
     * @param sensitivity sensitivity
     */
    //% blockId=MU3LsSetSensitivity block="%id|light sensor|set sensitivity%sensitivity"
    //% group="Light Sensor"
    void LsSetSensitivity(mu_id_t id, MuVsLsSensitivity sensitivity) {
        MU[id]->LsSetSensitivity(sensitivity);
    }
    /**
     * @brief Read proximity data.
     * @param id MU id
     * @retval proximity data, 0~255.
     */
    //% blockId=MU3LsReadProximity block="%id|light sensor|read proximity"
    //% group="Light Sensor"
    uint8_t LsReadProximity(mu_id_t id) {
        return MU[id]->LsReadProximity();
    }
    /**
     * @brief Read ambient light sensor data.
     * @param id MU id
     * @retval ambient light sensor data, 0~65536.
     */
    //% blockId=MU3LsReadAmbientLight block="%id|light sensor|read ambient light"
    //% group="Light Sensor"
    uint16_t LsReadAmbientLight(mu_id_t id) {
        return MU[id]->LsReadAmbientLight();
    }
    /**
     * @brief Read light sensor color data.
     * @param id MU id
     * @param color_t kLsColorLabel:      get color label
     *                    kLsColorRed:        get RGB R value(0~255)
     *                    kLsColorGreen:      get RGB G value(0~255)
     *                    kLsColorBlue:       get RGB B value(0~255)
     *                    kLsColorHue:        get HSV H value(0~360)
     *                    kLsColorSaturation: get HSV S value(0~255)
     *                    kLsColorValue:      get HSV V value(0~255)
     * @retval Light sensor detected color data value
     */
    //% blockId=MU3LsReadColor block="%id|light sensor|read color %color_t"
    //% group="Light Sensor"
    //% deprecated=true
    uint16_t LsReadColor(mu_id_t id, MuVsLsColorType color_t) {
        return MU[id]->LsReadColor(color_t);
    }
}

//% color="#ff6600" weight=50 icon="\uf110"
namespace muvs {
    /**
     * get vision result data, this function will update vision result automatically.
     * @param id MU id
     * @param vision_type: vision type.
     * @param object_inf:  object information
    */
    //% blockId=MU3GetValue
    int GetValue(mu_id_t id, MuVsMessageVisionType vision_type, MuVsObjectInf object_inf) {
        return MU[id]->GetValue((MuVisionType)visionTypeEnumToMacro(vision_type), object_inf);
    }
    /**
     * @brief  write vision parameter.
     * @param id MU id
     * @param vision_type vision type.
     * @param object_inf  object information
     * @param value  value
     */
    //% blockId=MU3write
    void write(mu_id_t id, MuVsMessageVisionType vision_type, MuVsObjectInf object_inf, uint8_t value) {
        MU[id]->write((MuVisionType)visionTypeEnumToMacro(vision_type), object_inf, value);
    }
    /**
     * @brief Read gesture sensor data.
     * @retval Gesture witch MU detected.
     */
    //% blockId=MU3LsReadGesture
    MuVsLsGesture LsReadGesture(mu_id_t id) {
        return MU[id]->LsReadGesture();
    }
    //%
    int get_value(int id, int type, int item){
        MuVisionSensor *mu = MU[id];
        type=1<<(type-1);
        return mu->GetValue(type,MuVsObjectInf(item));    
    }
    //%
    int get_color_value(int id, int item){
        MuVisionSensor *mu = MU[id];
        return mu->GetValue(VISION_COLOR_RECOGNITION,MuVsObjectInf(item));    
    }
}

//% color="#11ACEF" icon="\uf1eb"
namespace MUVisionSensor_AT {
    int Mu3AtRead8() {
        uint64_t time = system_timer_current_time();  
        int c = 0;
        do {
            c = uBit.serial.read(MicroBitSerialMode::ASYNC);
        } while(c==MICROBIT_NO_DATA && system_timer_current_time()-time<1000);  
        if (system_timer_current_time()-time>=1000) {    
            return -1;  
        }
        return c;
    }
    void Mu3AtWrite8(uint8_t c) {
        uBit.serial.sendChar(c);
    }
    MuVisionSensor3_AT MU3_AT(Mu3AtRead8, Mu3AtWrite8);

    /**
     * Read SIP 
     */
    //% blockId=MU3ATWifiSIP block="MU|read SIP"
    //% group="MUVisionSensor3_AT"
    String WifiSIP() {
        return MU3_AT.WifiSIP();
    }
    /**
     * Read CIP 
     */
    //% blockId=MU3ATWifiCIP block="MU|read CIP"
    //% group="MUVisionSensor3_AT"
    String WifiCIP() {
        return MU3_AT.WifiCIP();
    }
    /**
     * MU AT wifi set
     */
    //% blockId=MU3ATWifiSet block="MU|WiFi set|ssid|%ssid|password|%password|apmode|%apmode"
    //% group="MUVisionSensor3_AT"
    void WifiSet(String ssid, String password, mu3at_mode_t apmode) {
        switch (apmode) {
            case kModeSTA:
                MU3_AT.WifiSet(ssid->getUTF8Data(), password->getUTF8Data(), "STA");
                break;
            case kModeAP:
                MU3_AT.WifiSet(ssid->getUTF8Data(), password->getUTF8Data(), "AP");
                break;
            default:
                break;
        }
    }
    /**
     * MU AT wifi connect, return `true`=success
     */
    //% blockId=MU3ATWifiCon block="MU|WiFi connect|%status"
    //% status.shadow="toggleOnOff" status.defl="true"
    //% group="MUVisionSensor3_AT"
    bool WifiCon(bool status) {
        if (status) {
            return !MU3_AT.WifiCon("1");
        } else {
            return !MU3_AT.WifiCon("0");
        }
    }
    /**
     * MU AT wifi set target IP&port 
     */
    //% blockId=MU3ATWifiUDP block="MU|WiFi set target IP|%ip|port|%port"
    //% group="MUVisionSensor3_AT"
    void WifiUDP(String ip, String port) {
        MU3_AT.WifiUDP(ip->getUTF8Data(), port->getUTF8Data());
    }
    /**
     * MU AT wifi read data 
     */
    //% blockId=MU3ATWifiRead block="MU|WiFi read"
    //% group="MUVisionSensor3_AT"
    int WifiRead() {
        if (MU3_AT.available()) {
            return MU3_AT.read8();;
        }
        return Mu3AtRead8();
    }
}
