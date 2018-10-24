
BINARYPATH = ./bin-lin
BULLETINCLUDEPATH = /usr/local/include/bullet


CFLAGS += -I$(BULLETINCLUDEPATH)
LIBS += -lSM -lICE -lX11 -lXext -lXcursor -lXpm -lXi -lXinerama -lXrandr -lGL -lGLU -lpng16 -lz -lpulse-simple -lpulse -lasound -lopenal -lfreetype -lallegro_monolith-static

EXECUTABLE = main


