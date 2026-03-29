_This project has been created as part of the 42 curriculum by stkloutz_
# Minitalk
## Description
The goal is to create a __communication program__ in the form of a __client__ and a __server__.  
The project must be written in C.  
- The server must be started first. Upon launch, it must print its PID.
- The client takes two parameters:
    - The server PID.
    - The string to send.
- The client must send the specified string to the server.
- Once received, the server must print it.
- The server must display the string without delay. If displaying 100 characters takes 1 second, the program is too slow!
- The server should be able to receive strings from several clients in a row without needing to restart.
- Communication between the client and server must exclusively use __UNIX signals__.
- You can only use these two signals: __SIGUSR1__ and __SIGUSR2__.
## Instructions
- git clone the repository
- enter the repository you just cloned
- run make  
- run the server: ```./server```
- run the client: ```./client [server PID] "the string to send"```  
  
_Note:_ Valgrind does not work very well with this project. It is better to use __AddressSanitizer__ to check for memory leaks by adding the flag: ```-fsanitize=address```.  
You can run  
```make server_san```  
to compile the server with this flag.
## Resources
### Bitwise and shift operators
To send a string using only 2 signals, we must decompose each char into bits (1 char = 1 byte = 8 bits).  
This video shows how to print chars as bits, it helped me understand bitshift and bitwise operators:  
https://www.youtube.com/watch?v=Kj3iboADvUc  
### Sending and intercepting a signal
In my opinion, the main difficulty of this project was understanding how signals work. It was not easy to handle the asynchronous nature of signals communication: the signal can be intercepted by the program at any time, and not necessarily when I want it to be.  
This website was very helpful:  
https://www.codequoi.com/envoyer-et-intercepter-un-signal-en-c  
Here is the man page of _sigaction_, which was very useful too:  
https://man7.org/linux/man-pages/man2/sigaction.2.html
### Use of AI
I asked AI to help me correct the mistakes in this README. I hope there are not too many left.
