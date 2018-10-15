# electron-desktop-frame


## description
Make electron window as Windows wallpaper.

This is only for Windows.


## screenshot
![demo](https://raw.githubusercontent.com/hiitiger/electron-desktop-frame/master/demo/screenshot.gif)


## install
```
```

## usage

```
    const desktopframe = require("../js/index.js");
    desktopframe.setAsDesktopFrame(embedWindow.getNativeWindowHandle().readUInt32LE(0));
    ...
    desktopframe.clearDesktopFrame(embedWindow.getNativeWindowHandle().readUInt32LE(0));
```

## todo
    - [ ] multi desktop support