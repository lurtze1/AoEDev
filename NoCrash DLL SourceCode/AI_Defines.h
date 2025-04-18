#pragma once

#ifndef AI_DEFINES_H
#define AI_DEFINES_H

/********************************************************************************/
/* 	BETTER_BTS_AI_MOD					03/08/10				jdog5000		*/
/* 																				*/
/* 																				*/
/********************************************************************************/
// Could increase this value now that player closness is fixed
#define DEFAULT_PLAYER_CLOSENESS 7

#define AI_DAGGER_THRESHOLD			100  //higher is a lower chance

// Military strategies based on power, war status
#define AI_DEFAULT_STRATEGY             (1 << 0)
#define AI_STRATEGY_DAGGER              (1 << 1)   // Aggressive early game
#define AI_STRATEGY_CRUSH				(1 << 2)   // Convert units to City Attack
#define AI_STRATEGY_ALERT1				(1 << 3)   // Likely attack from neighbor
#define AI_STRATEGY_ALERT2				(1 << 4)   // Seemingly immenient attack from enemy
#define AI_STRATEGY_TURTLE              (1 << 5)   // Defensive shell
#define AI_STRATEGY_LAST_STAND			(1 << 6)
#define AI_STRATEGY_FINAL_WAR			(1 << 7)
// Military strategies based on analysis of trainable units
#define AI_STRATEGY_GET_BETTER_UNITS	(1 << 8)
#define AI_STRATEGY_FASTMOVERS          (1 << 9)
#define AI_STRATEGY_LAND_BLITZ			(1 << 10)
#define AI_STRATEGY_AIR_BLITZ			(1 << 11)
#define AI_STRATEGY_OWABWNW				(1 << 12)
// Domestic strategies
#define AI_STRATEGY_PRODUCTION          (1 << 13)
#define AI_STRATEGY_MISSIONARY          (1 << 14)
#define AI_STRATEGY_BIG_ESPIONAGE		(1 << 15)
/*************************************************************************************************/
/**	AI Altar Victory				08/04/10								Snarko				**/
/**																								**/
/**							Convince the AI to go for altar victory								**/
/*************************************************************************************************/
#define AI_STRATEGY_BUILDING_VICTORY	(1 << 16)
/*************************************************************************************************/
/**	AI Altar Victory					END														**/
/*************************************************************************************************/


// AI victory stages
//#define AI_DEFAULT_VICTORY_STRATEGY     (1 << 0)
#define AI_VICTORY_SPACE				0
#define AI_VICTORY_CONQUEST				4
#define AI_VICTORY_CULTURE				8
#define AI_VICTORY_DOMINATION			12
#define AI_VICTORY_DIPLOMACY			16
#define AI_VICTORY_RELIGION				20
#define AI_VICTORY_TOWERMASTERY			24
#define AI_VICTORY_ALTAR				28
/*Snarko temp rewriting this stuff, commented out in case new code doesn't work.
#define AI_VICTORY_SPACE1				(1 << 1)
#define AI_VICTORY_SPACE2				(1 << 2)
#define AI_VICTORY_SPACE3				(1 << 3)
#define AI_VICTORY_SPACE4				(1 << 4)
#define AI_VICTORY_CONQUEST1			(1 << 5)
#define AI_VICTORY_CONQUEST2			(1 << 6)
#define AI_VICTORY_CONQUEST3			(1 << 7)
#define AI_VICTORY_CONQUEST4			(1 << 8)
#define AI_VICTORY_CULTURE1				(1 << 9)   //religions and wonders
#define AI_VICTORY_CULTURE2				(1 << 10)  //mass culture buildings
#define AI_VICTORY_CULTURE3				(1 << 11)  //culture slider
#define AI_VICTORY_CULTURE4				(1 << 12)
#define AI_VICTORY_DOMINATION1			(1 << 13)
#define AI_VICTORY_DOMINATION2			(1 << 14)
#define AI_VICTORY_DOMINATION3			(1 << 15)
#define AI_VICTORY_DOMINATION4			(1 << 16)
#define AI_VICTORY_DIPLOMACY1			(1 << 17)
#define AI_VICTORY_DIPLOMACY2			(1 << 18)
#define AI_VICTORY_DIPLOMACY3			(1 << 19)
#define AI_VICTORY_DIPLOMACY4			(1 << 20)

// Tholal AI - Added victories for FFH2
#define AI_VICTORY_RELIGION1			(1 << 21)
#define AI_VICTORY_RELIGION2			(1 << 22)
#define AI_VICTORY_RELIGION3			(1 << 23)
#define AI_VICTORY_RELIGION4			(1 << 24)
#define AI_VICTORY_TOWERMASTERY1		(1 << 25) //Tower victory is any victory where we need buildings that require a certain bonus.
#define AI_VICTORY_TOWERMASTERY2		(1 << 26)
#define AI_VICTORY_TOWERMASTERY3		(1 << 27)
#define AI_VICTORY_TOWERMASTERY4		(1 << 28)
#define AI_VICTORY_ALTAR1				(1 << 29) //Altar victory is any victory where we need buildings created by great people.
#define AI_VICTORY_ALTAR2				(1 << 30)
#define AI_VICTORY_ALTAR3				(1 << 31)
#define AI_VICTORY_ALTAR4				(1 << 32)
// End Tholal AI
*/

/********************************************************************************/
/* 	BETTER_BTS_AI_MOD							END							*/
/********************************************************************************/

#define AI_CITY_ROLE_VALID              (1 <<  1)    //zero is bad
#define AI_CITY_ROLE_BIG_CULTURE        (1 <<  2)    //culture victory, probably
#define AI_CITY_ROLE_BIG_PRODUCTION     (1 <<  3)    //don't build girly NW's
#define AI_CITY_ROLE_BIG_MILITARY       (1 <<  4)    //stick with military stuff
#define AI_CITY_ROLE_SCIENCE            (1 <<  5)    //
#define AI_CITY_ROLE_GOLD               (1 <<  6)    //
#define AI_CITY_ROLE_PRODUCTION         (1 <<  7)    //
#define AI_CITY_ROLE_SPECIALIST         (1 <<  8)    //
#define AI_CITY_ROLE_FISHING            (1 <<  9)    //
#define AI_CITY_ROLE_STAGING            (1 << 10)    //send troops here
#define AI_CITY_ROLE_LICHPIN            (1 << 11)    //this city must not fall

#endif // AI_DEFINES_H
