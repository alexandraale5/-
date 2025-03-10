#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct KnapsackItem {
    int value;
    int weight;
};

struct KnapsackNode {
    int level;
    int totalValue;
    int totalWeight;
    double upperBound;
};

bool compareItems(const KnapsackItem& a, const KnapsackItem& b) {
    return static_cast<double>(a.value) / a.weight > static_cast<double>(b.value) / b.weight;
}

double calculateBound(const KnapsackNode& node, int itemCount, int capacity, const vector<KnapsackItem>& items) {
    if (node.totalWeight >= capacity) return 0.0;
    
    double potentialProfit = node.totalValue;
    int idx = node.level + 1;
    int currentWeight = node.totalWeight;

    while (idx < itemCount && currentWeight + items[idx].weight <= capacity) {
        currentWeight += items[idx].weight;
        potentialProfit += items[idx].value;
        idx++;
    }

    if (idx < itemCount) {
        potentialProfit += (static_cast<double>(capacity - currentWeight) * items[idx].value / items[idx].weight);
    }

    return potentialProfit;
}

int knapsackProblem(int capacity, vector<KnapsackItem>& items) {
    int itemCount = items.size();
    sort(items.begin(), items.end(), compareItems);

    queue<KnapsackNode> nodesQueue;
    nodesQueue.push({-1, 0, 0, 0.0});

    int maximumProfit = 0;

    while (!nodesQueue.empty()) {
        KnapsackNode currentNode = nodesQueue.front();
        nodesQueue.pop();

        if (currentNode.level == itemCount - 1) continue;

        KnapsackNode newNode;

        newNode.level = currentNode.level + 1;
        newNode.totalWeight = currentNode.totalWeight + items[newNode.level].weight;
        newNode.totalValue = currentNode.totalValue + items[newNode.level].value;
        
        if (newNode.totalWeight <= capacity) {
            maximumProfit = max(maximumProfit, newNode.totalValue);
        }
        
        if (newNode.totalWeight <= capacity) {
            newNode.upperBound = calculateBound(newNode, itemCount, capacity, items);
            
            if (newNode.upperBound > maximumProfit) {
                nodesQueue.push(newNode);
            }
        }

        newNode.level = currentNode.level + 1;
        newNode.totalWeight = currentNode.totalWeight;
        newNode.totalValue = currentNode.totalValue;
        newNode.upperBound = calculateBound(newNode, itemCount, capacity, items);

        if (newNode.upperBound > maximumProfit) {
            nodesQueue.push(newNode);
        }
    }

    return maximumProfit;
}

int main() {
    int itemCount, capacity;
    cin >> itemCount >> capacity;

    vector<KnapsackItem> items(itemCount);
    for (int i = 0; i < itemCount; i++) {
        cin >> items[i].value >> items[i].weight;
    }

    cout << knapsackProblem(capacity, items) << endl;

    return 0;
}
