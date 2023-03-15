//
// This header file is for mapping different button layouts 
// of the Gamecube controller. 
// There are 8 SNES buttons that can be mapped to any button 
// on the Gamecube controller.
// The SNES controller's inputs are stored in an array.
// When the Gamecube report is created, these array values
// are used according to the indexes that are provided
// in this header file.
// "ayIdxGamecubeX" is the array of indexes for Gamecube
// button X.
// If the mapping is to be altered, the function 
// "ButtonMappingInit" has to be modified.
// The active mapping set is determined by "yActiveSet".
// Default is MAPPING_SET_1. 

// SNES Array Index
//
#define SNES_A      ( 0 )
#define SNES_B      ( 1 )
#define SNES_X      ( 2 )
#define SNES_Y      ( 3 )
#define SNES_SELECT ( 4 )
#define SNES_START  ( 5 )
#define SNES_L      ( 6 )
#define SNES_R      ( 7 )

#define MAPPING_SET_NUM  ( 4 )
#define MAPPING_SET_1    ( 0 )
#define MAPPING_SET_2    ( 1 )
#define MAPPING_SET_3    ( 2 )
#define MAPPING_SET_4    ( 3 )

// Button function mappings
//
uint8_t ayIdxGamecubeX[ MAPPING_SET_NUM ];
uint8_t ayIdxGamecubeA[ MAPPING_SET_NUM ];
uint8_t ayIdxGamecubeY[ MAPPING_SET_NUM ];
uint8_t ayIdxGamecubeB[ MAPPING_SET_NUM ];
uint8_t ayIdxGamecubeZ[ MAPPING_SET_NUM ];
uint8_t ayIdxGamecubeSTART[ MAPPING_SET_NUM ];
uint8_t ayIdxGamecubeL[ MAPPING_SET_NUM ];
uint8_t ayIdxGamecubeR[ MAPPING_SET_NUM ];

uint8_t yActiveSet = MAPPING_SET_1;

void ButtonMappingInit ( void )
{
  // Mapping Set 1 (Gameboy Player):
  ayIdxGamecubeX     [ MAPPING_SET_1 ] = SNES_A;      
  ayIdxGamecubeA     [ MAPPING_SET_1 ] = SNES_B;      
  ayIdxGamecubeY     [ MAPPING_SET_1 ] = SNES_SELECT;      // GBA Select: Gamecube Y
  ayIdxGamecubeB     [ MAPPING_SET_1 ] = SNES_Y;      
  ayIdxGamecubeZ     [ MAPPING_SET_1 ] = SNES_X;
  ayIdxGamecubeSTART [ MAPPING_SET_1 ] = SNES_START;  
  ayIdxGamecubeL     [ MAPPING_SET_1 ] = SNES_L;      
  ayIdxGamecubeR     [ MAPPING_SET_1 ] = SNES_R;     
  
  // Mapping Set 2 (Default: Normal 1 to 1):
  ayIdxGamecubeX     [ MAPPING_SET_2 ] = SNES_X;      
  ayIdxGamecubeA     [ MAPPING_SET_2 ] = SNES_A;      
  ayIdxGamecubeY     [ MAPPING_SET_2 ] = SNES_Y;      
  ayIdxGamecubeB     [ MAPPING_SET_2 ] = SNES_B;      
  ayIdxGamecubeZ     [ MAPPING_SET_2 ] = SNES_SELECT;
  ayIdxGamecubeSTART [ MAPPING_SET_2 ] = SNES_START;  
  ayIdxGamecubeL     [ MAPPING_SET_2 ] = SNES_L;      
  ayIdxGamecubeR     [ MAPPING_SET_2 ] = SNES_R;     
  
  // Mapping Set 3 (Gameboy Player A/B Style):
  ayIdxGamecubeX     [ MAPPING_SET_3 ] = SNES_X;      
  ayIdxGamecubeA     [ MAPPING_SET_3 ] = SNES_A;      
  ayIdxGamecubeY     [ MAPPING_SET_1 ] = SNES_SELECT;      // GBA Select: Gamecube Y
  ayIdxGamecubeB     [ MAPPING_SET_3 ] = SNES_B;      
  ayIdxGamecubeZ     [ MAPPING_SET_3 ] = SNES_Y;
  ayIdxGamecubeSTART [ MAPPING_SET_3 ] = SNES_START;  
  ayIdxGamecubeL     [ MAPPING_SET_3 ] = SNES_L;      
  ayIdxGamecubeR     [ MAPPING_SET_3 ] = SNES_R;     
  
  // Mapping Set 4 (Currently unused):
  ayIdxGamecubeX     [ MAPPING_SET_4 ] = SNES_X;      
  ayIdxGamecubeA     [ MAPPING_SET_4 ] = SNES_A;      
  ayIdxGamecubeY     [ MAPPING_SET_4 ] = SNES_Y;      
  ayIdxGamecubeB     [ MAPPING_SET_4 ] = SNES_B;      
  ayIdxGamecubeZ     [ MAPPING_SET_4 ] = SNES_SELECT;
  ayIdxGamecubeSTART [ MAPPING_SET_4 ] = SNES_START;  
  ayIdxGamecubeL     [ MAPPING_SET_4 ] = SNES_L;      
  ayIdxGamecubeR     [ MAPPING_SET_4 ] = SNES_R;     
}
