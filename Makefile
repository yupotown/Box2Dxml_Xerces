
CXX			= g++

SDL_CONFIG	= `sdl-config --cflags --libs`
#LIBXML2_CONFIG	= `xml2-config --cflags --libs`

CFLAGS		= -W -Wall -O2 -std=c++0x
LDFLAGS		= 
INCLUDES	= 
LIBS		= -lBox2D -lxerces-c $(SDL_CONFIG) -lSDL_image -lSDL_draw

TARGET		= box2dxml
OBJS		= main.o bodydrawer.o shapedrawer.o fpsctrl.o\
			box2dxml_cfg.o xmlworldreader.o xmlworldpartreader.o\
			convert.o constant.o xmlstring.o

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<

fpsctrl.o: fpsctrl.cpp fpsctrl.h
bodydrawer.o: bodydrawer.cpp bodydrawer.h
shapedrawer.o: shapedrawer.cpp shapedrawer.h position.hpp
box2dxml_cfg.o: box2dxml_cfg.cpp box2dxml_cfg.h
xmlworldreader.o: xmlworldreader.cpp xmlworldreader.h
xmlworldpartreader.o: xmlworldpartreader.cpp xmlworldpartreader.h
convert.o: convert.cpp convert.h
constant.o: constant.cpp constant.h
xmlstring.o: xmlstring.cpp xmlstring.hpp
xmlcomparestring.o: xmlcomparestring.hpp

clean:
	$(RM) $(TARGET) *.o

todo:
	grep -in "TODO" *.cpp *.h *.hpp | cat TODO -

