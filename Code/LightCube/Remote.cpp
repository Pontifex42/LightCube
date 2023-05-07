
//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#include <ESP8266WiFi.h>
#include <RemoteXY.h>
#include "Remote.h"
#include "LEDChain.h"

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "LightCube"
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 42 bytes
{ 255,4,0,0,0,35,0,16,21,1,4,0,6,5,7,91,1,26,4,0,
23,5,7,91,12,26,4,0,37,5,7,91,6,26,4,0,51,5,7,91,
29,26 };

// this structure defines all the variables and events of your control interface 
struct {

    // input variables
    int8_t RED; // =0..100 slider position 
    int8_t GREEN; // =0..100 slider position 
    int8_t BLUE; // =0..100 slider position 
    int8_t BRIGHT; // =0..100 slider position 

    // other variable
    uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

bool IsRemoteConnected()
{
    return (RemoteXY.connect_flag == 1);
}

void SetupRemote()
{
    RemoteXY_Init();
}

void LoopRemote()
{
    RemoteXY_Handler();
    if (!IsRemoteConnected())
        return;

    static float sr = 0.0;
    static float sg = 0.0;
    static float sb = 0.0;
    static float sbright = 0.0;

    float r, g, b, bright;
    r = ((float)RemoteXY.RED) * 2.55;
    g = ((float)RemoteXY.GREEN) * 2.55;
    b = ((float)RemoteXY.BLUE) * 2.55;
    bright = ((float)RemoteXY.BRIGHT) * 2.55;

    if ((sr != r) || (sg != g) || (sb != b) || (sbright != bright))
    {
        SetWholeCube(r, g, b, bright);
        sr = r;
        sg = g;
        sb = b;
        sbright = bright;
    }
}