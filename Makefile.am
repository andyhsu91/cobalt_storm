noinst_HEADERS = BaseApplication.h Cobalt.h Physics.h Environment.h Player.h Network.h Sound.h GUI.h ControlManager.h

bin_PROGRAMS = cobaltstorm
cobaltstorm_CPPFLAGS = -I$(top_srcdir)
cobaltstorm_SOURCES = BaseApplication.cpp Cobalt.cpp Physics.cpp Environment.cpp Player.cpp Network.cpp Sound.cpp GUI.cpp ControlManager.cpp
cobaltstorm_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) -lSDL -lSDL_mixer -lSDL_net
cobaltstorm_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS) $(SDL_MIXER_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS)

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
