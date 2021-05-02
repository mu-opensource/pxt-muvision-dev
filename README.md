# MU Vision Sensor 3 - DEV Repo

The [MU Vision Sensor](http://mai.morpx.com/goods.php?id=162) is a sensor module for micro:bit and other hardware platform that support UART or I2C communication protocols.

## Basic usage

* Get vision result

```blocks
// Initialized MU with I2C port
muvision.begin(MuId.Mu00, MuVsMode.kI2CMode)
// Vision begin
muvision.visionBegin(MuId.Mu00, MuStatus.Enable, MuVsMessageVisionType.kVisionBall)
basic.forever(function () {
    // Get vision status first
    if (muvision.detected(MuId.Mu00, MuVsMessageVisionType.kVisionBall)) {
        // If vision detected, print vision result
        serial.writeValue("x", muvision.visionValue(MuId.Mu00, muvision.VisionType.VisionBallDetect, muvision.Params.Horizontal))
        serial.writeValue("y", muvision.visionValue(MuId.Mu00, muvision.VisionType.VisionBallDetect, muvision.Params.Vertical))
        serial.writeValue("w", muvision.visionValue(MuId.Mu00, muvision.VisionType.VisionBallDetect, muvision.Params.Width))
        serial.writeValue("h", muvision.visionValue(MuId.Mu00, muvision.VisionType.VisionBallDetect, muvision.Params.Height))
    }
})
```

* Get light sensor result

```blocks
muvision.begin(MuId.Mu00, MuVsMode.kI2CMode)
muvision.lsBegin(MuId.Mu00, MuStatus.Enable, MuLsType.LsProximity)
muvision.lsBegin(MuId.Mu00, MuStatus.Enable, MuLsType.LsAmbientLight)
basic.forever(function () {
    serial.writeValue("proximity", muvision.lsReadProximity(MuId.Mu00))
    serial.writeValue("als", muvision.lsReadAmbientLight(MuId.Mu00))
})
```

* Read WiFi data(AP mode)

Before you use this function, you need to set the FUNC switch to 10(WiFi AT mode) or 11(video transmission mode).

```blocks
serial.redirect(
    SerialPin.P12,
    SerialPin.P13,
    BaudRate.BaudRate9600
)
// Wait for serial initialization to complete
basic.pause(500)
// Show local IP on the screen
basic.showString(muvisionAT.wifiSIP())
basic.forever(function () {
    basic.showString(String.fromCharCode(muvisionAT.wifiRead()))
})
```

* Read WiFi data(STA mode)

Before you use this function, you need to set the FUNC switch to 10(WiFi AT mode) or 11(video transmission mode),
and your device and MU must be connected to the same network.

```blocks
serial.redirect(
SerialPin.P12,
SerialPin.P13,
BaudRate.BaudRate9600
)
// Wait for serial initialization to complete
basic.pause(500)
muvisionAT.wifiSet("your_ssid", "your_password", MuAtMode.ModeSTA)
basic.showIcon(IconNames.Heart)
// Check connect
if (muvisionAT.wifiCon(true)) {
    basic.showIcon(IconNames.Yes)
    // Show local IP on the screen
    basic.showString(muvisionAT.wifiSIP())
    // Set target IP
    muvisionAT.wifiUDP("your_target_ip", "3333")
} else {
    basic.showIcon(IconNames.No)
}
basic.forever(function () {
    basic.showString(String.fromCharCode(muvisionAT.wifiRead()))
})
```

## License

MIT

## Supported targets

* for PXT/microbit

## What about other libraries for the MU Vision Sensor?

* Arduino	    [https://github.com/mu-opensource/MuVisionSensor3](https://github.com/mu-opensource/MuVisionSensor3)
* Mixly		    [https://github.com/mu-opensource/MuVisionSensor3-Mixly](https://github.com/mu-opensource/MuVisionSensor3-Mixly)
* MicroPython	[https://github.com/mu-opensource/MuVisionSensor3-MicroPython](https://github.com/mu-opensource/MuVisionSensor3-MicroPython)

## For more information

Check out the official site [http://www.morpx.com/zn.index.html](http://www.morpx.com/zn.index.html) for links to documentation, issues, and news
