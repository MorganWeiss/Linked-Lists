//
//  main.cpp
//  Data Structure - LinkedList
//
//  Created by Morgan Weiss on 7/24/2018
//  Copyright © 2018 Morgan Weiss. All rights reserved.
//


#include <iostream>
#include <iterator>
#include <memory>
#include <utility>
#include <stdexcept>
#include <iosfwd>
#include <type_traits>
#include <ostream>
#include "SingleLinkedList.h"
#include "DoubleLinkedList.h"

int main(int argc, const char * argv[]) {


	///////////////////////////////////////////////////////////////////////
	///////////////////////////// Single Linked List //////////////////////
	///////////////////////////////////////////////////////////////////////

	SingleLinkedList<int> list1;
	list1.emplace_back(2);
	list1.emplace_back(4);
	list1.emplace_back(6);
	list1.emplace_back(8);
	list1.emplace_back(10);
	std::cout << list1 << "\n";
	

	std::cout<<"\n--------------------------------------------------\n";
	std::cout<<"----------------Inserting At Start----------------";
	std::cout<<"\n--------------------------------------------------\n";
	list1.push_front(50);
	std::cout << list1 << "\n";

	std::cout<<"\n--------------------------------------------------\n";
	std::cout<<"-------------inserting at particular--------------";
	std::cout<<"\n--------------------------------------------------\n";
	list1.insert_after(list1.cbegin(),60);
	std::cout << list1 << "\n";

	std::cout << "\n--------------------------------------------------\n";
	std::cout << "-------------Get current size ---=--------------------";
	std::cout << "\n--------------------------------------------------\n";
	std::cout << list1.size() << "\n";
	
	std::cout<<"\n--------------------------------------------------\n";
	std::cout<<"----------------deleting at start-----------------";
	std::cout<<"\n--------------------------------------------------\n";
	list1.pop_front();
	std::cout << list1 << "\n";
	
	std::cout<<"\n--------------------------------------------------\n";
	std::cout<<"----------------deleting at end-----------------------";
	std::cout<<"\n--------------------------------------------------\n";
	list1.pop_back();
	std::cout << list1 << "\n";
	
	
	std::cout<<"\n----------------------------------------------------------\n";
	std::cout<<"--------------Deleting after particular position--------------";
	std::cout<<"\n-----------------------------------------------------------\n";
	list1.erase_after(list1.cend());
	std::cout << list1 << "\n";
	
	 list1.search(8) ? printf("yes"):printf("no");

	 std::cout << "\n--------------------------------------------------\n";
	 std::cout << "--------------Testing copy----------------------------";
	 std::cout << "\n--------------------------------------------------\n";
	 SingleLinkedList<int> list11 = list1;
	 std::cout << list11 << "\n";

	 ///////////////////////////////////////////////////////////////////////
	 ///////////////////////////// Double Linked List //////////////////////
	 ///////////////////////////////////////////////////////////////////////
	 DoubleLinkedList<int> list2;
	 list2.push_back(2);
	 list2.push_back(4);
	 list2.push_back(6);
	 list2.push_back(8);
	 list2.push_back(10);
	 std::cout<<"\n--------------------------------------------------\n";
	 std::cout<<"---------------displaying all nodes---------------";
	 std::cout<<"\n--------------------------------------------------\n";
	 std::cout << list2 << "\n";

	 std::cout<<"\n--------------------------------------------------\n";
	 std::cout<<"----------------Inserting At Start----------------";
	 std::cout<<"\n--------------------------------------------------\n";
	 list2.push_front(1);
	 std::cout << list1 << "\n";

	 std::cout << "\n--------------------------------------------------\n";
	 std::cout << "-------------Get current size ---=--------------------";
	 std::cout << "\n--------------------------------------------------\n";
	 std::cout << list2.size() << "\n";

	 std::cout<<"\n--------------------------------------------------\n";
	 std::cout<<"----------------deleting at start-----------------";
	 std::cout<<"\n--------------------------------------------------\n";
	 list2.pop_front();
	 std::cout << list2 << "\n";

	 std::cout<<"\n--------------------------------------------------\n";
	 std::cout<<"----------------deleting at end-----------------------";
	 std::cout<<"\n--------------------------------------------------\n";
	 list2.pop_back();
	 std::cout << list1 << "\n";

	 std::cout<<"\n--------------------------------------------------\n";
	 std::cout<<"-------------inserting at particular--------------";
	 std::cout<<"\n--------------------------------------------------\n";
	 list2.insert(list2.cend(),60);
	 std::cout << list1 << "\n";

	 std::cout<<"\n----------------------------------------------------------\n";
	 std::cout<<"--------------Deleting after particular position--------------";
	 std::cout<<"\n-----------------------------------------------------------\n";
	 list2.erase(list2.cend());
	 std::cout << list1 << "\n";

	  list2.search(8) ? printf("yes"):printf("no");

	  std::cout << "\n--------------------------------------------------\n";
	  std::cout << "--------------Testing copy----------------------------";
	  std::cout << "\n--------------------------------------------------\n";
	  DoubleLinkedList<int> list22 = list2;
	  std::cout << list22 << "\n";

	std::cin.get();
}