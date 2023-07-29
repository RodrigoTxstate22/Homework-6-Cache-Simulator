#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <iomanip>
// Cache Entry Structure
struct CacheEntry {
    bool valid;
    int tag;

    CacheEntry() : valid(false), tag(-1) {}
};

// Cache Class
class Cache {
private:
    int cacheSize;
    int memSize;
    int blockSize;
    int numCacheEntries;
    int numMemBlocks;
    std::vector<CacheEntry> cache;
    std::unordered_set<int> cacheTags;

public:
    Cache(int cacheSize, int memSize, int blockSize) : cacheSize(cacheSize), memSize(memSize), blockSize(blockSize) {
        numCacheEntries = cacheSize / blockSize;
        numMemBlocks = memSize / blockSize;
        cache.resize(numCacheEntries);
    }

    // Function to check if the address is in cache
    bool isHit(int address) {
        int tag = address / blockSize;
        int index = tag % numCacheEntries;

        if (cache[index].valid && cache[index].tag == tag) {
            return true; // Cache Hit
        }

        return false; // Cache Miss
    }

    // Function to handle a memory access
    void accessMemory(int address) {
        int tag = address / blockSize;
        int index = tag % numCacheEntries;

        if (cache[index].valid && cache[index].tag == tag) {
            std::cout << address << " Hit\n";
        } else {
            cache[index].valid = true;
            cache[index].tag = tag;
            cacheTags.insert(tag);
            std::cout << address << " Miss\n";
        }
    }

    // Function to calculate and print hit/miss percentages
    void printStatistics() {
        int totalAccesses = cacheTags.size();
        int totalHits = totalAccesses - cacheTags.size();
        float hitRate = (static_cast<float>(totalHits) / totalAccesses) * 100;
        float missRate = 100 - hitRate;

        std::cout << "Out of " << totalAccesses << " memory accesses, there was a ";
        std::cout << std::setprecision(2) << std::fixed << hitRate << "% cache hit rate and a ";
        std::cout << missRate << "% miss rate.\n";
    }
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " mem_size cache_size access_filename\n";
        return 1;
    }

    int memSize = std::stoi(argv[1]);
    int cacheSize = std::stoi(argv[2]);
    std::string accessFilename = argv[3];

    if (memSize % 2 != 0 || cacheSize % 2 != 0 || cacheSize > memSize) {
        std::cerr << "Invalid cache and memory sizes. Both sizes must be divisible by 2, and cache size must be smaller than or equal to memory size.\n";
        return 1;
    }

    std::ifstream accessFile(accessFilename);
    if (!accessFile) {
        std::cerr << "Error opening access file: " << accessFilename << "\n";
        return 1;
    }

    Cache cache(cacheSize, memSize, 1);

    int address;
    while (accessFile >> address) {
        cache.accessMemory(address);
    }

    accessFile.close();

    std::ofstream outputFile("hitmiss-out.txt");
    std::streambuf* coutBuf = std::cout.rdbuf(); // Save old cout buffer
    std::cout.rdbuf(outputFile.rdbuf()); // Redirect cout to output file
    cache.printStatistics();
    std::cout.rdbuf(coutBuf); // Restore old cout buffer

    outputFile.close();

    return 0;

}



