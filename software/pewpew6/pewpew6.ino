// PewPew6 - ReMap and Rapid Fire
// buffi - 2019
// dewmanSNK - 2022

// Button configs below.
// Six autofire buttons with different frequencies are allowed.
// When "input_pin" of a button is connected to GND, "output_pin" will start auto-firing.
// Each pulse is "frames_active" frames long, with "frames_inactive" frames between each pulse.

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>


const int NUM_BUTT = 6;

const int JAMMA6 = 7;  //D7 PD7
const int JAMMA5 = 6;  //D6 PD6
const int JAMMA4 = 5;  //D5 PD5
const int JAMMA3 = 4;  //D4 PD4
const int JAMMA2 = 3;  //D3 PD3 
const int JAMMA1 = 2;  //D2 PD2

const int CONTROL6 = 5; //D13 PB5
const int CONTROL5 = 4; //D12 PB4
const int CONTROL4 = 3; //D11 PB3
const int CONTROL3 = 2; //D10 PB2
const int CONTROL2 = 1;  //D9  PB1
const int CONTROL1 = 0;  //D8  PB0


PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
String tagId = "None";
byte nuidPICC[4];


struct AutofireConfig {
  int frames_active, frames_inactive, input_pin, output_pin, autofire_state, autofire_active;
};

AutofireConfig BUTTON_1 = {
  /*frames_active=*/1,
  /*frames_inactive=*/1,
  /*input_pin=*/CONTROL1,
  /*output_pin=*/JAMMA1,
  /*autofire_state=*/0,
  /*autofire_active=*/0  
};

AutofireConfig BUTTON_2 = {
  /*frames_active=*/1,
  /*frames_inactive=*/1,
  /*input_pin=*/CONTROL2,
  /*output_pin=*/JAMMA2,
  /*autofire_state=*/0,
  /*autofire_active=*/0   
};

AutofireConfig BUTTON_3 = {
  /*frames_active=*/1,
  /*frames_inactive=*/1,
  /*input_pin=*/CONTROL3,
  /*output_pin=*/JAMMA3,
  /*autofire_state=*/0,
  /*autofire_active=*/0   
};

AutofireConfig BUTTON_4 = {
  /*frames_active=*/1,
  /*frames_inactive=*/1,
  /*input_pin=*/CONTROL4,
  /*output_pin=*/JAMMA4,
  /*autofire_state=*/0,
  /*autofire_active=*/0   
};

AutofireConfig BUTTON_5 = {
  /*frames_active=*/1,
  /*frames_inactive=*/1,
  /*input_pin=*/CONTROL5,
  /*output_pin=*/JAMMA5,
  /*autofire_state=*/0,
  /*autofire_active=*/0   
};

AutofireConfig BUTTON_6 = {
  /*frames_active=*/1,
  /*frames_inactive=*/1,
  /*input_pin=*/CONTROL6,
  /*output_pin=*/JAMMA3,
  /*autofire_state=*/0,
  /*autofire_active=*/1   
};

AutofireConfig *BUTTONS[] = { &BUTTON_1, &BUTTON_2, &BUTTON_3, &BUTTON_4, &BUTTON_5, &BUTTON_6 };


