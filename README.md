# LED Dimmer

This is an IoT dimmer for the power supply (LCM-25) of the LEDs of my [aquaterrarium](https://youtu.be/HS-m3dBo0aY). It allows to set
up to five intervals where the LEDs are turned on and off with a specific dimmed value.

# Hardware

- Controller: D1 Mini ESP8266 https://www.makershop.de/plattformen/d1-mini/3x-d1mini/
- LED power supply LCM-25 https://www.leds.de/meanwell-lcm-25-350-1050-ma-konstantstromquelle-95267.html
- Transistor IRLZ34N (First I used a bipolar BC 547B, because I had more lying around, but the mosfet decreases power consumption by almost 50%)
- USB power supply

# Setup

- Connect D1 Mini Port D4 to Base/Gate of Transistor
- Connect Emitter/Drain of Transistor to LCM-25 Port DIM+ 
- Connect D1 Mini Port GND to LCM-25 Port DIM-
- Connect Collector/Source of Transistor to DIM-

```
       ┌─────────────────────┐
       │        LED-BAR      │
       └──┬──┬───────────────┘
          │  │350mA
         ┌┴──┴──┐
         │ LCM- │
         │  25  │
         │      │
         └┬───┬─┘
     DIM+ │   │ DIM-
          │   ├────────────────────┐
┌───┐     │   │                    │
│   ├─────│───┘       ┌────────┐   │
│IRL│Sourc│e          │Arduino ├───┴── GND
│Z34├─────┘           │D1 Mini │
│N  │Drain            │ESP8266 ├────── 5V
│   ├─────────────────┤        │
│   |Gate           D4│        │
└───┘                 └────────┘
```

(Drawing with asciiflow.com)


# Quirks

When the Port D4 is connected to the transistor when starting, the arduino doesn't boot properly

# Arduino IDE
Set board to "LOLIN(WEMOS) D1 mini lite"

