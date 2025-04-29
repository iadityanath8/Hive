#include "../include/hive.hpp"
#include "../include/iterutils.hpp"

using namespace hive;



#include <cstring>
#include <cstdio>
#include <cstdlib>

class string {
    char* data;

public:
    string() : data(nullptr) {
        
    }

    string(const char* str) {
        
        if (str) {
            size_t len = strlen(str);
            data = (char*)calloc(len + 1,sizeof(char));
            if (!data) abort();
            memcpy(data, str, len + 1);
        } else {
            data = nullptr;
        }
    }

    string(const string& other) {
        if (other.data) {
            size_t len = strlen(other.data);
            data = (char*)calloc(len + 1,sizeof(char));
            if (!data) abort();
            memcpy(data, other.data, len + 1);
        } else {
            data = nullptr;
        }
    }

    string(string&& other) noexcept {
        
        data = other.data;
        other.data = nullptr;
    }

    string& operator=(const string& other) {
        
        if (this != &other) {
            if (data) free(data);
            if (other.data) {
                size_t len = strlen(other.data);
                data = (char*)calloc(len + 1,sizeof(char));
                if (!data) abort();
                memcpy(data, other.data, len + 1);
            } else {
                data = nullptr;
            }
        }
        return *this;
    }

    string& operator=(string&& other) noexcept {
        
        if (this != &other) {
            if (data) free(data);
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }

    bool operator==(const string& other) const {
        if (!data && !other.data) return true;
        if (!data || !other.data) return false;
        return strcmp(data, other.data) == 0;
    }

    const char* c_str() const {
        return data ? data : "";
    }

    ~string() {
        
        free(data);
    }
};



int main() {
	List<List<string>> a = {{"Meow","Meow","Meow"},{"saree"}};
	println(a[0].data());
	return 0;
}