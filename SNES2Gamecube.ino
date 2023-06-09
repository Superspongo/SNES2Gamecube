// SNES2Gamecube Controller by SuperSpongo (2023)
// Uses library from NicoHood   https://github.com/NicoHood/Nintendo/
// and h-file from bitluni      https://github.com/bitluni/ArduinoGameController/
// 

#include "Nintendo.h"
#include "GameControllers.h"
#include "ButtonMapping.h"

// #define DEBUG 1

// Pin definitions
#define PIN_LED           ( LED_BUILTIN )

// Gamecube
#define PIN_GC_DATA       (   2 )

//SNES
#define PIN_SNES_DATA     (  17 )  // Arduino Pin A3  
#define PIN_SNES_LATCH    (  18 )  // Arduino Pin A4
#define PIN_SNES_CLOCK    (  19 )  // Arduino Pin A5

// Constants
#define ANALOG_MIDDLE     ( 128 )  // Gamecube Analog Stick value for middle
#define ANALOG_MIN        (  42 )  // Gamecube Analog Stick value for minimum (128 - 86)
#define ANALOG_MAX        ( 214 )  // Gamecube Analog Stick value for maximum (128 + 86)

#define BUTTON_NUM        (   8 )  // Number of Buttons on SNES Pad excluding DPAD

// Define SNES Controller
GameControllers tSNESController;

// Define a Gamecube console instance
CGamecubeConsole GamecubeConsole1( PIN_GC_DATA );

// Structure for data to be sent to console
Gamecube_Data_t tInputData = defaultGamecubeData;   

// DPad to Gamecube Mapping
bool bAxisModeAnalog = false;
bool bJustToggled    = false;

// SNES Button Array
bool abSNESButtons[ BUTTON_NUM ];

void setup() 
{
  // Start debug serial
  Serial.begin( 115200 );
    
  // Set up debug led (indicates AxisMode )
  pinMode( PIN_LED, OUTPUT );

  // initialize shared pins
  tSNESController.init( PIN_SNES_LATCH, PIN_SNES_CLOCK ); 
  
  // activate first controller ans set the type to SNES
  tSNESController.setController( 0, GameControllers::SNES, PIN_SNES_DATA );

  // initialize Button Mapping
  ButtonMappingInit();
  
  Serial.println( "Alles initialisiert" );
}

// Helper Function
bool PressedOrHeld ( GameControllers::Button eButton )
{
  bool bRetval = tSNESController.pressed( 0, eButton ) || tSNESController.down( 0, eButton );

  return bRetval;
}

