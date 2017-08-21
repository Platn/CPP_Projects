#ifndef marketH
#define marketH

#include "dsexceptions.h"
#include "MarketDriver.h"
#include <cstddef>
#include <iostream>
#include <cstring>

class Offer;

class InputOffer
{
	
	public:
	InputOffer() : price(-1), shares(0), ID(0), time(0){}
	InputOffer(const Offer &offer) : price(offer.price), shares(offer.shares), ID(offer.ID), time(offer.time){}
	double price;
	int shares;
	int ID;
	int time;
};


class BinaryHeapPQ
{
  void buildHeap()
  {
    for(int i = currentSize / 2; i > 0; i--)
      percolateDown(i);
  }
  
  void percolateDown(int hole)
  {
    //int t, int I, double pr, int shar, const char *sym, char typ
    InputOffer tempOffer(offersHold[hole]);
    for(int child; hole * 2 <= currentSize; hole = child)
    {
      child = hole * 2;
      if(child != currentSize && offersHold[child+1].price > offersHold[child].price)
      {
		  child++;
	  }
		 
      else if(child != currentSize && offersHold[child].price == offersHold[child+1].price)
      {
		if(offersHold[child].time > offersHold[child + 1].time)
		{
			child++;
		}
	  }
		 
	  if(offersHold[child].price > tempOffer.price)
	  {
			 offersHold[hole] = offersHold[child];
	  }
	  
	  else if(offersHold[child].price == tempOffer.price && tempOffer.time > offersHold[child].time)
	  {
		  offersHold[hole] = offersHold[child];
	  }
      
      else
        break;
    }
    
    offersHold[hole] = tempOffer;
  }
  
public:
  int currentSize;
  InputOffer *offersHold;
  BinaryHeapPQ()
  {
    currentSize = 0;
    offersHold = new InputOffer[1000];
  }
  
  ~BinaryHeapPQ();
  
  InputOffer * findMax() const
  {
    
    if(isEmpty())
    {
      // cout << "Overflow in BHeap" << endl;
      return NULL;
    }
    // cout << "Buyers Max: " << endl;
    for(int i = 0; i <= currentSize; i++)
    {
      if(offersHold[i].price == -1)
      {
        continue;
      }
//      // cout << "i: " << i << endl;
      // cout << offersHold[i].price << endl;
    }
    // cout << "Time: " << offersHold[1].time << endl << endl;
    return &offersHold[1];
  }
  
  bool isFull() const
  {
    return currentSize == 1000;
  }
  bool isEmpty() const
  {
    return currentSize == 0;
  }
  
  void deleteMax()
  {
    {
      if(isEmpty())
      {
        // cout << "Overflow" << endl;
        return;
      }
      offersHold[1] = offersHold[currentSize];
      currentSize--;
      percolateDown( 1 );
    }
  }
  void deleteMax(InputOffer & inputOffer)
  {
    {
      if(isEmpty())
        throw Underflow();
      
      //maxItem = offersHold[0];
      offersHold[1] = offersHold[currentSize--];
      percolateDown( 1 );
    }
  }
  
  void deleteNode(InputOffer x)
  {
	  int i = 0;
	  for(; i < currentSize; ++i)
	  {
		  if(&offersHold[i] == &x)
		  {
			  break;
		  } 
	  }
	  
	  offersHold[i] = offersHold[currentSize];
	  currentSize--;
	  percolateDown(i);
  }
  
 double findNext()
  {
	if(currentSize == 1)
		return -1;
	if(currentSize == 2)
		return offersHold[2].price;
	if(currentSize > 2)
	{
		if(offersHold[2].price >= offersHold[3].price)
		{
			return offersHold[2].price;
		}
		
		else
		{
			return offersHold[3].price;
		}
	}
	
	return -1;
  }
  
  void insert(InputOffer &inputOffer)
  { 
    if(isFull())
    {
      // cout << "BHeap OverFlow" << endl;
      throw Overflow();
    }
    int hole = ++currentSize;
    // cout << currentSize << endl;
    // cout << "Price Check: " << offersHold[hole / 2].price << endl;
    for(; hole > 1 && inputOffer.price >= offersHold[hole / 2].price; hole /= 2)
    {
		if(inputOffer.price == offersHold[hole / 2].price)
		{
			if(inputOffer.time < offersHold[hole / 2].time)
			{
				offersHold[hole] = offersHold[hole / 2];
			}
			
			else if(inputOffer.time > offersHold[ hole / 2].time)
			{
				break;
			}
		}
		else
			offersHold[hole] = offersHold[hole / 2];
    }
    
    offersHold[hole] = inputOffer;
    //Might need to add this in here
  }
};

