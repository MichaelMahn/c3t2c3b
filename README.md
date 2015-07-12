# c3t2c3b
Converts cocos2d-x .c3t files to .c3b files

This program is intended to convert .c3t files to .c3b files. It's build upon cocos2d-x 3.6 and fbx-conv from https://github.com/cocos2d-x/fbx-conv.

A few changes had to be done to cocos2d-x framework: I added a method in cocos2d/cocos/3d/CCBundle3D.h / .cpp to load all animation names and changed the include path for the json library.

The main load part takes place in file Classes/src/readers/C3TConverter.h and the file it converts is hardcoded in proj.linux/main.cpp.

BE CAUTIOUS THE C3T FILE IT CONVERTS WILL ALSO BE OVERWRITTEN WITH A NEW VERSION AND A C3B FILE WILL BE CREATED.

Currently only works for linux.
