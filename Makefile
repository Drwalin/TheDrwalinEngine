
LIBS = -lm -lpthread -lfreetype
include win.mk


HEADERPATH = ./src/css
SOURCEPATH = ./src/mtd
LIBRARYPATH = ./src/lib
LIBPNGPATH = ./lib

SOURCES_ = Engine.cpp Model.cpp EngineRayTraceData.cpp BasicWindow.cpp Camera.cpp EventResponser.cpp StringToEnter.cpp TextPrinter.cpp Event.cpp Window.cpp Main.cpp World.cpp Object.cpp CustomCollisionData.cpp CollisionShapeManager.cpp SmartPtr.cpp TimeCounter.cpp EventReceiverIrrlicht.cpp
OBJECTS_ = $(SOURCES_:.cpp=.o)
SOURCES = $(addprefix $(SOURCEPATH)/,$(SOURCES_))
OBJECTS = $(addprefix $(BINARYPATH)/,$(OBJECTS_))


GAMESOURCEPATH = ./src/game

GAMESOURCES_ = Converter.cpp MeshLoader.cpp Character.cpp CharacterEvent.cpp Player.cpp
GAMEOBJECTS_ = $(GAMESOURCES_:.cpp=.o)
GAMESOURCES = $(addprefix $(GAMESOURCEPATH)/,$(GAMESOURCES_))
GAMEOBJECTS = $(addprefix $(BINARYPATH)/,$(GAMEOBJECTS_))

OBJECTS += $(GAMEOBJECTS)


CFLAGS += -m64 -s -Ofast -std=c++11 -I$(HEADERPATH) -I$(LIBRARYPATH) -w
LIBS += -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath -lIrrlicht
CC = g++


$(BINARYPATH)/%.o: $(SOURCEPATH)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINARYPATH)/%.o: $(GAMESOURCEPATH)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -L$(LIBPNGPATH) -o $@ $^ $(LIBS)



.PHONY : clean

clean:
	del /F main.exe
	del /F $(BINARYPATH)/*.o
	rm -f main
	rm -f $(BINARYPATH)/*.o



