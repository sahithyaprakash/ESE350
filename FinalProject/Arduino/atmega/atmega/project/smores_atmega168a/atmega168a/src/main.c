/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "common_peripherals.h"
#include "TWI_slave.h"
#include "drive_coil.h"
#include "wiper_encoder.h"

//#define BOTTOM_FACE

/*   coil communication command    */
#define SPEAK  0xB5
#define LISTEN 0xB6

/*   coil macros:   */
#define FIRST_COIL  0xB1
#define SECOND_COIL 0xB2
#define THIRD_COIL  0xB3
#define FORTH_COIL  0xB4
#define MAGNETS_ON            0xA2  // command to turn on the blue led
#define MAGNETS_OFF           0xA3  // command to turn on green led
#define FACE_PING             0xA5  // Ping request to tell if face is alive
#define SEND_STATES           0xA4
#define SEND_ADC_DATA         0xA6

/*   macros for mapping B6 B7 input values to i2c address  */
#define BOTTOM_FACE_I2C_PININ 0b00000000
#define LEFT_FACE_I2C_PININ   0b00000001
#define RIGHT_FACE_I2C_PININ  0b00000010
#define TOP_FACE_I2C_PININ    0b00000011

/*   face i2c address macros:  */
#define BOTTOM_FACE_I2C_ADDRESS 0x0B
#define LEFT_FACE_I2C_ADDRESS   0x0A
#define RIGHT_FACE_I2C_ADDRESS  0x0C
#define TOP_FACE_I2C_ADDRESS    0x0D

/* Default Parameters */
#define DEFAULT_FREQUENCY 166
#define DEFAULT_DUTY 0.5f
#define DEFAULT_PULSE_NUM 3

unsigned char messageBuf[TWI_BUFFER_SIZE];
unsigned char determine_address();

unsigned char coil_id[2];

DIRECTION coil1_direction;
DIRECTION coil2_direction;
DIRECTION coil3_direction;
DIRECTION coil4_direction;

void process_twi(unsigned char TWI_Add);

int main(void)
{
    /* insert your hardware initialization here */
    m_clockdivide(0);
    InitPeripherals();
    
    unsigned char TWI_SlaveAddress = determine_address(); //default case
    TWI_Slave_Initialise(TWI_SlaveAddress<<TWI_ADR_BITS);
    TWI_Start_Transceiver();
    init_wiper_encoder();
    // coils initialization
    init_coil(DEFAULT_FREQUENCY, DEFAULT_DUTY, DEFAULT_PULSE_NUM);

    coil1_direction = STOP;
    coil2_direction = STOP;
    coil3_direction = STOP;
    coil4_direction = STOP;
    
    m_green(ON);m_yellow(ON);m_blue(ON);
    m_wait(1000);
    m_green(OFF);m_yellow(OFF);m_blue(OFF);

    sei(); 
    while(1)
    {
        process_twi(TWI_SlaveAddress);
    }
    return 0; 
}

unsigned char determine_address()
{
    clear(DDRB, 6);
    clear(DDRB, 7);
    
    char slave_address_read = (check(PINB, 6)<<1) | (check(PINB, 7));
    unsigned char TWI_SlaveAddress;

    if(slave_address_read == BOTTOM_FACE_I2C_PININ)
    {
        TWI_SlaveAddress = BOTTOM_FACE_I2C_ADDRESS;
    }
    else if (slave_address_read == LEFT_FACE_I2C_PININ)
    {
        TWI_SlaveAddress = LEFT_FACE_I2C_ADDRESS;
    }
    else if (slave_address_read == RIGHT_FACE_I2C_PININ)
    {
        TWI_SlaveAddress = RIGHT_FACE_I2C_ADDRESS;
    }
    else if (slave_address_read == TOP_FACE_I2C_PININ)
    {
        TWI_SlaveAddress = TOP_FACE_I2C_ADDRESS;
    }

    return TWI_SlaveAddress;
}

