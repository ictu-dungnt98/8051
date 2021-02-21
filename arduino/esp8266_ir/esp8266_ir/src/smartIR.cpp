#include "smartIR.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include <macro.h>
#include <string.h>

#include "database.h"

/* lib for IR */
#include <ir_Amcor.h>        //ok
#include <ir_Argo.h>         //ok
#include <ir_Carrier.h>      //ok
#include <ir_Coolix.h>       //ok
#include <ir_Corona.h>       //ok
#include <ir_Daikin.h>       //ok
#include <ir_Delonghi.h>     //ok
#include <ir_Electra.h>      //ok
#include <ir_Fujitsu.h>      //ok
#include <ir_Goodweather.h>  //ok
#include <ir_Gree.h>         //ok
#include <ir_Haier.h>        //ok
#include <ir_Hitachi.h>      //ok
#include <ir_Kelvinator.h>   //ok
#include <ir_LG.h>           //ok
#include <ir_Magiquest.h>    //error
#include <ir_Midea.h>        //ok
#include <ir_Mitsubishi.h>   //ok
#include <ir_Neoclima.h>     //ok
#include <ir_Panasonic.h>    //ok
#include <ir_Samsung.h>      //ok
#include <ir_Sharp.h>        //ok
#include <ir_Tcl.h>          //ok
#include <ir_Teco.h>         //ok
#include <ir_Toshiba.h>      //ok
#include <ir_Trotec.h>       //ok
#include <ir_Vestel.h>       //ok
#include <ir_Whirlpool.h>    //ok

#define jsonPower "power"
#define jsonTemp "temp"
#define jsonMode "mode"
#define jsonModel "model"
#define jsonFan "fan"
#define jsonSwing "swing"
#define jsonBrand "brand"

const uint16_t kRecvPin = D1; /*Chân mắt thu hồng ngoại của esp8266  (pin 5) */
const uint8_t kIrLed = D2; /*Chân mắt phát hồng ngoại của esp8266  (pin 5) */

IRrecv irrecv(kRecvPin);
IRsend irsend(kIrLed);

IRLgAc lgAc(kIrLed);
IRSamsungAc samsungAc(kIrLed);
IRSharpAc shapAc(kIrLed);
IRPanasonicAc panasonicAC(kIrLed);
IRToshibaAC toshibaAc(kIrLed);
IRFujitsuAC fujisuAc(kIrLed);
IRMitsubishiAC mitsubishiAC(kIrLed);
IRHitachiAc hitachiAC(kIrLed);
IRTcl112Ac tclAc(kIrLed);
IRDaikinESP daikinAc(kIrLed);
IRCoolixAC coolixAc(kIrLed);
IRAmcorAc amcorAc(kIrLed);
IRArgoAC argoAc(kIrLed);
IRCarrierAc64 carrierAc(kIrLed);
IRCoronaAc coronaAC(kIrLed);
IRDelonghiAc delonghiAc(kIrLed);
IRElectraAc electraAC(kIrLed);
IRGoodweatherAc goodweatherAc(kIrLed);
IRGreeAC greeAc(kIrLed);
IRHaierACYRW02 haierAC(kIrLed);
IRKelvinatorAC kelvinatorAc(kIrLed);
IRMideaAC mideaAc(kIrLed);
IRNeoclimaAc neoAc(kIrLed);
IRTecoAc tecoAc(kIrLed);
IRTrotecESP trotecAc(kIrLed);
IRVestelAc vestelAc(kIrLed);
IRWhirlpoolAc whirlpooAc(kIrLed);
IRDaikin128 daikinAc128(kIrLed);
IRDaikin152 daikinAc152(kIrLed);
IRDaikin160 daikinAc160(kIrLed);
IRDaikin176 daikinAc176(kIrLed);
IRDaikin216 daikinAc216(kIrLed);
IRDaikin2 daikinAc2(kIrLed);
IRDaikin64 daikinAc64(kIrLed);


