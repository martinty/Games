#include "Player.h"

#include <math.h>

#include <iomanip>

ostream& operator<<(ostream& os, const HandRank& rhs) {
    return os << TextColor(ColorId::yellow) << handRankStringMap.at(rhs)
              << TextColor();
}

ostream& operator<<(ostream& os, const Act& rhs) {
    return os << actStringMap.at(rhs);
}

ostream& operator<<(ostream& os, const Player& rhs) {
    os << rhs.name << "'s hand: ";
    for (const Card& card : rhs.hand) {
        os << setw(6) << card;
    }
    os << "\nThe rank of the hand: " << rhs.handRank
       << "\nThe value of the hand: " << rhs.handValue
       << "\nCash: " << rhs.cash;
    return os;
}

void Player::newCard(Card card) { hand.push_back(card); }

void Player::blind(int value) {
    cash -= value;
    totalBet += value;
    action = Act::bet;
}

void Player::calculateHandRank() {
    set<Card> cards;
    map<Suit, int> suitCount;
    map<Rank, int> rankCount;
    for (const Card& c : hand) {
        cards.insert(c);
        suitCount[c.getSuit()]++;
        rankCount[c.getRank()]++;
    }
    for (const Card& c : table) {
        cards.insert(c);
        suitCount[c.getSuit()]++;
        rankCount[c.getRank()]++;
    }

    bool royalFlush = false;
    int straightFlush = 1;
    int quads = 0;
    bool flush = false;
    int straight = 1;
    vector<int> trips;
    vector<int> pairs;

    // Flush
    Suit flushType{1};
    for (const auto& s : suitCount) {
        if (s.second >= 5) {
            flush = true;
            flushType = s.first;
            break;
        }
    }

    // One pair, two pairs, trips, full house and quad
    for (const auto& r : rankCount) {
        if (r.second == 4) {
            quads = (int)r.first;
            break;
        } else if (r.second == 3) {
            trips.push_back((int)r.first);
        } else if (r.second == 2) {
            pairs.push_back((int)r.first);
            if (pairs.size() == 3) {
                pairs.erase(pairs.begin());
            }
        }
    }

    // Straight
    int value = 0;
    if (cards.crbegin()->ace()) value = 1;
    for (const auto& c : cards) {
        if (c.value() == value + 1) {
            straight++;
            value++;
        } else if (c.value() != value) {
            if (straight >= 5) break;
            straight = 1;
            value = c.value();
        }
    }
    int lastStraightCard = value;

    // Royal and straight flush
    if (flush && straight >= 5) {
        value = 0;
        if (cards.crbegin()->ace() && cards.crbegin()->getSuit() == flushType) {
            value = 1;
        }
        for (const auto& c : cards) {
            if (c.value() == value + 1) {
                if (c.getSuit() == flushType) {
                    straightFlush++;
                    value++;
                    if (straightFlush >= 5 && c.ace()) {
                        royalFlush = true;
                        break;
                    }
                }
            } else if (c.value() != value) {
                straightFlush = 1;
                value = c.value();
            }
        }
    }

    // The rank of the hand
    if (royalFlush) {
        handRank = HandRank::royalFlush;
    } else if (straightFlush >= 5) {
        handRank = HandRank::straightFlush;
    } else if (quads) {
        handRank = HandRank::quads;
    } else if (pairs.size() >= 1 && trips.size() == 1 || trips.size() >= 2) {
        handRank = HandRank::fullHouse;
    } else if (flush) {
        handRank = HandRank::flush;
    } else if (straight >= 5) {
        handRank = HandRank::straight;
    } else if (trips.size() == 1) {
        handRank = HandRank::trips;
    } else if (pairs.size() >= 2) {
        handRank = HandRank::twoPair;
    } else if (pairs.size() == 1) {
        handRank = HandRank::onePair;
    } else {
        handRank = HandRank::highCard;
    }

    // Calculate the value of the hand
    if ((int)cards.size() == 7) {
        handValue = 0;
        switch (handRank) {
            case HandRank::highCard: {
                auto it = cards.cbegin();
                advance(it, 2);
                for (it; it != cards.cend(); it++) {
                    handValue += (int)pow(2, it->value());
                }
                break;
            }
            case HandRank::onePair: {
                int n = 0;
                handValue = pairs.front() * 2 * (int)pow(2, 15);
                for (auto it = cards.crbegin(); it != cards.crend(); it++) {
                    if (it->value() != pairs.front()) {
                        handValue += (int)pow(2, it->value());
                        n++;
                        if (n == 3) break;
                    }
                }
                break;
            }
            case HandRank::twoPair: {
                handValue = (int)pow(2, pairs.front()) * 2 +
                            (int)pow(2, pairs.back()) * 2;
                for (auto it = cards.crbegin(); it != cards.crend(); it++) {
                    if (it->value() != pairs.front() &&
                        it->value() != pairs.back()) {
                        handValue += it->value();
                        break;
                    }
                }
                break;
            }
            case HandRank::trips: {
                int n = 0;
                handValue = trips.front() * 3 * (int)pow(2, 15);
                for (auto it = cards.crbegin(); it != cards.crend(); it++) {
                    if (it->value() != trips.front()) {
                        handValue += (int)pow(2, it->value());
                        n++;
                        if (n == 2) break;
                    }
                }
                break;
            }
            case HandRank::straight: {
                handValue = lastStraightCard;
                break;
            }
            case HandRank::flush: {
                int n = 0;
                for (auto it = cards.crbegin(); it != cards.crend(); it++) {
                    if (it->getSuit() == flushType) {
                        handValue += (int)pow(2, it->value());
                        n++;
                        if (n == 5) break;
                    }
                }
                break;
            }
            case HandRank::fullHouse: {
                if (trips.size() == 2) {
                    handValue = trips.back() * 3 + trips.front() * 2;
                } else {
                    handValue = trips.back() * 3 + pairs.back() * 2;
                }
                break;
            }
            case HandRank::quads: {
                handValue = quads * 4;
                for (auto it = cards.crbegin(); it != cards.crend(); it++) {
                    if (it->value() != quads) {
                        handValue += it->value();
                        break;
                    }
                }
                break;
            }
            case HandRank::straightFlush: {
                handValue = lastStraightCard;
                break;
            }
            case HandRank::royalFlush: {
                handValue = 1;
                break;
            }
            default: {
                break;
            }
        }
    }
    return;
}

