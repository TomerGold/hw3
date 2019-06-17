#include "eurovision.h"


///Start of Participant class methods:

Participant::Participant(string state, string song, int len,
                         string singer) {
    state_name = state;
    song_name = song;
    song_len = len;
    singer_name = singer;
    is_registered = false;
}

const string Participant::state() const {
    return state_name;
}

string Participant::song() {
    return song_name;
}

string Participant::singer() {
    return singer_name;
}

int Participant::timeLength() {
    return song_len;
}

bool Participant::isRegistered() {
    if (is_registered == false) {
        return false;
    } else return true;
}

void Participant::updateRegistered(bool status) {
    is_registered = status;
}

void Participant::update(string song, int len, string singer) {
    if (isRegistered()) {
        return;
    }
    if (song != "") {
        song_name = song;
    }
    if (singer != "") {
        singer_name = singer;
    }
    if (len > 0) {
        song_len = len;
    }
}

ostream &operator<<(ostream &os, Participant &participant) {
    return os << "[" << participant.state() << "/" << participant.song() <<
              "/" << participant.timeLength() << "/" << participant.singer()
              << "]";
}

///End of Participant calss methods:

///Start of Voter class methods:

Voter::Voter(string state, VoterType type) {
    state_name = state;
    voter_type = type;
    num_of_votes = 0;
}

const string Voter::state() const {
    return state_name;
}

const VoterType Voter::voterType() const {
    return voter_type;
}

int Voter::timesOfVotes() {
    return num_of_votes;
}

Voter &Voter::operator++() {
    ++num_of_votes;
    return *this;
}

ostream &operator<<(ostream &os, Voter &voter) {
    if (voter.voterType() == Regular) {
        return os << "<" << voter.state() << "/" << "Regular" << ">";
    }
    if (voter.voterType() == Judge) {
        return os << "<" << voter.state() << "/" << "Judge" << ">";
    }
    return os;
}

///End of Voter class methods:

///Start of Vote type methods:

Vote::Vote(Voter &current_voter, string vote1, string vote2, string vote3,
           string vote4, string vote5, string vote6, string vote7,
           string vote8, string vote9, string vote10) : voter(current_voter) {
    list_of_states[0] = vote1;
    list_of_states[1] = vote2;
    list_of_states[2] = vote3;
    list_of_states[3] = vote4;
    list_of_states[4] = vote5;
    list_of_states[5] = vote6;
    list_of_states[6] = vote7;
    list_of_states[7] = vote8;
    list_of_states[8] = vote9;
    list_of_states[9] = vote10;
}

///End of Participant type methods:

///Start of MainControl class methods:

///constructor function:
MainControl::MainControl(int max_states, int max_len, int max_votes_init) {
    max_participants = max_states;
    max_votes = max_votes_init;
    max_song_len = max_len;
    states_counter = 0;
    phase = Registration;
    regular_votes = new int[max_states];
    for (int i = 0; i < max_states; ++i) {
        regular_votes[i] = 0;
    }
    judge_votes = new int[max_states];
    for (int i = 0; i < max_states; ++i) {
        judge_votes[i] = 0;
    }
    list_of_participants = new Participant *[max_states];
    for (int i = 0; i < max_states; ++i) {
        list_of_participants[i] = nullptr;
    }
}

MainControl::~MainControl() {
    delete[] list_of_participants;
    delete[] regular_votes;
    delete[] judge_votes;
}

void MainControl::setPhase(Phase next_phase) {
    if (phase == Registration && next_phase == Contest) {
        phase = Contest;
    }
    if (phase == Contest && next_phase == Voting) {
        phase = Voting;
        sortStates(list_of_participants, states_counter);//alphabetic sort
    }
}

bool MainControl::legalParticipant(Participant temp_participant) {
    if (temp_participant.state() == "" || temp_participant.song() == "" ||
        temp_participant.singer() == "") {
        return false;
    }
    if (temp_participant.timeLength() > max_song_len) {
        return false;
    }
    return true;
}

bool MainControl::participate(string state_name) {
    for (int i = 0; i < states_counter; ++i) {
        if (state_name == list_of_participants[i]->state()) {
            return true;
        }
    }
    return false;
}

ostream &operator<<(ostream &os, const MainControl &eurovision) {
    if (eurovision.states_counter == 0) {//no Participates in contest yet
        os << "{" << endl;
        os << "Registration" << endl;
        os << "}" << endl;
        return os;
    }
    if (eurovision.phase == Voting) { //output for voting phase
        os << "{" << endl;
        os << "Voting" << endl;
        for (int i = 0; i < eurovision.states_counter; ++i) {
            os << eurovision.list_of_participants[i]->state() << " : " <<
               "Regular(" << eurovision.regular_votes[i] << ") Judge(" <<
               eurovision.judge_votes[i] << ")" << endl;
        }
        os << "}" << endl;
        return os;
    } else if (eurovision.phase == Registration) { //output for reg. phase
        sortStates(eurovision.list_of_participants, eurovision.states_counter);
        os << "{" << endl;
        os << "Registration" << endl;
        for (int i = 0; i < eurovision.states_counter; ++i) {
            os << "[" << eurovision.list_of_participants[i]->state() << "/" <<
               eurovision.list_of_participants[i]->song() << "/" <<
               eurovision.list_of_participants[i]->timeLength() << "/" <<
               eurovision.list_of_participants[i]->singer() << "]" << endl;
        }
        os << "}" << endl;
        return os;
    }
    return os;
}

