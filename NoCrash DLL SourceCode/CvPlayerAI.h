#pragma once

// playerAI.h

#ifndef CIV4_PLAYER_AI_H
#define CIV4_PLAYER_AI_H

#include "CvPlayer.h"
#include "AI_defines.h"

class CvEventTriggerInfo;

class CvPlayerAI : public CvPlayer
{

public:

	CvPlayerAI();
	virtual ~CvPlayerAI();

  // inlined for performance reasons
#ifdef _USRDLL
  static CvPlayerAI& getPlayer(PlayerTypes ePlayer)
  {
	  FAssertMsg(ePlayer != NO_PLAYER, "Player is not assigned a valid value");
	  FAssertMsg(ePlayer < MAX_PLAYERS, "Player is not assigned a valid value");
	  return m_aPlayers[ePlayer];
  }
#endif
	DllExport static CvPlayerAI& getPlayerNonInl(PlayerTypes ePlayer);

	static void initStatics();
	static void freeStatics();
	DllExport static bool areStaticsInitialized();

	void AI_init();
	void AI_uninit();
	void AI_reset(bool bConstructor);

	int AI_getFlavorValue(FlavorTypes eFlavor) const;

	void AI_doTurnPre();
	void AI_doTurnPost();
	void AI_doTurnUnitsPre();
	void AI_doTurnUnitsPost();

	void AI_doPeace();

	void AI_updateFoundValues(bool bStartingLoc = false) const;
	void AI_updateAreaTargets();

	int AI_movementPriority(CvSelectionGroup* pGroup) const;
	void AI_unitUpdate();

	void AI_makeAssignWorkDirty();
	void AI_assignWorkingPlots();
	void AI_updateAssignWork();

	void AI_makeProductionDirty();

	void AI_conquerCity(CvCity* pCity);

	bool AI_acceptUnit(CvUnit* pUnit) const;
	bool AI_captureUnit(UnitTypes eUnit, CvPlot* pPlot) const;

	DomainTypes AI_unitAIDomainType(UnitAITypes eUnitAI) const;

	int AI_yieldWeight(YieldTypes eYield) const;
	int AI_commerceWeight(CommerceTypes eCommerce, CvCity* pCity = NULL) const;

	int AI_foundValue(int iX, int iY, int iMinRivalRange = -1, bool bStartingLoc = false) const;

	bool AI_isAreaAlone(CvArea* pArea) const;
	bool AI_isCapitalAreaAlone() const;
	bool AI_isPrimaryArea(CvArea* pArea) const;

	int AI_militaryWeight(CvArea* pArea) const;

	int AI_targetCityValue(CvCity* pCity, bool bRandomize, bool bIgnoreAttackers = false) const;
	CvCity* AI_findTargetCity(CvArea* pArea) const;

	bool AI_isCommercePlot(CvPlot* pPlot) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      08/20/09                                jdog5000      */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/

	bool AI_getAnyPlotDanger(CvPlot* pPlot, int iRange = -1, bool bTestMoves = true) const;
/*************************************************************************************************/
/**	Speedup								11/02/12										Snarko	**/
/**																								**/
/**		For the few cases where getPlotDanger is still used, use a threshold where possible		**/
/*************************************************************************************************/
/**								---- Start Original Code ----									**
	int AI_getPlotDanger(CvPlot* pPlot, int iRange = -1, bool bTestMoves = true) const;
/**								----  End Original Code  ----									**/
	int AI_getPlotDanger(CvPlot* pPlot, int iRange = -1, bool bTestMoves = true, int iThreshold = 50) const; //Default to 50 because we almost never want to check beyond that, too slow and 50 = VERY FREAKING DANGEROUS.
/*************************************************************************************************/
/**	Speedup									END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	Unit power						18/02/12										Snarko		**/
/**																								**/
/**							Rewriting unit power system											**/
/*************************************************************************************************/
	int AI_getEnemyPower(CvPlot* pPlot, int iRange = -1, bool bTestMoves = true, int iThreshold = MAX_INT) const;
/*************************************************************************************************/
/**	Unit power						END															**/
/*************************************************************************************************/
	//int AI_getUnitDanger(CvUnit* pUnit, int iRange = -1, bool bTestMoves = true, bool bAnyDanger = true) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	int AI_getWaterDanger(CvPlot* pPlot, int iRange, bool bTestMoves = true) const;

