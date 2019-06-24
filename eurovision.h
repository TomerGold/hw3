#ifndef EUROVISION_H_
#define EUROVISION_H_
#define MAX_PARTICIPANTS  26
#define MAX_LEN 180
#define MAX_VOTES 5
#define MAX_NUM_OF_STATES 10
#define DOUZE_PUA 12
#define SEMI_DOUZE_PUA 10
#define FIRST 0


#include <iostream>
#include <string>


using std::string;
using std::ostream;
using std::endl;

//---------------------------------------------------

enum VoterType {
    All, Regular, Judge
};
enum Phase {
    Registration, Contest, Voting
};

//---------------------------------------------------

class Participant {
    string state_name;
    string song_name, singer_name;
    int song_len;
    bool is_registered; //boolean value to check if the Par. is registered

public :
    explicit Participant(string state, string song, int len,
                         string singer);

    Participant() = delete;

    ~Participant() = default;

    Participant(Participant &participant) = delete;

    Participant &operator=(Participant &participant) = delete;

    const string state() const;

    string song();

    int timeLength();

    string singer();

    bool isRegistered();

    void update(string song, int len, string singer);

    void updateRegistered(bool status);
};

ostream &operator<<(ostream &os, Participant &participant);

//---------------------------------------------------


class Voter {
    string state_name;
    VoterType voter_type;
    int num_of_votes; //how many times the voter has voted

public :
    explicit Voter(string state, VoterType type = Regular);

    ~Voter() = default;

    const string state() const;

    const VoterType voterType() const;

    int timesOfVotes();

    Voter &operator++();
};

ostream &operator<<(ostream &os, Voter &voter);

// -----------------------------------------------------------

struct Vote {
    Voter &voter;
    string list_of_states[MAX_NUM_OF_STATES]; //the states the voter voted to
public:
    explicit Vote(Voter &voter, string vote1 = "", string vote2 = "",
                  string vote3 = "", string vote4 = "", string vote5 = "",
                  string vote6 = "", string vote7 = "", string vote8 = "",
                  string vote9 = "", string vote10 = "");

    ~Vote() = default;

};
// -----------------------------------------------------------


class MainControl {
    int max_participants;
    int max_song_len;
    int max_votes;
    int states_counter; // holds how many states are registered in any time
    Phase phase;

    struct VoteForParticipant { //holds the votes for each Participant
        Participant *participant;
        int regular_votes;
        int judge_votes;
    };

    VoteForParticipant **participants;

    //this function sorts Participants alphabetically
    void sortParticipants() const;

public :

    explicit MainControl(int max_len = MAX_LEN,
                         int max_participants = MAX_PARTICIPANTS,
                         int max_votes_init =
                         MAX_VOTES);

    ~MainControl();

    void setPhase(Phase next_phase);

    bool legalParticipant(Participant &temp_participant);

    bool participate(string state_name);

    MainControl &operator+=(Participant &current_participant);

    MainControl &operator+=(const Vote &temp);

    MainControl &operator-=(Participant &temp);

    // used for finding the winner according to i parameter and voter_type
    const string operator()(int i, VoterType voter_type) const;

    friend ostream &operator<<(ostream &os, const MainControl &temp);

    class Iterator;

    Iterator begin() const;

    Iterator end() const;

    //this function compares between two votes for a participant
    friend bool compareVotes(VoteForParticipant &vote1, VoteForParticipant
    &vote2, VoterType voter_type);

    //prints the participant in voteForParticipant
    friend ostream &operator<<(ostream &os, VoteForParticipant &vote);


};

//this function checks if the vote is legal according to the criteria
bool isVoteLegal(MainControl &eurovision, const Vote &current_vote, int
max_votes);

///Iterator class:

class MainControl::Iterator {
    const MainControl *mainControl;
    int index;

public:

    explicit Iterator(const MainControl *mainControl = nullptr,
                      int index = 0);

    ~Iterator() = default;

    Iterator(const Iterator &iterator) = default;

    Iterator &operator=(const Iterator &iterator) = default;

    VoteForParticipant &operator*();

    Iterator &operator++();

    bool operator==(const Iterator &iterator) const;

    bool operator!=(const Iterator &iterator) const;

    bool operator<(const Iterator &iterator) const;

    int operator-(const Iterator &i2) const;

};

//this template is used for finding the winner according to i and voter-type
// (mostly used by operator ())
template<typename T>
T get(const T start, const T end, int i, VoterType voter_type) {
    if (i < 1 || i > end - start) { //unusable i
        return end;
    }
    T matching_participant = start;
    T *already_won = new T[i]; //array of iterators who already won
    for (int iteration_counter = 1;
         iteration_counter <= i; iteration_counter++) {
        for (T current = start; current < end; ++current) {
            if (find(already_won, matching_participant, i)) {
                ++matching_participant;
            }
            if (find(already_won, current, i)) {
                continue;
            }
            if (compareVotes(*current, *matching_participant,
                             voter_type)) {
                matching_participant = current;
            }
        } //winner for this round:
        already_won[iteration_counter - 1] = matching_participant;
        if (iteration_counter != i) {
            matching_participant = start;
        }
    }
    delete[] already_won;
    return matching_participant; //winner according to i
}

//this template is used in "get" template in order to find out if a specific
// iterator is in the array
template<typename T>
bool find(T *array, T &iterator_to_find, int size) {
    for (int i = 0; i < size; ++i) {
        if (array[i] == iterator_to_find) {
            return true;
        }
    }
    return false;

}



// -----------------------------------------------------------

#endif