brandToCallBack_t fnOfBrand[] =
{
    AC_DaiKin_Callback,  // 0
    AC_daikinAc2_Callback,
    AC_daikinAc64_Callback,
    AC_daikinAc128_Callback,
    AC_daikinAc152_Callback,
    AC_daikinAc160_Callback,
    AC_daikinAc176_Callback,
    AC_daikinAc216_Callback,
    AC_SAMSUNG_Callback,
    AC_LG_Callback,
    AC_Toshiba_Callback,
    AC_Panasonic_Callback,
    AC_Fujisu_Callback,
    AC_AmcorAc_Callback,
    AC_Argo_Callback,
    AC_Carrier_Callback,
    AC_Coolix_Callback,
    AC_Corona_Callback,
    AC_Delonghi_Callback,
    AC_Electra_Callback,
    AC_Goodweather_Callback,
    AC_Gree_Callback,
    AC_Haier_Callback,
    AC_Hitachi_Callback,
    AC_Kelvinator_Callback,
    AC_Mitshu_Callback,
    AC_Neo_Callback,
    AC_Shap_Callback,
    AC_Tcl_Callback,
    AC_Teco_Callback,
    AC_Trotec_Callback,
    AC_Vestel_Callback,
    AC_whirlpool_Callback,
    AC_Midea_Callback
};

void SmartIR_begin(void)
{
    /* start sender  */
    irsend.begin();
    daikinAc.begin();
    daikinAc2.begin();
    daikinAc64.begin();
    daikinAc128.begin();
    daikinAc152.begin();
    daikinAc160.begin();
    daikinAc176.begin();
    daikinAc216.begin();
    greeAc.begin();
    samsungAc.begin();
    lgAc.begin();
    shapAc.begin();
    panasonicAC.begin();
    toshibaAc.begin();
    fujisuAc.begin();
    mitsubishiAC.begin();
    hitachiAC.begin();
    tclAc.begin();
    coolixAc.begin();
    amcorAc.begin();
    argoAc.begin();
    coronaAC.begin();
    delonghiAc.begin();
    electraAC.begin();
    goodweatherAc.begin();
    haierAC.begin();
    kelvinatorAc.begin();
    mideaAc.begin();
    neoAc.begin();
    tecoAc.begin();
    trotecAc.begin();
    vestelAc.begin();
    whirlpooAc.begin();

    /* start receiver */
    irrecv.enableIRIn();
}

/* {"brand":21, "power":1, "temp":18, "mode":1, "fan":1, "swing":1} */

bool ir_control_AC(JsonDocument &doc)
{
    bool result = false;
    uint8_t brand = doc[jsonBrand];

    if (doc[jsonPower].isNull() || doc[jsonTemp].isNull() ||
        doc[jsonMode].isNull() || doc[jsonFan].isNull())
        return result;

    if (brand >= 0 && brand < ARRAY_SIZE(fnOfBrand)) {
        fnOfBrand[brand](doc);
        result = true;
    } else {
        _BUGF_("brand invalid\n");
    }

    return result;
}

/* function control AC*/
void AC_LG_Callback(JsonDocument &root)
{
    lgAc.setTemp(root[jsonTemp]);
    lgAc.setMode(root[jsonMode]);
    lgAc.setFan(root[jsonFan]);
    lgAc.setPower(root[jsonPower] ? 1 : 0);
    lgAc.send();

    _BUG_("AC_LG");
    _BUG_(lgAc.toString());
}

void AC_SAMSUNG_Callback(JsonDocument &root)
{
    samsungAc.setTemp(root[jsonTemp]);
    samsungAc.setMode(root[jsonMode]);
    samsungAc.setFan(root[jsonFan]);
    samsungAc.setPower(root[jsonPower] ? 1 : 0);
    samsungAc.send();

    _BUG_("AC_SamSung");
    _BUG_(samsungAc.toString());
}

void AC_AmcorAc_Callback(JsonDocument &root)
{
    amcorAc.setTemp(root[jsonTemp]);
    amcorAc.setMode(root[jsonMode]);
    amcorAc.setFan(root[jsonFan]);
    amcorAc.setPower(root[jsonPower] ? 1 : 0);
    amcorAc.send();

    _BUG_("AC_AmcorAc");
    _BUG_(amcorAc.toString());
}

void AC_Argo_Callback(JsonDocument &root)
{
    argoAc.setTemp(root[jsonTemp]);
    argoAc.setMode(root[jsonMode]);
    argoAc.setFan(root[jsonFan]);
    argoAc.setPower(root[jsonPower] ? 1 : 0);
    argoAc.send();

    _BUG_("AC_Argo");
    _BUG_(argoAc.toString());
}

