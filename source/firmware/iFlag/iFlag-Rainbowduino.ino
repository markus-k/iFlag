// MY9221 driver interfaces 
#define DDR_Data  DDRB
#define DDR_Clk   DDRB

#define PORT_Data PORTB
#define PORT_Clk  PORTB

#define BIT_Data  0x01
#define BIT_Clk   0x02

// 3-to-8 Decoder Lines

#define DDR_Lines  DDRD
#define PORT_Lines PORTD
#define BIT_Lines  0xF0

#define CmdMode 0b0000000000000000

uint8_t write_buffer = 1;
uint8_t read_buffer = 0;
uint8_t framebuffer[2][3][8][8] = {
  { // first buffer
    {
      // blue
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
      // green
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
      // red
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
    },
  },
  { // second buffer
    {
      // blue
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
      // green
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
      // red
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0 },
    },
  }
};

// functions for rainbowduino

void rb_init() {
  DDR_Lines |= BIT_Lines;
  PORT_Lines &=~ BIT_Lines;

  DDRD |= 0x04;

  DDR_Data |= BIT_Data;
  DDR_Clk |= BIT_Clk;
  PORT_Data &=~ BIT_Data;
  PORT_Clk &=~ BIT_Clk;

  DDRB |= 0x20;

  rb_clear_display();
  rb_init_timer1();
}

void rb_swap_buffers() {
  uint8_t b = read_buffer;
  read_buffer = write_buffer;
  write_buffer = b;
}

void rb_blank() {
  for (uint8_t x = 0; x < 8; x++) {
    for (uint8_t y = 0; y < 8; y++) {
      framebuffer[write_buffer][0][x][y] = 0;
      framebuffer[write_buffer][1][x][y] = 0;
      framebuffer[write_buffer][2][x][y] = 0;
    }
  }
}

void rb_set_pixel(uint8_t x, uint8_t y, uint8_t color_r, uint8_t color_g, uint8_t color_b) {
  framebuffer[write_buffer][0][x][y] = color_b;
  framebuffer[write_buffer][1][x][y] = color_g;
  framebuffer[write_buffer][2][x][y] = color_r;
}

void rb_fill(uint8_t color_r, uint8_t color_g, uint8_t color_b) {
  for (uint8_t x = 0; x < 8; x++) {
    for (uint8_t y = 0; y < 8; y++) {
      rb_set_pixel(x, y, color_r, color_g, color_b);
    }
  }
}

void rb_init_timer1() {
  TCCR1A = 0; // clear control register A
  TCCR1B = _BV(WGM13); // set mode as phase and frequency correct pwm, stop the timer
  ICR1 = 10000; // (XTAL * microseconds) / 2000000  1mS cycles
  TIMSK1 = _BV(TOIE1);
  TCNT1 = 0;
  TCCR1B |= _BV(CS10);
  sei(); // enable global interrupt 
}

void rb_send_data(uint16_t data) {
  for (uint8_t i = 0; i < 16; i++) {
    if (data & 0x8000) {
      PORT_Data |= BIT_Data;
    } else {
      PORT_Data &=~ BIT_Data;
    }

    PORT_Clk ^= BIT_Clk;
    data <<= 1;
  }
}

void rb_latch_data() {
  PORT_Data &=~ BIT_Data;

  delayMicroseconds(10);
  rb_switch_off_drive();
  for (uint8_t i = 0; i < 8; i++) {
    PORT_Data ^= BIT_Data;
  }
}

void rb_switch_off_drive() {
  PORT_Lines &=~ 0x80;
}

void rb_switch_on_drive(uint8_t line) {
  uint8_t LineBits = ((line)<<4);
  PORT_Lines &=~ BIT_Lines;
  PORT_Lines |= LineBits;
  PORT_Lines |= 0x80;
}

void rb_clear_display() {
  uint8_t i = 0;
  uint8_t f = 0;

  rb_send_data(CmdMode);
  PORT_Data &=~ BIT_Data;
  for(i=0; i < 192; i++) {
    PORT_Clk ^= BIT_Clk;
  }

  rb_send_data(CmdMode);
  PORT_Data &=~ BIT_Data;
  for(i = 0; i < 192; i++) {
    PORT_Clk ^= BIT_Clk;
  }
  rb_latch_data();
}

ISR(TIMER1_OVF_vect)          
{
  static uint8_t line_drive = 0;
  
  rb_clear_display();
  
  uint8_t line = (line_drive & 0x07);

  rb_send_data(CmdMode);

  rb_send_data(framebuffer[read_buffer][0][line][7]);
  rb_send_data(framebuffer[read_buffer][0][line][6]);
  rb_send_data(framebuffer[read_buffer][0][line][5]);
  rb_send_data(framebuffer[read_buffer][0][line][4]);
  rb_send_data(framebuffer[read_buffer][0][line][3]);
  rb_send_data(framebuffer[read_buffer][0][line][2]);
  rb_send_data(framebuffer[read_buffer][0][line][1]);
  rb_send_data(framebuffer[read_buffer][0][line][0]);

  rb_send_data(framebuffer[read_buffer][1][line][7]);
  rb_send_data(framebuffer[read_buffer][1][line][6]);
  rb_send_data(framebuffer[read_buffer][1][line][5]);
  rb_send_data(framebuffer[read_buffer][1][line][4]);
  
  rb_send_data(CmdMode);

  rb_send_data(framebuffer[read_buffer][1][line][3]);
  rb_send_data(framebuffer[read_buffer][1][line][2]);
  rb_send_data(framebuffer[read_buffer][1][line][1]);
  rb_send_data(framebuffer[read_buffer][1][line][0]);
  rb_send_data(framebuffer[read_buffer][2][line][7]);
  rb_send_data(framebuffer[read_buffer][2][line][6]);
  rb_send_data(framebuffer[read_buffer][2][line][5]);
  rb_send_data(framebuffer[read_buffer][2][line][4]);
  rb_send_data(framebuffer[read_buffer][2][line][3]);
  rb_send_data(framebuffer[read_buffer][2][line][2]);
  rb_send_data(framebuffer[read_buffer][2][line][1]);
  rb_send_data(framebuffer[read_buffer][2][line][0]);

  rb_latch_data();
  rb_switch_on_drive(line);
  line_drive++;

  PORTD &=~ 0x04;
}

