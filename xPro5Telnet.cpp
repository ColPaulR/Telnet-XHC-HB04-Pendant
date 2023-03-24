const config = require('./xhcrc');
const { Telnet_Init, CNC_state } = require('./myTelnet');
const HID = require('node-hid');

// Array used to transmit selected axis
const axischars = "XYZA";

// initialize buttons to nothing pressed;
var prevButtons = [0, 0];

// Knobs
var feedknob;
var axisknob;

// Create USB transfer buffer
var buff = new Buffer.alloc(21);

// Set fixed headers that never get changed
buff[0] = 0xFE;
buff[1] = 0xFD;
buff[2] = 0x04;

// USB Output device
var dev_USB_OUT;

// Initialize Telnet
const mySocket = Telnet_Init(config, xhc_set_display);

// Get all settings
mySocket.write("$$\n");

// Initialize USB
USB_Init();

// Find XHC-HB04
function USB_Init() {
    const devices = HID.devices(config.HID_VID, config.HID_PID);

    if (devices.length === 0) {
        console.error("Could not find HID device with VID=0x%s and PID=0x%s", config.HID_VID.toString(16), config.HID_PID.toString(16));
        process.exit(1);
    }

    var dev_USB_IN;

    if (devices.length > 1) {
        // Windows finds multiple HID devices for single XHC-HB04. 1 is input device and other is output device
        for (iLooper = 0; iLooper < devices.length; iLooper++) {
            // This works for 1 windows setup. Not sure if it is portable
            if (devices[iLooper].path.includes("col01")) {
                dev_USB_IN = new HID.HID(devices[iLooper].path);
            }

            if (devices[iLooper].path.includes("col02")) {
                dev_USB_OUT = new HID.HID(devices[iLooper].path);
            }
        }
    } else {
        // Single device found for both input and output. 1 call to new HID with duplicate reference
        dev_USB_IN = new HID.HID(devices[0].path);
        dev_USB_OUT = dev_USB_IN;
    }

    if (!dev_USB_IN) {
        console.log('USB Pendant not found');
        process.exit(1);
    }
    if (!dev_USB_OUT) {
        console.log('USB Pendant not found');
        process.exit(1);
    }

    dev_USB_IN.on('error', function (error) {
        console.log("on error", error);
    });
    dev_USB_IN.on('end', function () {
        console.log("on end");
    });

    console.log("found XHC-HB04 device");

    // Setup callback for data in
    dev_USB_IN.on('data', function (data) {
        parseButtonData(data);
    });
}

function xhc_encode_float(v, buff_offset) {
    // Make integer part fraction into unsigned integer number
    var unsigned_v = Math.abs(v);

    // Separate into whole and fractional parts
    var int_part = Math.trunc(unsigned_v);
    // truncateDecimals(unsigned_v,0);
    var frac_part = Math.trunc((unsigned_v - int_part) * 10000);

    // Write to buffer
    xhc_uint16_to_buffer(int_part, buff_offset);
    xhc_uint16_to_buffer(frac_part, buff_offset + 2);

    // Set negative bit if required
    if (v < 0)
        buff[buff_offset + 3] = buff[buff_offset + 3] | 0x80;
}

function xhc_uint16_to_buffer(v, offset) {
    buff[offset + 1] = v >> 8;
    buff[offset] = v & 0xff;
}

function xhc_set_display() {
    // Format the display data into a buffer
    var DispAxis;

    if (config.WorkPos) {
        DispAxis = CNC_state.WPos;
        buff[3] |= 0x80;
    } else {
        DispAxis = CNC_state.MPos;
        buff[3] &= !0x80;
    }

    // Set display to step
    buff[3] |= 0x01;

    if (DispAxis.length < 1) {
        return;
    }
    // Stp, Cont, MPG or nothing [0:1]
    // MC 0; WC 1 [7]
    //buff[16]=6;

    // Update XYZ
    xhc_encode_float(DispAxis[0], 4);
    xhc_encode_float(DispAxis[1], 8);
    xhc_encode_float(DispAxis[2], 12);

    // Packetize buffer
    var packets = Buffer.allocUnsafe(8);
    packets[0] = 6;

    // Send "6" and then 7 bytes of buffer
    var iIndex = 0;

    for (var iPacket = 0; iPacket < 3; iPacket++) {
        // Copy 7 bytes into packets[1:7]
        buff.copy(packets, 1, iIndex, iIndex + 7);
        // Move index to beginning of next 7 bytes
        iIndex += 7;

        // send packets
        dev_USB_OUT.sendFeatureReport(packets);
    }
}

