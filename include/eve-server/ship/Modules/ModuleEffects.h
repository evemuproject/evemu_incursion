/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2006 - 2011 The EVEmu Team
	For the latest information visit http://evemu.org
	------------------------------------------------------------------------------------
	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2 of the License, or (at your option) any later
	version.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place - Suite 330, Boston, MA 02111-1307, USA, or go to
	http://www.gnu.org/copyleft/lesser.txt.
	------------------------------------------------------------------------------------
	Author:		Luck
*/

#ifndef MODULE_EFFECTS_H
#define MODULE_EFFECTS_H

#include "ModuleDB.h"


class MEffect
{
public:
	MEffect();
	~MEffect();

	void Populate(uint32 effectID);

	int * m_TargetAttributeIDs;
	int * m_SourceAttributeIDs;
	int * m_CalculationTypeID;
	int * m_ReverseCalculationTypeID;
    int * m_EffectAppliedWhenIDs;
    int * m_EffectAppliedTargetIDs;
	int m_numOfIDs;

	int m_EffectID;
	std::string m_EffectName;
	int m_EffectCategory;
	int m_PreExpression;
	int m_PostExpression;
	std::string m_Description;
	std::string m_Guid;
	int m_IconID;
	int m_IsOffensive;
	int m_IsAssistance;
	int m_DurationAttributeID;
	int m_TrackingSpeedAttributeID;
	int m_DischargeAttributeID;
	int m_RangeAttributeID;
	int m_FalloffAttributeID;
	int m_DisallowAutoRepeat;
	int m_Published;
	std::string m_DisplayName;
	int m_IsWarpSafe;
	int m_RangeChance;
	int m_ElectronicChance;
	int m_PropulsionChance;
	int m_Distribution;
	std::string m_SfxName;
	int m_NpcUsageChanceAttributeID;
	int m_NpcActivationChanceAttributeID;
	int m_FittingUsageChanceAttributeID;

};

//class contained by all modules that is populated on construction of the module
//this will contain all information about the effects of the module
class ModuleEffects
{
public:
	ModuleEffects(uint32 typeID);
	~ModuleEffects();

	//useful accessors - probably a better way to do this, but at least it's fast
	bool isHighSlot();
	bool isMediumSlot();
	bool isLowSlot();

	//this will need to be reworked to implement a singleton architecture...i'll do it later -luck

	//this class uses a system to set the "active effect" that you want to get information about
	//returns false if the effect specified is not found
	bool SetActiveEffect(uint32 effectID);
	bool SetDefaultEffectAsActive();

	//accessors for selected effect
	uint32 GetEffectID()										{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_EffectID; }
	std::string GetEffectName()									{ return (m_Effects == NULL) ? std::string("") : m_Effects[m_SelectedEffect].m_EffectName; }
	uint32 GetEffectCategory()									{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_EffectCategory; }
	uint32 GetPreExpression()									{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_PreExpression; }
	uint32 GetPostExpression()									{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_PostExpression; }
	std::string GetDescription()								{ return (m_Effects == NULL) ? std::string("") : m_Effects[m_SelectedEffect].m_Description; }
	std::string GetGuid()										{ return (m_Effects == NULL) ? std::string("") : m_Effects[m_SelectedEffect].m_Guid; }
	uint32 GetIconID()											{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_IconID; }
	bool GetIsOffensive()										{ return (m_Effects == NULL) ? false : m_Effects[m_SelectedEffect].m_IsOffensive == 1; }
	bool GetIsAssistance()										{ return (m_Effects == NULL) ? false : m_Effects[m_SelectedEffect].m_IsAssistance == 1; }
	uint32 GetDurationAttributeID()								{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_DurationAttributeID; }
	uint32 GetTrackingSpeedAttributeID()						{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_TrackingSpeedAttributeID; }
	uint32 GetDischargeAttributeID()							{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_DischargeAttributeID; }
	uint32 GetRangeAttributeID()								{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_RangeAttributeID; }
	uint32 GetFalloffAttributeID()								{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_FalloffAttributeID; }
	bool GetDisallowAutoRepeat()								{ return (m_Effects == NULL) ? false : m_Effects[m_SelectedEffect].m_DisallowAutoRepeat == 1; }
	bool GetIsPublished()										{ return (m_Effects == NULL) ? false : m_Effects[m_SelectedEffect].m_Published == 1; }
	std::string GetDisplayName()								{ return (m_Effects == NULL) ? std::string("") : m_Effects[m_SelectedEffect].m_DisplayName; }
	bool GetIsWarpSafe()										{ return (m_Effects == NULL) ? false : m_Effects[m_SelectedEffect].m_IsWarpSafe == 1; }
	bool GetRangeChance()										{ return (m_Effects == NULL) ? false : m_Effects[m_SelectedEffect].m_RangeChance == 1; }
	bool GetPropulsionChance()									{ return (m_Effects == NULL) ? false : m_Effects[m_SelectedEffect].m_PropulsionChance == 1; }
	bool GetElectronicChance()									{ return (m_Effects == NULL) ? false : m_Effects[m_SelectedEffect].m_ElectronicChance == 1; }
	uint32 GetDistribution()									{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_Distribution; }
	std::string GetSfxName()									{ return (m_Effects == NULL) ? std::string("") : m_Effects[m_SelectedEffect].m_DisplayName; }
	uint32 GetNpcUsageChanceAttributeID()						{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_NpcUsageChanceAttributeID; }
	uint32 GetNpcActivationChanceAttributeID()					{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_NpcActivationChanceAttributeID; }
	uint32 GetFittingUsageChanceAttributeID()					{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_FittingUsageChanceAttributeID; }

	//accessors for the effects targetAttributeID, sourceAttributeID and calculation type
	uint32 GetSizeOfAttributeList()								{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_numOfIDs; }
	uint32 GetTargetAttributeID(uint32 count)					{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_TargetAttributeIDs[count]; }
	uint32 GetSourceAttributeID(uint32 count)					{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_SourceAttributeIDs[count]; }
	EVECalculationType GetCalculationType(uint32 count)			{ return (m_Effects == NULL) ? (EVECalculationType)0 : (EVECalculationType)m_Effects[m_SelectedEffect].m_CalculationTypeID[count];}
	EVECalculationType GetReverseCalculationType(uint32 count)	{ return (m_Effects == NULL) ? (EVECalculationType)0 : (EVECalculationType)m_Effects[m_SelectedEffect].m_ReverseCalculationTypeID[count];}
	uint32 GetModuleStateWhenEffectApplied(uint32 count)		{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_EffectAppliedWhenIDs[count]; }
	uint32 GetTargetTypeToWhichEffectApplied(uint32 count)		{ return (m_Effects == NULL) ? 0 : m_Effects[m_SelectedEffect].m_EffectAppliedTargetIDs[count]; }

private:

	void _populate(uint32 typeID);

	//data members
	int m_TypeID;
	int *m_EffectIDs;
	MEffect * m_Effects;
	uint32 m_DefaultEffect;

	//internal counters
	uint32 m_EffectCount;
	uint32 m_SelectedEffect;

	//cached stuff
	bool m_HighPower, m_MediumPower, m_LowPower, m_Cached;

};

#endif /* MODULE_EFFECTS_H */
