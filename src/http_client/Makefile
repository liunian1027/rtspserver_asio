CC = g++
OBJS=CClient.o CHttpParser.o CHttpSocket.o CParser.o main.o
TARGET = httpClient
$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS)  
$(OBJS): %.o : %.cpp
	$(CC) -c $< -o $@

