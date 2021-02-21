#ifndef _SMARTIR_H_
#define _SMARTIR_H_

#include <ArduinoJson.h>

#define F_IR 38 /* Tan so hoat dong cua IR */

typedef void (*brandToCallBack_t)(JsonDocument &root);

void AC_LG_Callback(JsonDocument &root);
void AC_SAMSUNG_Callback(JsonDocument &root);
void AC_AmcorAc_Callback(JsonDocument &root);
void AC_Argo_Callback(JsonDocument &root);
void AC_Carrier_Callback(JsonDocument &root);
void AC_Coolix_Callback(JsonDocument &root);
void AC_Corona_Callback(JsonDocument &root);
void AC_DaiKin_Callback(JsonDocument &root);
void AC_Delonghi_Callback(JsonDocument &root);
void AC_Electra_Callback(JsonDocument &root);
void AC_Fujisu_Callback(JsonDocument &root);
void AC_Goodweather_Callback(JsonDocument &root);
void AC_Gree_Callback(JsonDocument &root);
void AC_Haier_Callback(JsonDocument &root);
void AC_Hitachi_Callback(JsonDocument &root);
void AC_Kelvinator_Callback(JsonDocument &root);
void AC_Midea_Callback(JsonDocument &root);
void AC_Mitshu_Callback(JsonDocument &root);
void AC_Neo_Callback(JsonDocument &root);
void AC_Panasonic_Callback(JsonDocument &root);
void AC_Shap_Callback(JsonDocument &root);
void AC_Tcl_Callback(JsonDocument &root);
void AC_Teco_Callback(JsonDocument &root);
void AC_Toshiba_Callback(JsonDocument &root);
void AC_Trotec_Callback(JsonDocument &root);
void AC_Vestel_Callback(JsonDocument &root);
void AC_whirlpool_Callback(JsonDocument &root);
void AC_daikinAc128_Callback(JsonDocument &root);
void AC_daikinAc152_Callback(JsonDocument &root);
void AC_daikinAc160_Callback(JsonDocument &root);
void AC_daikinAc176_Callback(JsonDocument &root);
void AC_daikinAc216_Callback(JsonDocument &root);
void AC_daikinAc2_Callback(JsonDocument &root);
void AC_daikinAc64_Callback(JsonDocument &root);

/* mảng danh sách tham chiếu các hãng điều hòa */
typedef enum {
    AC_DaiKin = 0,
    AC_daikinAc2,
    AC_daikinAc64,
    AC_daikinAc128,
    AC_daikinAc152,
    AC_daikinAc160,
    AC_daikinAc176,
    AC_daikinAc216,
    AC_SAMSUNG,
    AC_LG,
    AC_Toshiba,
    AC_Panasonic,
    AC_Fujisu,
    AC_AmcorAc,
    AC_Argo,
    AC_Carrier,
    AC_Coolix,
    AC_Corona,
    AC_Delonghi,
    AC_Electra,
    AC_Goodweather,
    AC_Gree,
    AC_Haier,
    AC_Hitachi,
    AC_Kelvinator,
    AC_Mitshu,
    AC_Neo,
    AC_Shap,
    AC_Tcl,
    AC_Teco,
    AC_Trotec,
    AC_Vestel,
    AC_whirlpool,
    AC_Midea
} id_AC_brand_t;



void SmartIR_begin(void);
bool ir_control_AC(JsonDocument &doc);

#endif /* _SMARTIR_H_ */