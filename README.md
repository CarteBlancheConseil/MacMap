# MacMap

MacMap is a GIS application running in a native Mac OS X Cocoa/Carbon environment. It consists of a single core on which the essential functions is added as plugins. It also uses different frameworks , for example for access to the database.

**Compilation :**
What you have to do :
- compile MacMapSuite and MacMapStats frameworks, then install them at the proper location,
- compile the internal frameworks included in the MacMap®.xcodeproj file (schemes),
- compile Components bundles, then install them in a "Plug-Ins" directory (located at : ../../Plug-Ins),
- compile Instances bundles, then install them in the "Plug-Ins" directory,
- compile MacMap®

Compilation can be done by opening projects with Xcode or with the xcodebuild command line tool.

Note: MacMap projects are currently built with Xcode 6 against Mac OS X 10.8 SDK.

