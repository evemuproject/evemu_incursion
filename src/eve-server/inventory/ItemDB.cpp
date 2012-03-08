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
	Author:		Zhur
*/


#include "EVEServerPCH.h"


PyRep *ItemDB::ListStations( uint32 characterID )
{
	DBQueryResult res;

	if( !sDatabase.RunQuery( res,
		"SELECT locationID AS stationID,"
		" count( itemID ) AS itemCount,"
		" count( invblueprints.blueprintID ) AS blueprintCount"
		" FROM entity"
		" LEFT JOIN invblueprints ON invblueprints.blueprintID = itemID"
		" WHERE ownerID=%u"
		" AND NOT locationID=%u"
		" AND flag=4"
		" GROUP BY locationID", characterID ))
	{
		_log(DATABASE__ERROR, "Cant find items for character %u in any station", characterID );
		return NULL;
	}

	return DBResultToCRowset( res );
}

PyRep *ItemDB::ListStationItems( uint32 characterID, uint32 stationID )
{
	DBQueryResult res;

	if( !sDatabase.RunQuery( res,
		"SELECT itemID,"
		" entity.typeID,"
		" quantity AS stacksize,"
		" locationID,"
		" flag AS flagID,"
		" singleton,"
		" contraband,"
		" invGroups.categoryID,"
		" invTypes.groupID"
		" FROM entity"
		" LEFT JOIN invTypes ON entity.typeID = invTypes.typeID"
		" LEFT JOIN invGroups ON invTypes.groupID = invGroups.groupID"
		" WHERE ownerID=%u"
		" AND locationID=%u"
		" AND flag=4", characterID, stationID ))
	{
		_log(DATABASE__ERROR, "Cant find items for character %u in station %u", characterID, stationID );
		return NULL;
	}

	return DBResultToCRowset( res );
}


PyRep *ItemDB::ListItems( uint32 characterID )
{
	DBQueryResult res;

	if( !sDatabase.RunQuery( res,
		"SELECT itemID,"
		" entity.typeID,"
		" quantity AS stacksize,"
		" locationID,"
		" flag AS flagID,"
		" singleton,"
		" contraband,"
		" ownerID,"
		" invGroups.categoryID,"
		" invTypes.groupID"
		" FROM entity"
		" LEFT JOIN invTypes ON entity.typeID = invTypes.typeID"
		" LEFT JOIN invGroups ON invTypes.groupID = invGroups.groupID"
		" WHERE ownerID=%u"
		" AND flag=4", characterID ))
	{
		_log( DATABASE__ERROR, "Cant find items for character %u", characterID );
		return NULL;
	}

	return DBResultToCRowset( res );
}
