void AC_Carrier_Callback(JsonDocument &root)
{
    carrierAc.setTemp(root[jsonTemp]);
    carrierAc.setMode(root[jsonMode]);
    carrierAc.setFan(root[jsonFan]);
    carrierAc.setPower(root[jsonPower] ? 1 : 0);
    carrierAc.send();

    _BUG_("ACCarrier");
    _BUG_(carrierAc.toString());
}

void AC_Coolix_Callback(JsonDocument &root)
{
    coolixAc.setTemp(root[jsonTemp]);
    coolixAc.setMode(root[jsonMode]);
    coolixAc.setFan(root[jsonFan]);
    coolixAc.setPower(root[jsonPower] ? 1 : 0);
    coolixAc.send();

    _BUG_("Coolix");
    _BUG_(coolixAc.toString());
}

void AC_Corona_Callback(JsonDocument &root)
{
    coronaAC.setTemp(root[jsonTemp]);
    coronaAC.setMode(root[jsonMode]);
    coronaAC.setFan(root[jsonFan]);
    coronaAC.setPower(root[jsonPower] ? 1 : 0);
    coronaAC.send();

    _BUG_("Corona");
    _BUG_(coronaAC.toString());
}

void AC_DaiKin_Callback(JsonDocument &root)
{
    daikinAc.setTemp(root[jsonTemp]);
    daikinAc.setMode(root[jsonMode]);
    daikinAc.setFan(root[jsonFan]);
    daikinAc.setPower(root[jsonPower] ? 1 : 0);
    daikinAc.send();

    _BUG_("DaiKin");
    _BUG_(daikinAc.toString());
}

void AC_Delonghi_Callback(JsonDocument &root)
{
    delonghiAc.setTemp(root[jsonTemp]);
    delonghiAc.setMode(root[jsonMode]);
    delonghiAc.setFan(root[jsonFan]);
    delonghiAc.setPower(root[jsonPower] ? 1 : 0);
    delonghiAc.send();

    _BUG_("Delonghi");
    _BUG_(delonghiAc.toString());
}

void AC_Electra_Callback(JsonDocument &root)
{
    electraAC.setTemp(root[jsonTemp]);
    electraAC.setMode(root[jsonMode]);
    electraAC.setFan(root[jsonFan]);
    electraAC.setPower(root[jsonPower] ? 1 : 0);
    electraAC.send();

    _BUG_("Electra");
    _BUG_(electraAC.toString());
}

void AC_Fujisu_Callback(JsonDocument &root)
{
    fujisuAc.setTemp(root[jsonTemp]);
    fujisuAc.setModel(root[jsonModel]);
    fujisuAc.setMode(root[jsonMode]);
    fujisuAc.setFanSpeed(root[jsonFan]);
    fujisuAc.setPower(root[jsonPower] ? 1 : 0);
    fujisuAc.send();

    _BUG_("Fujisu");
    _BUG_(fujisuAc.toString());
}

void AC_Goodweather_Callback(JsonDocument &root)
{
    goodweatherAc.setTemp(root[jsonTemp]);
    goodweatherAc.setMode(root[jsonMode]);
    goodweatherAc.setFan(root[jsonFan]);
    goodweatherAc.setPower(root[jsonPower] ? 1 : 0);
    goodweatherAc.send();

    _BUG_("Goodweather");
    _BUG_(goodweatherAc.toString());
}

void AC_Haier_Callback(JsonDocument &root)
{
    haierAC.setTemp(root[jsonTemp]);
    haierAC.setMode(root[jsonMode]);
    haierAC.setFan(root[jsonFan]);
    haierAC.setPower(root[jsonPower] ? 1 : 0);
    haierAC.send();

    _BUG_("Haier");
    _BUG_(haierAC.toString());
}

void AC_Hitachi_Callback(JsonDocument &root)
{
    hitachiAC.setTemp(root[jsonTemp]);
    hitachiAC.setMode(root[jsonMode]);
    hitachiAC.setFan(root[jsonFan]);
    hitachiAC.setPower(root[jsonPower] ? 1 : 0);
    hitachiAC.send();

    _BUG_("Hitachi");
    _BUG_(hitachiAC.toString());
}

