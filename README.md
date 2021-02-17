# SMART_TELESCOPE_ARDUINO

Arduino code made to work with [smart_telescope_rpi](https://github.com/Egorpr0/smart_telescope_rpi) project

## Hardware
Original project board - **Arduino nano** (Atmega328P)

Drivers - **DRV8825**/**A4988**

Soldered on breadboard, but **PCB Coming Soon!**

## API

Commands are sent over serial port at `115200` baud rate.
You can change baud rate by your preference, but `115200` is the fastest baud rate with 30cm cable that is stable, and is not slow enough to be noticeable.

API is made using [ArduinoJson](https://arduinojson.org/) library.

Sample message: 
```
{"n" : *action_name*, "p": *action_params*}
```

 - `action_name` - name from [list of possible actions](#actions-list)
 - `action_params` - params for a certain action also from [list of possible actions](#actions-list)  


## Actions list

Developement stages:
- :new_moon: - not implemented, but planned
- :last_quarter_moon: - implemented, but not finalized and tested
- :full_moon: - implemented and tested

| Command | Description | Impl. status | Params |
| ----------- | ----------- | ----------- | ----------- |
| `ping` | Returns `pong` and firmware version | :full_moon: | None |
| `moveto` | Aims telescope at given cordinates | :last_quarter_moon: | ->[:milky_way:](#moveto)<-|
| `track` | Starts compensating for earth rotation at set position | :last_quarter_moon: | ->[:milky_way:](#track)<-|
|`stop`| Stops all steppers | :full_moon: | None |

<HA> <Dec>

### Ping
 - Returns arduino firmware version to check the compitability of **API**s
 
Response:
```
{"type": "pong", "vesrion": *firmware version*}
```
### Test  - **will be removed in future updates!**
 - Sets stepper speeds to 1 dec/sec

### MoveTo
Params:
  - `"ha"`  - Hour Angle of an object
  - `"dec"` - Declination of an nedeed object

### Track
Params:
 - `"t"` - Time to automatically stop tracking
  
## Other features coming soon...