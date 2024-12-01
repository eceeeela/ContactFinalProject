#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>

using namespace std;
typedef std::string String;

class TreeNode {
public:
    String medicareId;
    int directContacts = 0;
    int totalCases = 1;
    TreeNode* parentPtr;
    vector<TreeNode*> directContactsPtrList;

    TreeNode(const String& medId) : medicareId(medId), parentPtr(nullptr) {}
    TreeNode(TreeNode* parent, const String& medId) : medicareId(medId), parentPtr(parent) {}

    ~TreeNode() {
        for (auto child : directContactsPtrList) {
            delete child;
        }
    }
};

class ContactTree {
private:
    TreeNode* root;
    unordered_map<String, TreeNode*> nodeMap;
    int size;

public:
    ContactTree() : root(nullptr), size(0) {}

    ~ContactTree() {
        delete root;
        root = nullptr;
    }

    bool IsEmpty() const {
        return root == nullptr;
    }

    int GetSize() const {
        return size;
    }

    void AddPatient0(const String& medicareId) {
        if (root == nullptr) {
            root = new TreeNode(medicareId);
            nodeMap[medicareId] = root;
            size = 1;
        }
    }

    void AddContact(const String& parentId, const String& childId) {
        auto parentIt = nodeMap.find(parentId);
        if (parentIt != nodeMap.end()) {
            TreeNode* parentNode = parentIt->second;
            TreeNode* childNode = new TreeNode(parentNode, childId);
            parentNode->directContactsPtrList.push_back(childNode);
            parentNode->directContacts++;
            nodeMap[childId] = childNode;

            TreeNode* current = parentNode;
            while (current) {
                current->totalCases++;
                current = current->parentPtr;
            }
            size++;
        }
        else {
            cout << "Parent not found." << endl;
        }
    }

    TreeNode* LookUpContact(const String& medicareId) {
        auto it = nodeMap.find(medicareId);
        if (it != nodeMap.end()) {
            return it->second;
        }
        return nullptr;
    }

    void DeleteContact(const String& medicareId) {
        TreeNode* nodeToDelete = LookUpContact(medicareId);
        if (nodeToDelete == nullptr) {
            cout << "Node with Medicare ID " << medicareId << " not found." << endl;
            return;
        }

        queue<TreeNode*> queueNodesToDelete;
        queueNodesToDelete.push(nodeToDelete);

        while (!queueNodesToDelete.empty()) {
            TreeNode* current = queueNodesToDelete.front();
            queueNodesToDelete.pop();

            for (auto child : current->directContactsPtrList) {
                queueNodesToDelete.push(child);
            }

            if (current->parentPtr) {
                vector<TreeNode*>& siblings = current->parentPtr->directContactsPtrList;
                siblings.erase(remove(siblings.begin(), siblings.end(), current), siblings.end());
                current->parentPtr->directContacts--;
            }

            nodeMap.erase(current->medicareId);
            delete current;
            size--;
        }
    }

    void DisplayContact(const String& medicareId) {
        TreeNode* node = LookUpContact(medicareId);
        DisplayContact(node);
    }

    void DisplayContact(TreeNode* node) {
        if (node) {
            cout << "Medicare Id: " << node->medicareId << endl;
            cout << "Direct Contacts: " << node->directContacts << endl;
            cout << "Total Cases: " << node->totalCases << endl;
            if (node->parentPtr)
                cout << "Parent Id: " << node->parentPtr->medicareId << endl;
            else
                cout << "Parent Id: It's a Root Node" << endl;

            cout << "Child Ids: ";
            for (auto child : node->directContactsPtrList) {
                cout << child->medicareId << " ";
            }
            cout << endl;
        }
        else {
            cout << "Node is null" << endl;
        }
    }

    void PrintHierarchicalTree(TreeNode* node, int levelsTree = 0) {
        if (node == nullptr) {
            cout << "There is no root, so there is no Contact Tree." << endl;
            return;
        }

        for (int i = 0; i < levelsTree; ++i) {
            cout << "--";
        }

        DisplayContact(node);

        for (auto child : node->directContactsPtrList) {
            PrintHierarchicalTree(child, levelsTree + 1);
        }
    }

    void PrintContactTree() {
        PrintHierarchicalTree(root);
    }
};

int main() {
    ContactTree contactTree;
    contactTree.AddPatient0("A");
    contactTree.AddContact("A", "B");
    contactTree.AddContact("A", "C");
    contactTree.AddContact("A", "D");
    contactTree.AddContact("B", "childrenB1");
    contactTree.AddContact("C", "childrenC1");
    contactTree.AddContact("C", "childrenC2");
    contactTree.AddContact("D", "childrenD1");
    contactTree.AddContact("D", "childrenD2");
    contactTree.AddContact("D", "childrenD3");

    cout << "Initial Contact Tree:" << endl;
    contactTree.PrintContactTree();

    cout << "\nContact Tree after deleting 'childrenD1':" << endl;
    contactTree.DeleteContact("childrenD1");
    contactTree.PrintContactTree();

    return 0;
}
