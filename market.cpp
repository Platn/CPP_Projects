#include "market.h"
#include "MarketDriver.h"
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

Market::Market(int numStocks, int offerCount, int IDs)
{
  
  this->numStocks = numStocks;
  time = 0;
  arrayIndex = offerCount;
  companyNames = new CompanyStock*[numStocks]; // Set the pointer equal to this array of stocks // This ideally should be holding all of them
  for(int i = 0; i < numStocks; i++)
  {
    companyNames[i] = NULL;
    // cout << "NULL " << i << ": " << (companyNames)[i] << endl;
  }
  // Uncomment the above if all of them are not set to NULL by default
  
  //  companyNames = new CompanyStock*[numStocks];
  
} // Market()


int Market::hashFunction(const Offer &offer)
{
  // cout << "Offer Symbol: " << offer.symbol << endl;
  int asciiValue = 0;
  
  for(int i = 0; i < 7; i++)
  {
    asciiValue += (int)offer.symbol[i];
  }
  asciiValue = (asciiValue % numStocks);
  
  
  int i = 1;
  int store = asciiValue;
  // cout << "Ascii: " << asciiValue << endl;
  // cout << "Current Store: " << store << endl;
  
  if(companyNames[store] == NULL)
  {
    CompanyStock *newStock = new CompanyStock();
    strcpy(newStock->name, offer.symbol);
    companyNames[store] = newStock;
    // cout << "NewStockNULL: " << newStock->name << endl;
    // cout << "companyStockNULL: " << companyNames[store]->name << endl;
    // cout << "StockPosition: " << store << endl;
    if(offer.type == 'B')
    {
      // cout << "Inside of B1: " << endl;
      InputOffer *tempOffer = new InputOffer(offer);
      companyNames[store]->BuyersHold->insert(*tempOffer);
      return store;
    }
    
    else
    {
      // cout << "Inside of S1: " << endl;
      InputOffer *tempOffer = new InputOffer(offer);
      companyNames[store]->sellerHoldInsert(*tempOffer); // Insert here
      companyNames[store]->sellerPosition++;
      return store;
    }

    return store; // Anywhere that a new stock is inserted then we have to go in and plug in stuff
  }
  
  int rotation = 0;
  //~ // cout << "Store Check2: " << store << endl;
  
  while(companyNames[store] != NULL)
  {
    rotation++;
    
//    // cout << "Rotation: " << rotation << endl;
    if(strcmp((companyNames)[store]->name,offer.symbol) == 0) // If the two strings compare to each other
    { // Insert into either binary queue or seller queue
//      // cout << strcmp(companyNames[store]->name,offer.symbol) << endl;
      if(offer.type == 'B')
      {
        // cout << "Inside of B2: " << endl;
        InputOffer *tempOffer = new InputOffer(offer);
        companyNames[store]->BuyersHold->insert(*tempOffer);
      }
      
      else
      {
        // cout << "Inside of S2: " << endl;
        InputOffer *tempOffer = new InputOffer(offer);
       	companyNames[store]->sellerHoldInsert(*tempOffer); // Insert here
       	companyNames[store]->sellerPosition++;
      }
      //~ // cout << "Strings are the same." << endl;
//      // cout << "CurrentStock: " << companyNames[store]->name << endl;
//      // cout << "StockPosition: " << store << endl;
      return store;
    }
    
    store = ( asciiValue + (i*i) ) % numStocks;
//    // cout << "Store Check: " << store << endl;
    
    //~ // cout << "Store After Rotation: " << store << endl;
    i++;
  }
  
  // cout << endl;
  
  CompanyStock *newStock = new CompanyStock();
  strcpy(newStock->name, offer.symbol);
  companyNames[store] = newStock;
  // cout << "NewStock: " << newStock->name << endl;
  // cout << "companyStock: " << companyNames[store]->name << endl; // Hash should be done!
  // cout << "StockPosition: " << store << endl;
  
  if(offer.type == 'B')
  {
    // cout << "Inside of B3: " << endl;
    InputOffer *tempOffer = new InputOffer(offer);
    companyNames[store]->BuyersHold->insert(*tempOffer);
  }
  
  else
  {
    // cout << "Inside of S3: " << endl;
    InputOffer *tempOffer = new InputOffer(offer);
    companyNames[store]->sellerHoldInsert(*tempOffer); // Insert here
    companyNames[store]->sellerPosition++;
  }
  return store; // Place holder
}