class CompanyStock
{
  
public:
  CompanyStock()
  {
		sellerPosition = 0;
    BuyersHold = new BinaryHeapPQ();
    SellersHold = new InputOffer*[1000000];
  }
  
  CompanyStock(const Offer &offer)
  {
		sellerPosition = 0;
    BuyersHold = new BinaryHeapPQ();
    SellersHold = new InputOffer*[1000000];
  }
  
  int sellerPosition;
  char name[7];
  BinaryHeapPQ *BuyersHold;
  InputOffer **SellersHold;
  
  void sellerHoldInsert(InputOffer &offer)
  {
	  if(sellerPosition < 1000000)
		{
			SellersHold[sellerPosition] = &offer;
			sellerPosition++;
		}
    
    else if(sellerPosition == 1000000)
    {
      sellerPosition = 0;
      while(SellersHold[sellerPosition]->shares != -1)
      {
        sellerPosition++;
      }
      SellersHold[sellerPosition] = &offer;
    }
	
    else
		{
   	 // cout << "Seller Insert Overflow" << endl;
			throw Overflow();
		}
  }
  
  int findMinSeller()
  {
	  int tempReturn = 0;
	  //~ // cout << "E: " << sellerPosition << endl;
    // cout << "Stock Seller List: " << endl;
	  for(int j = 0; j < sellerPosition; j++)
	  {
		  //~ // cout << "Error!" << endl;
		  
		  if(SellersHold[j] != NULL && SellersHold[j]->price == SellersHold[tempReturn]->price)
		  {
			  if(SellersHold[j]->time < SellersHold[tempReturn]->time)
					continue;
		  }
		  
		  if(SellersHold[j] != NULL && SellersHold[j]->price < SellersHold[tempReturn]->price)
		  {
        
        // cout << SellersHold[j]->price << endl;
			  tempReturn = j;
		  }
		  
	  }
	  
    // cout << endl << endl;
	  
	  //~ // cout << "Error2!" << endl;
	  //~ // cout << "Temp: " << tempReturn << endl;
	  // cout << "SellersHold Return Price: " << SellersHold[tempReturn]->price << endl;
    
//    for(int j = 1; j < sellerPosition; j++)
//    {
//      if(SellersHold[j] != NULL)
//      	// cout << SellersHold[j]->price << endl;
//    }
//    
	  return tempReturn;
  } // If we have time, we can just have this return the index instead and then use that to output again.
  
  int findClosest(double buyerPrice)
  {
	  double tempReturn = -1;
    int holder = -1;
	  //~ // cout << "E: " << sellerPosition << endl;
    // cout << "Find closest to: " << buyerPrice << endl;
    // cout << "Stock Seller List: " << endl;
	  for(int j = 0; j < sellerPosition; j++)
	  {
      if(SellersHold[j] == NULL || SellersHold[j]->shares == 0) // Check to see if index is NULL, if so, move on
      {
        continue;
      }
      
      if(SellersHold[j]->price == buyerPrice) // What happens if we reset the array? Do we ever? Probably not
      {
        // cout << "J: " << endl;
        return j;
      }
      
//      if(SellersHold[j] != NULL)
//      {
//        // cout << "Not NULL: ";
//        // cout << SellersHold[j]->price << endl;
//      }
      
      if(SellersHold[j]->price > tempReturn && SellersHold[j]->price < buyerPrice) // Check Price
      {
        
        
        // cout << "Change Temp: " << SellersHold[j]->price << endl;
        tempReturn = SellersHold[j]->price;
        holder = j;
      }
		  //~ // cout << "Error!" << endl;
		  
	  }
    
    // cout << "Temp: " << tempReturn << endl << endl;
    // cout << "SellerR: " << tempReturn << endl << endl;
    if(tempReturn > buyerPrice)
    {
      // cout << "Return -1" << endl << endl;
      return -1;
    }
    
    return holder;
  } // If we have time change this to return the index and use that instead
  
//  void deleteSeller(InputOffer &inputOffer)
//  {
//  	
//	  inputOffer.price = -1;
//	  inputOffer.shares = 0;
//	  inputOffer.ID = 2000000;
//	  inputOffer.time = 2000000;
//	 
//  }
  
};

class Market
{
  int arrayIndex;
  CompanyStock **companyNames;
  
public:
  Market(int numStocks, int offerCount, int IDs);
  int time;
  int numStocks;
  
  int currentStock;
  int hashFunction(const Offer &offer);
  void newOffer(const Offer &offer);
  bool newTransaction(Transaction *transaction);
}; // class Market




#endif
