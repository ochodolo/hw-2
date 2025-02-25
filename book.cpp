#include "book.h"
#include "util.h"
#include <sstream>

Book::Book(const std::string &name, double price, int qty, const std::string &isbn, const std::string &author) : Product("movie", name, price, qty), isbn_(isbn), author_(author){

}

Book::~Book() {
  
}

std::set<std::string> Book::keywords() const{
  //get author and name keywords
  std::set<std::string> results = parseStringToWords(name_);
  std::set<std::string> authorKeywords = parseStringToWords(author_);
  //add keywords
  for(const std::string &word : authorKeywords){
    results.insert(word);
  }

  results.insert(isbn_);
  return results;
}

std::string Book::displayString() const {
  std::stringstream s;

  s << name_ << "\n";
  s << "Author: " << author_ << " ISBN: " << isbn_ << "\n";
  s << price_ << " " << qty_ << " left.";
  return s.str();
}

void Book::dump(std::ostream &os) const {
  //dumps data
  os << "book" << "\n" << name_ << "\n" << price_ << "\n";
  os << qty_ << "\n" << isbn_ << "\n" << author_ << "\n";
}