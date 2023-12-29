# Backgammon++

Online Backgammon multiplayer game written in C++ using QT framework.

Backgammon++ supports multiple game modes - playing online, playing against our custom built neural network, or playing locally (2 players using the same machine to take turns). 

Additionally, Backgammon++ also includes multiple games played on the [tables](https://en.wikipedia.org/wiki/Tables_game) board:
- [Backgammon](https://en.wikipedia.org/wiki/Backgammon)
- [Long Nardy](https://en.wikipedia.org/wiki/Long_Nardy)

## Building instructions

### Using QT Creator
Install QT for Open Source Development from [qt.io](https://www.qt.io/download-open-source). While installing, make sure to select **Custom installation** and select the following components:
- QT 6.6.1 (in the QT dropdown)
- QT Multimedia (under QT / Additional Libraries)
- QT Creator (under Development and Designer Tools)
- CMake (under Development and Designer Tools)
- Ninja (under Development and Designer Tools)

Before building, you will need to [clone](https://docs.gitlab.com/ee/gitlab-basics/start-using-git.html#choose-a-repository) or [download](https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2023-2024/backgammon/-/archive/main/backgammon-main.zip) the repository.

Now open QT Creator, and click _Open Project_. Navigate to the project folder and inside into the _BackgammonPP_ directory, and open _CMakeLists.txt_. 

Building and running the project now is as simple as pressing the big green button on the bottom left of the screen.

### Using CMake without QT Creator (on Linux)
You will need `git`, `cmake`, and `g++` in order to clone and build the project, as well as two QT dependencies: `qt6-base-dev` and `qt6-multimedia-dev`. For example, on Debian you may run the following command to install the above packages:
```bash
sudo apt-get install git cmake g++ qt6-base-dev qt6-multimedia-dev
```

Next you will need to to clone the repository, and position yourself inside of it:
```bash
git clone https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2023-2024/backgammon.git && cd ./backgammon
```

Now build the project by running the following command:
```bash
cmake -S BackgammonPP -B ./build && make --directory=build
```

The binary is located inside the build directory, and can be launched from the terminal like this:
```bash
./build/BackgammonPP
```



## The team
 - <a href="https://gitlab.com/Brankonymous">Branko Grbić 2/2020</a>
 - <a href="https://gitlab.com/ivangogic">Ivan Gogić 29/2020</a>
 - <a href="https://gitlab.com/mgrujcic">Marko Grujčić 15/2020</a>
 - <a href="https://gitlab.com/igorkandic">Igor Kandić 70/2020</a>
 - <a href="https://gitlab.com/mladenpuzic">Mladen Puzić 18/2020</a>
 - <a href="https://gitlab.com/daniilgrbic">Daniil Grbić 42/2020</a>
 - <a href="https://gitlab.com/azecevic0">Aleksandar Zečević 45/2020</a>
