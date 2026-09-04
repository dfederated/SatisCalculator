# Satiscalculator
Overview:
A simple production calculator for Satisfactory. It is able to store created tables locally, or remotely using MySQL.
Uses python scripts for the calculator and recipe updater and c++ for everything else.
I'm currently using MySQL 9.4 community edition, and that is the version of the driver used. So in order
to use that feature, you must have the same version of MySQL installed. I haven't tested it on later versions,
so I can't confirm if it works for later versions.

Install:
-Note: In order for the python scripts to work properly, you must run as admin. (Looking into making that not necessary for future updates.)
-If you're planning on using the remote feature setup MySQL following their install instructions.
-Run the provided Python Install.bat file to install python and the required modules.
-Run SatisCalculator.exe.
-Enjoy!

Features:
-Update
Checks for new recipes, or updates to existing recipes.
-Login
Tickbox to remember user info.(It's not secured, so it's not recommended. Mainly used for testing.)
Can switch between a local and remote database freely. 
-New
Creates a table entry into the selected database with all of the production values for the selected product and output.
Prints out the created table and allows you to either keep it, or delete it.
-View
Displays a list of saved tables in the selected database.
Once selected, the production values of the table will be displayed.
-Delete
Displays a list of saved tables in the selected database.
Once selected, the table will be deleted.

Bugs:
Some recipes aren't working 100%. They will calculate the necessary 
ingredients properly, but will not save raw ingredients if the product
only has raw ingredients.

Currently, the particle accelerator and quantum encoder have static wattage.

Future Features:(These will be worked on when I have time.)
-Ability to change oc settings for machines. 
-Add proper wattage settings for particle accelerator and quantum encoder.
-Add a scroll bar for the view screen.
-Add a dark theme. 
