#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

string curr_date(){
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%d/%m/%Y", &tstruct);
    // strftime(buf, sizeof(buf), "%Y/%m/%d.%X", &tstruct);

    return buf;
}

string curr_time(){
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%X", &tstruct);
    // strftime(buf, sizeof(buf), "%Y/%m/%d.%X", &tstruct);
    return buf;
}

//Server side
int main(int argc, char *argv[])
{
    //for the server, we only need to specify a port number
    if(argc != 2)
    {
        cerr << "Informe uma porta para executar" << endl;
        exit(0);
    }
    //grab the port number
    int port = atoi(argv[1]);
    //buffer to send and receive messages with
    char msg[1500];
     
    //setup a socket and connection tools
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0){
        cerr << "Erro estabelecendo socket do servidor!" << endl;
        exit(0);
    }
    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0){
        cerr << "Erro ao criar socket com porta! tente outra." << endl;
        exit(0);
    }
    cout << "Esperando o cliente se conectar..." << endl;
    //listen for up to 5 requests at a time
    listen(serverSd, 5);
    //receive a request from client using accept
    //we need a new address to connect with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to 
    //handle the new connection with client
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Erro aceitando requisicao do cliente!" << endl;
        exit(1);
    }
    cout << "Cliente conectado!" << endl;

    //also keep track of the amount of data sent as well
    int bytesRead, bytesWritten = 0;
    
    bool flag = true;
    while(flag){
        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);            

        string data;
        if(atoi(msg) != 0){
            //receive a message from the client (listen)

            cout << "\nCliente enviou: " << msg << endl;
            switch(atoi(msg)){
            case 1:
                data = "Daniel, Josué e Beatriz";
                break;
            case 2:
                data = curr_date();
                break;
            case 3:
                data = curr_time();
                break;
            case 4:
                data = "Muito obrigado por utilizar nossos serviços!";
                break;
            default:
                data = "Não compreendi a sua solicitação!";
                break;
            }	
            memset(&msg, 0, sizeof(msg)); //clear the buffer
            strcpy(msg, data.c_str());    
            //send the message to client
            bytesWritten += send(newSd,(char*)&msg,strlen(msg),0);
        }
        else flag = false;
    }
    //we need to close the socket descriptors after we're all done
    close(newSd);
    close(serverSd);
    return 0;   
}