void loop() 
{
  uint8_t i;

  // This resets and establishes all the values after the controller 
  // sends them to the console and helps with initial "zeroing"

  //DPAD
  uint8_t signalLEFT  = 0;
  uint8_t signalRIGHT = 0;
  uint8_t signalDOWN  = 0;
  uint8_t signalUP    = 0;

  uint8_t signalAnalogLeft  = 0;
  uint8_t signalAnalogRight = 0;
  uint8_t signalxAxis = ANALOG_MIDDLE;
  uint8_t signalyAxis = ANALOG_MIDDLE;
  
  for ( i=0; i<BUTTON_NUM; i++ )
  {
    abSNESButtons[ i ] = false;
  }

  //---------------------
  // SNES Controller Code
  //---------------------

  // Poll SNES Controller
  //
  tSNESController.poll();

  // Toggle Axis Mode by pressing L, R, Start and Select simultaneously:
  //
  if (     PressedOrHeld( GameControllers::R      )
       &&  PressedOrHeld( GameControllers::START  )
       &&  PressedOrHeld( GameControllers::SELECT )
       && !bJustToggled
     ) 
  {
    bAxisModeAnalog = !bAxisModeAnalog;
    bJustToggled    = true;
  }
  else
  {
    if (    !PressedOrHeld( GameControllers::R      )
         && !PressedOrHeld( GameControllers::START  )
         && !PressedOrHeld( GameControllers::SELECT ) 
       )
    {
      bJustToggled = false;
    }
  }
  
  // Set Mapping Mode
  //
  if (     PressedOrHeld( GameControllers::L      )
       &&  PressedOrHeld( GameControllers::R      )
       &&  PressedOrHeld( GameControllers::START  )
     )
  {
         if ( PressedOrHeld( GameControllers::UP    ) ) yActiveSet = MAPPING_SET_1;
    else if ( PressedOrHeld( GameControllers::DOWN  ) ) yActiveSet = MAPPING_SET_2;
    else if ( PressedOrHeld( GameControllers::LEFT  ) ) yActiveSet = MAPPING_SET_3;
    else if ( PressedOrHeld( GameControllers::RIGHT ) ) yActiveSet = MAPPING_SET_4;
    else                                                yActiveSet = yActiveSet;
  }
  
  // Read Buttons
  //
  abSNESButtons[ SNES_START  ] = PressedOrHeld( GameControllers::START  );
  abSNESButtons[ SNES_SELECT ] = PressedOrHeld( GameControllers::SELECT );
  abSNESButtons[ SNES_A ]      = PressedOrHeld( GameControllers::A      );
  abSNESButtons[ SNES_B ]      = PressedOrHeld( GameControllers::B      );
  abSNESButtons[ SNES_X ]      = PressedOrHeld( GameControllers::X      );
  abSNESButtons[ SNES_Y ]      = PressedOrHeld( GameControllers::Y      );
  abSNESButtons[ SNES_L ]      = PressedOrHeld( GameControllers::L      );
  abSNESButtons[ SNES_R ]      = PressedOrHeld( GameControllers::R      );
  
  // Process DPAD depending on axis Mode
  //
  if ( bAxisModeAnalog )
  {
    // X-Axis
    if (    (  PressedOrHeld( GameControllers::LEFT   ) ) 
         && ( !PressedOrHeld( GameControllers::RIGHT  ) ) ) signalxAxis  = ANALOG_MIN;
    
    if (    ( !PressedOrHeld( GameControllers::LEFT   ) )
         && (  PressedOrHeld( GameControllers::RIGHT  ) ) ) signalxAxis  = ANALOG_MAX;
    
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
  
  // Analog Triggers are also pushed all the way in, when the
  // clicky L/R buttons on the Gamecube pad are detected
  //
  if ( abSNESButtons[ ayIdxGamecubeL [ yActiveSet ] ] ) signalAnalogRight = ANALOG_MAX;
  if ( abSNESButtons[ ayIdxGamecubeR [ yActiveSet ] ] ) signalAnalogLeft  = ANALOG_MAX;
  
  // Report data to Console
  //                         See ButtonMapping.h for Index-Mapping!
  tInputData.report.a      = abSNESButtons[ ayIdxGamecubeA     [ yActiveSet ] ];
  tInputData.report.b      = abSNESButtons[ ayIdxGamecubeB     [ yActiveSet ] ];
  tInputData.report.x      = abSNESButtons[ ayIdxGamecubeX     [ yActiveSet ] ];
  tInputData.report.y      = abSNESButtons[ ayIdxGamecubeY     [ yActiveSet ] ];
  tInputData.report.z      = abSNESButtons[ ayIdxGamecubeZ     [ yActiveSet ] ];
  tInputData.report.start  = abSNESButtons[ ayIdxGamecubeSTART [ yActiveSet ] ];
  tInputData.report.r      = abSNESButtons[ ayIdxGamecubeL     [ yActiveSet ] ];
  tInputData.report.l      = abSNESButtons[ ayIdxGamecubeR     [ yActiveSet ] ];
  tInputData.report.right  = signalAnalogRight;
  tInputData.report.left   = signalAnalogLeft;
  tInputData.report.xAxis  = signalxAxis;
  tInputData.report.yAxis  = signalyAxis;
  tInputData.report.dleft  = signalLEFT;       // DPad Left
  tInputData.report.dright = signalRIGHT;      // DPad Right
  tInputData.report.ddown  = signalDOWN;       // DPad Down
  tInputData.report.dup    = signalUP;         // DPad Up

  //sends the complied data to console when console polls for the input
  //
  GamecubeConsole1.write( tInputData );

  // Write LED
  //
  // Normal indicator light for Analog/DPAD mode
  digitalWrite( PIN_LED, bAxisModeAnalog );
}
