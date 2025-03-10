#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;
using namespace chrono;

struct Item {
    int value;
    int weight;
};

struct Individual {
    vector<int> genes;
    int fitnessScore;
};

int itemCount, maxWeight;
vector<Item> itemList;
const int populationSize = 100;
const int numGenerations = 1000;
const double mutationChance = 0.1;

random_device randomDev;
mt19937 randomGen(randomDev());

int calculateFitness(const vector<int>& genes) {
    int totalValue = 0, totalWeight = 0;
    for (int i = 0; i < itemCount; ++i) {
        if (genes[i]) {
            totalValue += itemList[i].value;
            totalWeight += itemList[i].weight;
        }
    }
    return (totalWeight <= maxWeight) ? totalValue : 0;
}

vector<int> generateRandomGeneSet() {
    vector<int> genes(itemCount);
    for (int &gene : genes) {
        gene = randomGen() % 2;
    }
    return genes;
}

vector<int> performCrossover(const vector<int>& parentA, const vector<int>& parentB) {
    int crossoverPoint = randomGen() % itemCount;
    vector<int> offspring(itemCount);
    for (int i = 0; i < itemCount; ++i) {
        offspring[i] = (i < crossoverPoint) ? parentA[i] : parentB[i];
    }
    return offspring;
}

void performMutation(vector<int>& genes) {
    for (int &gene : genes) {
        if ((randomGen() % 100) / 100.0 < mutationChance) {
            gene = 1 - gene;
        }
    }
}

int main() {
    cin >> itemCount >> maxWeight;
    itemList.resize(itemCount);
    for (int i = 0; i < itemCount; ++i) {
        cin >> itemList[i].value >> itemList[i].weight;
    }
    
    auto start = high_resolution_clock::now();
    
    vector<Individual> population(populationSize);
    for (auto &individual : population) {
        individual.genes = generateRandomGeneSet();
        individual.fitnessScore = calculateFitness(individual.genes);
    }
    
    for (int generation = 0; generation < numGenerations; ++generation) {
        sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
            return a.fitnessScore > b.fitnessScore; 
        });
        
        vector<Individual> nextGeneration;
        for (int i = 0; i < populationSize / 2; ++i) {
            auto newGenes = performCrossover(population[i].genes, population[i + 1].genes);
            performMutation(newGenes);
            nextGeneration.push_back({newGenes, calculateFitness(newGenes)});
        }

        population.insert(population.end(), nextGeneration.begin(), nextGeneration.end());
        
        sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
            return a.fitnessScore > b.fitnessScore; 
        });
        
        population.resize(populationSize);
    }
    
    auto end = high_resolution_clock::now();
    duration<double> durationElapsed = end - start;

    cout << "Best fitness score: " << population.front().fitnessScore << endl;
    cout << "Elapsed time: " << durationElapsed.count() << " seconds" << endl;
    
    return 0;
}
