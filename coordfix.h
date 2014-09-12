#ifndef COORDFIX_H
#define COORDFIX_H

#include "Strategy.h"

using namespace std;
using namespace model;

class CoordFix {
public:
    CoordFix();
    CoordFix(double width, double height, bool isOnLeft);

    double invX(double x) const;
    double invY(double y) const;
    double invAngle(double angle) const;
    double invDx(double dx) const;
    double invDy(double dy) const;

    double fixX(double x) const;
    double fixY(double y) const;
    double fixAngle(double angle) const;
    double fixDx(double dx) const;
    double fixDy(double dy) const;

private:
    double _w;
    double _h;
    bool _isOnLeft;
};

//

class MoveF {
public:
    MoveF();
    MoveF(Move* m, const CoordFix& fix);

    double getSpeedUp() const;
    void setSpeedUp(const double speedUp);
    double getTurn() const;
    void setTurn(const double turn);
    ActionType getAction() const;
    void setAction(const ActionType action);
    double getPassPower() const;
    void setPassPower(const double passPower);
    double getPassAngle() const;
    void setPassAngle(const double passAngle);
    int getTeammateIndex() const;
    void setTeammateIndex(const int teammateIndex);

private:
    Move* _move;
    CoordFix _fix;
};

//

class PlayerF : public Player {
public:
    PlayerF(const Player& p, const CoordFix& fix);

    double getNetLeft() const;
    double getNetRight() const;
    double getNetFront() const;
    double getNetBack() const;

private:
    CoordFix _fix;
};

//

class UnitF : public Unit {
public:
    UnitF(const Unit& u, const CoordFix& fix);

    double getX() const;
    double getY() const;
    double getSpeedX() const;
    double getSpeedY() const;
    double getAngle() const;
    double getAngleTo(double x, double y) const;
    double getDistanceTo(double x, double y) const;

private:
    CoordFix _fix;
};

//

class HockeyistF : public Hockeyist {
public:
    HockeyistF();
    HockeyistF(const Hockeyist& h, const CoordFix& fix);

    double getX() const;
    double getY() const;
    double getSpeedX() const;
    double getSpeedY() const;
    double getAngle() const;
    double getAngleTo(double x, double y) const;
    double getDistanceTo(double x, double y) const;

private:
    CoordFix _fix;
};

//

class PuckF : public Puck {
public:
    PuckF(const Puck& p, const CoordFix& fix);

    double getX() const;
    double getY() const;
    double getSpeedX() const;
    double getSpeedY() const;
    double getAngle() const;
    double getAngleTo(double x, double y) const;
    double getDistanceTo(double x, double y) const;

private:
    CoordFix _fix;
};

//

class WorldF : public World {
public:
    WorldF();
    WorldF(const World& w, const CoordFix& fix);

    const vector<PlayerF> getPlayers() const;
    PlayerF getMyPlayer() const;
    PlayerF getOpponentPlayer() const;

    const PuckF getPuck() const;

private:
    CoordFix _fix;
};

#endif // COORDFIX_H
