#include <iostream>
#include <csignal>
#include <vector>
#include <unistd.h>

bool canPrint = true;
int processIndex = 0;
std::vector<pid_t> pidsVector;

void processManager(int sigNum) {
    if (sigNum == SIGUSR1) {
        if (canPrint) {
            canPrint = false;
        } else {
            if(++processIndex >= pidsVector.size())
                processIndex = 0;
        }
        kill(pidsVector.at(processIndex), SIGUSR2);
    }

    if (sigNum == SIGUSR2) {
        canPrint = true;
    }
}

void childPrint() {
    std::string pid = std::to_string(getpid());
    while(1) {
        canPrint = false;
        kill(getppid(),SIGUSR1);
        while(!canPrint) {
            sleep(1);
        }
        std::cout << "Pid : ";
        for(int i = 0;i < pid.size();i++) {
            std::cout << pid[i];
            sleep(1);
        }
        std::cout << std::endl;
    }
}

int main()
{
    struct sigaction sa;
    sa.sa_handler = processManager;
    sigaction(SIGUSR2, &sa, 0);
    sigaction(SIGUSR1, &sa, 0);
    std::cout << "\"+\" : create process\n\"-\" : delete last process\n\"q\" : close programm" << std::endl;
    canPrint = true;
    while (true) {
        char c = std::cin.get();
        std::cin.clear();
        switch (c) {
            case '+':
                switch (int pid = fork()) {
                    case -1:
                        std::cout << "Error." << std::endl;
                        exit(1);
                    case 0:
                        childPrint();
                        break;
                    default:
                        pidsVector.push_back(pid);
                        break;
                }
                break;
            case '-':
                if (!pidsVector.empty()) {
                    kill(pidsVector.at(pidsVector.size() - 1), SIGTERM);
                    pidsVector.pop_back();
                    if (pidsVector.empty()) {
                        std::cout << "No more processes running, enter \"+\" to add new one." << std::endl;
                    }
                } else {
                    std::cout << "No processes running." << std::endl;
                }
                break;
            case 'q':
                while (!pidsVector.empty()) {
                    kill(pidsVector.at(pidsVector.size() - 1), SIGTERM);
                    pidsVector.pop_back();
                }
                exit(0);
        }
    }
}