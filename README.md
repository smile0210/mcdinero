# McDinero

![McDinero icon](https://github.com/smile0210/mcdinero/blob/master/proj.ios_mac/ios/Images.xcassets/AppIcon.appiconset/Icon-App-83.5x83.5%402x.png)

McDinero is a mini video game inspired by the viral video https://youtu.be/DKUCGh1mH0c

You need to collect as much money as you can, win the rap fights and avoid the "Error" and the "Monarquía Española"

This game use the open source framework **Cocos2dx V4** https://www.cocos.com/en/docs#2dx

# iOS project

Once you downloaded the repo:
- In the root project directory create a folder called **ios-build** with `mkdir ios-build`
- Go to the new folder `cd ios-build`
- Create the Xcode project with `cmake .. -GXcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphoneos`
- Open the Xcode created at `ios-build`
- At the top left corner select `McDinero` and click play
- Have fun!

# Android project
Once you downloaded the repo:
- Open the project **proj.android** with Android Studio
- Open your simulator or connect your device
- Run the project
- Have fun!

# Requirements & References
- Both platforms require `cmake` in order to compile the source files, install it before compile
  - I recommend install it with `homebrew` but you can also refer to the official documentation https://cmake.org/install/
- For iOS you need to install Xcode in order to compile the game, also, you can refer to this video to setup your development environment https://youtu.be/GKT_40cc-a0
- For Android you need to install Android Studio and the NDK, please refer to this video to setup you development environment https://youtu.be/veBSTU0c2YY

