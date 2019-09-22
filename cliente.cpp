#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

using namespace std;
//Client side
int main(int argc, char *argv[])
{
    //we need 2 things: ip address and port number, in that order
    if(argc != 3){
        cerr << "Digite o IP(localhost) e a porta do servidor!" << endl; 
        exit(0); 
    } //grab the IP address and port number 
    char *serverIp = argv[1]; int port = atoi(argv[2]); 
    //create a message buffer 
    char msg[1500]; 
    //setup a socket and connection tools 
    struct hostent* host = gethostbyname(serverIp); 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    //try to connect...
    int status = connect(clientSd,(sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0){
        cout << "Erro ao conectar com o socket!" << endl;
        cout << "Verifique se o servidor está ligado!" << endl;
        cout << "Verifique se o IP e a porta do servidor estão corretos!" << endl;
        return 0;
    }
    cout << "Conectado com o servidor!" << endl;
    int bytesRead, bytesWritten = 0;

    bool flag = true;
    while(flag){
        string data = "";
        cout << "\n[1] Qual é os integrantes deste grupo?" << endl;
        cout << "[2] Qual é a data de hoje?" << endl;
        cout << "[3] Que horas são?" << endl;
        cout << "[4] Sair." << endl;
        getline(cin, data);

        memset(&msg, 0, sizeof(msg));//clear the buffer
        strcpy(msg, data.c_str());    
        bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);

        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);

        cout << "Resposta do servidor: " << msg << endl;
        
        if(atoi(data.c_str()) == 4) flag = false;
    }
    close(clientSd);
    return 0;    
}
