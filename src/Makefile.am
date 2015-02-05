noinst_PROGRAMS = joyfly_cpp

joyfly_cpp_SOURCES = \
	main.cpp	\
	joystickexception.h \
	sdljoystick.h \
	sdljoystick.cpp \
	joystickinputer.h \
	joystickinputer.cpp \
	abstractchopper.cpp \
	abstractchopper.h \
	serialstream.h \
	serialstream.cpp \
	simulatedchopper.h \
	simulatedchopper.cpp \
	choppercontrol.cpp \
	choppercontrol.h \
	c-joy-fly-controller.cpp \
	c-joy-fly-controller.h \
	c-joy-fly-view.cpp \
	c-joy-fly-view.h \
	c-terminal-view.cpp \
	c-terminal-view.h 

AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)

joyfly_cpp_LDADD = \
	$(SDL_LIBS) \
	$(BOOST_SYSTEM_LIB)
