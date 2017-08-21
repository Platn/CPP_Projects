#include <iostream>
#include <fstream>
#include "StackAr.h"
#include "QueueAr.h"

using namespace std;

/* What we need to implement first is to have them traverse through a queue of the stuff, we need to have
 * a finished queue, and a non-finished queue. *
 * We iterate through each and every piece of the queue.
 * When we get to one row, we need to check it for the waiting in the aisle first, then we need to clear the aisle
 * Move whichever person back in every 5 seconds
 */

enum State{EMPTY, WAIT, STOR_L1, STOR_L2, AC_OUT, AC_IN, DF_OUT, DF_IN};
// Empty and wait are for the seats and the ones sitting in the aisle, (Row)
// Store L1 and Store L2 or are for A-C and D-F (Row Into Seat)
// AC_Out and AC_In are for the ones coming out and the ones coming in for AC (Seat check)
// DF_Out and DF_In are for the ones coming out and in for DF (Seat check)

class Seat
{
public:
    /* Class Variables */
    
    int seatWait; // Might need this later
    char letter;
    State seatState; // Need to check each seat for if it is empty or not
    
    /* Constructors */
    Seat(){}
    Seat(char letter_, State seatState_) : letter(letter_), seatState(seatState_){}
    Seat(char letter_, State seatState_, int seatWait_) : letter(letter_), seatState(seatState_), seatWait(seatWait_){}
    
    /* Functions */
    
    void setState(State changeState)
    {
        seatState = changeState;
    }
    
    State getState()
    {
        return seatState;
    }
    
    void setLetter(char letter_)
    {
        letter = letter_;
    }
    
    char getLetter()
    {
        return letter;
    }
    
    void setWait(int seatWait_)
    {
        seatWait = seatWait_;
    }
    
    int getWait()
    {
        return seatWait;
    }
};

class Row
{
public:
    
    int rowNum;
    Queue<Seat> *seatWaitingMisc; // This seatWaiting should have the highest priority
    StackAr<Seat> *seatLetterAC; // Seated Left Side
    StackAr<Seat> *seatLetterDF; // Seated Right Side
    Queue<Seat> *seatWaitingAC; // Waiting in the aisle to get to seat left
    Queue<Seat> *seatWaitingDF; // Waiting in the aisle to get to seat right
    Queue<Seat> *seatOutAC; // Stepping outside to let the other person in left
    Queue<Seat> *seatOutDF; // Stepping outside to let the other person in right
    StackAr<Seat> *seatHolder; // Hold the seat when we move stuff out
    State rowState; // This needs to be checked and changed each time we move over or in
    
    /* Constructor */
    Row()
    {
        seatLetterAC = new StackAr<Seat>(3);
        seatLetterDF = new StackAr<Seat>(3);
        seatHolder = new StackAr<Seat>(3);
        seatOutAC = new Queue<Seat>(3); // Having to go out to let someone in
        seatOutDF = new Queue<Seat>(3);
        seatWaitingAC = new Queue<Seat>(3);
        seatWaitingDF = new Queue<Seat>(3);
        rowState = EMPTY;
        
        seatLetterAC->makeEmpty();
        seatLetterDF->makeEmpty();
        seatOutAC->makeEmpty();
        seatOutDF->makeEmpty();
        seatWaitingAC->makeEmpty();
        seatWaitingDF->makeEmpty();
        
        
        /* Set all the characters at the bottom to their empty state so we can work with the seats alone */
        char A = 'A';
        char B = 'B';
        char C = 'C';

        char D = 'D';
        char E = 'E';
        char F = 'F';
        
        Seat newSeatA(A, EMPTY);
        seatLetterAC->push(newSeatA);
        
        Seat newSeatB(B, EMPTY);
        seatLetterAC->push(newSeatB);

        Seat newSeatC(C, EMPTY);
        seatLetterAC->push(newSeatC);
        
        
        Seat newSeatD(D, EMPTY);
        seatLetterDF->push(newSeatD);

        Seat newSeatE(E, EMPTY);
        seatLetterDF->push(newSeatE);
        
        Seat newSeatF(F, EMPTY);
        seatLetterDF->push(newSeatF);
    }
    
    /* Creation Constructor */
    Row(int rowNum_, State inputState) : rowNum(rowNum_), rowState(inputState){}
    
    void setState(State changeState)
    {
        rowState = changeState;
    }
    
    State getState()
    {
        return rowState;
    }
    
