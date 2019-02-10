//
//  main.c
//  nscale
//
//  Created by Mirren Malcolm-Neale on 10/02/2019.
//  Copyright © 2019 MirrenMN. All rights reserved.
//
/* nscale.c Displays the Equal Temp. frequencies for an N-Note octave, from a given MIDI note.
 Input Args: no. notes in octave (N) up to 24, given MIDI note to start scale on */



#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, const char * argv[]) {
    
    
    int notes, midinote;
    double frequency, ratio;
    double c0, c5;
    double intervals[24];
    
    //Arg 1 note checks
    notes = atoi(argv[1]);
    if(notes < 1)
    {
        printf("Error: notes must be positive values\n");
        return 1;
    }
    if (notes > 24)
    {
        printf("Error: max value for notes is 24\n");
        return 1;
    }
    
    //Arg 2 midinote checks
    midinote = atoi(argv[2]);
    if(midinote < 0)
    {
        printf("Error: cannot have a negative MIDI note!");
        return 1;
    }
    if(midinote > 127)
    {
        printf("Error: max MIDI note is 127\n");
        return 1;
    }

    //Total arg checks
    if(argc != 3)
    {
        printf("Usage: nscale notes midinote\n");
        return 1;
    }
    
    ratio = pow(2.0, 1.0 / 12.0);
    
    //Find middle C, three semitones above low A = 220
    c5 = 220 * pow(ratio, 3);

    //MIDI note 0 is C, 5 octaves below middle C
    c0 = c5 * pow(0.5, 5);
    frequency = c0 * pow(ratio, midinote);
    
    //Calc intervals for N notes per octave
    ratio = pow(2.0, 1.0 / notes);
    
    //Generate al frequencies for scale
    
    for (int i = 0; i < notes + 1; i++)
    {
        intervals[i] = frequency;
        frequency *= ratio;
    }
    
    //Read array and write to screen
    for(int i = 0; i < notes + 1; i++)
    {
        printf("Note in scale: %d Frequency: %f\n", i, intervals[i]);
    }
    
    return 0;
}
