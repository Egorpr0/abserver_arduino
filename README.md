# SMART_TELESCOPE_ARDUINO

Arduino code made to work with [smart_telescope_rpi](https://github.com/Egorpr0/smart_telescope_rpi) project

## API

Commands are sent over serial port at `115200` baud rate.
You can change baud rate by your preference, but `115200` is the fastest baud rate with 30cm cable that is stable, and is not slow enough to be noticeable.

- :new_moon: - not implemented, but planned
- :first_quarter_moon: - implemented, but not finalized and tested
- :full_moon: - implemented and tested

| Command | Description | Impl. status | todo |
| ----------- | ----------- | ----------- | ----------- |
| `ping` | returns `pong` and firmware version | :full_moon: | None |
| `moveTo <HA> <Dec>` | Aims telescope at given cordinates | :new_moon: | - Add feature|
| `track` | Starts compensating for earth rotation at set position | :new_moon: | - Add feature|

## Other features coming soon...