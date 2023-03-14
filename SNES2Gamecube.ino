// SNES2Gamecube Controller
// Uses library from NicoHood   https://github.com/NicoHood/Nintendo/
// and h-file from bitluni      https://github.com/bitluni/ArduinoGameController/
// 

#include "Nintendo.h"
#include "GameControllers.h"

// Pin definitions
#define PIN_LED           ( LED_BUILTIN )

// Gamecube
#define PIN_GC_DATA       (   2 )

//SNES
#define PIN_SNES_DATA     (  26 )  // Arduino Pin A3  
#define PIN_SNES_LATCH    (  27 )  // Arduino Pin A4
#define PIN_SNES_CLOCK    (  28 )  // Arduino Pin A5

// Constants
#define ANALOG_MIDDLE     ( 128 )  // Gamecube Analog Stick value for middle
#define ANALOG_MIN        (  42 )  // Gamecube Analog Stick value for minimum
#define ANALOG_MAX        ( 214 )  // Gamecube Analog Stick value for maximum

#define DELAY_MS          (   5 )  // Delay after one cycle

// Define NES Controller
GameControllers tSNESController;

// Define a Gamecube Console Class
CGamecubeConsole GamecubeConsole1( PIN_GC_DATA );

// Structure for data to be sent to console
Gamecube_Data_t tInputData = defaultGamecubeData;   

// DPad to Gamecube Mapping
bool AxisModeAnalog = false;
bool JustToggled    = false;

void setup() 
{
  // Start debug serial
  Serial.begin( 115200 );
  
  
  // Set up debug led
  pinMode( PIN_LED, OUTPUT );

  // initialize shared pins
  tSNESController.init( PIN_SNES_LATCH, PIN_SNES_CLOCK ); 
  
  // activate first controller ans set the type to SNES
  tSNESController.setController( 0, GameControllers::SNES, PIN_SNES_DATA );
}

// Helper Function
bool PressedOrHeld ( GameControllers::Button eButton )
{
  bool bRetval = tSNESController.pressed( 0, eButton ) || tSNESController.down( 0, eButton );

  return bRetval;
}

void loop() 
{
  // This resets and establishes all the values after the controller 
  // sends them to the console and helps with initial "zeroing"
  uint8_t signalA = 0;
  uint8_t signalB = 0;
  uint8_t signalX = 0;
  uint8_t signalY = 0;
  uint8_t signalZ = 0;
  uint8_t signalSTART = 0;

  uint8_t signalR = 0;
  uint8_t signalL = 0;

  //DPAD
  uint8_t signalLEFT  = 0;
  uint8_t signalRIGHT = 0;
  uint8_t signalDOWN  = 0;
  uint8_t signalUP    = 0;

  uint8_t signalxAxis = ANALOG_MIDDLE;
  uint8_t signalyAxis = ANALOG_MIDDLE;

  //---------------------
  // SNES Controller Code
  //---------------------

  // Poll SNES Controller
  //
  tSNESController.poll();

  // Toggle Axis Mode by pressing L, R, Start and Select simultaneously:
  //
  if (     PressedOrHeld( GameControllers::L      )
       &&  PressedOrHeld( GameControllers::R      )
       &&  PressedOrHeld( GameControllers::START  )
       &&  PressedOrHeld( GameControllers::SELECT )
       && !JustToggled
     ) 
  {
    AxisModeAnalog = !AxisModeAnalog;
    JustToggled    = true;
  }
  else
  {
    JustToggled = false;
  }
  
  // Read Buttons
  //
  signalSTART = PressedOrHeld( GameControllers::START  );
  signalZ     = PressedOrHeld( GameControllers::SELECT );
  signalA     = PressedOrHeld( GameControllers::A      );
  signalB     = PressedOrHeld( GameControllers::B      );
  signalX     = PressedOrHeld( GameControllers::X      );
  signalY     = PressedOrHeld( GameControllers::Y      );
  signalL     = PressedOrHeld( GameControllers::L      );
  signalR     = PressedOrHeld( GameControllers::R      );
  
  // Process DPAD depending on axis Mode
  //
  if ( AxisModeAnalog )
  {
    // X-Axis
    if (    (  PressedOrHeld( GameControllers::LEFT   ) ) 
         && ( !PressedOrHeld( GameControllers::RIGHT  ) ) ) signalxAxis  = ANALOG_MAX;
    
    if (    ( !PressedOrHeld( GameControllers::LEFT   ) )
         && (  PressedOrHeld( GameControllers::RIGHT  ) ) ) signalxAxis  = ANALOG_MIN;
    
    // Y-Axis
    if (    (  PressedOrHeld( GameControllers::UP     ) )
         && ( !PressedOrHeld( GameControllers::DOWN   ) ) ) signalyAxis  = ANALOG_MAX;
    if (    ( !PressedOrHeld( GameControllers::UP     ) )
         && (  PressedOrHeld( GameControllers::DOWN   ) ) ) signalyAxis  = ANALOG_MIN;
  }
  else
  {
    if( PressedOrHeld( GameControllers::UP     ) ) signalUP    = 1;
    if( PressedOrHeld( GameControllers::DOWN   ) ) signalDOWN  = 1;
    if( PressedOrHeld( GameControllers::LEFT   ) ) signalLEFT  = 1;
    if( PressedOrHeld( GameControllers::RIGHT  ) ) signalRIGHT = 1;
  }

  // Report data to Console
  //
  tInputData.report.a      = signalA;
  tInputData.report.b      = signalB;
  tInputData.report.x      = signalX;
  tInputData.report.y      = signalY;
  tInputData.report.z      = signalZ;
  tInputData.report.start  = signalSTART;
  tInputData.report.r      = signalR;
  tInputData.report.l      = signalL;
  tInputData.report.xAxis  = signalxAxis;
  tInputData.report.yAxis  = signalyAxis;
  tInputData.report.dleft  = signalLEFT;       // DPad Left
  tInputData.report.dright = signalRIGHT;      // DPad Right
  tInputData.report.ddown  = signalDOWN;       // DPad Down
  tInputData.report.dup    = signalUP;         // DPad Up

  //sends the complied data to console when console polls for the input
  //
  GamecubeConsole1.write( tInputData );

  // Delay polling cycle
  //
  delay( DELAY_MS );
}
