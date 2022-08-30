

server:
	g++ server.cpp message.cpp utils.cpp socket.cpp -o server

client:
	g++ client.cpp message.cpp utils.cpp socket.cpp -o client


clean:
	rm server
	rm client