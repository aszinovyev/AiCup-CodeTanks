#ifndef COORDFIX_H
#define COORDFIX_H

#include "Strategy.h"

class CoordFix {
public:
    CoordFix();
    CoordFix(double width, double height, bool isOnLeft);

    double invX(double x);
    double invY(double y);
    double invAngle(double angle);

    double fixX(double x);
    double fixY(double y);
    double fixAngle(double angle);

private:
    double _w;
    double _h;
    bool _isOnLeft;
};

class HockeyistF : public model::Hockeyist {
public:
    HockeyistF();
    HockeyistF(const model::Hockeyist& h);
};

class WorldF : public model::World {
public:
    WorldF();
    WorldF(const model::World& w);
};

class GameF : public model::Game {
public:
    GameF();
    GameF(const model::Game& g);
};

class MoveF {
public:
    MoveF();
    MoveF(model::Move* m);

    double getSpeedUp() const;
    void setSpeedUp(const double speedUp);
    double getTurn() const;
    void setTurn(const double turn);
    model::ActionType getAction() const;
    void setAction(const model::ActionType action);
    double getPassPower() const;
    void setPassPower(const double passPower);
    double getPassAngle() const;
    void setPassAngle(const double passAngle);
    int getTeammateIndex() const;
    void setTeammateIndex(const int teammateIndex);

private:
    model::Move* _move;
};

#endif // COORDFIX_H