ISR(TIMER1_COMPA_vect) {

  //Pin data read in
  int pinb_r = PINB;

  //Port data to be set
  int portd_w = 0;
  int ddrd_w = 0;
  boolean firing = false;

  for (int i=0; i<NUM_BUTT; i++){

    if (pinb_r & ( 1 << BUTTONS[i]->input_pin)){ //Button NOT PRESSED
    
      BUTTONS[i]->autofire_state = 0;
      portd_w |= ~( 1 << BUTTONS[i]->output_pin); //- Set Pin LOW
      //ddrd_w  |= ~( 1 << BUTTONS[i]->output_pin); //- Sets Pin as INPUT // Take no action when button no pressed
    
    } else {
      if (BUTTONS[i]->autofire_active){
        if (BUTTONS[i]->autofire_state++ < BUTTONS[i]->frames_active){
      
          portd_w |= ~( 1 << BUTTONS[i]->output_pin); //- Set Pin LOW
          ddrd_w  |=  ( 1 << BUTTONS[i]->output_pin); //- Sets Pin as OUTPUT
          firing = true;
      
        } else {
      
			portd_w &= ~( 1 << BUTTONS[i]->output_pin); //- Set Pin LOW
			ddrd_w  &= ~( 1 << BUTTONS[i]->output_pin); //- Sets Pin as INPUT
      
		  }
          if (BUTTONS[i]->autofire_state > (BUTTONS[i]->frames_active + BUTTONS[i]->frames_inactive) ){
			  BUTTONS[i]->autofire_state = 0;
          }
      } else {
        
        portd_w |= ~( 1 << BUTTONS[i]->output_pin); //- Set Pin LOW
        ddrd_w  |=  ( 1 << BUTTONS[i]->output_pin); //- Sets Pin as OUTPUT

        }
    
      }
      
  //Aggrigate pin states through the loop
  portd_w |= portd_w;
  ddrd_w  |= ddrd_w;
  }
  
  //Assign pin states to pins (Single write to port registers per ISR)
  //PORTD = portd_w;
  DDRD  = ddrd_w;
}

void setup() {
  //Serial.begin(9600);
  nfc.begin();	
  
  DDRB  = B00000000; // set PORTB (digital 13~8) to INPUT
  PORTB = B11111111; // set PORTB to HIGH. INPUT_PULLUP
  
  DDRD  = B11111111; // set PORTD (digital 7~0) to OUTPUT

  // Setup timer.
  cli();
  TCCR1A = 0x00; // Timer 1 in normal mode
  TCCR1B = 0x02; // 16MHz clock with prescaler, TCNT1 increments every .5 uS
  TCNT1  = 0;    // Initialize counter to 0
  OCR1A = 33333; // = 16666 microseconds (each count is .5 us)
  TIMSK1 |= (1 << OCIE1A); // Enable compare interrupt
  sei();
  
}

void loop(){ 

  delay(5000);
  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    Serial.println(tag.getTagType());
    Serial.print("UID: ");Serial.println(tag.getUidString()); // Retrieves the Unique Identification from your tag

    if (tag.hasNdefMessage()) // If your tag has a message
    {

      NdefMessage message = tag.getNdefMessage();
      Serial.print("\nThis Message in this Tag is ");
      Serial.print(message.getRecordCount());
      Serial.print(" NFC Tag Record");
      if (message.getRecordCount() != 1) { 
        Serial.print("s");
      }
      Serial.println(".");

			// If you have more than 1 Message then it wil cycle through them
			int recordCount = message.getRecordCount();
			for (int i = 0; i < recordCount; i++)
			{
				Serial.print("\nNDEF Record ");Serial.println(i+1);
				NdefRecord record = message.getRecord(i);
			
				int payloadLength = record.getPayloadLength();
				byte payload[payloadLength];
				record.getPayload(payload);
				Serial.println(payloadLength);
			
				char *token = 0;
				//Sneaky skip first 3 garbage characters in NFC card read 
				token = strtok((payload+3), ",");
				int tk = 0;
				while (token != NULL){
					//Buttons A-F (ASCII A=65, F=70)
					tk = token[0];
					if (tk >= 65 & tk <= 70 ){
					Serial.println(token);
					token = strtok(NULL, ",");
					Serial.println(token);
					BUTTONS[(tk-65)]->autofire_active = atoi(token);
					token = strtok(NULL, ",");
					Serial.println(token);
					BUTTONS[(tk-65)]->frames_active = atoi(token);
					token = strtok(NULL, ",");
					Serial.println(token);
					BUTTONS[(tk-65)]->frames_inactive = atoi(token);
					token = strtok(NULL, ",\n");
					Serial.println(token);
					BUTTONS[(tk-65)]->output_pin = (atoi(token) + 1);
					}
					
					token = strtok(NULL, ",");
			
				}

			}
    }
	
  }

}
