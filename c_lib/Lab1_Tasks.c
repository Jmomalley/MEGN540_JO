#include "Lab1_Tasks.h"

// This function takes two values, multiplies them, and sends the result over serial back to the Host
void Multiply_And_Send( float value_left, float value_right )
{
    float ret_val = value_left * value_right;
    USB_Send_Msg( "cf", '*', &ret_val, sizeof( ret_val ) );
};

void Divide_And_Send( float value_left, float value_right )
{
    float ret_val = value_left / value_right;
    USB_Send_Msg( "cf", '/', &ret_val, sizeof( ret_val ) );
};

void Add_And_Send( float value_left, float value_right )
{
    float ret_val = value_left + value_right;
    USB_Send_Msg( "cf", '+', &ret_val, sizeof( ret_val ) );
};

void Subtract_And_Send( float value_left, float value_right )
{
    float ret_val = value_left - value_right;
    USB_Send_Msg( "cf", '-', &ret_val, sizeof( ret_val ) );
};