void AC_Kelvinator_Callback(JsonDocument &root)
{
    kelvinatorAc.setTemp(root[jsonTemp]);
    kelvinatorAc.setMode(root[jsonMode]);
    kelvinatorAc.setFan(root[jsonFan]);
    kelvinatorAc.setPower(root[jsonPower] ? 1 : 0);
    kelvinatorAc.send();

    _BUG_("Kelvinator");
    _BUG_(kelvinatorAc.toString());
}

void AC_Midea_Callback(JsonDocument &root)
{
    mideaAc.setTemp(root[jsonTemp]);
    mideaAc.setMode(root[jsonMode]);
    mideaAc.setFan(root[jsonFan]);
    mideaAc.setPower(root[jsonPower] ? 1 : 0);
    mideaAc.send();

    _BUG_("Midea");
    _BUG_(mideaAc.toString());
}

void AC_Mitshu_Callback(JsonDocument &root)
{
    mitsubishiAC.setTemp(root[jsonTemp]);
    mitsubishiAC.setMode(root[jsonMode]);
    mitsubishiAC.setFan(root[jsonFan]);

    mitsubishiAC.send();

    _BUG_("Mitshu");
    _BUG_(mitsubishiAC.toString());
}

void AC_Neo_Callback(JsonDocument &root)
{
    neoAc.setTemp(root[jsonTemp]);
    neoAc.setMode(root[jsonMode]);
    neoAc.setFan(root[jsonFan]);
    neoAc.setPower(root[jsonPower] ? 1 : 0);
    neoAc.send();

    _BUG_("Neo");
    _BUG_(neoAc.toString());
}

void AC_Panasonic_Callback(JsonDocument &root)
{
    panasonicAC.setTemp(root[jsonTemp]);
    panasonicAC.setMode(root[jsonMode]);
    panasonicAC.setFan(root[jsonFan]);
    panasonicAC.setPower(root[jsonPower] ? 1 : 0);
    panasonicAC.send();

    _BUG_("Panasonic");
    _BUG_(panasonicAC.toString());
}

void AC_Shap_Callback(JsonDocument &root)
{
    shapAc.setTemp(root[jsonTemp]);
    shapAc.setMode(root[jsonMode]);
    shapAc.setFan(root[jsonFan]);
    shapAc.setPower(root[jsonPower] ? 1 : 0);
    shapAc.send();

    _BUG_("Shap");
    _BUG_(shapAc.toString());
}

void AC_Tcl_Callback(JsonDocument &root)
{
    tclAc.setTemp(root[jsonTemp]);
    tclAc.setMode(root[jsonMode]);
    tclAc.setFan(root[jsonFan]);
    tclAc.setPower(root[jsonPower] ? 1 : 0);
    tclAc.send();

    _BUG_("Tcl");
    _BUG_(tclAc.toString());
}

void AC_Teco_Callback(JsonDocument &root)
{
    tecoAc.setTemp(root[jsonTemp]);
    tecoAc.setMode(root[jsonMode]);
    tecoAc.setFan(root[jsonFan]);
    tecoAc.setPower(root[jsonPower] ? 1 : 0);
    tecoAc.send();

    _BUG_("Teco");
    _BUG_(tecoAc.toString());
}

void AC_Toshiba_Callback(JsonDocument &root)
{
    toshibaAc.setTemp(root[jsonTemp]);
    toshibaAc.setMode(root[jsonMode]);
    toshibaAc.setFan(root[jsonFan]);
    toshibaAc.setPower(root[jsonPower] ? 1 : 0);
    toshibaAc.send();

    _BUG_("Toshiba");
    _BUG_(toshibaAc.toString());
}

void AC_Vestel_Callback(JsonDocument &root)
{
    vestelAc.setTemp(root[jsonTemp]);
    vestelAc.setMode(root[jsonMode]);
    vestelAc.setFan(root[jsonFan]);
    vestelAc.setPower(root[jsonPower] ? 1 : 0);
    vestelAc.send();

    _BUG_("Vestel");
    _BUG_(vestelAc.toString());
}

