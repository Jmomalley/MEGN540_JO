#include "Encoder.h"

// include Led Interface for trobleshooting

#include "led_interface.h"

/**
 * Internal counters for the Interrupts to increment or decrement as necessary.
 */
static volatile bool _last_right_A   = 0;  // Static limits it's use to this file
static volatile bool _last_right_B   = 0;  // Static limits it's use to this file
static volatile bool _last_right_XOR = 0;

static volatile bool _last_left_A   = 0;  // Static limits it's use to this file
static volatile bool _last_left_B   = 0;  // Static limits it's use to this file
static volatile bool _last_left_XOR = 0;  // Necessary to check if PB4 triggered the ISR or not

static volatile int32_t _left_counts  = 0;  // Static limits it's use to this file
static volatile int32_t _right_counts = 0;  // Static limits it's use to this file

float gear_ratio = 0.00690687;  // The conversion for the encoder counts to radians 1/(909.7/2pi)

/** Helper Funcions for Accessing Bit Information */
// *** MEGN540 Lab 3 TODO ***
// Hint, use avr's bit_is_set function to help
static inline bool Right_XOR()
{
    return bit_is_set( PINE, PINE6 );
}  // MEGN540 Lab 3 TODO
static inline bool Right_B()
{
    return bit_is_set( PINF, PINF0 );
}  // MEGN540 Lab 3 TODO
static inline bool Right_A()
{
    bool B   = Right_B();
    bool XOR = Right_XOR();
    return XOR ^ B;
}  // MEGN540 Lab 3 TODO

static inline bool Left_XOR()
{
    return bit_is_set( PINB, PINB4 );
}  // MEGN540 Lab 3 TODO
static inline bool Left_B()
{
    return bit_is_set( PINE, PINE2 );
}  // MEGN540 Lab 3 TODO
static inline bool Left_A()
{
    bool B   = Left_B();
    bool XOR = Left_XOR();
    return XOR ^ B;
}  // MEGN540 Lab 3 TODO

/**
 * Function Encoders_Init initializes the encoders, sets up the pin change interrupts, and zeros the initial encoder
 * counts.
 */
void Initialize_Encoders()
{

    // CURRENTLY SETUP LEFT AND RIGHT SEPARATE FOR SAKE OF DEBUGGING, ONCE WORKING COMBINE REGISTER ACTIVATION AGAIN

    // *** MEGN540 Lab3 ***
    // SETUP LEFT ENCODER //
    // PORTE |= ( 1 << PORTE2 ) || ( 1 << PORTE6 );  // PE2 Set Port E pint 2 to input Encoder Channel B and pin 6 for Right XOR
    PORTE |= ( 1 << PORTE2 );  // PE2
    PORTB |= ( 1 << PORTB4 );  // PB4

    // DDRE &= ( 1 << DDE2 ) || ( 1 << DDE6 );  // Set the DDR to INPUT for Port E pin2 and pin6 to input "0"
    DDRE &= ( 1 << DDE2 );  // INPUT
    DDRB &= ( 1 << DDB4 );  // INPUT

    PCMSK0 |= ( 1 << PCINT4 );  // Set the interupt for left side XOR

    PCICR |= ( 1 << PCIE0 );  // Enable left XOR interrupt PCINT 0-7

    // SETUP RIGHT ENCODED //

    PORTE |= ( 1 << PORTE6 );  // Setup right XOR signal REMOVE ONCE DEBUG COMPLETE
    DDRE &= ( 1 << DDE6 );     // Setup right XOR signal REMOVE ONCE DEBUG COMPLETE

    PORTF |= ( 1 << PORTF0 );  // Setup right XOR signal REMOVE ONCE DEBUG COMPLETE
    DDRF &= ( 1 << DDF0 );     // Setup right XOR signal REMOVE ONCE DEBUG COMPLETE

    EICRB |= ( 1 << ISC60 );  // Set the External Interrupt Control to flag on ang change of INT6 right XOR

    EIMSK |= ( 1 << INT6 );  // Enable Interrupts for INT6 which will be the INT6_vect

    PCIFR |= ( 1 << PCIF0 );  // Clears the interrupt flags for PCINT 0-7

    // Left encoder uses PB4 and PE2 pins as digital inputs. External interrupt PCINT4 is necessary to detect
    // the change in XOR flag. You'll need to see Section 11.1.5 - 11.1.7 for setup and use.
    // Note that the PCINT interrupt is trigered by any PCINT pin. In the ISR you should check to make sure
    // the interrupt triggered is the one you intend on processing.

    // Right encoder uses PE6 adn PF0 as digital inputs. External interrupt INT6 is necessary to detect
    // the changes in XOR flag. You'll need to see Sections 11.1.2-11.1.4 for setup and use.
    // You'll use the INT6_vect ISR flag.

    // Initialize static file variables. These probably need to be updated.
    _last_right_A  = 0;  // MEGN540 Lab 3 TODO
    _last_right_B  = 0;  // MEGN540 Lab 3 TODO
    _last_left_XOR = 0;  // MEGN540 Lab 3 TODO

    _last_left_A   = 0;  // MEGN540 Lab 3 TODO
    _last_left_B   = 0;  // MEGN540 Lab 3 TODO
    _last_left_XOR = 0;  // MEGN540 Lab 3 TODO

    _left_counts  = 0;  // MEGN540 Lab 3 TODO
    _right_counts = 0;  // MEGN540 Lab 3 TODO
}

