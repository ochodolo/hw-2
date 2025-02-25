#ifndef ARMANIDS_H
#define ARMANIDS_H

#include "datastore.h"
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <map>

class ArmaniDS : public DataStore{
  public:
  ArmaniDS();
  virtual ~ArmaniDS();

  virtual void addProduct(Product* p); //add product to store 

  virtual void addUser(User* u); //add User

  //search according to keywords
  virtual std::vector<Product*> search(std::vector<std::string>& terms, int type);

  virtual void dump(std::ostream& ofile); //dump current database

  //cart
  void addToCart(const std::string& username, int hitIdx, const std::vector<Product*>& hits);
  void viewCart(const std::string& username);
  void buyCart(const std::string& username);

  private:
  //products and users
  std::vector<User*> users_;
  std::vector<Product*> products_;
  
  //map keyword to the sets of products containing int
  std::map<std::string, std::set<Product*>> keywordToProducts_;

  //update keyword mapping for a product
  void updateKeywordMap(Product* p);
};

#endif