/*Quadratics
 * for(int i = 0; i < 7; i++)
 {
 asciiValue += (int)offer.symbol[i];
 }
 
 asciiValue = (asciiValue % numStocks) + (i*i);
 *
 *
 *
 *
 */
//int g = 0; // This needs to be taken out, it is just used as a counter

void Market::newOffer(const Offer &offer)
{
  // cout << "G: #" << g << "#" << endl;
//  g++;
  currentStock = hashFunction(offer);
  // cout << "New Offer Time: #" << time << "#" << endl << endl;
  // cout << "Price Offer: " << offer.price << endl;
  
  this->time++;
//  if(time == 136)
//  {
//    // cout << "\nExiting Now" << endl;
//    exit(EXIT_FAILURE);
//  }
} // newOffer()


bool Market::newTransaction(Transaction *transaction)
{
	// cout << "CurrentStockT: " << companyNames[currentStock]->name << endl << endl;
//  static int count = 0;
  /* Steps for comparing stocks:
   1. Get Buyer's max
   2. Get Seller's closest to buyer max
   3. Check if max findNext() price is equal to or greater than Seller's closest
   
   Buyer >= seller
  */
  if(companyNames[currentStock] != NULL && companyNames[currentStock]->sellerPosition != 0 && companyNames[currentStock]->BuyersHold->isEmpty() != true) // Check that both are null
  {
    
    InputOffer *tempBuyer = companyNames[currentStock]->BuyersHold->findMax();
    // Here is the problem
    if(companyNames[currentStock]->findClosest(tempBuyer->price) == -1)
       return false;
       
    InputOffer *tempSeller = companyNames[currentStock]->SellersHold[companyNames[currentStock]->findClosest(tempBuyer->price)];
    
    /* First we check to see if the there is any match for closest. If there is no match in closest, then that means that the buyer price was not matched,
     * which means that if we didn't find a match then the lowest has not been fulfilled such that buyer price is greater than seller price.
     */
    
    if(tempSeller == NULL || tempSeller->price > tempBuyer->price || tempSeller->price == -1)
    {
      // cout << "False" << endl;
      return false;
    }
    
    /* In the if/else statement, it should be that the price is already matched up due to the top. If this is the case, then
     * we should have it that if the find next is greater than or equal to tempSeller, we change to the second/third index
     * If not, then we set the price as Seller Price since it has already been matched.
     */
    if(companyNames[currentStock]->BuyersHold->findNext() >= tempSeller->price)
    {
      transaction->price = companyNames[currentStock]->BuyersHold->findNext();
    }
    
    else
    {
      transaction->price = tempSeller->price;
    }
    
    /* From here, we should move onto the stocks and make it so that we can set the stock amounts */
    
    
  	// First let's set the ID and the rest of the identification
    transaction->buyerID = tempBuyer->ID;
    transaction->sellerID = tempSeller->ID;
    transaction->time = time - 1;
    strcpy(transaction->symbol,companyNames[currentStock]->name);
    
    // Now we can do the shares based on what they had before.
    
    if(tempBuyer->shares > tempSeller->shares)
    {
      tempBuyer->shares -= tempSeller->shares;
      transaction->shares = tempSeller->shares;
      
      tempSeller->shares = 0;
//      companyNames[currentStock]->deleteSeller(*tempSeller);
      tempSeller->price = -1;
      
//      if(tempBuyer->shares == 0)
//      {
//        companyNames[currentStock]->BuyersHold->deleteMax();
//      }
      
      // cout << "TempB: " << tempBuyer->price << endl;
      
      // cout << endl;
      // cout << "New Transaction BS > SS" << endl;
      // cout << "Symbol: " << transaction->symbol << endl;
      // cout << "Price " << transaction->price << endl;
      // cout << "Buyer: " << transaction->buyerID << endl;
      // cout << "Seller: " << transaction->sellerID << endl;
      // cout << "Time: " << transaction->time << endl;
      // cout << "Num Shares: " << transaction->shares << endl << endl;
    }
    
    else if(tempBuyer->shares < tempSeller->shares)
    {
      tempSeller->shares -= tempBuyer->shares;
      transaction->shares = tempBuyer->shares;
      
      
      // cout << endl;
      // cout << "New Transaction BS < SS" << endl;
      // cout << "Symbol: " << transaction->symbol << endl;
      // cout << "Price: " << transaction->price << endl;
      // cout << "Buyer: " << transaction->buyerID << endl;
      // cout << "Seller: " << transaction->sellerID << endl;
      // cout << "Time: " << transaction->time << endl;
      // cout << "Num Shares: " << transaction->shares << endl << endl;
      
//      if(tempSeller->shares == 0)
//      {
//        tempSeller->price = -1;
////        companyNames[currentStock]->deleteSeller(*tempSeller);
//      }
      companyNames[currentStock]->BuyersHold->deleteMax();
      
    }
    
    else if(tempBuyer->shares == tempSeller->shares)
    {
      transaction->shares = tempSeller->shares;
      
      // cout << endl;
      // cout << "New Transaction BS = SS" << endl;
      // cout << "Symbol: " << transaction->symbol << endl;
      // cout << "Price: " << transaction->price << endl;
      // cout << "Buyer: " << transaction->buyerID << endl;
      // cout << "Seller: " << transaction->sellerID << endl;
      // cout << "Time: " << transaction->time << endl;
      // cout << "Num Shares: " << transaction->shares << endl << endl;
      tempSeller->shares = 0;
//      tempSeller->price = -1;
//      companyNames[currentStock]->deleteSeller(*tempSeller);
      companyNames[currentStock]->BuyersHold->deleteMax();
    }
    
    // cout << endl;
    // cout << "New Transaction Done" << endl;
    // cout << "Symbol: " << transaction->symbol << endl;
    // cout << "Price: " << transaction->price << endl;
    // cout << "Buyer: " << transaction->buyerID << endl;
    // cout << "Seller: " << transaction->sellerID << endl;
    // cout << "Time: " << transaction->time << endl;
    // cout << "Num Shares: " << transaction->shares << endl;
    
//    count++;
    
    
    return true;
  }
  
  	// cout << endl;
  

//	if(companyNames[currentStock] != NULL && companyNames[currentStock]->sellerPosition != 0) // Might need to check that both the binary heap and queue are created
//	{
//		static int count = 0;
//		InputOffer *tempBuyer = NULL;
//		
//		if(companyNames[currentStock]->BuyersHold->isEmpty())
//		{
//			// cout << "Hello" << endl;
//			return false;
//		}
//			
//		tempBuyer = companyNames[currentStock]->BuyersHold->findMax();
//		
//		// cout << "TPB: " << tempBuyer->price << endl;
//		
//		
//		if(tempBuyer->price < companyNames[currentStock]->findMinSeller()->price)
//		{
//			//// cout << "Error!" << endl;
//			return false;
//		}
//		
//		
//		InputOffer *tempSeller = companyNames[currentStock]->findClosest(tempBuyer->price);
//    
//		// Edit here
//		if(tempBuyer->price >= tempSeller->price)
//		{
//      
//			strcpy(transaction->symbol, companyNames[currentStock]->name); // Might have to use pointer
//			double tempPrice = companyNames[currentStock]->BuyersHold->findNext(); // This gets the next value or third value
//			if(tempPrice < tempSeller->price)
//			{
//      
//				transaction->price = tempSeller->price;
//				transaction->sellerID = tempSeller->ID;
//				transaction->buyerID = tempBuyer->ID;
//				transaction->time = this->time - 1;
//				
//				//~ // cout << "New Transaction B<SP" << endl;
//				//~ // cout << "Symbol: " << transaction->symbol << endl;
//				//~ // cout << "Price: " << transaction->price << endl;
//				//~ // cout << "Buyer: " << transaction->buyerID << endl;
//				//~ // cout << "Seller: " << transaction->sellerID << endl;
//				//~ // cout << "Time: " << transaction->time << endl;
//				//~ // cout << "Num Shares: " << transaction->shares << endl << endl;
//			}
//			
//			
//			else
//			{
//				transaction->price = companyNames[currentStock]->BuyersHold->findNext();
//				transaction->sellerID = tempSeller->ID;
//				transaction->buyerID = tempBuyer->ID;
//				transaction->time = this->time - 1;
//				
//				//~ // cout << "New Transaction S>B" << endl;
//				//~ // cout << "Symbol: " << transaction->symbol << endl;
//				//~ // cout << "Price: " << transaction->price << endl;
//				//~ // cout << "Buyer: " << transaction->buyerID << endl;
//				//~ // cout << "Seller: " << transaction->sellerID << endl;
//				//~ // cout << "Time: " << transaction->time << endl;
//				//~ // cout << "Num Shares: " << transaction->shares << endl << endl;
//			}
//			
//			if(tempBuyer->shares > tempSeller->shares)
//			{
//				tempBuyer->shares -= tempSeller->shares;
//				transaction->shares = tempSeller->shares;
//				
//				tempSeller->shares = 0;
//				companyNames[currentStock]->deleteSeller(*tempSeller);
//				if(tempBuyer->shares == 0)
//				{
//					companyNames[currentStock]->BuyersHold->deleteMax();
//				}
//				
//				// cout << "TempB: " << tempBuyer->price << endl;
//				
//				// cout << endl;
//				// cout << "New Transaction BS > SS" << endl;
//				// cout << "Symbol: " << transaction->symbol << endl;
//				// cout << "Price " << transaction->price << endl;
//				// cout << "Buyer: " << transaction->buyerID << endl;
//				// cout << "Seller: " << transaction->sellerID << endl;
//				// cout << "Time: " << transaction->time << endl;
//				// cout << "Num Shares: " << transaction->shares << endl << endl;
//			}
//			
//			else if(tempBuyer->shares < tempSeller->shares)
//			{
//				tempSeller->shares -= tempBuyer->shares;
//				transaction->shares = tempBuyer->shares;
//				
//				
//				// cout << endl;
//				// cout << "New Transaction BS < SS" << endl;
//				// cout << "Symbol: " << transaction->symbol << endl;
//				// cout << "Price: " << transaction->price << endl;
//				// cout << "Buyer: " << transaction->buyerID << endl;
//				// cout << "Seller: " << transaction->sellerID << endl;
//				// cout << "Time: " << transaction->time << endl;
//				// cout << "Num Shares: " << transaction->shares << endl << endl;
//        
//        if(tempSeller->shares == 0)
//        {
//          companyNames[currentStock]->deleteSeller(*tempSeller);
//        }
//        companyNames[currentStock]->BuyersHold->deleteMax();
//        
//			}
//			
//			else if(tempBuyer->shares == tempSeller->shares)
//			{
//				transaction->shares = tempSeller->shares;
//				
//				// cout << endl;
//				// cout << "New Transaction BS = SS" << endl;
//				// cout << "Symbol: " << transaction->symbol << endl;
//				// cout << "Price: " << transaction->price << endl;
//				// cout << "Buyer: " << transaction->buyerID << endl;
//				// cout << "Seller: " << transaction->sellerID << endl;
//				// cout << "Time: " << transaction->time << endl;
//				// cout << "Num Shares: " << transaction->shares << endl << endl;
//        companyNames[currentStock]->deleteSeller(*tempSeller);
//        companyNames[currentStock]->BuyersHold->deleteMax();
//			}
//			
//			// cout << endl;
//			// cout << "New Transaction" << endl;
//			// cout << "Symbol: " << transaction->symbol << endl;
//			// cout << "Price: " << transaction->price << endl;
//			// cout << "Buyer: " << transaction->buyerID << endl;
//			// cout << "Seller: " << transaction->sellerID << endl;
//			// cout << "Time: " << transaction->time << endl;
//			// cout << "Num Shares: " << transaction->shares << endl;
//			
//			// cout << "Count: " << count << endl;
//			count++;
//			return true;
////			// cout << endl;
//		}
//		
//		// cout << endl;
//	}
  
  // cout << "Count: " << count << endl;
  
  // what do we do with the transaction here? What is stored? // No more to be stored?
  // Are we passing in the transactions that have been matched all at once? That's interesting
  return false; // means no more transactions, and transaction will be ignored
} // newTransaction()
