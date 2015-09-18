========================================================================
    CONSOLE APPLICATION : barCodeScanner Project Overview
========================================================================

barCodeScanner.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:


/////////////////////////////////////////////////////////////////////////////
Other notes:

This project requires OpenCV to be installed and OPENCV_DIR system variable to
be defined. Make sure you change settings for C++ and Linker to include these
additional  include directories

$(OPENCV_DIR)\..\..\include
$(OPENCV_DIR)\lib

Add the following Additional Dependencies to the Linker input
opencv_ts300d.lib;opencv_world300d.lib;

/////////////////////////////////////////////////////////////////////////////
