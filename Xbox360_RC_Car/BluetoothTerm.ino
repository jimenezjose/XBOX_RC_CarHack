/****************************************************************************
                                                Jose Jorge Jimenez-Olivas
                                                July 25, 2018

File Name:      BluetoothTerm.ino
Description:    Reduce print statements to Serial streams and handle 
                terminal commands.
****************************************************************************/
#include "BLuetoothTerm.h"

static bool disable = false; /* disable xbox controller input */

/***************************************************************************
% Routine Name : handleTerminal
% File :         BluetoothTerm.ino
% Parameters:    None
% Description :  Reads bluetooth data into a buffer and invokes subroutune 
%                interpretCmd
% Return:        Nothing
***************************************************************************/
void handleTerminal() {
  char buffer[ BUFFER_SIZE ] = { 0 }; /* BT terminal message */
  int index = 0; /* number of read bytes */
  
  if( BT.available() == 0 ) {
    /* no incomming bluetooth data */
    return;
  }

  while( BT.available() > 0 ) {
    /* read Serial bluetooth bytes into buffer */
    if( index == BUFFER_SIZE - 1 ) {
      /* flush out the overflow of data */
      clearSerialBuff();
      break;
    }
    buffer[ index ] = BT.read();
    if( buffer[ index ] == '\n' ) buffer[ index ] = '\0';
    index++;
    delay( BT_TX_DELAY );
  }

  /* null termination for string conversion */ 
  buffer[ index ] = '\0'; 

  String data = String( buffer );

  print( "\ncommand entered: " );
  println( data );

  /* interpret bluetooth data */
  if( interpretCmd(data) == false) {
    /* notify user of invalid command */
    println( "Invalid Command" );
  }
}

/***************************************************************************
% Routine Name : interpretCmd
% File :         BluetoothTerm.ino
% Parameters:    data - String of bytes from bluetooth data buffer 
% Description :  Invokes appropriate logic linked to command via function
%                pointers
% Return:        True - if valid command is parse, False - otherwise.
***************************************************************************/
int interpretCmd( String data ) {
  const String command[] = COMMANDS;
  void (* commandFunc[])() = COMMAND_FUNCS;
  int index = 0;

  while( command[ index ] != "" ) {
    /* validate commands string */
    if( command[ index ].equals(data) ) {
      /* executing command */
      commandFunc[ index ]();
      return true;
    }
    index++;
  }
  
  return false;
}

/***************************************************************************
% Routine Name : clearSerialBuff
% File :         BluetoothTerm.ino
% Parameters:    None 
% Description :  Clears Serial buffer residue
% Return:        Nothing
***************************************************************************/
void clearSerialBuff() {
  while( BT.available() > 0 ) {
    BT.read();
  }
}

/***************************************************************************
% Routine Name : terminalOverride
% File :         BluetoothTerm.ino
% Parameters:    None 
% Description :  Overrides Xbox 360 controller input.
% Return:        True if Xbox controller is disabled, or false otherwise.
***************************************************************************/
bool terminalOverride() {
  return disable;
}

/***************************************************************************
% Routine Name : disableController
% File :         Controller.ino
% Parameters:    None
% Description :  Sets global variable disable to true.
% Return:        Nothing
***************************************************************************/
void disableController() {
  println( "controller disabled" );
  disable = true;
}

/***************************************************************************
% Routine Name : enableController
% File :         BluetoothTerm.ino
% Parameters:    Nothing
% Description :  Enables contrioller input to be valid again by setting
%                global variable disable to false.
% Return:        Nothing
***************************************************************************/
void enableController() {
  println( "controller enabled" );
  disable = false;
}

/***************************************************************************
% Routine Name : toggleDebug
% File :         BluetoothTerm.ino
% Parameters:    None 
% Description :  Toggles Velocity and Steering notifications from Xbox 
%                controller
% Return:        Nothing
***************************************************************************/
void toggleDebug() {
  debug = !debug;

  print( "Velocity and Steering Notification Turned: " );

  if( debug ) {
    println( "ON" );
  }
  else {
    println( "OFF" );
  }
  
}

/***************************************************************************
% Routine Name : help
% File :         BluetoothTerm.ino
% Parameters:    None 
% Description :  Notifies user of all terminal commands and their usages
% Return:        Nothing
***************************************************************************/
void help() {
  println( HELP );
}

/***************************************************************************
% Routine Name : controllerHelp
% File :         BluetoothTerm.ino
% Parameters:    None 
% Description :  Notifies user of Xbox controller button mapping
% Return:        Nothing
***************************************************************************/
void controllerHelp() {
  println( USAGE );
}

/***************************************************************************
% Routine Name : print
% File :         BluetoothTerm.ino
% Parameters:    message - string to casted to serial and bluetooth streams
% Description :  Emulate original print() subroutine in the Stream class
% Return:        Number of bytes written to print streams
***************************************************************************/
long print( String message ) {
  Serial.print( message );
  BT.print( message );
  return sizeof( message );
}

/***************************************************************************
% Routine Name : print
% File :         BluetoothTerm.ino
% Parameters:    number - integer to be printed to the print streams
% Description :  Emulate original print() subroutine in the Stream class
% Return:        Number of bytes written to print streams
***************************************************************************/
long print( long number ) {
  String message = String( number );
  print( message );
  return sizeof( message );
}

/***************************************************************************
% Routine Name : println
% File :         BluetoothTerm.ino
% Parameters:    message - string to casted to serial and bluetooth streams
% Description :  Emulate original println() subroutine in the Stream class
% Return:        Number of bytes written to print streams.
***************************************************************************/
long println( String message ) {
  Serial.println( message );
  BT.println( message );
  /* + 1 for newline */
  return sizeof( message ) + 1;
}

/***************************************************************************
% Routine Name : println
% File :         BluetoothTerm.ino
% Parameters:    number - number to casted to the print streams
% Description :  Emulate original println() subroutine in the Stream class
% Return:        Number of bytes written to print streams
***************************************************************************/
long println( long number ) {
  String message = String( number );
  println( message );
  /* + 1 for newline */
  return sizeof( message ) + 1;
}

/***************************************************************************
% Routine Name : println
% File :         BluetoothTerm.ino
% Parameters:    None
% Description :  Emulate original println() subroutine in the Stream class
% Return:        Number of bytes written to print streams
***************************************************************************/
long println() {
  String newline = "\n";
  print( newline );
  return sizeof( newline );
}

