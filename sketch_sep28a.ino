/*int [] arr = {01,02,03,04};
   size_t n = sizeof(arr) / sizeof(int);
   void print()
   {
   for(i=0;i<=n;i++)
   {


      SS_check_press_status();

      if(right_button_released==1)
      {
        uint16_t num = 0x30303038
        SS_display_image_from_memory_address(1,i);
    }

   }
   int recur  = arr[3];
   if(recur)
   {
   print()
   }

   if(left_button_released==1)
   {
    show the direction to the isle
   }
    void isle()
    {
      // display from mem-address based on GPS storage and i will be dependent on the GPS
        SS_display_image_from_memory_address(2,i);

    }
*/
int  left_button_pressed = 0;
int  right_button_pressed = 0;
int  left_button_released = 0;
int  right_button_released = 0;
uint16_t  index = 0001;
int toggle = 0;

void setup() {
  // put your setup code here, to run once:
  //uint16_t num = 30303038;
  Serial.begin(115200);
  //Serial.print(num);
  Serial.print(12);
  index = 0001;
  SS_reboot();
  //Serial.print(SS_comm_check());
  //SS_display_next_image(2);

}

void loop() {
  // put xyour main code here, to run repeatedly:
  if (Serial.available() > 0) {
    SS_check_press_status();
    //Serial.println(right_button_released);
    if (right_button_released == 1)
    {
      if(toggle == 1) {
        SS_display_image_from_memory_address(2,2);
        SS_display_image_from_memory_address(1,1);
        toggle = 0;
        return;
      }
      index++;
      if(index == 0005)
        index = 1;
      if(index == 0002)
        index++;
      SS_display_image_from_memory_address(1, index);
      
    }
    else if (left_button_released == 1)
    {
      index = 0001;
      toggle = 1;
      SS_display_image_from_memory_address(2, uint16_t(9));
      for(uint16_t i=5;i<9;i++) {
        SS_display_image_from_memory_address(1, i);
        delay(3000);
      }
    }
    SS_clear_button_variables();
  }

}


/*
   Inputs: None
   Outputs: None
   Function: Reboots the SmartSwitch
*/
void SS_reboot() {
  Serial.write(0x24);
}

/*
   Inputs: None
   Outputs: 1 if communication check is successful, 0 if communication check is failed
   Function: Checks if the SmartSwitch is successfully communicating with the Arduino
*/
int SS_comm_check() {
  Serial.write(0x1);
  delay(3);

  return Serial.read() == 0x61;
}

/*
   Inputs: brightness: 8-bit integer from 1-15 representing the desired brightness (15 is brightest)
   Outputs: 1 if SmartSwitch acknowledges command, 0 if SmartSwitch does not acknowledge command
   Function: Changes the brightness of the SmartSwitch
*/
int SS_set_brightness(uint8_t brightness) {
  int brightnessAsciiHex1 = nibble_to_ascii_hex((brightness & 0xF0) >> 4);
  int brightnessAsciiHex2 = nibble_to_ascii_hex(brightness & 0x0F);

  uint8_t msg[] = {0x27, 0x4E, brightnessAsciiHex1, brightnessAsciiHex2, 0x00};
  Serial.write(msg, sizeof(msg));
  delay(3);

  return Serial.read() == 0x61;
}

/*
   Inputs: offColor: 16-bit integer representing the background colors of text characters (must be in 565 BGR format)
           onColor: 16-bit integer representing the colors of text characters(must be in 565 BGR format)
   Outputs: 1 if SmartSwitch acknowledges command, 0 if SmartSwitch does not acknowledge command
   Function: Changes the color of text characters to the onColor and the background of those characters to the offColor
*/
int SS_set_onoff_color(uint16_t offColor, uint16_t onColor) {
  int offColorAsciiHex1 = nibble_to_ascii_hex((offColor & 0xF000) >> 12);
  int offColorAsciiHex2 = nibble_to_ascii_hex((offColor & 0x0F00) >> 8);
  int offColorAsciiHex3 = nibble_to_ascii_hex((offColor & 0x00F0) >> 4);
  int offColorAsciiHex4 = nibble_to_ascii_hex(offColor & 0x000F);
  int onColorAsciiHex1 = nibble_to_ascii_hex((onColor & 0xF000) >> 12);
  int onColorAsciiHex2 = nibble_to_ascii_hex((onColor & 0x0F00) >> 8);
  int onColorAsciiHex3 = nibble_to_ascii_hex((onColor & 0x00F0) >> 4);
  int onColorAsciiHex4 = nibble_to_ascii_hex(onColor & 0x000F);

  uint8_t msg[] = {0x27, 0x49, offColorAsciiHex1, offColorAsciiHex2, offColorAsciiHex3, offColorAsciiHex4, onColorAsciiHex1, onColorAsciiHex2, onColorAsciiHex3, onColorAsciiHex4};
  Serial.write(msg, sizeof(msg));
  delay(3);

  return Serial.read() == 0x61;
}

