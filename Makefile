all: server client


server:	sqlite3-server.cpp
	g++ sqlite3-server.cpp -o server  -l sqlite3

client:	client.cpp
	g++ client.cpp -o client -l sqlite3
clean:
	 rm -f client *~; rm -f server *~