void Player::printTable() {
    cout << "Table: ";
    for (const Card& card : table) {
        cout << setw(6) << card;
    }
    cout << "\n\n";
}

Move Player::makeMove(int standingBet, Act standingAct) {
    printTable();
    cout << *this << "\n\n";
    int number = 0;
    int bet = 0;

    cout << "The standing bet is: " << standingBet << "\n";
    cout << "The previous action is: " << standingAct << "\n";
    cout << "My current bet in the pot: " << totalBet << "\n";

    while (status == Status::waiting) {
        cout << "Act: Fold(1), Check(2), Call(3), Bet(4), Raise(5), "
                "Re-raise(6) or All in(7): ";
        number = getIntInput(1, 7);
        action = Act{number};

        switch (action) {
            case Act::check: {
                if (standingAct != Act::check) {
                    cout << "Action not allowed!\n";
                    break;
                }
                status = Status::ready;
                break;
            }
            case Act::call: {
                if (standingAct == Act::check) {
                    cout << "Action not allowed!\n";
                    break;
                }
                bet = standingBet - totalBet;
                cash -= bet;
                totalBet += bet;
                status = Status::ready;
                break;
            }
            case Act::bet: {
                if (standingBet > 0) {
                    cout << "Action not allowed!\n";
                    break;
                }
                cout << "Size of your bet: ";
                bet = getIntInput(standingBet, cash);
                cash -= bet;
                totalBet = bet;
                status = Status::ready;
                break;
            }
            case Act::raise: {
                if (standingAct == Act::check || standingAct == Act::raise ||
                    standingAct == Act::reReise || standingAct == Act::allIn) {
                    cout << "Action not allowed!\n";
                    break;
                }
                cout << "Size of your raise: ";
                bet = getIntInput(standingBet, cash) + standingBet;
                cash -= bet - totalBet;
                totalBet = bet;
                status = Status::ready;
                break;
            }
            case Act::reReise: {
                if (standingAct == Act::check || standingAct == Act::bet) {
                    cout << "Action not allowed!\n";
                    break;
                }
                cout << "Size of your re-raise: ";
                bet = getIntInput(standingBet, cash) + standingBet;
                cash -= bet - totalBet;
                totalBet = bet;
                status = Status::ready;
                break;
            }
            case Act::allIn: {
                bet = cash;
                cash = 0;
                totalBet += bet;
                status = Status::ready;
                break;
            }
            default: {
                status = Status::ready;
                break;
            }
        }
    }
    if (cash == 0) action = Act::allIn;
    return Move{bet, action};
}

bool Player::operator<(const Player& rhs) const {
    if (handRank != rhs.handRank) {
        return handRank < rhs.handRank;
    } else {
        return handValue < rhs.handValue;
    }
}

bool Player::operator==(const Player& rhs) const {
    return (handRank == rhs.handRank && handValue == rhs.handValue);
}

void Player::reset() {
    handRank = HandRank::highCard;
    handValue = 0;
    action = Act::check;
    totalBet = 0;
    status = Status::waiting;
    hand.clear();
}

string getStringInput() {
    string input;
    cin >> input;
    cin.clear();
    cin.ignore(10000, '\n');
    return input;
}

int getIntInput(int min, int max) {
    for (;;) {
        string input = getStringInput();
        try {
            int number = stoi(input);
            if (number >= min && number <= max) {
                return number;
            } else {
                cout << "Number not in interval [" << min << ", " << max
                     << "], try again: ";
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }
}