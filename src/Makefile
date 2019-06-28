CC = g++
CFLAGS = -g -Wall -std=c++14 -lpthread
TARGET = server client
OBJDIR = ../bin

all: $(TARGET)

server: $(OBJDIR)/server.o $(OBJDIR)/message_queue.o
	$(CC) $(OBJDIR)/server.o $(OBJDIR)/message_queue.o $(CFLAGS) -o server

client: $(OBJDIR)/client.o
	$(CC) $(OBJDIR)/client.o $(CFLAGS) -o client

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJDIR)/*.o $(TARGET)
