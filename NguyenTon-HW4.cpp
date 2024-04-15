#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Function implementing LRU (Least Recently Used) Page Replacement Algorithm
int lruPageReplacement(const vector<int>& pageReference, int numFrames) {
    int pageFaults = 0;
    unordered_set<int> pageTable; // I use an unordered_set to keep track of currently loaded pages.
    vector<int> frames(numFrames, -1); // I use a vector to represent memory slots, initialized with -1 to denote empty slots.
    unordered_map<int, int> pageIndexMap; 

    for (int page : pageReference) {
        if (pageTable.count(page)) {
            // If the page is already in memory, I update its position to the most recently used.
            int index = pageIndexMap[page];
            for (int i = 0; i < numFrames; i++) {
                if (frames[i] == page) {
                    frames.erase(frames.begin() + i); // I remove the page from its current position.
                    frames.push_back(page); 
                    pageIndexMap[page] = numFrames - 1; 
                    break;
                }
            }
        }
        else if (pageTable.size() < numFrames) {
            // If the page is not in memory and there is space available, I add the page.
            pageTable.insert(page); 
            int index = pageTable.size() - 1;
            frames[index] = page; 
            pageIndexMap[page] = index; 
            pageFaults++; 
        }
        else {
            // If the page is not in memory and all frames are full, I perform replacement.
            int victim = frames[0]; 
            pageTable.erase(victim); 
            pageIndexMap.erase(victim); 
            frames.erase(frames.begin());
            frames.push_back(page); 
            pageTable.insert(page); 
            pageIndexMap[page] = numFrames - 1; 
            pageFaults++; 
        }
    }

    return pageFaults; 
}

// Function implementing Optimal Page Replacement Algorithm
int optimalPageReplacement(const vector<int>& pageReference, int numFrames) {
    int pageFaults = 0;
    unordered_set<int> pageTable; 
    vector<int> frames(numFrames, -1); 
    vector<int> nextUse(pageReference.size(), -1); 

    // I determine next use distance for each page in pageReference.
    for (int i = 0; i < pageReference.size(); i++) {
        for (int j = i + 1; j < pageReference.size(); j++) {
            if (pageReference[j] == pageReference[i]) {
                nextUse[i] = j; // I store next use distance.
                break;
            }
        }
    }

    for (int i = 0; i < pageReference.size(); i++) {
        int page = pageReference[i];

        if (pageTable.count(page)) {
            continue; // Page is already in memory.
        }

        if (pageTable.size() < numFrames) {
            // If there is space available in memory, I add the page.
            pageTable.insert(page);
            int index = pageTable.size() - 1;
            frames[index] = page;
            pageFaults++; 
        }
        else {
            // If memory is full, I perform page replacement.
            int maxIndex = -1;
            int victim = -1;
            for (int frame : frames) {
                int index = find(pageReference.begin() + i + 1, pageReference.end(), frame) - pageReference.begin();
                if (index > maxIndex) {
                    maxIndex = index;
                    victim = frame;
                }
            }
            pageTable.erase(victim); 
            pageTable.insert(page); 
            auto it = find(frames.begin(), frames.end(), victim);
            *it = page; 
            pageFaults++; 
        }
    }

    return pageFaults; // I return the total number of page faults.
}

// Function implementing FIFO (First In First Out) Page Replacement Algorithm
int fifoPageReplacement(const vector<int>& pageReference, int numFrames) {
    int pageFaults = 0;
    unordered_set<int> pageTable; // I use an unordered_set to keep track of currently loaded pages.
    vector<int> frames(numFrames, -1); 
    int front = 0; 

    for (int page : pageReference) {
        if (pageTable.count(page)) {
            continue; 
        }

        if (pageTable.size() < numFrames) {
            // If there is space available in memory, I will add the page.
            pageTable.insert(page);
            int index = pageTable.size() - 1;
            frames[index] = page;
            pageFaults++; // Page fault count is incremented .
        }
        else {
            // If the memory is full, I show page replacement using FIFO.
            int victim = frames[front]; // I get the oldest page (front of frames).
            pageTable.erase(victim); 
            pageTable.insert(page); 
            frames[front] = page; 
            front = (front + 1) % numFrames; 
            pageFaults++; 
        }
    }

    return pageFaults; 
}

int main() {
    // You can change the number below to see the faults.
    vector<int> pageReference = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int numFrames = 4;

    cout << "For LRU Algorithm:" << endl;
    int lruFaults = lruPageReplacement(pageReference, numFrames);
    cout << "Total Page Faults: " << lruFaults << endl << endl;

    cout << "For Optimal Algorithm:" << endl;
    int optimalFaults = optimalPageReplacement(pageReference, numFrames);
    cout << "Total Page Faults: " << optimalFaults << endl << endl;

    cout << "For FIFO Algorithm:" << endl;
    int fifoFaults = fifoPageReplacement(pageReference, numFrames);
    cout << "Total Page Faults: " << fifoFaults << endl;

    return 0;
}
