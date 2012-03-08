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
 * Office
 */
Office::Office(
    ItemFactory &_factory,
    uint32 _officeID,
    // InventoryItem stuff:
    const ItemType &_type,
    const ItemData &_data)
: InventoryItem(_factory, _officeID, _type, _data),
  m_officeFolderID( _data.locationID )
{
}

OfficeRef Office::Load(ItemFactory &factory, uint32 officeID)
{
    return InventoryItem::Load<Office>( factory, officeID );
}

template<class _Ty>
RefPtr<_Ty> Office::_LoadOffice(ItemFactory &factory, uint32 officeID,
    // InventoryItem stuff:
    const ItemType &itemType, const ItemData &data)
{
    // we don't need any additional stuff
    return OfficeRef( new Office( factory, officeID, itemType, data ) );
}

OfficeRef Office::Spawn(ItemFactory &factory,
    // InventoryItem stuff:
    ItemData &data
) {
    uint32 officeID = Office::_Spawn( factory, data );
    if( officeID == 0 )
        return OfficeRef();
    return Office::Load( factory, officeID );
}

uint32 Office::_Spawn(ItemFactory &factory,
    // InventoryItem stuff:
    ItemData &data
) {
    // make sure it's a cargo container
    const ItemType *st = factory.GetType(data.typeID);
    if(st == NULL)
        return 0;

    // store item data
    uint32 officeID = InventoryItem::_Spawn(factory, data);
    if(officeID == 0)
        return 0;

    // nothing additional

    return officeID;
}

double Office::GetCapacity(EVEItemFlags flag) const
{
	return 25802525.0; // Should this be infinite ?
}

bool Office::_Load()
{
    // load contents
    if( !LoadContents( m_factory ) )
        return false;

    return InventoryItem::_Load();
}

void Office::Delete()
{
    // delete contents first
    DeleteContents( m_factory );

    InventoryItem::Delete();
}

void Office::ValidateAddItem( EVEItemFlags flag, InventoryItemRef item, Client* c )
{
	/*
	CharacterRef character = c->GetChar();
	
	if( flag == flagCargoHold )
	{
		EvilNumber capacityUsed( 0 );

		std::vector<InventoryItemRef> items;
		c->GetShip()->FindByFlag( flag, items );

		for( size_t i = 0; i < items.size(); i ++ )
		{
			capacityUsed += items[ i ]->GetAttribute( AttrVolume );
		}

		if( capacityUsed + item->GetAttribute( AttrVolume ) > c->GetShip()->GetAttribute( AttrCapacity ) )
			throw PyException( MakeCustomError( "Not enough cargo space!") );
	}*/
}

void Office::AddItem( InventoryItemRef item )
{
	InventoryEx::AddItem( item );
}

void Office::RemoveItem( InventoryItemRef item )
{
	InventoryEx::RemoveItem( item );
}