// Data available parsing function
function parseButtonData(data) {
    //console.log("usb data:", data, " len:", data.length);

    feedknob = data[4];

    // Process axis selector switch
    axisknob = data[5] - 0x11;

    // If axis selector is "off", clear last buttons and ignore everything else
    if (data[5] == 6) {
        // Axis selector is off
        // Clear all prior button presses
        prevButtons = [0, 0];

        // Don't process message any further
        return;
    }

    // Create newButtons slice of data buffer
    var newButtons = [data[2], data[3]];;

    // At least one button was pressed
    // Check to see if button 1 was recorded previously
    if ((newButtons[0]) && (!prevButtons.includes(newButtons[0]))) {
        // Button1 was not recorded previous
        // console.log("Button %d is down",newButtons[0]);

        // Process button press
        doButton(newButtons, 0, data[4]);
    }

    // Check to see if button 2 was recorded previously
    if ((newButtons[1]) && (!prevButtons.includes(newButtons[1]))) {
        // Button2 was not recorded previous
        // console.log("Button %d is down",newButtons[1]);

        // Process button press
        doButton(newButtons, 1, data[4]);
    }

    // Check to see if previous button 1 is release
    // if ((prevButtons[0]) && (!newButtons.includes(prevButtons[0]))) {
    // Previous Button 1 is released
    // console.log("Button %d is up",prevButtons[0]);
    // }

    // Check to see if previous button 2 is release
    // if ((prevButtons[1]) && (!newButtons.includes(prevButtons[1]))) {
    // Previous Button 2 is released
    // console.log("Button %d is up",prevButtons[1]);
    // }

    // Record new buttons
    prevButtons = newButtons;

    // Process jog dial
    if (data[6]) {
        // data[6] is a int8 need to determine sign
        var iJog = (data[6] > 127 ? data[6] - 256 : data[6]);
        //console.log("Jog dial is %i", iJog);

        switch (data[4]) {
            case 13:
                // 13 = 0.001
                iJog *= 0.001;
                break;
            case 14:
                // 14 = 0.01         
                iJog *= 0.01;
                break;
            case 15:
                // 15 = 0.1
                iJog *= 0.1;
                break;
            case 16:
                // 16 = 1
                iJog *= 1;
                break;
            case 26:
                // 26 = 60%
                iJog *= 10;
                break;
            case 27:
                // 27 = 100%
                iJog *= 100;
                break;
            case 28:
                // 28 = Lead 
                iJog *= 250;
                break;
            default:
                console.log("Feed select value %d not handled", data[4]);
                return;
        }
        // Send string to socket
        if (!CNC_state.MaxRate[axisknob]) {
            console.log("CNC_state.MaxRate for " + axischars[axisknob] + " axis not set");
            return;
        }
        doJog("$J=G21G91" + axischars[axisknob] + iJog.toPrecision(4)
            + "F" + CNC_state.MaxRate[axisknob]);
    }
}

