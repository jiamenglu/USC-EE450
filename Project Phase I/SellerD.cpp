#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <arpa/inet.h>

#define SellerD_TCP_PORT 3976

using namespace std;

int tcp_sockfd;
struct sockaddr_in tcp_servaddr, tcp_cliaddr;

string readFile(string filename) {
    string res = "";
    ifstream infile(filename);
    string line;
    while (getline(infile, line)) {
        stringstream iss(line);
        string s;
        while (iss.good()) {
            getline(iss, s, ':');
            res += (s+",");
        }
    }

    return "sellerD,"+res.substr(0, res.length()-1);
}

/* Phase I Part 4 */
void create_tcp_server() {
    char ipaddr[INET_ADDRSTRLEN];

    tcp_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_sockfd < 0) perror("ERROR opening socket");

    bzero((char *) &tcp_servaddr, sizeof(tcp_servaddr)); // make sure the struct is empty

    tcp_servaddr.sin_family = AF_INET;
    tcp_servaddr.sin_addr.s_addr = INADDR_ANY;
    tcp_servaddr.sin_port = htons(SellerA_TCP_PORT);

    if (bind(tcp_sockfd, (struct sockaddr *) &tcp_servaddr, sizeof(tcp_servaddr)) < 0) perror("ERROR on binding");
    strcpy(ipaddr, inet_ntoa(tcp_servaddr.sin_addr));

    if (listen(tcp_sockfd, 5) < 0) perror("ERROR on listening");

    cout << "<SellerD> has TCPport " << SellerD_TCP_PORT << " and IP address " << ipaddr << " for Phase I part 4" << endl;
}

/* Phase I Part 1 */
void create_tcp_client() {
    char ipaddr[INET_ADDRSTRLEN];

    tcp_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (tcp_sockfd == -1) perror("Socket");

    bzero((void *) &tcp_servaddr, sizeof(tcp_servaddr));
    tcp_servaddr.sin_family = AF_INET;
    tcp_servaddr.sin_port = htons(4076);
    tcp_servaddr.sin_addr.s_addr = INADDR_ANY;

    strcpy(ipaddr, inet_ntoa(tcp_servaddr.sin_addr));
    cout << "<SellerD> has TCPport " << SellerD_TCP_PORT << " and IP address " << ipaddr << " for Phase 1part 1" << endl;
}

/* Phase I Part 1 */
void send_to_agent() {
    char buf[1024];
    int numbyte;

    if (-1 == connect(tcp_sockfd, (struct sockaddr *)&tcp_servaddr, sizeof(tcp_servaddr))) perror("Connect");

    cout << "<SellerD> is now connected to the <Agent2>" << endl;

    string data = readFile("seller/sellerD.txt");
    strcpy(buf, data.c_str());  
    
    if ((numbyte = send(tcp_sockfd, buf, 1024, 0)) > 0) {
        cout << "<SellerD> has sent " << buf << " to the agent" << endl;
    }
    cout << "End of Phase I part 1 for <SellerD>" << endl;
}

int main(int argc, char *argv[]) {
    create_tcp_client();
    send_to_agent();
    create_tcp_server();

    return 0;
}