#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // goes through candidates list and check if the vote is successful
    for (int i = 0; i < candidate_count ; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    //prints all winner
    
    // keeps track of the current winner Vote
    int top = 0;
    
    // keeps track of how many people are in a current tie
    int win_counter;
    
    // array of the current winners
    string winner[candidate_count];
    
    //
    for (int i = 0; i < candidate_count; i++)
    {
        
        if (candidates[i].votes > top)
        {
            //if new max vote: reset winner array
            memset(winner, 0, candidate_count);
            
            //update max vote
            top = candidates[i].votes;
            
            //reset win counter; pos in winner array
            win_counter = 1;
            winner[0] = candidates[i].name;
        }
        
        else if (candidates[i].votes == top)
        {
            winner[win_counter] = candidates[i].name;
            win_counter++;
        }
    }  
    
    for (int i = 0; i < win_counter; i++)
        // print all winners
    {
        printf("%s\n", winner[i]);
    }
    return;
}

