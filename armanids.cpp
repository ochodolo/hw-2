#include "armanids.h"
#include "util.h"
#include <fstream>
#include <iostream>

ArmaniDS::ArmaniDS(){

}

ArmaniDS::~ArmaniDS(){
  //Delete users and products
  for(size_t i = 0; i < users_.size(); ++i){
    delete users_[i];
  }

  for(size_t i = 0; i < products_.size(); ++i){
    delete products_[i];
  }
}

void ArmaniDS::addProduct(Product* p){
  products_.push_back(p);
  updateKeywordMap(p);
}

void ArmaniDS::addUser(User* u) {
  users_.push_back(u);
}

void ArmaniDS::updateKeywordMap(Product*p){
  std::set<std::string> keys = p->keywords(); //get keywords
  //insert product into mapping
  for(std::set<std::string>::iterator it = keys.begin(); it != keys.end(); ++it) {
    //convert to lowercase
    std::string key = *it;
    key = convToLower(key);
    keywordToProducts_[key].insert(p);
  }
}

std::vector<Product*> ArmaniDS::search(std::vector<std::string>& terms, int type){
  std::set<Product*> finalList;

  //if empty return empty
  if(terms.empty()) {
    return std::vector<Product*>();
  }

  //vector of sets to hold results
  std::vector<std::set<Product*>> setResults;

  for(size_t i = 0; i < terms.size(); i++){
    std::string term = convToLower(terms[i]);
    //look up term
    if(keywordToProducts_.find(term) != keywordToProducts_.end()){
      setResults.push_back(keywordToProducts_[term]);
    } else{
      //return empty if not found
      if(type == 0){
        return std::vector<Product*>();
      }
    }
  }
  if(setResults.empty()){
    return std::vector<Product*>();
  }

  //combine search types
  if(type == 0) {
    finalList = setResults[0];
    for(size_t i = 1; i < setResults.size(); i++){
      finalList = setIntersection(finalList, setResults[i]);
    }
  } else {
    finalList = setResults[0];
    for(size_t i = 1; i < setResults.size(); i++){
      finalList = setUnion(finalList, setResults[i]);
    }
  }

  //convert to vector
  std::vector<Product*> results(finalList.begin(), finalList.end());
  return results;
}

void ArmaniDS::dump(std::ostream& ofile) {
  ofile << "<products>\n";
  for(size_t i = 0; i < products_.size(); ++i){
    products_[i]->dump(ofile);
  }
  ofile << "</products>\n";
  ofile << "<users>\n";
  for(size_t i = 0; i < users_.size(); ++i){
    users_[i]->dump(ofile);
  }
  ofile << "</users>\n";
}

//cart functions

void ArmaniDS::viewCart(const std::string& username) {
  User* targetUser = 0;
  std::string userLower = convToLower(username);
  for(size_t i = 0; i < users_.size(); ++i){
    if(convToLower(users_[i]->getName()) == userLower) {
      targetUser = users_[i];
      break;
    }
  }
  if(targetUser == 0){
    std::cout << "Invalid username" << std::endl;
    return;
  }
  std::vector<Product*>& cart = targetUser->getCart();
  if(cart.empty()) {
    std::cout << "Cart is empty." << std::endl;
  }
  for(size_t i = 0; i <cart.size(); i++) {
    std::cout << "Item " << (i + 1) << "\n" << cart[i]->displayString() << "\n\n";
  }
}

void ArmaniDS::buyCart(const std::string& username){
  User* targetUser = 0;
  std::string userLower = convToLower(username);
  for(size_t i = 0; i < users_.size(); ++i){
    if(convToLower(users_[i]->getName()) == userLower){
      targetUser = users_[i];
      break;
    }
  }
  if(targetUser == 0){
    std::cout << "Invalid username" << std::endl;
    return;
  }
  std::vector<Product*>& cart = targetUser->getCart();
  std::vector<Product*> newCart;

  for(size_t i = 0; i < cart.size(); ++i){
    Product* prod = cart[i];
    double price = prod->getPrice();
    int qty = prod->getQty();

    if(qty > 0 && targetUser->getBalance() >= price) {
      prod->subtractQty(1);
      targetUser->deductAmount(price);
    } else {
      newCart.push_back(prod);
    }
  }

  targetUser->setCart(newCart);
}

void ArmaniDS::addToCart(const std::string& username, int hitIdx, const std::vector<Product*>& hits) {
  User* targetUser = 0;
  std::string userLower = convToLower(username);
  for(size_t i = 0; i < users_.size(); ++i){
    if(convToLower(users_[i]->getName()) == userLower){
      targetUser = users_[i];
      break;
    }
  } 
  if(targetUser == 0) {
    std::cout << "Invalid request" << std::endl;
    return;
  }
  if(hitIdx < 1 || hitIdx > static_cast<int>(hits.size())) {
    std::cout << "Invalid request" << std::endl;
    std::fflush(stdout);
    return;
  }
  targetUser->addToCart(hits[hitIdx - 1]);
  std::cout << "Product added to cart." << std::endl;
}