/**
 * Function Encoder_Counts_Left returns the number of counts from the left encoder.
 * @return [int32_t] The count number.
 */
int32_t Encoder_Counts_Left()
{
    cli();  // Disable global Interrups

    int32_t leftwheel = _left_counts;

    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    // Note: Interrupts can trigger during a function call and an int32 requires
    // multiple clock cycles to read/save. You may want to stop interrupts, copy the value,
    // and re-enable interrupts to prevent this from corrupting your read/write.
    sei();  // Re enable global interupts
    return leftwheel;
}

/**
 * Function Encoder_Counts_Right returns the number of counts from the right encoder.
 * @return [int32_t] The count number.
 */
int32_t Encoder_Counts_Right()
{
    cli();  // Disable global Interrups

    int32_t rightwheel = _right_counts;

    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    // Note: Interrupts can trigger during a function call and an int32 requires
    // multiple clock cycles to read/save. You may want to stop interrupts, copy the value,
    // and re-enable interrupts to prevent this from corrupting your read/write.
    sei();  // Re enable global interupts
    return rightwheel;
}

/**
 * Function Encoder_Rad_Left returns the number of radians for the left encoder.
 * @return [float] Encoder angle in radians
 */
float Encoder_Rad_Left()
{
    // Gear ratio of 75.81:1 = 909.7 CPR

    // YOUR CODE HERE.  How many counts per rotation???
    return gear_ratio * _left_counts;
}

/**
 * Function Encoder_Rad_Right returns the number of radians for the left encoder.
 * @return [float] Encoder angle in radians
 */
float Encoder_Rad_Right()
{

    return gear_ratio * _right_counts;
}

/**
 * Interrupt Service Routine for the right Encoder.
 * @return
 */
ISR( INT6_vect )
{
    // Clear the interupt flag
    EIFR |= ( 1 << INTF6 );

    _right_counts += ( Right_A() ^ _last_right_B ) - ( _last_right_A ^ Right_B() );

    // Clear the interupt flag

    // morse_dot( 200 );
    _last_right_A = Right_A();  // MEGN540 Lab 3 TODO
    _last_right_B = Right_B();  // MEGN540 Lab 3 TODO
                                // _last_right_XOR = Right_XOR();  // MEGN540 Lab 3 TODO

    // morse_dot( 100 );
    // Read value from PINE bit 2
}

/**
 * Interrupt Service Routine for the left Encoder.Note: May need to check that it is actually PCINT4 that triggered, as
 * the Pin Change Interrupts can trigger for multiple pins i.e PCINT0
 * @return
 */
ISR( PCINT0_vect )
{
    PCIFR |= ( 1 << PCIF0 );  // Clears the interrupt flags for PCINT 0-7

    _left_counts += ( Left_A() ^ _last_left_B ) - ( _last_left_A ^ Left_B() );

    _last_left_A = Left_A();  // MEGN540 Lab 3 TODO
    _last_left_B = Left_B();  // MEGN540 Lab 3 TODO
                              // _last_left_XOR = Left_XOR();  // MEGN540 Lab 3 TODO
}
