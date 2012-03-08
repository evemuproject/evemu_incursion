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
    Author:     Almamu
*/

#include "EVEServerPCH.h"

/*
 * OfficeFolder
 */

OfficeFolder::OfficeFolder(
    ItemFactory &_factory,
    uint32 _officeFolderID,
    // InventoryItem stuff:
    const ItemType &_itemType,
    const ItemData &_data)
: InventoryItem(_factory, _officeFolderID, _itemType, _data) {}

OfficeFolderRef OfficeFolder::Load(ItemFactory &factory, uint32 officeFolderID)
{
    return InventoryItem::Load<OfficeFolder>( factory, officeFolderID );
}

template<class _Ty>
RefPtr<_Ty> OfficeFolder::_LoadOfficeFolder(ItemFactory &factory, uint32 officeFolderID,
    // InventoryItem stuff:
    const ItemType &itemType, const ItemData &data)
{
    // we don't need any additional stuff
    return OfficeFolderRef( new OfficeFolder( factory, officeFolderID, itemType, data ) );
}

OfficeFolderRef OfficeFolder::Spawn(ItemFactory &factory,
    // InventoryItem stuff:
    ItemData &data
) {
    uint32 officeFolderID = OfficeFolder::_Spawn( factory, data );
    if( officeFolderID == 0 )
        return OfficeFolderRef();
    return OfficeFolder::Load( factory, officeFolderID );
}

uint32 OfficeFolder::_Spawn(ItemFactory &factory,
    // InventoryItem stuff:
    ItemData &data
) {
    // make sure it's a OfficeFolder
    const ItemType *st = factory.GetType(data.typeID);
    if(st == NULL)
        return 0;

    // store item data
    uint32 officeFolderID = InventoryItem::_Spawn(factory, data);
    if(officeFolderID == 0)
        return 0;

    // nothing additional

    return officeFolderID;
}

bool OfficeFolder::_Load()
{
    // load contents
    if( !LoadContents( m_factory ) )
        return false;

    return InventoryItem::_Load();
}

void OfficeFolder::Delete()
{
    // delete contents first
    DeleteContents( m_factory );

    InventoryItem::Delete();
}

double OfficeFolder::GetCapacity(EVEItemFlags flag ) const
{
	return 0.0;
}

void OfficeFolder::ValidateAddItem(EVEItemFlags flag, InventoryItemRef item, Client *c)
{
}

uint32 OfficeFolder::GetOfficeSlotsLeft() const
{
	std::vector<InventoryItemRef> items;
	FindByFlagRange( flagOfficeSlotFirst, flagOfficeSlotLast, items );

	return 16 - items.size();
}

OfficeRef OfficeFolder::InstallOffice( uint32 corporationID )
{
	// Check how many office slots are used
	std::vector<InventoryItemRef> items;
	std::map<uint32, bool> used;
	uint32 count = 0;
	int32 firstUnused = -1;

	FindByFlagRange( flagOfficeSlotFirst, flagOfficeSlotLast, items );

	for( size_t i = 0; i < items.size(); i ++ )
	{
		uint32 slot = items[ i ]->flag() - flagOfficeSlotFirst;
		used.insert( std::make_pair<uint32, bool>( slot, true ) );
		count ++;
	}

	for( size_t i = 0; i < 16; i ++ )
	{
		// We've got and empty slot
		if( used.find( i ) == used.end() )
		{
			firstUnused = i;
			break;
		}
	}

	if( firstUnused == -1 )
		return OfficeRef();

	uint32 officeID = 0;
	if( m_factory.db().NewOffice( itemID(), corporationID, officeID ) == false )
	{
		return OfficeRef();
	}

	AddItem( m_factory.GetItem( officeID ) );

	return Office::Load( m_factory, officeID );
}

void OfficeFolder::UninstallOffice( uint32 officeID )
{
	// TODO: Check if the office is under this office folder
	RemoveItem( officeID );
}

