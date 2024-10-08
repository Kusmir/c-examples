CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2


SRCDIR = src
BUILDDIR = build
LIBDIR = lib


CONNECTION_SRC = \
    $(SRCDIR)/connection/Connection.cpp \
    $(SRCDIR)/connection/IPEndpoint.cpp \
    $(SRCDIR)/connection/PacketManager.cpp \
    $(SRCDIR)/connection/Socket.cpp


GAMEENGINE_SRC = \
    $(SRCDIR)/GameEngine/Card.cpp \
    $(SRCDIR)/GameEngine/GameEngine.cpp \
    $(SRCDIR)/GameEngine/Player.cpp \
    $(SRCDIR)/GameEngine/Seat.cpp \
    $(SRCDIR)/utils/parser.cpp


COMMON_SRC = \
    $(SRCDIR)/common.cpp \
    $(SRCDIR)/utils/client_options.cpp \
    $(SRCDIR)/utils/server_options.cpp


CLIENT_SRC = \
    $(SRCDIR)/client.cpp


SERVER_SRC = \
    $(SRCDIR)/server.cpp


CONNECTION_OBJ = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(CONNECTION_SRC))
GAMEENGINE_OBJ = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(GAMEENGINE_SRC))
COMMON_OBJ = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(COMMON_SRC))
CLIENT_OBJ = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(CLIENT_SRC))
SERVER_OBJ = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SERVER_SRC))


LIBGAMEENGINE = $(LIBDIR)/libGameEngine.a
LIBCONNECTION = $(LIBDIR)/libConnection.a


CLIENT = kierki-klient
SERVER = kierki-serwer
TARGETS = $(CLIENT) $(SERVER)

all: $(TARGETS)

$(CLIENT): $(CLIENT_OBJ) $(LIBCONNECTION) $(LIBGAMEENGINE) $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(CLIENT_OBJ) $(COMMON_OBJ) -L$(LIBDIR) -lConnection -lGameEngine

$(SERVER): $(SERVER_OBJ) $(LIBCONNECTION) $(LIBGAMEENGINE) $(COMMON_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(SERVER_OBJ) $(COMMON_OBJ) -L$(LIBDIR) -lConnection -lGameEngine

$(LIBCONNECTION): $(CONNECTION_OBJ)
	mkdir -p $(LIBDIR)
	ar rcs $@ $^

$(LIBGAMEENGINE): $(GAMEENGINE_OBJ)
	mkdir -p $(LIBDIR)
	ar rcs $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILDDIR) $(LIBDIR) $(TARGETS)

.PHONY: all clean
