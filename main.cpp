#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
#include <iomanip>

class WebAddressMapper {
private:
    std::vector<std::string> map;
public:
    std::vector<std::string> from_pages;
    std::vector<std::string> to_pages;
    void addAddress(std::string& address) {
        map.push_back(address);
    }
    void sortAlpha() {
        std::sort(map.begin(), map.end());
    }
    int getID(const std::string &address) const {
        return std::distance(map.begin(), std::find(map.begin(), map.end(), address));
    }
    std::string getAddress(unsigned int id) const {
        return map.at(id);
    }
    const unsigned int getNumAddresses() const {
        return map.size();
    }
    void printMap() const {
        // for debugging purposes
        int id = 0;
        for(const std::string& address : map) {
            std::cout << id << " -> " << address << std::endl;
            ++id;
        }
    }
};

class AdjacencyMatrix {
private:
    double** matrix;
    const unsigned int size;
public:
    explicit AdjacencyMatrix(unsigned int size): size(size) {
        matrix = new double*[size];
        for(int i = 0; i < size; ++i) {
            matrix[i] = new double[size];
        }
        for(int i = 0; i < size; ++i) {
            for(int j = 0; j < size; ++j) {
                matrix[i][j] = 0;
            }
        }
    }
    ~AdjacencyMatrix() {
        for(int i = 0; i < size; ++i) {
            delete matrix[i];
        }
        delete[] matrix;
    }
    void set(int i, int j, double value) {
        matrix[i][j] = value;
    }
    const double& get(int i, int j) const {
        return matrix[i][j];
    }
    const unsigned int& getSize() const {
        return size;
    }
    void printGraph() const {
        // for debugging purposes
        for(int i = 0; i < size; ++i) {
            for(int j = 0; j < size; ++j) {
                std::cout << matrix[i][j] << '\t';
            }
            std::cout << '\n';
        }
        std::cout << std::endl;
    }
};

int getInput(WebAddressMapper& mapper) {
    std::string line;
    std::string from_page;
    std::string to_page;
    std::vector<std::string> input;
    std::set<std::string> addresses;
    while(std::getline(std::cin, line)) {
        input.push_back(line);
    }
    std::size_t split_index;
    for(unsigned int i = 1; i < input.size(); ++i) {
        split_index = input.at(i).find(' ');
        from_page = input.at(i).substr(0, split_index);
        to_page = input.at(i).substr(split_index + 1);
        addresses.insert(from_page);
        addresses.insert(to_page);
        mapper.from_pages.push_back(from_page);
        mapper.to_pages.push_back(to_page);
    }
    for(std::string address : addresses) {
        mapper.addAddress(address);
    }
    mapper.sortAlpha();
    split_index = input.at(0).find(' ');
    int num_lines = std::stoi(input.at(0).substr(0, split_index));
    if(num_lines < 0) {
        exit(-1); // makes sure number of lines is positive, or else exit
    }
    int power_count = std::stoi(input.at(0).substr(split_index + 1));
    if(power_count < 1) {
        exit(-1); // number of power iterations must be at least 1
    }
    return power_count;
}

void fillGraph(AdjacencyMatrix& graph, WebAddressMapper& mapper) {
    for(unsigned int i = 0; i < mapper.from_pages.size(); ++i) {
        std::string from_page = mapper.from_pages.at(i);
        std::string to_page = mapper.to_pages.at(i);
        int col = mapper.getID(from_page);
        int row = mapper.getID(to_page);
        int out_degree = std::count(mapper.from_pages.begin(), mapper.from_pages.end(), from_page);
        double value;
        if(out_degree == 0) {
            value = 0; // not sure exactly what this should be if the outdegree of a website is 0
        }
        else {
            value = 1.0/out_degree;
        }
        graph.set(row, col, value);
    }
}

double multiply(const AdjacencyMatrix &graph, const std::vector<double> &r, int row) {
    double sum = 0;
    for(unsigned int col = 0; col < graph.getSize(); ++col) {
        sum += graph.get(row,col) * r.at(col);
    }
    return sum;
}

std::vector<double> powerIterate(const AdjacencyMatrix& graph, const int& num_steps) {
    std::vector<double> r_old (graph.getSize());
    std::vector<double> r_new (graph.getSize());
    std::fill(r_old.begin(), r_old.end(), (1.0/(graph.getSize())));
    if(num_steps == 1) {
        return r_old;
    }
    for(int i = 1; i <= num_steps - 1; ++i) {
        for(int j = 0; j < r_old.size(); ++j) {
            r_new[j] = multiply(graph, r_old, j);
        }
        r_old = r_new;
    }
    return r_new;
}

void printPageRanks(const WebAddressMapper& mapper, const std::vector<double>& page_ranks) {
    for(unsigned int i = 0; i < page_ranks.size(); ++i) {
        std::cout << mapper.getAddress(i) << ' ' << std::fixed << std::setprecision(2) << page_ranks.at(i);
        if(i != page_ranks.size() - 1) {
            std::cout << std::endl;
        }
    }
}

int main() {
    WebAddressMapper mapper;
    const int num_power_iterations = getInput(mapper);
    AdjacencyMatrix graph(mapper.getNumAddresses());
    fillGraph(graph, mapper);
    std::vector<double> page_ranks = powerIterate(graph, num_power_iterations);
    printPageRanks(mapper, page_ranks);
    return 0;
}
