### Chicken_Bots
# Resource allocation swarms with motion planning grammar

## About
# This project tries to mimic the behaviour of chickens looking for food in a barnyard. When a chicken finds something of interest it begins to cluck loudly to the other checkens around. The chickens in the surrounding area chase after the clucking chicken in hopes of finding food.
# This project replicates this behaviour using Arduino Unos and a swarm robot platform. The robots search around an arena for a line. When one robot finds the line it will light-up, to signal to the other robots, and then start to follow the line. The other robots will (hopefully) pick-up the increase in light from photoresistors and attempt to locate the lit-up bot.

## To Run
# 1. Download Arduino IDE
# 2. Connect Arduino Uno to PC using USB cable
# 3. Import Final_Proj.ino into Arduino IDE
# 4. Select board type (Tools -> Board -> "Arduino Uno")
# 5. Select port (Tools -> port -> <select Uno port>)
# 1. Click "Upload"
