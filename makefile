# makefile written for gnu make
#CROSS=sh4-linux-uclibc-
CXX = $(CROSS)g++
CC = $(CROSS)gcc
AR = $(CROSS)ar
ASIOLIB = ./lib/asio-1.4.1
INCDIR = ./include
SRCDIR = ./src
CPPFLAGS	= -I$(INCDIR) -I$(ASIOLIB)/include -I.
DEBUG		= 
OPTIMIZE	= -O2
GCCWARN		= -Wall 
CXXFLAGS	= $(DEBUG) $(GCCWARN) $(OPTIMIZE) $(INCLUDES)
STRIP	= @echo " strip  $@"; $(CROSS)strip


CFLAGS	=	-Wall -O2 -Os -Wno-uninitialized
CFLAGS	+=	-fomit-frame-pointer

LFLAGS = 
#LFLAGS  +=	-Wl,-lc,-lrt
LFLAGS	+=	-lpthread -ldl


OBJHTTPS = $(SRCDIR)/http_server/main.o

OBJHTTPC = $(SRCDIR)/http_client/CClient.o \
          $(SRCDIR)/http_client/CHttpParser.o \
          $(SRCDIR)/http_client/CHttpSocket.o \
          $(SRCDIR)/http_client/CParser.o \
          $(SRCDIR)/http_client/main.o

OBJRTSPS = $(SRCDIR)/rtsp_server/main.o \
          $(SRCDIR)/rtsp_server/debug.o \
          $(INCDIR)/CompLib/RTP/CRTPPacket.o \
          $(INCDIR)/CompLib/RTP/RTPStructs.o

OBJRTSPC = $(SRCDIR)/rtsp_client/main.o \

DIST := http_server http_client rtsp_server rtsp_client

MULTITASK:= libmultitask.a


all:		$(DIST)

client: $(DISTCLIENT)

$(DISTCLIENT):$(OBJCLIENT) $(MONGOOSE) $(LIB) $(SYSTEMLIBS)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -o $@ $^ $(LFLAGS)
	$(STRIP) $(DISTCLIENT)
	@chmod 777 $@
	@echo "#######################################"

http_server:  $(OBJHTTPS)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -o $@ $^ $(LFLAGS)
	@chmod 777 $@
	@echo "#######################################"

http_client:  $(OBJHTTPC)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -o $@ $^ $(LFLAGS)
	@chmod 777 $@
	@echo "#######################################"

rtsp_server:  $(OBJRTSPS)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -o $@ $^ $(LFLAGS)
	@chmod 777 $@
	@echo "#######################################"

rtsp_client:  $(OBJRTSPC)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -o $@ $^ $(LFLAGS)
	@chmod 777 $@
	@echo "#######################################"


lib: $(LIB)
$(LIB):		$(OBJ)
		$(AR) $(ARFLAGS) $(LIB) $(OBJ)

tests:		$(LIB)
		cd test && $(MAKE) CXX=$(CXX) CXXFLAGS="$(CXXFLAGS)" SYSTEMLIBS="$(SYSTEMLIBS)"

doc doxygen:
		cd src && doxygen Doxyfile

clean:
		rm -f ./*.o
		rm -f $(SRCDIR)/http_server/*.o
		rm -f $(SRCDIR)/http_client/*.o
		rm -f $(SRCDIR)/rtsp_server/*.o
		rm -f $(DIST)
		
copy:
	cp jsonrpcclient /home/share/xu_aiwei/bin/G6/DVR_NVE/UltraView
	cp jsonrpcserver /home/share/xu_aiwei/bin/G6/DVR_NVE/UltraView
