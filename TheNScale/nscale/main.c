//
//  main.c
//  nscale
//
//  Created by Mirren Malcolm-Neale on 10/02/2019.
//  Copyright © 2019 MirrenMN. All rights reserved.
//
/*  nscale.c generates the Equal Temp. frequency tables for an N-Note octave (N <= 24), from a given MIDI note.
 Input Args: no. notes in octave (N), given MIDI note to start scale on */

/* Usage nscale [-m] [-i] N startval [outfile.txt]
 -m: sets startval to be MIDI note not frequency
 -i: prints the calculated intervals as well as the abs frequency
 outfile: optional text filename for outut data
 */



#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, const char * argv[]) {
    
    int notes, i;
    int ismidi = 0;
    int write_interval = 0;
    int err = 0;
    double startval, basefreq, ratio;
    FILE* fp;
    double intervals[25];
    
    /// Check first arg for flag option, argc at least 2
    while(argc > 1)
    {
        if(argv[1][0] == '-')
        {
            if(argv[1][1] == 'm')
            {
                ismidi = 1;
            }
            else if(argv[1][1] == 'i')
            {
                write_interval = 1;
            }
            else
            {
                printf("Error: unrecognised option %s\n", argv[1]);
                return 1;
            }
                
            //Move to next arg
            argc--;
            argv++;
                
        }
        else
        {
            break;
        }
    }
    
    if(argc < 3)
    {
        printf("Insufficient arguments passed\n");
        printf("Usage: nscale [-m] [-i] N startval [outfile.txt]\n");
        printf(" -m: sets startval to be MIDI note not frequency\n -i: prints the calculated intervals as well as the abs frequency\n outfile: optional text filename for outut data\n");
        return 1;
    }
    
    
    /// Read and check all args
    /// argv[1] should hold N and argv[2] startval
    notes = atoi(argv[1]);
    if(notes < 1 || notes > 24)
    {
        printf("Error: N is out of range. N must be between 1 and 24\n");
        return 1;
    }
    
    startval = atof(argv[2]);
    if(ismidi)
    {
        if(startval > 127.0)
        {
            printf("Error: MIDI startval must be <= 127\n");
            return 1;
        }
        
        //for MIDI, startval can be 0.
        if(startval < 0.0)
        {
            printf("Error: MIDI startval must be >= 0.0");
            return 1;
        }
    }
    
    /// Check for correct frequency input as not MIDI
    else
    {
        if(startval <= 0.0)
        {
            printf("Error: Frequency startval must be positive\n");
            return 1;
        }
    }
    
    /// Check for optional filename
    fp = NULL;
    if(argc == 4)
    {
        fp = fopen(argv[3], "w");
        if(fp == NULL)
        {
            printf("WARNING: Unable to create file %s\n", argv[3]);
            perror("");
        }
    }
    
    /* All params are ready - can now fill array and write to file if created */
    
    
    /// Find basefreq if startval is MIDI
    if(ismidi)
    {
        double c0, c5;
        ratio = pow(2.0, 1.0/12);
        c5 = 220 * pow(ratio, 3);
        c0 = c5 * pow(0.5, 5);
        basefreq = c0 * pow(ratio, startval);
    }
    else
    {
        basefreq = startval;
    }
    
    ///Calc ratio from notes and fill array
    ratio = pow(2.0, 1.0 / notes);
    for(i = 0; i <= notes; i++)
    {
        intervals[i] = basefreq;
        basefreq *= ratio;
    }
    
    ///Read array, write to screen and optional to file
    for(i = 0; i <= notes; i++)
    {
        double ratio_power = pow(ratio, i);
        double interval = intervals[i];
        
        if(write_interval)
        {
            printf("%d:\t%f\t%f\n", i, ratio_power, interval);
        }
        else
        {
            printf("%d: \t%f\n", i, interval);
        }
        
        if(fp)
        {
            if(write_interval)
            {
                err = fprintf(fp, "%d: \t%f\t%f\n", i, ratio_power, interval);
            }
            else
            {
                err = fprintf(fp, "%d: \t%f\n", i, interval);
            }
            if(err < 0)
            {
                break;
            }
        }
    }
    
    if(err < 0)
    {
        perror("There was an error writing to the file.\n");
    }
    if(fp)
    {
        fclose(fp);
    }
    
    return 0;
    
    
}
