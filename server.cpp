#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

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

//Lado do Servidor
int main(int argc, char *argv[]){

    //Nessa parte iremos forçar o usuário entrar com a porta do servidor.
    if(argc != 2){
        cerr << "Informe uma porta para executar" << endl;
        exit(0);
    }

    //Atribui a Porta infomada pelo usuário a esta variável.
    int port = atoi(argv[1]);
    
    //Cria um buffer para enviar e receber dados(mensagens);
    char msg[1500];
     
    //Configura o socket e as ferramentas de conexão.
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    //Ssocket orientado a fluxo aberto com endereço da Internet e também acompanha o descritor de socket
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0){
        cerr << "Erro estabelecendo socket do servidor!" << endl;
        exit(0);
    }

    //Vincula o socket com o endereço local.
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr));
    if(bindStatus < 0){
        cerr << "Erro ao criar socket com porta! tente outra." << endl;
        exit(0);
    }
    cout << "Esperando o cliente se conectar..." << endl;

    //Aceita até 5 solicitações por vez.
    listen(serverSd, 5);

    //Recebe uma solicitação do cliente usando a função "accept".
    //Precisamos de um novo endereço para conectar com o cliente.
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    // "accept", cria um novo descritor de soquete para manipular a nova conexão com o cliente.
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0){
        cerr << "Erro aceitando requisicao do cliente!" << endl;
        exit(1);
    }
    cout << "Cliente conectado!" << endl;

    //Acompanha a quantidade de dados enviados também.
    int bytesRead, bytesWritten = 0;
    
    int state = 1;
    while(state){
        memset(&msg, 0, sizeof(msg));//Limpa o buffer.
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);            

        string data;

        // cout << "msg: " << msg << endl;
        // cout << "atoi msg: " << atoi(msg) << endl;

        // Se ele enviar algo diferente das opções, vai retornar um 0 e não entra nesse if.
        // if(atoi(msg) != 0){
            cout << "nao entrei\n";
            // Recebe uma mensagem do cliente(escuta).
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
        // }
        // else{
        //     cout << "ENTREI NO ELSE\n";
        //     memset(&msg, 0, sizeof(msg));//Limpa o buffer.
        //     strcpy(msg, data.c_str());    
        //     //Envia a mensagem para o cliente.
        //     bytesWritten += send(newSd,(char*)&msg,strlen(msg),0);
        //     state = 0; //Modifica o estado do automato para parar a execução.
        // }
    }
    // Fecha os descritores do socket depois de tudo pronto.
    close(newSd);
    close(serverSd);
    return 0;   
}
