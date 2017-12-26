DATA_STRUCT=game_engine/data_struct/list.c
LIB=game_engine/lib/gettickcount.c \
	game_engine/lib/lvmv_memory.c
GAME_ENGINE=game_engine/game_engine.c \
			game_engine/text_graphic.c \
			game_engine/kbhit.c
OBJECT=object/objtext.c \
	   object/objgamemaster.c \
	   object/objplayer.c \
	   object/objball.c \
	   object/objblock.c

all:
	gcc -g -o game $(DATA_STRUCT) $(LIB) $(GAME_ENGINE) $(OBJECT) game_main.c -lcurses -I.
