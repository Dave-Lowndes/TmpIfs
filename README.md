# TmpIfs V1.2.1.0

TmpIfs is a Win32 console program that outputs the contents of the Temporary Internet Files folders in a tab delimited format so that you can obtain a listing of this folder in a similar representation to how Explorer displays them.

It outputs the following items:

 - Internet Address
 - Size
 - Last Modified Date
 - Expires Date
 - Last Accessed Date
 - Last Checked Date
 - Hit Rate
 
*TmpIfs - Copyright © 2001-2023 JD Design, All Rights Reserved*

---

## Installation

There is no installation - just copy the TmpIfs.exe program to a sub-directory of your hard disk and run it from there.


## How Do You Use TmpIfs?

From the sub-folder where you've installed it just enter the command. There are no command line options.

Since there are normally a lot of files in the Temporary Internet Folders you'll want to pipe the output to a file like this:

`C:\TmpIfs>tmpifs > tifs.txt`

You can then import the file (tifs.txt in the above example) into another application that supports tab delimited text (most spreadsheets support it) and process the information there.

## Removal

Just delete the TmpIfs.exe file.

## Revision History

**V1.2.1.0 4 Dec 2023**
Removed licensing, updated source code and made public.

**V1.2.0.0  3 Feb 2007**
Uses the new registration scheme.

**V1.1.0.0  16 Jan 2002**
Now outputs the time as well as the date for the timestamp fields.
Registration now compatible with ExpPrint V1.6.

**V1.0.0.0  2 May 2001**
First released version.