	bool AI_avoidScience() const;
	bool AI_isFinancialTrouble() const;
/*************************************************************************************************/
/**	Improved AI							16/06/10										Snarko	**/
/**																								**/
/**						Financial trouble is not a simple yes/no.								**/
/*************************************************************************************************/
	int AI_getFinancialTrouble() const;
/*************************************************************************************************/
/**	Improved AI								END													**/
/*************************************************************************************************/
	int AI_goldTarget() const;
/*************************************************************************************************/
/**	Improved AI							24/07/10										Snarko	**/
/**																								**/
/**								Teaching AI to tech better										**/
/*************************************************************************************************/
/**								---- Start Original Code ----									**
	TechTypes AI_bestTech(int iMaxPathLength = 1, bool bIgnoreCost = false, bool bAsync = false, TechTypes eIgnoreTech = NO_TECH, AdvisorTypes eIgnoreAdvisor = NO_ADVISOR);
/**								----  End Original Code  ----									**/
	TechTypes AI_bestTech(int iMaxPathLength = 1, bool bIgnoreCost = false, bool bAsync = false, TechTypes eIgnoreTech = NO_TECH, AdvisorTypes eIgnoreAdvisor = NO_ADVISOR, bool bResearch = false);
	int AI_techValue(TechTypes eTech, int* aiTechValues, int* iNumTechs);
	bool AI_researchTech(TechTypes eTech, int* aiTechValues);
/*************************************************************************************************/
/**	Improved AI								END													**/
/*************************************************************************************************/
	void AI_chooseFreeTech();
	void AI_chooseResearch();

	DllExport DiploCommentTypes AI_getGreeting(PlayerTypes ePlayer) const;
	bool AI_isWillingToTalk(PlayerTypes ePlayer) const;
	bool AI_demandRebukedSneak(PlayerTypes ePlayer) const;
	bool AI_demandRebukedWar(PlayerTypes ePlayer) const;
	bool AI_hasTradedWithTeam(TeamTypes eTeam) const;

	AttitudeTypes AI_getAttitude(PlayerTypes ePlayer, bool bForced = true) const;
	int AI_getAttitudeVal(PlayerTypes ePlayer, bool bForced = true) const;
	static AttitudeTypes AI_getAttitudeFromValue(int iAttitudeVal);

	int AI_calculateStolenCityRadiusPlots(PlayerTypes ePlayer) const;
	int AI_getCloseBordersAttitude(PlayerTypes ePlayer) const;

	int AI_getWarAttitude(PlayerTypes ePlayer) const;
	int AI_getPeaceAttitude(PlayerTypes ePlayer) const;
	int AI_getSameReligionAttitude(PlayerTypes ePlayer) const;
	int AI_getDifferentReligionAttitude(PlayerTypes ePlayer) const;
	int AI_getBonusTradeAttitude(PlayerTypes ePlayer) const;
	int AI_getOpenBordersAttitude(PlayerTypes ePlayer) const;
	int AI_getDefensivePactAttitude(PlayerTypes ePlayer) const;
	int AI_getRivalDefensivePactAttitude(PlayerTypes ePlayer) const;
	int AI_getRivalVassalAttitude(PlayerTypes ePlayer) const;
	int AI_getShareWarAttitude(PlayerTypes ePlayer) const;
	int AI_getFavoriteCivicAttitude(PlayerTypes ePlayer) const;
	int AI_getTradeAttitude(PlayerTypes ePlayer) const;
	int AI_getRivalTradeAttitude(PlayerTypes ePlayer) const;
	int AI_getMemoryAttitude(PlayerTypes ePlayer, MemoryTypes eMemory) const;
	int AI_getColonyAttitude(PlayerTypes ePlayer) const;

