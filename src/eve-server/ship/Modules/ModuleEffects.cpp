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

#include "EVEServerPCH.h"
//#include "ModuleDB.h"


// ////////////////// MEffect Class ///////////////////////////

MEffect::MEffect()
{
    /* nothing to do */
}

MEffect::~MEffect()
{
    /* nothing to do */
}

void MEffect::Populate(uint32 effectID)
{
	DBQueryResult *res = new DBQueryResult();
	ModuleDB::GetDgmEffects(effectID, *res);

	DBResultRow row1;
	if( !res->GetRow(row1) )
		sLog.Error("MEffect","Could not populate effect information for effectID: %u", effectID);
	else
	{
		//get all the data from the query
		m_EffectID = effectID;
		m_EffectName = row1.GetText(0);
		m_EffectCategory = row1.GetInt(1);
		m_PreExpression = row1.GetInt(2);
		m_PostExpression = row1.GetInt(3);
		if( !row1.IsNull(4) )
			m_Description = row1.GetText(4);
		if( !row1.IsNull(5) )
			m_Guid = row1.GetText(5);
		if( !row1.IsNull(6) )
			m_IconID = row1.GetInt(6);
		m_IsOffensive = row1.GetInt(7);
		m_IsAssistance = row1.GetInt(8);
		if( !row1.IsNull(9) )
			m_DurationAttributeID = row1.GetInt(9);
		if( !row1.IsNull(10) )
			m_TrackingSpeedAttributeID = row1.GetInt(10);
		if( !row1.IsNull(11) )
			m_DischargeAttributeID = row1.GetInt(11);
		if( !row1.IsNull(12) )
			m_RangeAttributeID = row1.GetInt(12);
		if( !row1.IsNull(13) )
			m_FalloffAttributeID = row1.GetInt(13);
		if( !row1.IsNull(14) )
			m_DisallowAutoRepeat = row1.GetInt(14);
		m_Published = row1.GetInt(15);
		if( !row1.IsNull(16) )
			m_DisplayName = row1.GetText(16);
		m_IsWarpSafe = row1.GetInt(17);
		m_RangeChance = row1.GetInt(18);
		m_ElectronicChance = row1.GetInt(19);
		m_PropulsionChance = row1.GetInt(20);
		if( !row1.IsNull(21) )
			m_Distribution = row1.GetInt(21);
		if( !row1.IsNull(22) )
			m_SfxName = row1.GetText(22);
		if( !row1.IsNull(23) )
			m_NpcUsageChanceAttributeID = row1.GetInt(23);
		if( !row1.IsNull(24) )
			m_NpcActivationChanceAttributeID = row1.GetInt(24);
		if( !row1.IsNull(25) )
			m_FittingUsageChanceAttributeID = row1.GetInt(25);
	}

	//next get the info from the dgmEffectsInfo table
	ModuleDB::GetDgmEffectsInfo(effectID, *res);

	DBResultRow row2;

	//initialize the new tables
	m_TargetAttributeIDs = new int[res->GetRowCount()];
	m_SourceAttributeIDs = new int[res->GetRowCount()];
	m_CalculationTypeID = new int[res->GetRowCount()];
	m_ReverseCalculationTypeID = new int[res->GetRowCount()];
    m_EffectAppliedWhenIDs = new int[res->GetRowCount()];
    m_EffectAppliedTargetIDs = new int[res->GetRowCount()];

	//counter
	int count = 0;

	while( res->GetRow(row2) )
	{
		m_TargetAttributeIDs[count] = row2.GetInt(0);
		m_SourceAttributeIDs[count] = row2.GetInt(1);
		m_CalculationTypeID[count] = row2.GetInt(2);
		m_ReverseCalculationTypeID[count] = row2.GetInt(3);
        m_EffectAppliedWhenIDs[count] = row2.GetInt(4);
        m_EffectAppliedTargetIDs[count] = row2.GetInt(5);
		count++;
	}

	m_numOfIDs = count;

	delete res;
	res = NULL;
}


