# XDG Utils for Android

XDG Utils built for Android with C++

## Supported utils

- `xdg-open`
- `xdg-mime`

## Quick Tutorial
```shell
# Open Links in your default Browser
xdg-open "https://google.com"
# Open Fils an your Phone
xdg-open "/sdcard/MMRL/package.json"
# Open Android Apps
xdg-open "com.discord"

# Check Mime type of an File
xdg-mime "/sdcard/MMRL/package.json"
# > application/json
```