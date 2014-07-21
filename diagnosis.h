#ifndef DIAGNOSIS_H_
#define DIAGNOSIS_H_

#include <vector>
#include <string>
class Diagnosis {
public:
    static size_t whoUseMe(std::vector<std::string>& appList, std::string const& filePath);

};
#endif