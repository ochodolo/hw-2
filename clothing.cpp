#include "clothing.h"
#include "util.h"
#include <sstream>

Clothing::Clothing(const std::string &name, double price, int qty, const std::string &brand, const std::string &size) : Product("clothing", name, price, qty), brand_(brand), size_(size){

}

Clothing::~Clothing() {
  
}

std::set<std::string> Clothing::keywords() const{
  //keywords from name
  std::set<std::string> result = parseStringToWords(name_);
  //keywords from brand
  std::set<std::string> bKeywords = parseStringToWords(brand_);
  //combine name & brand
  for(const std::string &word :bKeywords){
    result.insert(word);
  }

  return result;
}

std::string Clothing::displayString() const {
  std::stringstream s;

  s << name_ << "\n";
  s << "Size: " << size_ << " Brand: " << brand_ << "\n";
  s << price_ << " " << qty_ << " left.";
  return s.str();
}

void Clothing::dump(std::ostream &os) const {
  //dumps data
  os << "clothing" << "\n" << name_ << "\n" << price_ << "\n";
  os << qty_ << "\n" << size_ << "\n" << brand_ << "\n";
}