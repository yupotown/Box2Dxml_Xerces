
CXX			= g++

SDL_CONFIG	= `sdl-config --cflags --libs`
LIBXML2_CONFIG	= `xml2-config --cflags --libs`

CFLAGS		= -W -Wall -O2
LDFLAGS		= 
INCLUDES	= 
LIBS		= -lBox2D $(LIBXML2_CONFIG) $(SDL_CONFIG) -lSDL_image -lSDL_draw

TARGET_CONVERT	= test_convert
OBJS_CONVERT	= test_convert.o ../convert.o

TARGET	= $(TARGET_CONVERT)

.PHONY: all clean

all: $(TARGET_CONVERT)

#$(TARGET): $(OBJS)
#	$(CXX) $(LDFLAGS) -o $@ $+ $(LIBS)

$(TARGET_CONVERT): $(OBJS_CONVERT)
	$(CXX) $(LDFLAGS) -o $@ $+ $(LIBS)

clean:
	$(RM) $(TARGET) *.o

