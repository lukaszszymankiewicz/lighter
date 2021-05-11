Lighter

Custom rougelike game engine (and maybe full game later on).


Features to be implemented:
 - 2D game enginge,
 - simple raycasting engine (dynamic light),


How to start developing:
 - ensure that you have 'make' and 'gcc' on you system (other compiler than gcc also work but to use
   makefile as-it-is gcc is required),
 - ensure that you have SDL2 on your system (neat installation guide can be found here:
   https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php),
 - game is using some SDL extensions, please ensure that you have 'libsdl2-image-dev' package
   installed as well.

You can start developing!


 How to run:
 - compile using make command:
```
make lighter
```
 - ensure that lighter.o file is executable (if not use 'chmod +x lighter.o'),
 - run game by running object file: './lighter',

How to use prototype scripts:
Prototype scripts are for testing only, and it is not required (at all) to run any of the game code.
Here lays some mathematical concepts for further implementation/checking etc.

 - ensure you have Python3.5+ on your system,
 - create virtual enviroment in main folder of this project:
```
python3.8 -m venv venv
```
 - activate virtual enviroment,
```
source venv/bin/activate
```
 - install required packages:
```
pip install -r proto.req.txt
```
Now you can run all prototype scripts!
