Please note:
This repository has been migrated to https://codeberg.org/andimachovec/Kottan and is therefore archived here. 

# Kottan

A viewer and editor for archived BMessage objects for the Haiku operating system 

![Screenshot](./screenshot.png)

## Prerequisites
Kottan should compile under any version of Haiku that is not too ancient. 
On the 32bit version please remember to issue *setarch x86* before compiling.

## Compiling 
Compiling is done using the makefile engine supplied with Haiku. A simple *make* compiles the code,
and *make bindcatalogs* adds the translations for other languages than English. 

After that, you can run *Kottan* from the generated *objects.xxxxx* directory. 

## Contributing 
Pull requests for bugfixes and functional enhancements are very welcome. For language translations please
use Haiku´s Polyglot tool at https://i18n.kacperkasper.pl

## Trivia
In case anyone is wondering: Kottan is named after a somewhat whacky police inspector in an austrian TV series of the same name from the 1980ies. 