function doButton(newButtons, iButton, feedknob) {
    // console.log("Button %d is down",newButtons[iButton]);

    switch (newButtons[iButton]) {
        case 1:
            // Reset button
            Send_Button("$X\n");
            break;
        case 2:
            // Stop button
            Send_Button("!\n");
            break;
        case 3:
            // Start/pause button
            switch (CNC_state.state) {
                case "Idle":
                case "Run":
                case "Jog":
                    Send_Button("!\n");
                    break;
                case "Hold":
                    Send_Button("~\n");
                    break;
                default:
                    console.log("Cannot toggle pause/run in %s state", CNC_state.state);
            }
            // console.log("$X\r\n");
            break;
        case 4:
            // Feed+ button
            if (newButtons.includes(12)) {
                // Function key is pressed.
                if (feedknob < 16) {
                    Send_Button(String.fromCharCode(0x93));
                } else {
                    Send_Button(String.fromCharCode(0x91));
                }
            } else {
                // Do Macro 1
                console.log("Macro 1");
            }
            break;
        case 5:
            // Feed- button
            if (newButtons.includes(12)) {
                // Function key is pressed.
                if (feedknob < 16) {
                    Send_Button(String.fromCharCode(0x94));
                } else {
                    Send_Button(String.fromCharCode(0x92));
                }
            } else {
                // Do Macro 2
                console.log("Macro 2");
            }
            break;
        case 6:
            // Spindle+ button
            if (newButtons.includes(12)) {
                // Function key is pressed.
                if (feedknob < 16) {
                    Send_Button(String.fromCharCode(0x9C));
                } else {
                    Send_Button(String.fromCharCode(0x9A));
                }
            } else {
                // Do Macro 3
                console.log("Macro 3");
            }
            break;
        case 7:
            // Spindle- button
            if (newButtons.includes(12)) {
                // Function key is pressed.
                if (feedknob < 16) {
                    Send_Button(String.fromCharCode(0x9D));
                } else {
                    Send_Button(String.fromCharCode(0x9B));
                }
            } else {
                // Do Macro 4
                console.log("Macro 4");
            }
            break;
        case 8:
            // M-Home button
            if (newButtons.includes(12)) {
                switch (CNC_state.state) {
                    case "Idle":
                    case "Check":
                    case "Home":
                        Send_Button("$H\r\n");
                        break;
                    default:
                        console.log("Cannot home in state %s", CNC_state.state)
                }
            } else {
                // Do Macro 5
                console.log("Macro 5");
            }
            break;
        case 9:
            // Safe-Z button
            if (newButtons.includes(12)) {
                // Function key is pressed.
                // Safe Z
                if (!CNC_state.Pull_Off) {
                    console.log("CNC_state.Pull_Off not set");
                    return;
                }
                if (!CNC_state.MaxRate[2]) {
                    console.log("CNC_state.MaxRate for Z axis not set");
                    return;
                }
                Send_Button("$J=G53G21Z-" + CNC_state.Pull_Off + "F" + CNC_state.MaxRate[2]);
            } else {
                // Do Macro 6
                console.log("Macro 6");
            }
            break;
        case 10:
            // W-Home button
            if (newButtons.includes(12)) {
                // Function key is pressed.
                switch (CNC_state.state) {
                    case "Idle":
                    case "Check":
                        Send_Button("G10 P1 L20 X0 Y0 Z0\n");
                        break;
                    default:
                        console.log("Cannot set workpiece home in state %s", CNC_state.state)
                }
            } else {
                // Do Macro 7
                console.log("Macro 7");
            }
            break;
        case 11:
            // Spindle On/Off button
            if (newButtons.includes(12)) {
                // Function key is pressed.
                // console.log("Spindle Toggle\r\n");
                switch (CNC_state.state) {
                    case "Idle":
                    case "Check":
                        if (CNC_state.SpindleSpeed > 0) {
                            Send_Button("M5");
                        } else {
                            Send_Button("M3");
                        }
                        break;
                    default:
                        console.log("Cannot toggle spindle in state %s", CNC_state.state)
                }
            } else {
                // Do Macro 8
                console.log("Macro 8");
            }
            break;
        case 13:
            // Probe-Z button
            if (newButtons.includes(12)) {
                // Function key is pressed.
                doProbeZ();
            } else {
                // Do Macro 9
                console.log("Macro 9");
            }
            break;
        case 16:
            // Do Macro 10
            // console.log("Macro 10");
            // Toggle between machine and work coordinates
            config.WorkPos = !config.WorkPos;
            break;
        default:
    }
}

function Send_Button(myGCode) {
    if (config.DryRunButtons) {
        console.log(myGCode);
    } else {
        // Append new line just in case (no harm with empty lines)
        mySocket.write(myGCode + "\n");
    }
}

function doProbeZ() {
    // Check to see if current state allows probing
    switch (CNC_state.state) {
        case "Idle":
            break;
        default:
            console.log("Cannot probe in %s state", CNC_state.state)
            return;
    }

    if (!config.ProbeZ) {
        console.log("No Probe macro defined");
        return;
    }
    if (config.DruRunProbeZ) {
        console.log(config.ProbeZ);
    } else {
        mySocket.write(config.ProbeZ);
    }
}

function doJog(myGCode) {
    // Check to see if current state allows jogging
    switch (CNC_state.state) {
        case "Idle":
        case "Jog":
        case "Check":
            break;
        default:
            console.log("Cannot jog in %s state", CNC_state.state)
            return;
    }

    if (config.DryRunJog) {
        console.log(myGCode);
    } else {
        mySocket.write(myGCode + '\n');
    }
}

