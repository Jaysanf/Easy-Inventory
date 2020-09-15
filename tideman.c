#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool cyclecheck(int winner, int loser, int j);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    int prefered_over;
    for (int i = 0; i < candidate_count - 1; i++)
    {
        prefered_over = ranks[i];
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[prefered_over][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences [j][i])
            {
                pairs[pair_count].winner = i ;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }

    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    pair dump;
    int max_dump;
    int max_pos;
    for (int i = 0; i < pair_count; i++)
    {
        max_dump = 0;
        max_pos = 0;
        for (int j = i ; j < pair_count; j++)
        {
            if ( preferences[pairs[j].winner][pairs[j].loser] > max_dump)
            {
                max_dump = preferences[pairs[j].winner][pairs[j].loser];
                max_pos = j;
            }
        }

        dump = pairs[i];
        pairs[i] = pairs[max_pos];
        pairs[max_pos] = dump;
    }
}


// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        //goes through all the pairs and lock them if it does not create a cycle
        if (!(cyclecheck(pairs[i].winner, pairs[i].loser, i)))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}



bool cyclecheck(int winner, int loser, int j)
{
    // if we end up in a situtation like Alice - Alice or A-A it means there is a cylce so return false
    if (winner == loser)
    {
        return true;
    }
    //loop that goes from the pair that is being checked to the first one locked;
    //EX; if you have D-A A-B B-C and want to add C-A; then you would want to check if you can simplify B-C
    // and C-A To B-A...
    for (int i = j; i >= 0; i--)
    {
        
        //Checks if we can simplify; ex; B-C and C-A to B-A AND that we simplify with something that has already been locked
        if (loser == pairs[i].winner && locked[pairs[i].winner][pairs[i].loser])
        {
            //Returns a Cyclecheck with the simplify version until it checked every option then it will skip the for loop and return false; no cycle
            return cyclecheck(winner, pairs[i].loser, j);
        }
        else if (winner == pairs[i].loser && locked[pairs[i].winner][pairs[i].loser])
        {
            //Returns a Cyclecheck with the simplify version until it checked every option then it will skip the for loop and return false; no cycle
            return cyclecheck(pairs[i].winner, loser, j );
        }
    }
    return false;
}


// Print the winner of the election
void print_winner(void)
{
    // TODO
    //checks if there is any true in a column AKA there is no one with an edge over the candidate column
    int check_true;

    for (int j = 0; j < candidate_count; j++)
    {
        check_true = 0;
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][j] == true)
            {
                check_true++;
            }
        }
        if (check_true == 0)
        {
            printf("%s\n", candidates[j]);
            return;
        }
    }
    printf("ERROR\n");
    return;
}

