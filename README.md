# Abserver_arduino

Arduino code made to work with [abserver_rpi](https://github.com/egor-khanko/abserver_rpi) project

## Hardware
Original project board - **Arduino nano** (Atmega328P)

Drivers - **DRV8825**/**A4988**

Soldered on breadboard, but **PCB Coming Soon!**

## API

Commands are sent over serial port at `115200` baud rate.
You can change baud rate by your preference, but `115200` is the fastest baud rate with 30cm cable that is stable, and is not slow enough to be noticeable.

API is made using [ArduinoJson](https://arduinojson.org/) library.

Sample message: 
```json
{"n" : *action_name*, "p": *action_params*}
```

 - `action_name` - name from [list of possible actions](#actions-list)
 - `action_params` - params for a certain action also from [list of possible actions](#actions-list)  


## Actions list
---
Developement stages:
- :new_moon: - not implemented, but planned
- :last_quarter_moon: - implemented, but not finalized and tested
- :full_moon: - implemented and tested

| Command  | Description                                            | Impl. status        | Params                     |
| -------- | ------------------------------------------------------ | ------------------- | -------------------------- |
| `ping`   | Returns `pong` and firmware version                    | :full_moon:         | None                       |
| `moveto` | Aims telescope at given cordinates                     | :last_quarter_moon: | ->[:milky_way:](#moveto)<- |
| `move`   | Aims telescope at relative cordinates                  | :full_moon:         | ->[:milky_way:](#move)<-   |
| `track`  | Starts compensating for earth rotation at set position | :last_quarter_moon: | ->[:milky_way:](#track)<-  |
| `config` | Used for parameters configuration                      | :last_quarter_moon: | ->[:milky_way:](#config)<- |
| `stop`   | Stops all steppers                                     | :full_moon:         | None                       |
---

### **Ping**
 - Returns arduino firmware version to check the compitability of **API**s
 
Response:
```json
{"type": "pong", "vesrion": *firmware version*}
```
### **MoveTo**
Params:
 - `"ha"`  - Hour Angle of an object
 - `"dec"` - Declination of an nedeed object

Example:
```json
{"n": "moveTo", "p": {"dec": 35.5}}
```

### **Move**
Params:
 - `"decDiff"` - the declination angle you you want to move your telescope on
 - `"haDiff"` - the hour angle you want to move your telescope on
  
Example:
```json
{"n": "move", "p": {"decDiff": 10}}
```
### **Track**
Params:
 - `"t"` - Time to automatically stop tracking

Example:
```json
{"n": "track", "p": {"t": 500}}
```
### **Config**
Params:
 - `"reportFrequency": float` - Report **frequency** for the status message. High frequencies can interrupt tracking. Will be fixed in future updates.

Example:
```json
{"n": "config", "p": {"reportFrequency": 0.25}} //Every 4 seconds
```


## Other features coming soon...