    void insertPerson(char letter) // Okay we still need to implement the part that is moving stuff out and checking for empty
    {
        // Inside here we need to sort
        
        // Do a check for EMPTY, if it is AC_IN or DC_IN then we need to make it so that it will go on the stack
        if(letter < 'D') // A-C
        {
            while(letter != seatLetterAC->top().letter)
            {
                seatHolder->push(seatLetterAC->topAndPop());
            }
            
            Seat changeSeat = seatLetterAC->topAndPop();
            changeSeat.seatState = AC_IN;
            
            while(!seatHolder->isEmpty())
            {
                seatLetterAC->push(seatHolder->topAndPop());
                
            }
        }
        
        else // D-F
        {
            // Check the waiting list for D-F here
        }
    }
};

class Aisle
{
public:
    
    Queue<Row> *seatNum; // Each Row
    Queue<Row> *seatHolder;
    Queue<Seat> *seatWaiting; // This is going to be for people who are in the aisle, but not a part of it, probably should just go inside of the row itself. Wouldn't this mean that we would have to do a lot more? we would have to enqueue with the Seat Waiting. But we could do this seatWaiting enqueue to have the next person move forward. So it wouldn't be that big of a problem. But that means for all seat waiting we could just process this. But it would take time to move people forward so it wouldn't be a good idea. It should have its own stuff to process who moves out and who moves in.
    
    
    bool busy; // Might need this to check how busy something is.
    bool queued;
    
    /* Constructor */
    Aisle()
    {
        queued = false;
        seatNum = new Queue<Row>(48);
        seatWaiting = new Queue<Seat>(1000);
        seatNum->makeEmpty();
        seatWaiting->makeEmpty();
        
        for(int i = 1; i < 49; i++)
        {
            State newState = EMPTY;
            Row newRow(i, newState);
            seatNum->enqueue(newRow);
        }
        
        for(int i = 1; i < 49; i++)
        {
            cout << seatNum->dequeue().rowNum << " ";
        }
        
        cout << endl;
    }
    
    void findAisleNumber(int seatNumPass, char letter)
    {
        Row *holdRow = NULL;
        for(int i = 1; i < 49; i++)
        {
            // Here we can dequeue and then enqueue again
            *holdRow = seatNum->getFront(); // Get the front as a const
            // We need to simulate people moving through the aisles and people that cannot move past them. How do we do this?
            if(queued == false && seatNum->getFront().rowState != EMPTY && holdRow->rowNum != seatNumPass) // Cannot move forward
            {
                /* If they can't move forward, then they will have to be placed in the seatWaiting of the current aisle
                 * seatNum holds the one in front of us. I think this should be checked.
                 * Must move them into the waiting aisle for this one and then have them move out.
                 * We have to have a queue for the waiting and clear them out based off of when they move out
                 */
                
                Seat holdSeat(letter, WAIT, seatNumPass);
                seatWaiting->enqueue(holdSeat); // Place them in the queue
                // After being placed in the queue, we should process this row
                
                queued = true; // If they are placed in a queue, then for the next one, we can't keep queueing
            }
            
            // Check if row number is found
            
            /* Aisle should now be empty so we can enter into the place.
             * So now we do a storage of the luggage for 10 seconds.
             * We should not run into a problem where the seatNums are the same except for here
             */

            else if(holdRow->rowNum == seatNumPass)
            {
                // We need to check if there are still people moving out or not from the seats of A-C and D-F
                
                if(holdRow->getState() != EMPTY) // If the state is not EMPTY
                {
                    // We need to go through each part and have it so that we clear through everything.
                    
                    // We need to move one thing at a time every 5 seconds to process it
                    // Process the rows ahead of us instead
                    
                    
                }
                
                else // If Row is empty
                {
                    // Put the person into it.
                    holdRow->insertPerson(letter);
                }
                
                
            }
            
            // Down here, we can process the rest of the stuff.
            
            // They are stuck at the one they're currently in, not the one they're moving into.
        }
        
        // We need another for loop here to put everything back into seatNum;
    }
};

int main()
{
    ifstream infile;
    int row;
    char seat;
    infile.open("/Users/Platn/Documents/ECS_60_Tutor/ECS_60_P1/ECS_60_Tutoring_P1/ECS_60_Tutoring_P1/passengers-1.txt");
    
    if(!infile)
    {
        cout << "File not found." << endl;
        exit(0);
    }
    
    Aisle seatArr;
    int count = 0;
    while(infile >> row >> seat)
    {
        
//        cout << "Row: " << row << " Seat: " << seat << endl;
//        count++;
    }
    cout << "Count: " << count << endl;
    
    return 0;
}