// ////////////////////// ModuleEffects Class ////////////////////////////

ModuleEffects::ModuleEffects(uint32 typeID) : m_TypeID( typeID ), m_Cached( false )
{
    m_TypeID = 0;
	m_EffectIDs = NULL;
	m_Effects = NULL;
	m_DefaultEffect = 0;
	m_EffectCount = 0;
	m_SelectedEffect = 0;
	m_HighPower = m_MediumPower = m_LowPower = m_Cached = false;

    _populate(typeID);
}

ModuleEffects::~ModuleEffects()
{
	//delete arrays
	delete[] m_Effects;
	delete[] m_EffectIDs;

	//null ptrs
	m_EffectIDs = NULL;
	m_Effects = NULL;
}

//useful accessors - probably a better way to do this, but at least it's fast
bool ModuleEffects::isHighSlot()
{
	if( m_Cached )
		return m_HighPower;
	else
	{
		for(uint32 i = 0; i < m_EffectCount; i++)
		{
			if( m_EffectIDs[i] == effectHiPower )
			{
				m_HighPower = true;
				m_MediumPower = false;
				m_LowPower = false;
				m_Cached = true; //cache the result
				return true;
			}
		}
	}

    return false;
}

bool ModuleEffects::isMediumSlot()
{
	if( m_Cached )
		return m_MediumPower;
	else
	{
		for(uint32 i = 0; i < m_EffectCount; i++)
		{
			if( m_EffectIDs[i] == effectMedPower )
			{
				m_HighPower = false;
				m_MediumPower = true;
				m_LowPower = false;
				m_Cached = true;  //cache the result
				return true;
			}
		}
	}

    return false;
}

bool ModuleEffects::isLowSlot()
{
	if( m_Cached )
		return m_LowPower;
	else
	{
		for(uint32 i = 0; i < m_EffectCount; i++)
		{
			if( m_EffectIDs[i] == effectLoPower )
			{
				m_HighPower = false;
				m_MediumPower = false;
				m_LowPower = true;
				m_Cached = true; //cache the result
				return true;
			}
		}
	}

    return false;
}

//this will need to be reworked to implement a singleton architecture...i'll do it later -luck

//this class uses a system to set the "active effect" that you want to get information about
//returns false if the effect specified is not found
bool ModuleEffects::SetActiveEffect(uint32 effectID)
{
	//iterate through the effects to find the one we want
	for(uint32 i = 0; i < m_EffectCount; i++)
	{
		if(m_EffectIDs[i] == effectID)
		{
			m_SelectedEffect = i;
			return true;
		}

	}

	return false;
}

bool ModuleEffects::SetDefaultEffectAsActive()
{
	//iterate through the effects to find the one we want
	if(m_DefaultEffect != 0)
	{
		m_SelectedEffect = m_DefaultEffect;
		return true;
	}

	return false;
}


// ////////////////// PRIVATE MEMBERS /////////////////////////

void ModuleEffects::_populate(uint32 typeID)
{
	//first get all of the effects associated with the typeID
	DBQueryResult *res = new DBQueryResult();
	ModuleDB::GetDgmTypeEffectsInformation(typeID, *res);

	//initialize our container to the correct size
	m_Effects = new MEffect[res->GetRowCount()];
	m_EffectIDs = new int[res->GetRowCount()];

	//counter
	int i = 0;

	//go through and populate each effect
	DBResultRow row;
	while( res->GetRow(row) )
	{
		//add new stuff to the arrays
		m_EffectIDs[i] = row.GetInt(0);
		m_Effects[i] = *new MEffect();

		//check if this is the default effect
		if( row.GetInt(1) )
			m_DefaultEffect = i;

		//populate the new MEffect
		m_Effects[i].Populate(row.GetInt(0));

		i++; //increment
	}

	m_EffectCount = i;

	//cleanup
	delete res;
	res = NULL;

}