	PlayerVoteTypes AI_diploVote(const VoteSelectionSubData& kVoteData, VoteSourceTypes eVoteSource, bool bPropose);

	int AI_dealVal(PlayerTypes ePlayer, const CLinkList<TradeData>* pList, bool bIgnoreAnnual = false, int iExtra = 1) const;
	bool AI_goldDeal(const CLinkList<TradeData>* pList) const;
	bool AI_considerOffer(PlayerTypes ePlayer, const CLinkList<TradeData>* pTheirList, const CLinkList<TradeData>* pOurList, int iChange = 1) const;
	bool AI_counterPropose(PlayerTypes ePlayer, const CLinkList<TradeData>* pTheirList, const CLinkList<TradeData>* pOurList, CLinkList<TradeData>* pTheirInventory, CLinkList<TradeData>* pOurInventory, CLinkList<TradeData>* pTheirCounter, CLinkList<TradeData>* pOurCounter) const;

	DllExport int AI_maxGoldTrade(PlayerTypes ePlayer) const;

	DllExport int AI_maxGoldPerTurnTrade(PlayerTypes ePlayer) const;
	int AI_goldPerTurnTradeVal(int iGoldPerTurn) const;

	int AI_bonusVal(BonusTypes eBonus, int iChange = 1) const;
	int AI_baseBonusVal(BonusTypes eBonus) const;
	int AI_bonusTradeVal(BonusTypes eBonus, PlayerTypes ePlayer, int iChange) const;
	DenialTypes AI_bonusTrade(BonusTypes eBonus, PlayerTypes ePlayer) const;
	int AI_corporationBonusVal(BonusTypes eBonus) const;

	int AI_cityTradeVal(CvCity* pCity) const;
	DenialTypes AI_cityTrade(CvCity* pCity, PlayerTypes ePlayer) const;

	int AI_stopTradingTradeVal(TeamTypes eTradeTeam, PlayerTypes ePlayer) const;
	DenialTypes AI_stopTradingTrade(TeamTypes eTradeTeam, PlayerTypes ePlayer) const;

	int AI_civicTradeVal(CivicTypes eCivic, PlayerTypes ePlayer) const;
	DenialTypes AI_civicTrade(CivicTypes eCivic, PlayerTypes ePlayer) const;

	int AI_religionTradeVal(ReligionTypes eReligion, PlayerTypes ePlayer) const;
	DenialTypes AI_religionTrade(ReligionTypes eReligion, PlayerTypes ePlayer) const;

	int AI_unitImpassableCount(UnitTypes eUnit) const;
	int AI_unitValue(UnitTypes eUnit, UnitAITypes eUnitAI, CvArea* pArea) const;
	int AI_totalUnitAIs(UnitAITypes eUnitAI) const;
	int AI_totalAreaUnitAIs(CvArea* pArea, UnitAITypes eUnitAI) const;
	int AI_totalWaterAreaUnitAIs(CvArea* pArea, UnitAITypes eUnitAI) const;
	int AI_countCargoSpace(UnitAITypes eUnitAI) const;

	int AI_neededExplorers(CvArea* pArea) const;
	int AI_neededWorkers(CvArea* pArea) const;
	int AI_neededMissionaries(CvArea* pArea, ReligionTypes eReligion) const;
	int AI_neededExecutives(CvArea* pArea, CorporationTypes eCorporation) const;

	int AI_missionaryValue(CvArea* pArea, ReligionTypes eReligion, PlayerTypes* peBestPlayer = NULL) const;
	int AI_executiveValue(CvArea* pArea, CorporationTypes eCorporation, PlayerTypes* peBestPlayer = NULL) const;

	int AI_corporationValue(CorporationTypes eCorporation, CvCity* pCity = NULL) const;

