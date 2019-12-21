#include "city.hpp"

// Määritellään City. 
City::City(int time, int time_step, int activity)
	:time_(time), time_step_(time_step), activity_(activity) {
}
	
// Lisää uudet ajajat teille kutsumalla funktiota Add_to_queue kullekin tielle
void City::Add_drivers(int amount_new_people) {
	/*Ensimmäiseksi luodaan for-loop "i = 1:1: amount_new_people", joka käy jokaisen luotavan ihmisen läpi. 
	Yhden loopin aikana:
	1. generoidaan kaksi targettia
	2.  */

}
    
// Update_targets päivittää ajajien asiointiajat targeteissa ja siirtää ajajat tarvittaessa seuraavaan paikkaan
void City::Update_drivers_in_targets(){

}
    
// City komentaa kunkin tien Drive() - funktiota
void City::Command_to_drive(){

}
 
    
// Lisää ajaja vektoriin drivers_in_targets. Road tulee kutsumaan tätä funktiota.
void City::add_drivers_in_targets(Driver& driver, int time){

}