/*
   Inputs: switchNumber: 8-bit integer from 1-2 representing the desired switch (1 for left, 2 for right)
   Outputs: 1 if SmartSwitch acknowledges command, 0 if SmartSwitch does not acknowledge command
   Function: Adds the text "ASU" to the selected switch
*/
int SS_add_text(uint8_t switchNumber) {
  uint8_t switchCommand = 0;
  if (switchNumber == 2)
    switchCommand = 0x52;
  else
    switchCommand = 0x51;
  uint8_t msg[] = {0x27, switchCommand, 0x30, 0x33, 0x30, 0x41, 0x30, 0x41, 0x34, 0x31, 0x35, 0x33, 0x35, 0x35};
  Serial.write(msg, sizeof(msg));

  delay(3);

  return Serial.read() == 0x61;
}

/*
   Inputs: switchNumber: 8-bit integer from 1-2 representing the desired switch (1 for left, 2 for right)
   Outputs: 1 if SmartSwitch acknowledges command, 0 if SmartSwitch does not acknowledge command
   Function: Displays the next image from flash memory to the selected switch
*/
int SS_display_next_image(uint8_t switchNumber) {
  uint8_t switchCommand = 0;
  if (switchNumber == 2)
    switchCommand = 0x34;
  else
    switchCommand = 0x33;
  uint8_t msg[] = {0x2E, switchCommand};
  Serial.write(msg, sizeof(msg));

  delay(3);

  return Serial.read() == 0x61;
}

/*
   Inputs: switchNumber: 8-bit integer from 1-2 representing the desired switch (1 for left, 2 for right)
   Outputs: 1 if SmartSwitch acknowledges command, 0 if SmartSwitch does not acknowledge command
   Function: Displays the previous image from flash memory to the selected switch
*/
int SS_display_previous_image(uint8_t switchNumber) {
  uint8_t switchCommand = 0;
  if (switchNumber == 2)
    switchCommand = 0x36;
  else
    switchCommand = 0x35;
  uint8_t msg[] = {0x2E, switchCommand};
  Serial.write(msg, sizeof(msg));

  delay(3);

  return Serial.read() == 0x61;
}

/*
   Inputs: switchNumber: 8-bit integer from 1-2 representing the desired switch (1 for left, 2 for right)
           address: 16-bit integer representing the address of the image to load
   Outputs: 1 if SmartSwitch acknowledges command, 0 if SmartSwitch does not acknowledge command
   Function: Displays a specific image from flash memory to the selected switch loaded from the specified address
*/
int SS_display_image_from_memory_address(uint8_t switchNumber, uint16_t address)
{
  uint8_t switchCommand = 0;
  if (switchNumber == 2)
    switchCommand = 0x32;
  else
    switchCommand = 0x31;
  int addressAsciiHex1 = nibble_to_ascii_hex((address & 0x0001) >> 12);
  int addressAsciiHex2 = nibble_to_ascii_hex((address & 0x0F00) >> 8);
  int addressAsciiHex3 = nibble_to_ascii_hex((address & 0x00F0) >> 4);
  int addressAsciiHex4 = nibble_to_ascii_hex(address & 0x000F);
  uint8_t msg[] = {0x2E, switchCommand, addressAsciiHex1, addressAsciiHex2, addressAsciiHex3, addressAsciiHex4};
  Serial.write(msg, sizeof(msg));

  delay(3);

  return Serial.read() == 0x61;
}

/*
   Inputs: None
   Outputs: 1 if data was read, 0 if no data was read
   Function: If data was read, that data is sent to the Serial monitor
*/
int SS_read_byte() {
  int incomingByte = 0;
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, HEX);
    return 1;
  }
  return 0;
}

/*
   Inputs: None
   Outputs: None
   Function: Changes the values of global variables based on whether buttons have been pressed or released
   Global Variables to define: left_button_released, left_button_pressed, right_button_pressed, right_button_released
*/
void SS_check_press_status() {
  int incomingByte = 0;
  while (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    Serial.print("I received: ");
    Serial.println(incomingByte, HEX);
    if (incomingByte == 0x81)
      left_button_pressed = 1;
    else if (incomingByte == 0x82)
      right_button_pressed = 1;
    else if (incomingByte == 0xB1)
      left_button_released = 1;
    else if (incomingByte == 0xB2)
      right_button_released = 1;
  }
}

/*
   Inputs: None
   Outputs: None
   Function: Clears the values of global variables
   Global Variables to define: left_button_released, left_button_pressed, right_button_pressed, right_button_released
*/
void SS_clear_button_variables() {
  left_button_pressed = 0;
  right_button_pressed = 0;
  left_button_released = 0;
  right_button_released = 0;
}

/*
   Inputs: 8-bit integer representing a 4-bit "nibble"
   Outputs: A nibble converted to ascii hex
   Function: Helps translate values to ascii hex
*/
int nibble_to_ascii_hex(uint8_t nibble) {
  if (nibble < 0xA) // If the ASCII value is 0-9
  {
    return (int)(nibble + 0x30);
  }
  else if (nibble >= 0xA) // Or if the ASCII value is A-F
  {
    return (int)(nibble + 0x37);
  }
  return 0;
}
