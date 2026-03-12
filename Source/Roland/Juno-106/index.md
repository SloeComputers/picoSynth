# SH-101

[Roland](Roland) 6 voice polyphonic synth with one VCO per voice.

# picoSynth emulation

## Status

+ Work in progress

## MIDI control

```
Akai MIDImix map:

   LFO WAV LFO RAT         BEND VC BEND VC LFO MOD PORTA M PORTMEN        
    .--.    .--.    .--.    .--.    .--.    .--.    .--.    .--.          
   ( 16 )  ( 20 )  (    )  ( 28 )  ( 46 )  ( 50 )  ( 54 )  ( 58 )         
    '--'    '--'    '--'    '--'    '--'    '--'    '--'    '--'          

    int 1   int 2   int 3  VCF FRE VCF RES VCF ENV VCF MOD VCF KYB        
    .--.    .--.    .--.    .--.    .--.    .--.    .--.    .--.          
   ( 17 )  ( 21 )  ( 25 )  ( 29 )  ( 47 )  ( 51 )  ( 55 )  ( 59 )         
    '--'    '--'    '--'    '--'    '--'    '--'    '--'    '--'          

   VCO RAN  TUNE   VCA MOD ENV MOD ENV ATT ENV DEC ENV SUS ENV REL        
    .--.    .--.    .--.    .--.    .--.    .--.    .--.    .--.          
   ( 18 )  ( 22 )  ( 26 )  ( 30 )  ( 48 )  ( 52 )  ( 56 )  ( 60 )         
    '--'    '--'    '--'    '--'    '--'    '--'    '--'    '--'          

   VCO MOD VCO P.W VCO PWM SRC SQU SRC RAM SRC SUB SRC SUB SRC NOI VOLUME 
    .--.    .--.    .--.    .--.    .--.    .--.    .--.    .--.    .--.  
   ( 19 )  ( 23 )  ( 27 )  ( 31 )  ( 49 )  ( 53 )  ( 57 )  ( 61 )  ( 62 ) 
    '--'    '--'    '--'    '--'    '--'    '--'    '--'    '--'    '--'  

Akai MPKmini map:

   SRC SQU SRC RAM SRC SUB SRC NOI
    .--.    .--.    .--.    .--.  
   (  2 )  (  3 )  (  4 )  (  5 ) 
    '--'    '--'    '--'    '--'  

   LFO RAT VCO MOD VCO P.W VOLUME 
    .--.    .--.    .--.    .--.  
   (  6 )  (  7 )  (  8 )  (  9 ) 
    '--'    '--'    '--'    '--'
```
