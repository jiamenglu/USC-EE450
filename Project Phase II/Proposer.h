#ifndef __PROPOSER_H__
#define __PROPOSER_H__

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "Acceptor.h"
#include <vector>

using namespace std;

struct Proposal
{
    string type;
    unsigned int proposalNum = 0;
    int proposalVal = -1;
    unsigned int proposerID;
};

class Proposer
{
public:
    Proposer(unsigned int id, vector<Acceptor> acceptors);
    void run(unsigned int id);
    vector<Acceptor> acceptors;

private:
    unsigned int id;
    int sockfd;
    struct sockaddr_in servaddr;
    unsigned int promiseNum;
    void propose(unsigned int id);
    void accept(unsigned int id);
    bool receive();
    string proposal(Proposal prop);
    unsigned int acceptCt = 0;
    unsigned int promiseCt = 0;
    int acceptedVal;
    unsigned int majority;
};
#endif