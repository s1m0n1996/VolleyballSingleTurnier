#include "player.h"
#include "QString"

player::player(const QString playerfirstname, const QString playername, const QString country, const QString birthday){
    _playerfirstname = playerfirstname;
    _playername = playername;
    _country = country;
    _birthday = birthday;
    if(isPlayerUnknown()){
        addPlayerToDatabank();
    }
}

player::player(const int id){

}

void player::addPlayerToDatabank(){

}


bool player::isPlayerUnknown(){

}
