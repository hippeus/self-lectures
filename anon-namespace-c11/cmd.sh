g++ -I controllers -I .  controllers/Logger.cpp controllers/Peripherals.cpp main.cpp -std=c++11 -Wall -Wextra -o a.out && time -f "%E real,\t%U user,\t%S sys"  ./a.out