// iFlag comms
// Version
byte major = 0;
byte minor = 19;

// Communication
#define DEVICE_ID      0xD2
#define PACKET_BYTE    0xFF
#define COMMAND_BYTE   0xFF
#define DRAW_COMMAND   0xA0
#define BLINK_COMMAND  0xA1
#define LUMA_COMMAND   0xA2
#define RESET_COMMAND  0xA9
#define PING_COMMAND   0xB0
int dataX;
int dataY;
int dataP;
uint8_t pinger;

// Color palette
const uint8_t colors[16][3] = {
  {   0,   0,   0 },   // 0x00 | black
  { 255, 255, 255 },   // 0x01 | white
  { 255,   0,   0 },   // 0x02 | red
  {   0, 255,   0 },   // 0x03 | green
  {   0,   0, 255 },   // 0x04 | blue
  { 255, 255,   0 },   // 0x05 | yellow
  {   0, 255, 255 },   // 0x06 | teal
  { 255,   0, 255 },   // 0x07 | purple
  { 255,  33,   0 },   // 0x08 | orange
  {  55,  55,  55 },   // 0x09 | dim white
  {  55,   0,   0 },   // 0x10 | dim red
  {   0,  55,   0 },   // 0x11 | dim green
  {   0,   0,  55 },   // 0x12 | dim blue
  {  55,  55,   0 },   // 0x13 | dim yellow
  {   0,  55,  55 },   // 0x14 | dim teal
  {  55,   0,  55 },   // 0x15 | dim purple
};
uint8_t balance[3] = { 36, 63, 63 }; // 0-63 RGB
                                    // Red is toned down to half here to limit the effects of its inevitable light
                                    // strength dominance due to physical custruction differences between all three
                                    // color chips in the matrix LED
                                    
uint8_t luma = 100;
uint8_t blinker;
uint8_t blink_speed = 0;

// Software reset
void (*resetFunc) (void) = 0;

void setup() {
  Serial.begin(9600);

  rb_init();
  
  uint8_t test[] = { 0x05, 0x06, 0x07, 0x01, 0x00 };
  for (int i = 0; i < sizeof(test); i++) {
    rb_fill(
        colors[test[i]][0], // R
        colors[test[i]][1], // G
        colors[test[i]][2]  // B
    );
    delay(1000);
  }
}

void loop() {
  if (!(pinger++)) {
    serialCommand(PING_COMMAND, DEVICE_ID, 0x00);
  }

  if (blink_speed && !(blinker += blink_speed)) {
    rb_swap_buffers();
  }

  delay(2);
}

void serialEvent(){
  while (Serial.available() && Serial.peek() != PACKET_BYTE) {
    Serial.read();
  }
  
  if (Serial.available() >= 8 && Serial.read() == PACKET_BYTE) {
    if (Serial.peek() != PACKET_BYTE) {
      // Stream
      dataX = Serial.read();          // (00-07) LED X
      dataY = Serial.read();          // (00-07) LED Y

      for (uint8_t i = 0; i < 4; i++) {
        dataP = Serial.read();      // (00-FE) four color pixels

        rb_set_pixel(
            dataX + i,                           // X
            dataY,                               // Y
            luma * colors[dataP][0] / 100,   // R
            luma * colors[dataP][1] / 100,   // G
            luma * colors[dataP][2] / 100    // B
        );
      }
    } else {
      // Command
      uint8_t command[4] = {
        Serial.read(),      // (FF) Command trigger byte
        Serial.read(),      // (00-FE) Command id
        Serial.read(),      // (00-FE) Command value
        Serial.read()       // (00-FE) Command extra value
      };
      
      switch (command[1]) {
        case DRAW_COMMAND:
          rb_swap_buffers();
          break;
        case BLINK_COMMAND:
          blink_speed = command[2];
          blinker = 0;
          break;
        case RESET_COMMAND:
          resetFunc();
          break;
        case LUMA_COMMAND:
          luma = command[2];
          break;
      }
    }
  }
}

void serialCommand(uint8_t command, uint8_t value, uint8_t extra) {
  uint8_t payload[8] = {
    PACKET_BYTE,      // (FF) start byte
    COMMAND_BYTE,     // (FF) serial command trigger byte
    major,            // (00-FE) major firmware version
    minor,            // (00-FE) minor firmware version
    command,          // (00-FE) command id
    value,            // (00-FE) command value
    extra,            // (00-FE) optional extra value
    0x00
  };

  for( int i = 0; i < sizeof(payload) / sizeof(payload[0]); i++) {
    Serial.println( payload[ i ] );
  }
}

