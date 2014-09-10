#include "coordfix.h"

HockeyistF::HockeyistF() {
}

HockeyistF::HockeyistF(const model::Hockeyist& h) : model::Hockeyist(h) {
}

//

WorldF::WorldF() {
}

WorldF::WorldF(const model::World &w) : model::World(w) {
}

//

GameF::GameF() {
}

GameF::GameF(const model::Game& g) : model::Game(g) {
}

//

MoveF::MoveF() {
    _move = 0;
}

MoveF::MoveF(model::Move* m) {
    _move = m;
}

double MoveF::getSpeedUp() const {
    return _move->getSpeedUp();
}

void MoveF::setSpeedUp(const double speedUp) {
    _move->setSpeedUp(speedUp);
}

double MoveF::getTurn() const {
    return _move->getTurn();
}

void MoveF::setTurn(const double turn) {
    return _move->setTurn(turn);
}

model::ActionType MoveF::getAction() const {
    return _move->getAction();
}

void MoveF::setAction(const model::ActionType action) {
    _move->setAction(action);
}

double MoveF::getPassPower() const {
    return _move->getPassPower();
}

void MoveF::setPassPower(const double passPower) {
    _move->setPassPower(passPower);
}

double MoveF::getPassAngle() const {
    return _move->getPassAngle();
}

void MoveF::setPassAngle(const double passAngle) {
    _move->setPassAngle(passAngle);
}

int MoveF::getTeammateIndex() const {
    return _move->getTeammateIndex();
}

void MoveF::setTeammateIndex(const int teammateIndex) {
    _move->setTeammateIndex(teammateIndex);
}
