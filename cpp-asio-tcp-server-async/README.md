# Async TCP server

## Where is the async occured

1. async write message through tcp socket
2. async accept client's connection (system call support)


## Who create the socket ? 

### Case1: Server
server owns the socket and accept client connection on that socket


### Case2: Client
server accept the connection and get the reference of socket 

## test with telnet

```shell
telnet localhost 3000
send "hello from client"
```