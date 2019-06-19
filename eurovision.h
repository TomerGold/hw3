#ifndef EUROVISION_H_
#define EUROVISION_H_
#define MAX_PARTICIPANTS  26
#define MAX_LEN 180
#define MAX_VOTES 5
#define MAX_NUM_OF_STATES 10
#define DOUZ_PUA 12
#define SEMI_DOUZ_PUA 10
#define FIRST 0
#define SECOND 1


#include <iostream>
#include <string>

using std::string;
using std::ostream;
using std::endl;

// it's allowed to define here any using statements, according to needs.
// do NOT define here : using namespace std;

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

    Participant()= delete;

    ~Participant() = default;

     Participant(Participant& participant) = delete;

     Participant& operator=(Participant& participant) = delete;

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
    explicit MainControl(int max_states = MAX_PARTICIPANTS,
                         int max_len = MAX_LEN, int max_votes_init = MAX_VOTES);

    ~MainControl();

    void setPhase(Phase next_phase);

    bool legalParticipant(Participant &temp_participant);

    bool participate(string state_name);

    MainControl &operator+=(Participant &current_participant);

    MainControl &operator+=(const Vote &temp);

    MainControl &operator-=(Participant &temp);

    friend ostream &operator<<(ostream &os, const MainControl &temp);

};

//this function checks if the vote is legal according to the criteria
bool isVoteLegal(MainControl &eurovision, const Vote &current_vote, int
max_votes);

// -----------------------------------------------------------

#endif
