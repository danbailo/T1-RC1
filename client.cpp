#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

using namespace std;

//Lado do Cliente
int main(int argc, char *argv[]){

    //Nessa parte iremos forçar o usuário entrar com o IP e porta do servidor.
    if(argc != 3){
        cerr << "Digite o IP(localhost) e a porta do servidor!" << endl; 
        exit(0); 
    } 
    
    //Atribui o IP e a Porta as seguintes variáveis.
    char *serverIp = argv[1]; int port = atoi(argv[2]); 

    //Cria um buffer(mensagem).
    char msg[1500]; 
    
    //Configura o socket e as ferramentas de conexão.
    struct hostent* host = gethostbyname(serverIp); 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);

    //Tenta conectar
    int status = connect(clientSd,(sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0){
        cout << "Erro ao conectar com o socket!" << endl;
        cout << "Verifique se o servidor está ligado!" << endl;
        cout << "Verifique se o IP e a porta do servidor estão corretos!" << endl;
        return -1;
    }

    cout << "Conectado com o servidor!" << endl;
    int bytesRead, bytesWritten = 0;

    //Automato que contém dois estados, o inicial é um loop com as opções, e o final é 
    // para parar a execução do cliente.
    int state = 1;
    while(state){
        string data = "";
        cout << "\n[1] Qual é os integrantes deste grupo?" << endl;
        cout << "[2] Qual é a data de hoje?" << endl;
        cout << "[3] Que horas são?" << endl;
        cout << "[4] Sair." << endl;
        getline(cin, data);

        memset(&msg, 0, sizeof(msg));//Limpa o buffer
        strcpy(msg, data.c_str());    
        bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);

        memset(&msg, 0, sizeof(msg));//Limpa o buffer
        bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);

        cout << "Resposta do servidor: " << msg << endl;
        
        //Opção para finalizar a conexão com o servidor.
        if(atoi(data.c_str()) == 4) state = 0;
    }

    //Finaliza a conexão com o servidor.
    close(clientSd);
    return 0;    
}
