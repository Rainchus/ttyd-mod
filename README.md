# TTYD++
A mod of the thousand year door to shorten the cutscenes/dialog for better replayability

# Environment for building gci mods for TTYD
Download and install https://gnuwin32.sourceforge.net/downlinks/make.php

After installing, click the windows start menu, type `environment` and click `edit the system environment variables`</br>
Click the `advanced` tab, then click `Environment Variables`</br>
You'll want to leave this open as we will need to add a few different things here later.</br>
In the bottom box, find the Path variable. Left click it, then click `Edit`</br>
In the new box that appears, click new and enter C:\Program Files (x86)\GnuWin32\bin</br>
Click ok to close the path box and save it</br>

# Installing devkitppc

Make sure gamecube is selected when installing
`https://github.com/devkitPro/installer/releases`</br>
After it finishes, we go back to our environment variables to add one for devkitppc</br>

Variable name: `DEVKITPPC`</br>
Variable value: `/opt/devkitpro/devkitPPC`</br>

# Other Eninvornment Variables And Setup
Now choose a static directory to put your mod folder (if you move this folder, you have to update your environment variables)</br>
In powershell, run `git clone https://github.com/Rainchus/ttyd-mod`</br>


Now we need to add an environment variable named `TTYDTOOLS`</br>
Variable name: `TTYDTOOLS`</br>
My path as an example below:</br>
Variable value: `C:/Users/Rainchus/Desktop/TTYD_Modding/ttyd-tools`</br>

MAKE SURE TO HAVE `/` AND NOT `\` IN THE PATH STRING.</br>


Now lastly, we need to add an environment variable named GCI_PATH for the path for the makefile to copy the gci to.</br>
You can have it either copy to the normal documents path, such as `C:/Users/Rainchus/Documents/Dolphin Emulator/GC/USA/Card A`</br>
OR</br>
You can add the file portable.txt to the folder of dolphin and it will create the filesystem seen in documents in the root directory of where your dolphin.exe is</br>

Variable name: `GCI_PATH`</br>
My portable path as an example:</br>
Variable value: `C:/Users/Rainchus/Desktop/Dolphin/User/GC/USA/Card A/`</br>

Now open powershell in the git cloned directory. then run</br>
`cd ttyd-tools/rel`</br>
Then run `make -j`</br>
It should compile and then copy the gci to the relevant dolphin directory</br>

# Running in Dolphin
To run this in dolphin, first make sure dolphin is set to use gci files. We also need to make sure cheats are on</br>
In dolphin click `Config` and then make sure `Enable Cheats` is checked</br>
Then from here, under `device settings`, click the drop down and select `GCI Folder` for Slot A</br>
Now dolphin uses gci files for saving/loading.</br>

Now we need to add the REL Loader cheat</br>
In your list of game in dolphin, right click on ttyd and select `Properties`</br>
Go to the gecko codes tab, click `Add New Code`</br>
Name it REL Loader and then put this for the code</br>
```
040040DC 72656C00
C206FE38 0000003A
3A000000 3E80802A
3EA0802B 3EC08000
3EE08003 3F008029
6283F594 7C6803A6
38600000 38800000
38A00000 4E800021
2C030000 40820194
6283FC9C 7C6803A6
38600000 808D1B98
80840004 38A00000
38C00000 4E800021
2C030000 4082016C
480000C9 2C030000
40820160 62A30BDC
7C6803A6 38600000
62C440DC 38A40010
4E800021 2C030000
40820120 38800200
48000089 38A00200
38C02000 480000B1
2C030000 408200EC
81EE0040 39EF01FF
55EF002C 7DC47378
48000059 7DE47B78
48000059 7DE57B78
38C02200 48000081
2C030000 408200BC
808E0020 4800003D
7C641B78 7C6F1B78
6303A8E4 7C6803A6
7DC37378 4E800021
2C030001 4082007C
91F6414C 91D64150
820E0034 4800008C
62E300C4 48000008
62E300F0 7C6903A6
38600000 4E800420
7E2802A6 6283C744
7C6803A6 38600000
4E800021 2C03FFFF
4182FFEC 7E2803A6
4E800020 7E2802A6
7C641B78 7C6E1B78
62A3138C 7C6803A6
62C340EC 38E00000
4E800021 2C030000
40820020 4BFFFFB8
6303AB40 7C6803A6
7DC37378 4E800021
7DE47B78 4BFFFF85
7DC47378 4BFFFF7D
62A30CF8 7C6803A6
62C340EC 4E800021
6283FED8 7C6803A6
38600000 4E800021
2C100000 4182000C
7E0803A6 4E800021
38600000 00000000
```

Now enabled the cheat by clicking the checkmark. Start ttyd and the mod should be applied each time on boot</br>
