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

///End of Participant class methods:

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

MainControl::MainControl(int max_len, int max_states, int max_votes_init) {
    max_participants = max_states;
    max_votes = max_votes_init;
    max_song_len = max_len;
    states_counter = 0;
    phase = Registration;
    participants = new VoteForParticipant *[max_states];

    for (int i = 0; i < max_states; ++i) {
        participants[i] = nullptr;
    }
}

MainControl::~MainControl() {
    for (int i = 0; i < states_counter; ++i) {
        delete (participants[i]);
    }
    delete[] participants;
}

void MainControl::setPhase(Phase next_phase) {
    if (phase == Registration && next_phase == Contest) {
        phase = Contest;
    }
    if (phase == Contest && next_phase == Voting) {
        phase = Voting;
        sortParticipants();//alphabetic sort
    }
}

bool MainControl::legalParticipant(Participant &temp_participant) {
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
        if (state_name == participants[i]->participant->state()) {
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
            os << eurovision.participants[i]->participant->state() << " : " <<
               "Regular(" << eurovision.participants[i]->regular_votes
               << ") Judge(" << eurovision.participants[i]->judge_votes << ")"
               << endl;
        }
        os << "}" << endl;
        return os;
    } else if (eurovision.phase == Registration) { //output for reg. phase
        os << "{" << endl;
        eurovision.sortParticipants();
        os << "Registration" << endl;
        for (int i = 0; i < eurovision.states_counter; ++i) {
            os << "[" << eurovision.participants[i]->participant->state() <<
               "/" << eurovision.participants[i]->participant->song() << "/" <<
               eurovision.participants[i]->participant->timeLength() << "/" <<
               eurovision.participants[i]->participant->singer() << "]" << endl;
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
        if (participants[i] == nullptr) {
            participants[i] = new VoteForParticipant;
            participants[i]->participant = &current_participant;
            participants[i]->participant->updateRegistered(true);
            participants[i]->judge_votes = 0;
            participants[i]->regular_votes = 0;
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
    //checks if it is the exact same par. (aka - also song len and singer)
    for (int i = 0; i < states_counter; ++i) {
        if (current_participant.state() ==
            participants[i]->participant->state()) {
            if (current_participant.singer() !=
                participants[i]->participant->singer() ||
                current_participant.song() !=
                participants[i]->participant->song()
                || current_participant.timeLength() !=
                   participants[i]->participant->timeLength()) {
                return *this;
            }
        }
    }
    for (int i = 0; i < states_counter; ++i) {
        if (current_participant.state() ==
            participants[i]->participant->state()) {
            participants[i]->participant->updateRegistered(false);
            delete participants[i]; //delete the struct but not the participant
            participants[i] = participants[states_counter - 1];
            participants[states_counter - 1] = nullptr;
            break;
        }
    }
    states_counter--;
    return *this;
}

MainControl &MainControl::operator+=(const Vote &current_vote) {
    bool is_judge_voted_correctly = false;
    if (!isVoteLegal(*this, current_vote, max_votes)) {
        return *this;
    }
    for (int i = 0; i < MAX_NUM_OF_STATES; ++i) {
        if (current_vote.list_of_states[i] == "") { //end of judge's votes
            continue;
        }
        for (int j = 0; j < states_counter; ++j) {
            if (current_vote.list_of_states[i] ==
                participants[j]->participant->state() &&
                current_vote.list_of_states[i] != current_vote.voter.state()) {
                if (i == 0 && current_vote.voter.voterType() == Judge) {
                    participants[j]->judge_votes += DOUZE_PUA;
                    is_judge_voted_correctly = true; //now the vote is OK
                }
                if (i == 0 && current_vote.voter.voterType() == Regular) {
                    participants[j]->regular_votes++;
                    ++current_vote.voter; //for regular voter only
                    return *this;
                }
                if (i == 1) {
                    participants[j]->judge_votes += SEMI_DOUZE_PUA;
                    is_judge_voted_correctly = true; //now the vote is OK
                }
                if (i >= 2) {
                    participants[j]->judge_votes += SEMI_DOUZE_PUA - i;
                    is_judge_voted_correctly = true; //now the vote is OK
                }
            }
        }
    }
    if (is_judge_voted_correctly) { // check if judge voted ok even once
        ++current_vote.voter; // for judge voter only;
    }
    return *this;
}

const string MainControl::operator()(int i, VoterType voter_type) const {
    MainControl::Iterator iterator = get(this->begin(), this->end(), i,
                                         voter_type);
    if (i > states_counter || i < 1) { // unusable i
        return "";
    }
    MainControl::VoteForParticipant current = *iterator;
    return current.participant->state();
}

//this function sorts Participants alphabetically
void MainControl::sortParticipants() const {
    // Sorting Participants in list using bubble sort
    for (int j = 0; j < states_counter - 1; j++) {
        for (int i = j + 1; i < states_counter; i++) {
            if (participants[j]->participant->state() >
                participants[i]->participant->state()) {
                MainControl::VoteForParticipant *temp = participants[j];
                participants[j] = participants[i];
                participants[i] = temp;
            }
        }
    }
}

ostream &operator<<(ostream &os, MainControl::VoteForParticipant &vote) {
    return os << "[" << vote.participant->state() << "/" <<
              vote.participant->song() << "/" << vote.participant->timeLength()
              << "/" << vote.participant->singer() << "]";
    //TODO right now this is code duplication...we should ask if there is a better way to do this.
}

///End of MainControl class method

///Start of MainControl::Iterator  methods

MainControl::Iterator::Iterator(const MainControl *mainControl, int index) :
        mainControl(mainControl),
        index(index) {
}

MainControl::Iterator MainControl::begin() const {
    this->sortParticipants();
    return MainControl::Iterator(this, 0);
}

MainControl::Iterator MainControl::end() const {
    this->sortParticipants();
    return MainControl::Iterator(this, states_counter);
}

MainControl::VoteForParticipant &MainControl::Iterator::operator*() {
    return *mainControl->participants[index];
}

MainControl::Iterator &MainControl::Iterator::operator++() {
    ++index;
    return *this;
}

bool MainControl::Iterator::operator==(const Iterator &iterator) const {
    if (((mainControl) == (iterator.mainControl)) && (index == iterator
            .index)) { //Todo what is ==  to main control? how does it works?
        return true;
    }
    return false;
}

bool MainControl::Iterator::operator!=(const Iterator &iterator) const {
    return !(*this == iterator);
}

bool MainControl::Iterator::operator<(const Iterator &iterator) const {
    return (index < iterator.index);
}

int MainControl::Iterator::operator-(const Iterator &i2) const {
    return (this->index - i2.index);
}

//this function checks if the vote is legal according to the criteria
bool isVoteLegal(MainControl &eurovision, const Vote &current_vote, int
max_votes) {
    if (!eurovision.participate(current_vote.voter.state())) {
        return false; // checks if voters country participates
    }
    if (current_vote.voter.voterType() == Regular) { //checks for regular only
        if (current_vote.voter.state() == current_vote.list_of_states[FIRST]) {
            return false;
        }
        if (!eurovision.participate(current_vote.list_of_states[FIRST])) {
            return false;
        }
        if (current_vote.voter.timesOfVotes() >= max_votes) {
            return false;
        }
    }
    if (current_vote.voter.voterType() == Judge &&
        current_vote.voter.timesOfVotes() > 0) {
        return false;
    }
    return true;
}

//this function compares between two votes for a participant
bool compareVotes(
        MainControl::VoteForParticipant vote1,
        MainControl::VoteForParticipant vote2, VoterType voter_type) {
    if (voter_type == Regular) {
        if (vote1.regular_votes > vote2.regular_votes) {
            return true;
        }
        if (vote1.regular_votes == vote2.regular_votes) {
            return vote1.participant->state() > vote2.participant->state();
        }
    }

    if (voter_type == Judge) {
        if (vote1.judge_votes > vote2.judge_votes) {
            return true;
        }
        if (vote1.judge_votes == vote2.judge_votes) {
            return vote1.participant->state() > vote2.participant->state();
        }
    }

    if (voter_type == All) {
        if (vote1.regular_votes + vote1.judge_votes >
            vote2.regular_votes + vote2.judge_votes) {
            return true;
        }
        if (vote1.regular_votes + vote1.judge_votes ==
            vote2.regular_votes + vote2.judge_votes) {
            return vote1.participant->state() > vote2.participant->state();
        }
    }
    return false;
}

