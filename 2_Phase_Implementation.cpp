// 2-Phase Implementation.cpp 

//Imports
#include <iostream>
#include <cstring>

//Node Structure
struct Node {
    const char* data;
    Node* next;
};

//Method headers
void Add(int iKey, const char* string);
void Delete(int iKey);
bool Commit(bool votes);
void Rollback(Node* headCopy);
void ToString(Node* localHead);

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
    ToString(_head);
    Delete(0); //nullptr
    ToString(_head);
    Add(0, b); //tyler
    ToString(_head);
    Add(0, c); //caleb tyler
    ToString(_head);
    Add(0, d); //pam caleb tyler
    ToString(_head);
    Delete(2); //pam caleb
    ToString(_head);
    Add(0, b); //tyler pam caleb
    ToString(_head);
    Delete(2); //tyler pam
    ToString(_head);
}

//Handles the Phase One, Commit Decision, and Phase Two transaction for the linkedlist add operation
void Add(int iKey, const char* string) {
    //Save a copy of the original linked list incase of a rollback
    Node* _headCopy;
    if (_head != nullptr) {
        _headCopy = new (std::nothrow) Node;
    }
    else {
        _headCopy = nullptr;
    }
    Node* currentCopyNode = _head;

    //If a linked list exists then we copy over the data
    if (_head != nullptr) {
        _headCopy->data = _head->data;
        _headCopy->next = _head->next;

        Node* currentCopyNode = _headCopy;
        Node* temp = _head->next;
        while (temp != nullptr) {
            //Create new node
            Node* localCopy = new (std::nothrow) Node;
            localCopy->data = temp->data;
            localCopy->next = temp->next;

            //Update linkedlist with new node
            currentCopyNode->next = localCopy;
            currentCopyNode = localCopy;
            temp = temp->next;
        }
    }

    //Phase One
    Node* newNode = new (std::nothrow) Node;
    bool continueToPhaseTwo = true;

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
                continueToPhaseTwo = false;
                break;
            }
            currentNode = currentNode->next;
        }
    }
    
    //Commit Decision
    bool commitResult = Commit(continueToPhaseTwo);
    if (!commitResult) {
        std::cout << "Commit participants votes to not continue to phase 2, terminating operation now." << std::endl;
        delete newNode;
        return;
    }
   
    //Phase Two
    try {
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
    catch (int x) {
        ToString(_headCopy);
    }
    
}


//Handles the Phase One, Commit Decision, and Phase Two transaction for the linkedlist delete operation
void Delete(int iKey) {
    //Save a copy of the original linked list incase of a rollback
    Node* _headCopy;
    if (_head != nullptr) {
        _headCopy = new (std::nothrow) Node;
    }
    else {
        _headCopy = nullptr;
    }
    Node* currentCopyNode = _head;

    //If a linked list exists then we copy over the data
    if (_head != nullptr) {
        _headCopy->data = _head->data;
        _headCopy->next = _head->next;

        Node* currentCopyNode = _headCopy;
        Node* temp = _head->next;
        while (temp != nullptr) {
            //Create new node
            Node* localCopy = new (std::nothrow) Node;
            localCopy->data = temp->data;
            localCopy->next = temp->next;

            //Update linkedlist with new node
            currentCopyNode->next = localCopy;
            currentCopyNode = localCopy;
            temp = temp->next;
        }
    }

    //Phase One
    bool continueToPhaseTwo = false;

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

        if (currentNode->next == nullptr) {
            continueToPhaseTwo = true;
        }
        
    }

    //Commit Decision
    bool commitResult = Commit(continueToPhaseTwo);
    if (!commitResult) {
        std::cout << "Commit participants votes to not continue to phase 2, terminating operation now." << std::endl;
        return;
    }

    //Phase Two
    
    try {
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

        if (i == iKey) {
            previousNode->next = nullptr;
            delete currentNode;
        }
    }
    catch (int i) {
        Rollback(_headCopy);
    }
    
}

//Takes in the votes and returns the votes back to the participant party
bool Commit(bool votes) {
    return votes;
}

//Rollback method in case of an error during phase 2 operations. This method helps maintain data integrity in the case of a critical error.
void Rollback(Node* _headCopy) {
    std::cout << "Error in Phase Two, triggering a rollback to maintain data integrity!" << std::endl;

    //Base case
    if (_headCopy == nullptr) {
        _head = _headCopy;
        return;
    }

    //Next case is to delete the old corrupted version and save the backup as main.
    Node* currentNode = _head;
    while (currentNode != nullptr) {
        Node* nextAddress = currentNode->next;
        delete currentNode;
        currentNode = nextAddress;
    }

    _head = _headCopy;
}

void ToString(Node* localHead) {
    if (localHead == nullptr) {
        std::cout << "NULLPTR" << std::endl;
        return;
    }

    Node* currentNode = localHead;
    while (currentNode->next != nullptr) {
        //Case if we are inserting the node within
        std::cout << currentNode->data << " ";
        currentNode = currentNode->next;
    }
    std::cout << currentNode->data << " " << std::endl;
}

