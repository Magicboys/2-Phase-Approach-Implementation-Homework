// 2-Phase Implementation.cpp 

//Imports
#include <iostream>
#include <cstring>

//Method headers
void Add(int iKey, const char* string);
void Delete(int iKey);
bool Commit(bool votes);
void Rollback();
void ToString();

struct Node {
    const char* data;
    Node* next;
};

//Linked list
Node* _head = nullptr;

int main()
{
    //Tests
    const char* a = "martina";
    const char* b = "tyler";
    const char* c = "caleb";
    const char* d = "pam";
    const char* e = "pam";

    Add(0, a); //martina
    ToString();
    Delete(0); //nullptr
    ToString();
    Add(0, b); //tyler
    ToString();
    Add(0, c); //caleb tyler
    ToString();
    Add(0, d); //pam caleb tyler
    ToString();
    Delete(1); //pam tyler
    ToString();
}

//Handles the Phase One, Commit Decision, and Phase Two transaction for the linkedlist add operation
void Add(int iKey, const char* string) {
    Node* newNode = new (std::nothrow) Node;
    bool continueToPhaseTwo = true;

    //Phase One
    //Check if we have enough memory to conduct the operation
    if (newNode == nullptr) {
        std::cout << "Ran out of memory, unable to add a new element!" << std::endl;
        continueToPhaseTwo = false;
    }

    //Check whether the element already exists
    Node* currentNode = _head;
    if (currentNode != nullptr) {
        while (currentNode->next != nullptr) {
            if (std::strcmp(currentNode->data, string) == 0) {
                std::cout << "Data already exists!" << std::endl;
                continueToPhaseTwo = false;
                break;
            }
            currentNode = currentNode->next;
        }
    }
    
    //Commit Decision
    bool commitResult = Commit(continueToPhaseTwo);
    if (!commitResult) {
        Rollback();
        return;
    }

    //Phase Two
    //If the list is already empty
    if (_head == nullptr) {
        newNode->data = string;
        newNode->next = nullptr;
        _head = newNode;
        return;
    }

    //If we add a new element at the very beginning
    if (iKey == 0) {
        newNode->data = string;
        newNode->next = _head;
        _head = newNode;
        return;
    }

    currentNode = _head;
    int i = 0;
    while (currentNode->next != nullptr) {
        //Case if we are inserting the node within
        if (i == iKey-1) {
            Node* saveNextNode = currentNode->next;
            newNode->next = saveNextNode;
            newNode->data = string;
            currentNode->next = newNode;
            return;
        }
        currentNode = currentNode->next;
        i++;
    }
    //Case if we are adding a new node to the end
    newNode->next = nullptr;
    newNode->data = string;
    currentNode->next = newNode;
}


//Handles the Phase One, Commit Decision, and Phase Two transaction for the linkedlist delete operation
void Delete(int iKey) {
    bool continueToPhaseTwo = false;

    //Phase One
    //Check whether the element exists
    Node* currentNode = _head;
    if (currentNode != nullptr) {
        int i = 0;
        while (currentNode->next != nullptr) {
            if (i == iKey) {
                continueToPhaseTwo = true;
                break;
            }
            currentNode = currentNode->next;
            i++;
        }
        if ((iKey == 0) && (_head != nullptr)) {
            continueToPhaseTwo = true;
        }
    }

    //Commit Decision
    bool commitResult = Commit(continueToPhaseTwo);
    if (!commitResult) {
        Rollback();
        return;
    }

    //Phase Two
    //If we want to delete the head
    if (iKey == 0) {
        Node* oldHead = _head;
        _head = _head->next;
        delete oldHead;
        return;
    }

    //If we want to delete an element in the middle
    Node* previousNode = _head;
    int i = 0;
    currentNode = _head;
    while (currentNode->next != nullptr) {
        if (i == iKey) {
            previousNode->next = currentNode->next;
            delete currentNode;
            return;
        }
        previousNode = currentNode;
        currentNode = currentNode->next;
        i++;
    }
}

//Takes in the votes and returns the votes back to the participant party
bool Commit(bool votes) {
    return votes;
}

//For delete and add operations we only perform data checks within phase one so if a roll back is needed there will be nothing to roll back.
//All add and delete operations will happen in phase two because we can't afford a failure when saving or removing data due to it being a linked list.
//If we were dealing with operations over a network where concurrent operations are a possibility, we would likely store recent transactions in the case of needing a roll back but since this is a single threaded operation it is not necessary
void Rollback() {
}

void ToString() {
    if (_head == nullptr) {
        std::cout << "NULLPTR" << std::endl;
        return;
    }

    Node* currentNode = _head;
    while (currentNode->next != nullptr) {
        //Case if we are inserting the node within
        std::cout << currentNode->data << " ";
        currentNode = currentNode->next;
    }
    std::cout << currentNode->data << " " << std::endl;
}

