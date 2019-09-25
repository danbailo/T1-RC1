#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

int server(){
    //Socket orientado a fluxo aberto com endereço da Internet e também acompanha o descritor de socket
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if(server < 0){
        cerr << "Erro ao estabeler conexão com o socket do servidor!" << endl;
        exit(0);
    }
    return server;
}

int bind(int port, int serverSd){
    //Configura o socket e as ferramentas de conexão.
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    //Vincula o socket com o endereço local.
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr));
    if(bindStatus < 0){
        cerr << "Erro ao criar socket com esta porta! Por favor, tente outra." << endl;
        exit(0);
    }
    return bindStatus;
}

int connect(int server){
    cout << "Esperando o cliente se conectar..." << endl;

    //Aceita 1 solicitação por vez.
    listen(server, 2);

    //Recebe uma solicitação do cliente usando a função "accept".
    //Precisamos de um novo endereço para conectar com o cliente.
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    // "accept", cria um novo descritor de soquete para manipular a nova conexão com o cliente.
    int newSd = accept(server, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0){
        cerr << "Erro ao aceitar requisição do cliente!" << endl;
        exit(1);
    }
    cout << "Cliente conectado!" << endl;
    return newSd;    
}

//Função para pegar a data do sistema.
string curr_date(){
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d/%m/%Y", &tstruct);
    return buf;
}

//Função para pegar a hora do sistema.
string curr_time(){
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}

void service(int newSd){
    //Cria um buffer para enviar e receber dados(mensagens);
    char msg[1500];

    //Acompanha a quantidade de dados enviados também.
    int bytesRead, bytesWritten = 0;
    int state = 1;
    while(state){
        string data;

        memset(&msg, 0, sizeof(msg));//Limpa o buffer.
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);            

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
            state = 0;
            break;
        default:
            data = "Não compreendi a sua solicitação!";
            break;
        }	
        memset(&msg, 0, sizeof(msg));//Limpa o buffer.
        strcpy(msg, data.c_str());    
        
        //Envia a mensagem para o cliente.
        bytesWritten += send(newSd,(char*)&msg,strlen(msg),0);
    }
    //Fecha a conexão com este cliente.
    close(newSd);
}


//Lado do Servidor
int main(int argc, char *argv[]){

    //Nessa parte iremos forçar o usuário entrar com a porta do servidor.
    if(argc != 2){
        cerr << "Informe uma porta para executar" << endl;
        exit(0);
    }

    //Atribui a Porta infomada pelo usuário a esta variável.
    int port = atoi(argv[1]);
    
    //Liga o servidor;
    int serverSd = server();

    //Configura o socket;
    int bindStatus = bind(port, serverSd);

    int state = 0;
    while(!state){
        //Recebe uma conexão(cliente);
        int client = connect(serverSd);

        //Realiza o serviço;
        cout << "\nEm que posso ajudar?" << endl;
        service(client);
        cout << "O Cliente encerrou a conexão!\n" << endl;   
    }
    // Fecha os descritores do socket depois de tudo pronto.(Desliga o servidor/porta pra qual ele foi vinculado)
    close(serverSd);
    return 0;   
}
