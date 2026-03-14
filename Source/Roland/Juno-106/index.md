# Juno-106

[Roland](..) 6 voice polyphonic synth with one VCO per voice.

# picoSynth emulation

## Status

+ Work in progress

## MIDI control

```
Akai MIDImix map:

   LFO RAT LFO DEL VCA MOD VCA LEV CHORUS          PORTA M PORTMEN        
    .--.    .--.    .--.    .--.    .--.    .--.    .--.    .--.          
   ( 16 )  ( 20 )  ( 24 )  ( 28 )  ( 46 )  (    )  ( 54 )  ( 58 )         
    '--'    '--'    '--'    '--'    '--'    '--'    '--'    '--'          

   DCO RAN DCO LFO DCO PWM DCO PWM DCO PWM DCO SAW DCO SUB DCO NOI        
    .--.    .--.    .--.    .--.    .--.    .--.    .--.    .--.          
   ( 17 )  ( 21 )  ( 25 )  ( 29 )  ( 47 )  ( 51 )  ( 55 )  ( 59 )         
    '--'    '--'    '--'    '--'    '--'    '--'    '--'    '--'          

   HPF MOD VCF FRE VCF RES VCF ENV VCF ENV VCF LFO VCF KYB                
    .--.    .--.    .--.    .--.    .--.    .--.    .--.    .--.          
   ( 18 )  ( 22 )  ( 26 )  ( 30 )  ( 48 )  ( 52 )  ( 56 )  (    )         
    '--'    '--'    '--'    '--'    '--'    '--'    '--'    '--'          

   BEND DC BEND VC BEND LF         ENV ATT ENV DEC ENV SUS ENV REL VOLUME 
    .--.    .--.    .--.    .--.    .--.    .--.    .--.    .--.    .--.  
   ( 19 )  ( 23 )  ( 27 )  (    )  ( 49 )  ( 53 )  ( 57 )  ( 61 )  ( 62 ) 
    '--'    '--'    '--'    '--'    '--'    '--'    '--'    '--'    '--'  

Akai MPKmini map:

   LFO RAT LFO DEL VCF FRE VCF RES
    .--.    .--.    .--.    .--.  
   (  2 )  (  3 )  (  4 )  (  5 ) 
    '--'    '--'    '--'    '--'  

   ENV ATT ENV DEC ENV SUS ENV REL
    .--.    .--.    .--.    .--.  
   (  6 )  (  7 )  (  8 )  (  9 ) 
    '--'    '--'    '--'    '--' 
```