void AC_Gree_Callback(JsonDocument &root)
{
    greeAc.setTemp(root[jsonTemp]);
    greeAc.setMode(root[jsonMode]);
    greeAc.setFan(root[jsonFan]);
    greeAc.setPower(root[jsonPower] ? 1 : 0);
    greeAc.send();

    _BUG_("Gree");
    _BUG_(greeAc.toString());
}

void AC_Trotec_Callback(JsonDocument &root)
{
    trotecAc.setTemp(root[jsonTemp]);
    trotecAc.setMode(root[jsonMode]);
    trotecAc.setPower(root[jsonPower] ? 1 : 0);
    trotecAc.send();

    _BUG_("Trotec");
    _BUG_(trotecAc.toString());
}

void AC_whirlpool_Callback(JsonDocument &root)
{
    whirlpooAc.setTemp(root[jsonTemp]);
    whirlpooAc.setMode(root[jsonMode]);
    whirlpooAc.setFan(root[jsonFan]);
    whirlpooAc.setPowerToggle(root[jsonPower] ? 1 : 0);
    whirlpooAc.send();

    _BUG_("whirlpool");
    _BUG_(whirlpooAc.toString());
}

/*Waring*/
void AC_daikinAc128_Callback(JsonDocument &root)
{
    daikinAc128.setTemp(root[jsonTemp]);
    daikinAc128.setMode(root[jsonMode]);
    daikinAc128.setFan(root[jsonFan]);
    daikinAc128.setPowerful(root[jsonPower] ? 1 : 0);
    daikinAc128.send();

    _BUG_("DaiKin 128");
    _BUG_(daikinAc128.toString());
}
/*Waring*/
void AC_daikinAc152_Callback(JsonDocument &root)
{
    daikinAc152.setTemp(root[jsonTemp]);
    daikinAc152.setMode(root[jsonMode]);
    daikinAc152.setFan(root[jsonFan]);
    daikinAc152.setPower(root[jsonPower] ? 1 : 0);
    daikinAc152.send();

    _BUG_("DaiKin 152");
    _BUG_(daikinAc152.toString());
}

/*Waring*/
void AC_daikinAc160_Callback(JsonDocument &root)
{
    daikinAc160.setTemp(root[jsonTemp]);
    daikinAc160.setMode(root[jsonMode]);
    daikinAc160.setFan(root[jsonFan]);
    daikinAc152.setPower(root[jsonPower] ? 1 : 0);
    daikinAc160.send();

    _BUG_("DaiKin 160");
    _BUG_(daikinAc160.toString());
}

/*Waring*/
void AC_daikinAc176_Callback(JsonDocument &root)
{
    daikinAc176.setTemp(root[jsonTemp]);
    daikinAc176.setMode(root[jsonMode]);
    daikinAc176.setFan(root[jsonFan]);
    daikinAc176.setPower(root[jsonPower] ? 1 : 0);
    daikinAc176.send();

    _BUG_("DaiKin 176");
    _BUG_(daikinAc176.toString());
}

/*Waring*/
void AC_daikinAc216_Callback(JsonDocument &root)
{
    daikinAc216.setTemp(root[jsonTemp]);
    daikinAc216.setMode(root[jsonMode]);
    daikinAc216.setFan(root[jsonFan]);
    daikinAc216.setPower(root[jsonPower] ? 1 : 0);
    daikinAc216.send();

    _BUG_("DaiKin 216");
    _BUG_(daikinAc216.toString());
}

/*Waring*/
void AC_daikinAc2_Callback(JsonDocument &root)
{
    daikinAc2.setTemp(root[jsonTemp]);
    daikinAc2.setMode(root[jsonMode]);
    daikinAc2.setFan(root[jsonFan]);
    daikinAc2.setPower(root[jsonPower] ? 1 : 0);
    daikinAc2.send();

    _BUG_("DaiKin 2");
    _BUG_(daikinAc2.toString());
}

/*Waring*/
void AC_daikinAc64_Callback(JsonDocument &root)
{
    daikinAc64.setTemp(root[jsonTemp]);
    daikinAc64.setMode(root[jsonMode]);
    daikinAc64.setFan(root[jsonFan]);
    daikinAc64.setPowerToggle(root[jsonPower] ? 1 : 0);
    daikinAc64.send();

    _BUG_("DaiKin 64");
    _BUG_(daikinAc2.toString());
}