	int AI_adjacentPotentialAttackers(CvPlot* pPlot, bool bTestCanMove = false) const;
	int AI_totalMissionAIs(MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_areaMissionAIs(CvArea* pArea, MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_plotTargetMissionAIs(CvPlot* pPlot, MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL, int iRange = 0) const;
	int AI_plotTargetMissionAIs(CvPlot* pPlot, MissionAITypes eMissionAI, int& iClosestTargetRange, CvSelectionGroup* pSkipSelectionGroup = NULL, int iRange = 0) const;
	int AI_plotTargetMissionAIs(CvPlot* pPlot, MissionAITypes* aeMissionAI, int iMissionAICount, int& iClosestTargetRange, CvSelectionGroup* pSkipSelectionGroup = NULL, int iRange = 0) const;
	int AI_unitTargetMissionAIs(CvUnit* pUnit, MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_unitTargetMissionAIs(CvUnit* pUnit, MissionAITypes* aeMissionAI, int iMissionAICount, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_enemyTargetMissionAIs(MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_enemyTargetMissionAIs(MissionAITypes* aeMissionAI, int iMissionAICount, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_wakePlotTargetMissionAIs(CvPlot* pPlot, MissionAITypes eMissionAI, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      05/19/10                                jdog5000      */
/*                                                                                              */
/* General AI                                                                                   */
/************************************************************************************************/
	int AI_cityTargetUnitsByPath(CvCity* pCity, CvSelectionGroup* pSkipSelectionGroup, int iMaxPathTurns) const;
	int AI_enemyTargetMissions(TeamTypes eTargetTeam, CvSelectionGroup* pSkipSelectionGroup = NULL) const;
	int AI_unitTargetMissionAIs(CvUnit* pUnit, MissionAITypes* aeMissionAI, int iMissionAICount, CvSelectionGroup* pSkipSelectionGroup, int iMaxPathTurns) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      07/19/09                                jdog5000      */
/*                                                                                              */
/* Civic AI                                                                                     */
/************************************************************************************************/
	CivicTypes AI_bestCivic(CivicOptionTypes eCivicOption, int* iBestValue) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/
	CivicTypes AI_bestCivic(CivicOptionTypes eCivicOption) const;
	int AI_civicValue(CivicTypes eCivic) const;

	ReligionTypes AI_bestReligion() const;
	int AI_religionValue(ReligionTypes eReligion) const;

	EspionageMissionTypes AI_bestPlotEspionage(CvPlot* pSpyPlot, PlayerTypes& eTargetPlayer, CvPlot*& pPlot, int& iData) const;
	int AI_espionageVal(PlayerTypes eTargetPlayer, EspionageMissionTypes eMission, CvPlot* pPlot, int iData) const;

	int AI_getPeaceWeight() const;
	void AI_setPeaceWeight(int iNewValue);

	int AI_getEspionageWeight() const;
	void AI_setEspionageWeight(int iNewValue);

	int AI_getAttackOddsChange() const;
	void AI_setAttackOddsChange(int iNewValue);

	int AI_getCivicTimer() const;
	void AI_setCivicTimer(int iNewValue);
	void AI_changeCivicTimer(int iChange);

	int AI_getReligionTimer() const;
	void AI_setReligionTimer(int iNewValue);
	void AI_changeReligionTimer(int iChange);

	int AI_getExtraGoldTarget() const;
	void AI_setExtraGoldTarget(int iNewValue);

	int AI_getNumTrainAIUnits(UnitAITypes eIndex) const;
	void AI_changeNumTrainAIUnits(UnitAITypes eIndex, int iChange);

	int AI_getNumAIUnits(UnitAITypes eIndex) const;
	void AI_changeNumAIUnits(UnitAITypes eIndex, int iChange);

	int AI_getSameReligionCounter(PlayerTypes eIndex) const;
	void AI_changeSameReligionCounter(PlayerTypes eIndex, int iChange);

	int AI_getDifferentReligionCounter(PlayerTypes eIndex) const;
	void AI_changeDifferentReligionCounter(PlayerTypes eIndex, int iChange);

	int AI_getFavoriteCivicCounter(PlayerTypes eIndex) const;
	void AI_changeFavoriteCivicCounter(PlayerTypes eIndex, int iChange);

	int AI_getBonusTradeCounter(PlayerTypes eIndex) const;
	void AI_changeBonusTradeCounter(PlayerTypes eIndex, int iChange);

	int AI_getPeacetimeTradeValue(PlayerTypes eIndex) const;
	void AI_changePeacetimeTradeValue(PlayerTypes eIndex, int iChange);

	int AI_getPeacetimeGrantValue(PlayerTypes eIndex) const;
	void AI_changePeacetimeGrantValue(PlayerTypes eIndex, int iChange);

	int AI_getGoldTradedTo(PlayerTypes eIndex) const;
	void AI_changeGoldTradedTo(PlayerTypes eIndex, int iChange);

	int AI_getAttitudeExtra(PlayerTypes eIndex) const;
	void AI_setAttitudeExtra(PlayerTypes eIndex, int iNewValue);
	void AI_changeAttitudeExtra(PlayerTypes eIndex, int iChange);

	bool AI_isFirstContact(PlayerTypes eIndex) const;
	void AI_setFirstContact(PlayerTypes eIndex, bool bNewValue);

	int AI_getContactTimer(PlayerTypes eIndex1, ContactTypes eIndex2) const;
	void AI_changeContactTimer(PlayerTypes eIndex1, ContactTypes eIndex2, int iChange);

	int AI_getMemoryCount(PlayerTypes eIndex1, MemoryTypes eIndex2) const;
	void AI_changeMemoryCount(PlayerTypes eIndex1, MemoryTypes eIndex2, int iChange);

	int AI_calculateGoldenAgeValue() const;

	void AI_doCommerce();

	EventTypes AI_chooseEvent(int iTriggeredId) const;
	virtual void AI_launch(VictoryTypes eVictory);

	int AI_getCultureVictoryStage(VictoryTypes eVictory) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      03/17/10                                jdog5000      */
/*                                                                                              */
/* Victory Strategy AI                                                                          */
/************************************************************************************************/
	int AI_getSpaceVictoryStage(VictoryTypes eVictory) const;
	int AI_getConquestVictoryStage(VictoryTypes eVictory) const;
	int AI_getDominationVictoryStage(VictoryTypes eVictory) const;
	int AI_getDiplomacyVictoryStage(VictoryTypes eVictory) const;
	bool AI_isDoVictoryStrategy(int iVictoryStrategy, int iLevel) const;
	bool AI_isDoVictoryStrategyLevel4() const;
	bool AI_isDoVictoryStrategyLevel3() const;
	void AI_forceUpdateVictoryStrategies();
	int AI_getVictoryStrategyHash() const;
	int AI_getVictoryHash(int iStart, int iNum) const;
	int AI_getStrategyRand() const;

	// Tholal AI - Added victories for FFH2
	int AI_getReligionVictoryStage(VictoryTypes eVictory) const;
	int AI_getTowerMasteryVictoryStage(VictoryTypes eVictory) const;
	int AI_getAltarVictoryStage(VictoryTypes eVictory) const;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	int AI_cultureVictoryTechValue(TechTypes eTech) const;

	bool AI_isDoStrategy(int iStrategy) const;
	void AI_forceUpdateStrategies();

	void AI_nowHasTech(TechTypes eTech);

	int AI_countDeadlockedBonuses(CvPlot* pPlot) const;

	int AI_getOurPlotStrength(CvPlot* pPlot, int iRange, bool bDefensiveBonuses, bool bTestMoves) const;
	int AI_getEnemyPlotStrength(CvPlot* pPlot, int iRange, bool bDefensiveBonuses, bool bTestMoves) const;

	int AI_goldToUpgradeAllUnits(int iExpThreshold = 0) const;

	int AI_goldTradeValuePercent() const;

	int AI_averageYieldMultiplier(YieldTypes eYield) const;
	int AI_averageCommerceMultiplier(CommerceTypes eCommerce) const;
	int AI_averageGreatPeopleMultiplier() const;
	int AI_averageCommerceExchange(CommerceTypes eCommerce) const;

	int AI_playerCloseness(PlayerTypes eIndex, int iMaxDistance) const;

	int AI_getTotalCityThreat() const;
	int AI_getTotalFloatingDefenseNeeded() const;


	int AI_getTotalAreaCityThreat(CvArea* pArea) const;
	int AI_countNumAreaHostileUnits(CvArea* pArea, bool bPlayer, bool bTeam, bool bNeutral, bool bHostile) const;
	int AI_getTotalFloatingDefendersNeeded(CvArea* pArea) const;
	int AI_getTotalFloatingDefenders(CvArea* pArea) const;

	RouteTypes AI_bestAdvancedStartRoute(CvPlot* pPlot, int* piYieldValue = NULL) const;
	UnitTypes AI_bestAdvancedStartUnitAI(CvPlot* pPlot, UnitAITypes eUnitAI) const;
	CvPlot* AI_advancedStartFindCapitalPlot() const;

	bool AI_advancedStartPlaceExploreUnits(bool bLand);
	void AI_advancedStartRevealRadius(CvPlot* pPlot, int iRadius);
	bool AI_advancedStartPlaceCity(CvPlot* pPlot);
	bool AI_advancedStartDoRoute(CvPlot* pFromPlot, CvPlot* pToPlot);
	void AI_advancedStartRouteTerritory();
	void AI_doAdvancedStart(bool bNoExit = false);

	int AI_getMinFoundValue() const;

	void AI_recalculateFoundValues(int iX, int iY, int iInnerRadius, int iOuterRadius) const;

	void AI_updateCitySites(int iMinFoundValueThreshold, int iMaxSites) const;
	void AI_invalidateCitySites(int iMinFoundValueThreshold) const;
	bool AI_isPlotCitySite(CvPlot* pPlot) const;
	int AI_getNumAreaCitySites(int iAreaID, int& iBestValue) const;
	int AI_getNumAdjacentAreaCitySites(int iWaterAreaID, int iExcludeArea, int& iBestValue) const;

	int AI_getNumCitySites() const;
	CvPlot* AI_getCitySite(int iIndex) const;

	int AI_bestAreaUnitAIValue(UnitAITypes eUnitAI, CvArea* pArea, UnitTypes* peBestUnitType = NULL) const;
	int AI_bestCityUnitAIValue(UnitAITypes eUnitAI, CvCity* pCity, UnitTypes* peBestUnitType = NULL) const;

	int AI_calculateTotalBombard(DomainTypes eDomain) const;

	int AI_getUnitClassWeight(UnitClassTypes eUnitClass) const;
	int AI_getUnitCombatWeight(UnitCombatTypes eUnitCombat) const;
/*************************************************************************************************/
/**	Xienwolf Tweak							03/27/09											**/
/**																								**/
/**									Accounts for Assimilation									**/
/*************************************************************************************************/
/**								---- Start Original Code ----									**
	int AI_calculateUnitAIViability(UnitAITypes eUnitAI, DomainTypes eDomain) const;
/**								----  End Original Code  ----									**/
	int AI_calculateUnitAIViability(UnitAITypes eUnitAI, DomainTypes eDomain, CivilizationTypes eCivilization = NO_CIVILIZATION) const;
/*************************************************************************************************/
/**	Tweak									END													**/
/*************************************************************************************************/

	void AI_updateBonusValue();
	void AI_updateBonusValue(BonusTypes eBonus);

	int AI_getAttitudeWeight(PlayerTypes ePlayer) const;

	ReligionTypes AI_chooseReligion();

	int AI_getPlotAirbaseValue(CvPlot* pPlot) const;
	int AI_getPlotCanalValue(CvPlot* pPlot) const;

	bool AI_isPlotThreatened(CvPlot* pPlot, int iRange = -1, bool bTestMoves = true) const;

	bool AI_isFirstTech(TechTypes eTech) const;

//FfH: Added by Kael 08/15/2007
	int AI_getAlignmentAttitude(PlayerTypes ePlayer) const;
/*************************************************************************************************/
/**	Lawful-Chaotic Alignments 				11/06/09								Valkrionn	**/
/**																								**/
/**							Adds a new alignment axis to the game								**/
/*************************************************************************************************/
	int AI_getEthicalAlignmentAttitude(PlayerTypes ePlayer) const;
/*************************************************************************************************/
/**	Lawful-Chaotic Alignments					END												**/
/*************************************************************************************************/
	int AI_getBadBonusAttitude(PlayerTypes ePlayer) const;
	int AI_getCompassionHighAttitude(PlayerTypes ePlayer) const;
	int AI_getCompassionLowAttitude(PlayerTypes ePlayer) const;
	int AI_getFavoriteWonderAttitude(PlayerTypes ePlayer) const;
	int AI_getGenderAttitude(PlayerTypes ePlayer) const;
	int AI_getTrustAttitude(PlayerTypes ePlayer) const;
	int AI_getCivicShareAttitude(PlayerTypes ePlayer) const;
	int AI_trueCombatValue(UnitTypes eUnit) const;
	int AI_combatValue(UnitTypes eUnit) const;
//FfH: End Add

/*************************************************************************************************/
/**	HatedCivic		Opera		31.05.09														**/
/*************************************************************************************************/
	int AI_getHatedCivicAttitude(PlayerTypes ePlayer) const;
	int AI_getHatedCivicCounter(PlayerTypes eIndex) const;
	void AI_changeHatedCivicCounter(PlayerTypes eIndex, int iChange) const;
/*************************************************************************************************/
/** End																								**/
/*************************************************************************************************/

/*************************************************************************************************/
/** BonusAttitudeModifier           Opera           30.07.09                                    **/
/*************************************************************************************************/
	int AI_getBonusAttitudeModifier(PlayerTypes ePlayer) const;
/*************************************************************************************************/
/** End                                                                                         **/
/*************************************************************************************************/

/*************************************************************************************************/
/**	New Tag Defs	(PlayerInfos)			09/01/08								Xienwolf	**/
/**																								**/
/**								Defines Function for Use in .cpp								**/
/*************************************************************************************************/
	void AI_setProximityDiploLastAdjust(PlayerTypes eIndex1);
	int AI_getProximityDiploLastAdjust(PlayerTypes eIndex1) const;
	void AI_changeProximityDiplo(PlayerTypes eIndex1, float fChange);
	float AI_getProximityDiplo(PlayerTypes eIndex1) const;
/*************************************************************************************************/
/**	New Tag Defs							END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	AITweak								15/07/10										Snarko	**/
/**																								**/
/**			Teaching AI which units it will be able to build and which it will not				**/
/*************************************************************************************************/
	bool AI_canTrainWithTech(UnitTypes eUnit);
	int AI_getMagicInterest() const;
	bool AI_isLandWar(CvArea* pArea) const;
/*************************************************************************************************/
/**	AITweak									END													**/
/*************************************************************************************************/
/*************************************************************************************************/
/**	AI anti barb force				12/04/11											Snarko	**/
/**					Helping the AI take out barbs, one unit at the time...						**/
/*************************************************************************************************/
	int AI_getAntiBarbGroupSize(CvArea* pArea) const;
/*************************************************************************************************/
/**	AI anti barb force						END													**/
/*************************************************************************************************/
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      09/03/09                       poyuzhe & jdog5000     */
/*                                                                                              */
/* Efficiency                                                                                   */
/************************************************************************************************/
	// From Sanguo Mod Performance, ie the CAR Mod
	// Attitude cache
	void AI_invalidateAttitudeCache(PlayerTypes ePlayer);
	void AI_invalidateAttitudeCache();
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	// for serialization
  virtual void read(FDataStreamBase* pStream);
  virtual void write(FDataStreamBase* pStream);

protected:

	static CvPlayerAI* m_aPlayers;

	int m_iPeaceWeight;
	int m_iEspionageWeight;
	int m_iAttackOddsChange;
	int m_iCivicTimer;
	int m_iReligionTimer;
	int m_iExtraGoldTarget;

	mutable int m_iStrategyHash;
	mutable int m_iStrategyHashCacheTurn;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      03/18/10                                jdog5000      */
/*                                                                                              */
/* Victory Strategy AI                                                                          */
/************************************************************************************************/
	mutable int m_iStrategyRand;
	mutable int m_iVictoryStrategyHash;
	mutable int m_iVictoryStrategyHashCacheTurn;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	mutable int m_iAveragesCacheTurn;

	mutable int m_iAverageGreatPeopleMultiplier;

	mutable int *m_aiAverageYieldMultiplier;
	mutable int *m_aiAverageCommerceMultiplier;
	mutable int *m_aiAverageCommerceExchange;

	mutable int m_iUpgradeUnitsCacheTurn;
	mutable int m_iUpgradeUnitsCachedExpThreshold;
	mutable int m_iUpgradeUnitsCachedGold;


	int *m_aiNumTrainAIUnits;
	int *m_aiNumAIUnits;
	int* m_aiSameReligionCounter;
	int* m_aiDifferentReligionCounter;
	int* m_aiFavoriteCivicCounter;
/*************************************************************************************************/
/** HatedCivic		Opera		31.05.09														**/
/*************************************************************************************************/
	int* m_aiHatedCivicCounter;
/*************************************************************************************************/
/** End																								**/
/*************************************************************************************************/
	int* m_aiBonusTradeCounter;
	int* m_aiPeacetimeTradeValue;
	int* m_aiPeacetimeGrantValue;
	int* m_aiGoldTradedTo;
	int* m_aiAttitudeExtra;
	int* m_aiBonusValue;
	int* m_aiUnitClassWeights;
	int* m_aiUnitCombatWeights;
/*************************************************************************************************/
/**	New Tag Defs	(PlayerInfos)			09/01/08								Xienwolf	**/
/**																								**/
/**								Defines Variable for Use in .cpp								**/
/*************************************************************************************************/
	float* m_afProximityDiploValue;
	int* m_aiProximityDiploLastAdjust;
/*************************************************************************************************/
/**	New Tag Defs							END													**/
/*************************************************************************************************/

	mutable int* m_aiCloseBordersAttitudeCache;

/************************************************************************************************/
/* BETTER_BTS_AI_MOD                      09/03/09                       poyuzhe & jdog5000     */
/*                                                                                              */
/* Efficiency                                                                                   */
/************************************************************************************************/
	// From Sanguo Mod Performance, ie the CAR Mod
	// Attitude cache
	mutable int* m_aiAttitudeCache;
/************************************************************************************************/
/* BETTER_BTS_AI_MOD                       END                                                  */
/************************************************************************************************/

	bool* m_abFirstContact;

	int** m_aaiContactTimer;
	int** m_aaiMemoryCount;

	mutable std::vector<int> m_aiAICitySites;

	bool m_bWasFinancialTrouble;
	int m_iTurnLastProductionDirty;

	void AI_doCounter();
	void AI_doMilitary();
	void AI_doResearch();
	void AI_doCivics();
	void AI_doReligion();
	void AI_doDiplo();
	void AI_doSplit();
	void AI_doCheckFinancialTrouble();

	bool AI_disbandUnit(int iExpThreshold, bool bObsolete);

	int AI_getStrategyHash() const;
	void AI_calculateAverages() const;

	int AI_getHappinessWeight(int iHappy, int iExtraPop) const;
	int AI_getHealthWeight(int iHealth, int iExtraPop) const;

	void AI_convertUnitAITypesForCrush();
	int AI_eventValue(EventTypes eEvent, const EventTriggeredData& kTriggeredData) const;

	void AI_doEnemyUnitData();
	void AI_invalidateCloseBordersAttitudeCache();

	friend class CvGameTextMgr;
};

// helper for accessing static functions
#ifdef _USRDLL
#define GET_PLAYER CvPlayerAI::getPlayer
#else
#define GET_PLAYER CvPlayerAI::getPlayerNonInl
#endif

#endif
