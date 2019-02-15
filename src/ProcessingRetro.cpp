/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
 * 
 * Class for processing retro reflective tape for the 2019 FRC Deep Space Challenge
*/

#include "ProcessingRetro.h"

ProcessingRetro::ProcessingRetro()
//    : ProcessingBase(   { 16, 255, 255 }, 
//                        {  1,  50,  30 } )     // TODO: Pick colors for Retro reflective tape
    : ProcessingBase(   { 179, 52, 255 }, 
                        {   0,  0, 180 } )     // White gaffer tape test
{
}

ProcessingRetro::~ProcessingRetro()
{
}
