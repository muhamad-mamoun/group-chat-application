# Group Chat Application

A C++ group chat application using socket programming for real-time communication. It allows multiple clients to connect to a server and exchange messages in real-time.

## Features
- Real-time group messaging
- Server handles multiple clients
- Clean exit with proper message broadcasting
- Client can join and leave the chat dynamically

## Requirements
- C++ compiler (GCC or Clang recommended)

## Installation
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd group-chat-application
   ```
2. Compile the project:
   ```bash
   g++ Server/*.cpp Utils/*.cpp -o Server/server-app.exe
   g++ Client/*.cpp Utils/*.cpp -o Client/client-app.exe
   ```

## Usage
### Run the Server
```bash
Server/server-app.exe
```
### Run the Client
```bash
Client/client-app.exe -i <server-ip> -p <server-port> -n <your-name>
```
- `-s`: Server Socket address
- `-i`: Server IP address
- `-p`: Server Port number
- `-n`: Client Name

### Example
```bash
./client -i 127.0.0.1 -p 3232 -n Mamoun
```
```bash
./client -s 127.0.0.1:3232 -n Mamoun
```

## How to Exit
- Type `goodbye` to leave the chat.
- Server can be terminated by typing `exit`.

## Known Issues
- The clients are stored in a fixed array and not being deleted (to be fixed).

## Contributing
Pull requests are welcome. For significant changes, please open an issue first to discuss your ideas.