void process_twi(unsigned char TWI_SlaveAddress)
{
    unsigned char twi_data_state[7]; //seven dwarfs
    unsigned char twi_data_adc[11];

    int current_position = 0;
    long current_velocity = 0;
    int adc0 = 0;
#ifndef BOTTOM_FACE
    int adc1 = 0;
#endif
    if ( ! TWI_Transceiver_Busy() )
    {
        if ( TWI_statusReg.RxDataInBuf )
        {
            TWI_Get_Data_From_Transceiver(messageBuf, 1);
            switch(messageBuf[0])
            {
            case MAGNETS_ON:
                m_green(ON);
                coil1_direction = PLUS;
                drive_coil1(coil1_direction);
                m_wait(20);
                coil2_direction = PLUS;
                drive_coil2(coil2_direction);
                m_wait(20);
                coil3_direction = PLUS;
                drive_coil3(coil3_direction);
                m_wait(20);
                coil4_direction = PLUS;
                drive_coil4(coil4_direction);
                m_wait(20);
                drive_coil1(STOP);
                drive_coil2(STOP);
                drive_coil3(STOP);
                drive_coil4(STOP);
                break;
            case MAGNETS_OFF:
                m_green(OFF);
                coil1_direction = MINUS;
                drive_coil1(coil1_direction);
                m_wait(20);
                coil2_direction = MINUS;
                drive_coil2(coil2_direction);
                m_wait(20);
                coil3_direction = MINUS;
                drive_coil3(coil3_direction);
                m_wait(20);
                coil4_direction = MINUS;
                drive_coil4(coil4_direction);
                m_wait(20);
                drive_coil1(STOP);
                drive_coil2(STOP);
                drive_coil3(STOP);
                drive_coil4(STOP);
                break;

			case SPEAK:
				TWI_Get_Data_From_Transceiver(coil_id, 2);
				switch (coil_id[0]) {
				case FIRST_COIL:
					m_blue(ON);
					break;
				case SECOND_COIL:
					m_green(ON);
					break;
				case THIRD_COIL:
					m_blue(OFF);
					m_green(OFF);
					break;
				case FORTH_COIL:
					m_blue(ON);
					m_green(ON);
					break;
				default:
					break;
				}
				break;

			case FIRST_COIL:
				;
				m_blue(ON);
				unsigned char data_1 = FIRST_COIL;
				TWI_Start_Transceiver_With_Data(&data_1, 1);
				break;
			case SECOND_COIL:
				;
				m_green(ON);
				unsigned char data_2 = SECOND_COIL;
				TWI_Start_Transceiver_With_Data(&data_2, 1);
				break;
			case THIRD_COIL:
				;
				m_blue(OFF);
				m_green(OFF);
				unsigned char data_3 = THIRD_COIL;
				TWI_Start_Transceiver_With_Data(&data_3, 1);
				break;
			case FORTH_COIL:
				;
				m_blue(ON);
				m_green(ON);
				unsigned char data_4 = FORTH_COIL;
				TWI_Start_Transceiver_With_Data(&data_4, 1);
				break;
				
            case FACE_PING:
                // send a response back
                ; // I need to put this in order to declare a variable next (?!?!!)
                m_blue(TOGGLE);
                unsigned char twi_data;
                twi_data = TWI_SlaveAddress;
                TWI_Start_Transceiver_With_Data(&twi_data, 1);
                break;
            case SEND_STATES:
                current_position = return_current_position();
                current_velocity = return_current_velocity();
                twi_data_state[0]=(current_velocity & 0x000000FF);
                twi_data_state[1]=((current_velocity & 0x0000FF00)>>8);
                twi_data_state[2]=((current_velocity & 0x00FF0000)>>16);
                twi_data_state[3]=((current_velocity & 0xFF000000)>>24);
                twi_data_state[4]=((current_position) & 0x00FF);
                twi_data_state[5]=((current_position & 0xFF00)>>8);
                twi_data_state[6]=0xAA;
                TWI_Start_Transceiver_With_Data(twi_data_state, 7);
                break;

            case SEND_ADC_DATA:
                current_position = return_current_position();
                current_velocity = return_current_velocity();

            if(TWI_SlaveAddress == BOTTOM_FACE_I2C_ADDRESS)
            {
                adc0 = return_adc0();
                twi_data_adc[0]=(current_velocity & 0x000000FF);
                twi_data_adc[1]=((current_velocity & 0x0000FF00)>>8);
                twi_data_adc[2]=((current_velocity & 0x00FF0000)>>16);
                twi_data_adc[3]=((current_velocity & 0xFF000000)>>24);
                twi_data_adc[4]=((current_position) & 0x00FF);
                twi_data_adc[5]=((current_position & 0xFF00)>>8);
                twi_data_adc[6]=(adc0 & 0x00FF);
                twi_data_adc[7]=((adc0 & 0xFF00)>>8);
                twi_data_adc[8]=0xAA;
                TWI_Start_Transceiver_With_Data(twi_data_adc, 9);
            }
            else
            {
                adc0 = return_adc0();
                adc1 = return_adc1();
                twi_data_adc[0]=(current_velocity & 0x000000FF);
                twi_data_adc[1]=((current_velocity & 0x0000FF00)>>8);
                twi_data_adc[2]=((current_velocity & 0x00FF0000)>>16);
                twi_data_adc[3]=((current_velocity & 0xFF000000)>>24);
                twi_data_adc[4]=((current_position) & 0x00FF);
                twi_data_adc[5]=((current_position & 0xFF00)>>8);
                twi_data_adc[6]=(adc0 & 0x00FF);
                twi_data_adc[7]=((adc0 & 0xFF00)>>8);
                twi_data_adc[8]=(adc1 & 0x00FF);
                twi_data_adc[9]=((adc1 & 0xFF00)>>8);
                /* twi_data_adc[2]=(adc_value[0] & 0x00FF); */
                /* twi_data_adc[3]=((adc_value[0] & 0xFF00)>>8); */
                /* twi_data_adc[4]=(adc_value[1] & 0x00FF); */
                /* twi_data_adc[5]=((adc_value[1] & 0xFF00)>>8); */
                twi_data_adc[10]=0xAA;
                TWI_Start_Transceiver_With_Data(twi_data_adc, 11);
            }

                break;
            }
        }
    }
}

