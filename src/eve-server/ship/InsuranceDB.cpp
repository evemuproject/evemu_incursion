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
	Author:		Almamu
*/

#include "EVEServerPCH.h"

PyRep* InsuranceDB::GetContracts( const std::vector<int32> &ships, uint32 characterID )
{
	DBQueryResult res;
	DBResultRow row;

	std::string ids;
	ListToINString(ships, ids, "-1");

	if( !sDatabase.RunQuery( res,
		"SELECT"
		" ownerID,"
		" shipID,"
		" fraction,"
		" startDate,"
		" endDate"
		" FROM chrshipinsurances"
		" WHERE ownerID=%u"
		" AND shipID IN (%s)", characterID, ids.c_str() ) )
	{
		_log(DATABASE__ERROR, "Can't fetch insurances for characterID %u. Error: %s", characterID, res.error.c_str() );
		return new PyNone;
	}

	return DBResultToPackedRowList( res );
}


bool InsuranceDB::GetStationItems( std::vector<int32> &into, uint32 stationID, uint32 characterID )
{
	DBQueryResult res;
	DBResultRow row;

	if( !sDatabase.RunQuery( res,
		"SELECT"
		" itemID"
		" FROM entity"
		" WHERE ownerID=%u"
		" AND locationID=%u", characterID, stationID ))
	{
		_log(DATABASE__ERROR, "Can't fetch items for character %u in station %u. Error: %s", characterID, stationID, res.error.c_str() );
		return false;
	}

	if( res.GetRowCount() == 0 )
	{
		return false;
	}

	while( res.GetRow( row ) )
	{
		into.push_back( row.GetUInt( 0 ) );
	}

	return true;
}


bool InsuranceDB::InsureShip( uint32 shipID, uint32 ownerID, double fraction )
{
	DBerror err;
	uint64 endDate = Win32TimeNow() + ( ( Win32Time_Day * 7 ) * 12 );

	if( !sDatabase.RunQuery( err,
		"INSERT INTO chrshipinsurances(ownerID,"
		" shipID,"
		" fraction,"
		" startDate,"
		" endDate"
		")VALUES("
		"%u, %u, %f, " I64u ", " I64u ")", ownerID, shipID, fraction, Win32TimeNow(), endDate ) )
	{
		_log(DATABASE__ERROR, "Cant insure ship %u for character %u. Error: %s", shipID, ownerID, err.c_str() );
		return false;
	}

	return true;
}


bool InsuranceDB::UnInsureShip( uint32 shipID, uint32 characterID )
{
	DBerror err;

	if( !sDatabase.RunQuery( err,
		"DELETE FROM chrshipinsurances"
		" WHERE ownerID=%u"
		" AND shipID=%u", characterID, shipID ))
	{
		_log(DATABASE__ERROR, "Cant UnInsure ship %u for character %u. Error: %s", shipID, characterID, err.c_str() );
		return false;
	}

	return true;
}




