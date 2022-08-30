

server:
	g++ -g server.cpp message.cpp utils.cpp socket.cpp api.cpp -o server

client:
	g++ -g client.cpp message.cpp utils.cpp socket.cpp api.cpp -o client


clean:
	rm server
	rm client