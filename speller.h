#inclue <limits.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "BinaryHeap.h"
#include <fstream>
#include <iomanip>
#ifndef SPELLER_H
#define SPELLER_H

#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)

using namespace std;

struct BinaryNode
{
	
	unsigned short freq;
	char letter[9];
	unsigned short count;
	
	BinaryNode *left;
	BinaryNode *right;
	BinaryNode *next;
	BinaryNode(): freq(0), left(NULL), right(NULL), next(NULL), count(0)
	{
		letter = '*';
		
	}
	BinaryNode(int freq_) : freq(freq_), left(NULL), right(NULL), next(NULL)
	{
		letter = '^';
	}
	
	void bitInsert(char value)
	{
		letter[count] = value;
		count++;
	}
	
	~BinaryNode()
	{
		
	}
};

struct QueueLL
{
	QueueLL() : root(NULL), back(NULL), count(0){}
	BinaryNode * root;
	BinaryNode * back;
	int count;
	
	~QueueLL()
	{
		root = NULL;
		back = NULL;
		delete root;
		delete back;
	}
	
	void insert(BinaryNode * insertNode)
	{
		if(root == NULL)
		{
			root = insertNode;
			back = insertNode;
			count++;
			return;
		}
		
		if(insertNode->freq < root->freq) // insert node becomes new root back is still there, first position insert
		{
			insertNode->next = root->next;
			root = insertNode;
			count++;
			return;
		}
		
		BinaryNode *traverser = root;
		BinaryNode *traverser2 = traverser;
		
		while(traverser->next != NULL)
		{
			if(traverser->next)
			{
				if(insertNode->freq < traverser->next->freq)
				{
					insertNode->next = traverser->next;
					traverser->next = insertNode;
					count ++;
					return;
				}
				
			}
			traverser2 = traverser;
			traverser = traverser->next;
		}
		
		if(insertNode->freq > back->freq)
		{
			back->next = insertNode;
			back = insertNode;
			count++;
		}
		
		else
		{
			insertNode->next = traverser2->next;
			traverser2->next = insertNode;
			count++;
		}
		// Back
		
		
	}
	
	void popOffTop()
	{
		if(root == NULL)
		{
			cout << "You done goofed." << endl;
			return;
		}
			
		else if(root->next == NULL)
		{
			cout << "Next to End" << endl;
		}
		
		BinaryNode *pops = root;
		root = root->next;
		pops->next = NULL;
		count--;
	}
	
	void printTree()
	{
		BinaryNode *traverser = root;
		cout << "PrintTree: " << endl;
		while(traverser != NULL)
		{
			cout << traverser->letter << ",";
			cout << traverser->freq << endl;
			traverser = traverser->next;
		}
	}
	
};

class Speller
{
public:
  Speller(char *dictionary[], int dictSize);
  ~Speller();
  void check(char *document[], int docSize, int misspelled[], int *misspelledCount);
  int positionMispelled;
  BinaryNode *root; 
  BinaryNode *letters;
  
  QueueLL *queue;
  
  //~ void printTree(BinaryNode * root, int indent) //~ Start Debug here if not working
  //~ {
		//~ if (root != 0)
		//~ {
			//~ printTree(root->right, indent + 8);
			//~ cout << setw(indent) << " " << root->letter << endl;
			//~ printTree(root->left, indent + 8);
		//~ }
  //~ }
}; 

#endif
