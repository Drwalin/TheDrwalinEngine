

HEADERPATH = ./src/css
SOURCEPATH = ./src/mtd
LIBRARYPATH = ./src/lib
BINARYPATH = ./bin
BULLETINCLUDEPATH = /usr/local/include/bullet

SOURCES_ = BasicWindow.cpp Keyboard.cpp Camera.cpp EventResponser.cpp StringToEnter.cpp TextPrinter.cpp Event.cpp Window.cpp Main.cpp World.cpp Engine.cpp Object.cpp Model.cpp Texture.cpp VBO.cpp
OBJECTS_ = $(SOURCES_:.cpp=.o)
SOURCES = $(addprefix $(SOURCEPATH)/,$(SOURCES_))
OBJECTS = $(addprefix $(BINARYPATH)/,$(OBJECTS_))

EXECUTABLE = main

CFLAGS = -m64 -s -Ofast -std=c++11 -I$(HEADERPATH) -I$(BULLETINCLUDEPATH) -I$(LIBRARYPATH)
LIBS = -lm -lpthread -lSM -lICE -lX11 -lXext -lXcursor -lXpm -lXi -lXinerama -lXrandr -lGL -lGLU -lpng -lz -lpulse-simple -lpulse -lasound -lopenal -lfreetype -lallegro_monolith-static -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath
CC = g++


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(BINARYPATH)/%.o: $(SOURCEPATH)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY : clean

clean:
	rm -f main
	rm -f $(BINARYPATH)/*.o