MainControl &MainControl::operator+=(Participant &current_participant) {
    if (phase != Registration) {
        return *this;
    }
    if (states_counter >= max_participants) {
        return *this;
    }
    if (participate(current_participant.state())) {
        return *this;
    }
    if (!legalParticipant(current_participant)) {
        return *this;
    }
    states_counter++;
    for (int i = 0; i < states_counter; ++i) {
        if (list_of_participants[i] == nullptr) {
            list_of_participants[i] = &current_participant;
            list_of_participants[i]->updateRegistered(true);
        }
    }
    return *this;
}

MainControl &MainControl::operator-=(Participant &current_participant) {
    if (phase != Registration) {
        return *this;
    }
    if (!participate(current_participant.state())) {
        return *this;
    }
    //checks if the exact same par. (aka - also song +len and singer
    for (int i = 0; i < states_counter; ++i) {
        if (current_participant.state() == list_of_participants[i]->state()) {
            if (current_participant.singer() !=
                list_of_participants[i]->singer() ||
                current_participant.song() != list_of_participants[i]->song()
                || current_participant.timeLength() !=
                   list_of_participants[i]->timeLength()) {
                return *this;
            }
        }
    }//TODO CAN PARTICIPATE BY STATE BUT NOT BY SONG OR SINGER OR LEN - IS IT
    // POSSIBLE?
    for (int i = 0; i < states_counter; ++i) {
        if (current_participant.state() == list_of_participants[i]->state()) {
            list_of_participants[i]->updateRegistered(false);
            list_of_participants[i] = list_of_participants[states_counter - 1];
            list_of_participants[states_counter - 1] = nullptr;
            break;
        }
    }
    states_counter--;
    return *this;
}

MainControl &MainControl::operator+=(const Vote &current_vote) {
    //bool is_judge_voted_correctly = false;
    if (!isVoteLegal(*this, current_vote, max_votes)) {
        return *this;
    }
    for (int i = 0; i < MAX_NUM_OF_STATES; ++i) {
        if (current_vote.list_of_states[i] == "") { //end of judge's votes
            break;
        }
        for (int j = 0; j < states_counter; ++j) {
            if (current_vote.list_of_states[i] ==
                list_of_participants[j]->state() &&
                current_vote.list_of_states[i] != current_vote.voter.state()) {
                if (i == 0 && current_vote.voter.voterType() == Judge) {
                    judge_votes[j] += DOUZ_PUA;
                    // is_judge_voted_correctly = true; //now the vote is OK
                }
                if (i == 0 && current_vote.voter.voterType() == Regular) {
                    regular_votes[j]++;
                    ++current_vote.voter; //for regular voter only
                    return *this;
                }
                if (i == 1) {
                    judge_votes[j] += SEMI_DOUZ_PUA;
                    // is_judge_voted_correctly = true; //now the vote is OK
                }
                if (i >= 2) {
                    judge_votes[j] += SEMI_DOUZ_PUA - i;
                    // is_judge_voted_correctly = true; //now the vote is OK
                }
            }
        }
    }
    //if (is_judge_voted_correctly) { // check if judge voted ok even once
    // TODO IF ALL OF JUDGES VOTES ARE BAD
    // DOES IT COUNT???? SAME FOR REGULAR BUT WITH ONE VOTE??
    ++current_vote.voter; // for judge voter only;

    return *this;
}

//this function sorts Participants alphabetically
void sortStates(Participant **list, int states_counter) {
    // Sorting Participants in list using bubble sort
    for (int j = 0; j < states_counter - 1; j++) {
        for (int i = j + 1; i < states_counter; i++) {
            if (list[j]->state() > list[i]->state()) {
                Participant *temp = list[j];
                list[j] = list[i];
                list[i] = temp;
            }
        }
    }
}

//this function checks if the vote is legal according to the criteria
bool isVoteLegal(MainControl &eurovision, const Vote &current_vote, int
max_votes) {
    if (!eurovision.participate(current_vote.voter.state())) {
        return false; // checks if voters country participates
    }
    if (current_vote.voter.voterType() == Regular) { //checks for regular only
        if (current_vote.voter.state() == current_vote
                .list_of_states[FIRST]) {
            return false;
        }
        if (!eurovision.participate(current_vote.list_of_states[FIRST])) {
            return false;
        }
        if (current_vote.voter.timesOfVotes() >= max_votes) {
            return false;
        }
        if (current_vote.list_of_states[SECOND] != "") {
            return false; //checks if regular voted more than one
        }
    }
    if (current_vote.voter.voterType() == Judge &&
        current_vote.voter.timesOfVotes() > 0) {
        return false;
    }
    return true;
}






