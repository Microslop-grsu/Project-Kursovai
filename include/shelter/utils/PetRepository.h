#ifndef SHELTERCORE_PETREPOSITORY_H
#define SHELTERCORE_PETREPOSITORY_H
#include "../core/Pet.h"
#include <memory>
#include <vector>
#include <string>


class PetRepository {
private:
    std::vector<std::unique_ptr<Pet>> pets;

public:
    void add(std::unique_ptr<Pet> pet);
    bool remove(short id);
    const std::vector<std::unique_ptr<Pet>>& getAll() const;

    void clear();
    void addRaw(Pet* pet);
    size_t size() const {return pets.size();};
    bool isEmpty() const {return pets.empty();};

    struct SearchCriteria {
        std::string name;
        std::string type;
        int min_age = -1;
        int max_age = -1;
        int min_weight = -1;
        int max_weight = -1;
    };

    std::vector<Pet*> findByCriteria(const SearchCriteria& crit);
    Pet* findByID(short id);
};




#endif //SHELTERCORE_PETREPOSITORY_H
