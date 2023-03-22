#include "Framework.h"
#include "Heap.h"

void Heap::Insert(Node* node)
{
    heap.push_back(node);
    UpdateUpper((int)heap.size() - 1);
}

void Heap::UpdateUpper(int index)
{
    int curIndex = index;
    int parent = (curIndex-1) / 2;
    while (curIndex != 0) {
        if (heap[parent]->GetCost() <= heap[curIndex]->GetCost())
            return;

        Swap(parent, curIndex);

        curIndex = parent;
        parent = (parent - 1) / 2;
    }
}

Node* Heap::DeleteRoot()
{
    Node* root = heap.front();
    Swap(0, (int)heap.size() - 1);
    heap.pop_back();
    UpdateLower(0);

    return root;
}

void Heap::UpdateLower(int index)
{
    int curIndex = index;
    int lChild = index * 2 + 1;
    int rChild = lChild + 1;

    while (true) {
        int minNode = curIndex;
        if (lChild < heap.size() && heap[lChild]->GetCost() < heap[minNode]->GetCost()) {
            minNode = lChild;
        }

        if (rChild < heap.size() && heap[rChild]->GetCost() < heap[minNode]->GetCost()) {
            minNode = rChild;
        }

        if (minNode == curIndex)
            return;

        Swap(curIndex, minNode);
        curIndex = minNode;
        lChild = curIndex * 2 + 1;
        rChild = lChild + 1;
    }
}

void Heap::Swap(int n1, int n2)
{
    auto tmp = heap[n1];
    heap[n1] = heap[n2];
    heap[n2] = tmp;
}
