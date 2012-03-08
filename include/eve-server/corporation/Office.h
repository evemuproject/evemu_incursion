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

#ifndef __OFFICE__H__INCL__
#define __OFFICE__H__INCL__

class Office
: public InventoryItem,
  public InventoryEx
{
	friend class InventoryItem; // to let it construct us
public:
	/**
	 * Loads office.
	 *
	 * @param[in] factory
	 * @param[in] officeID ID of office to load.
	 * @return Pointer to new Office object; NULL if fails.
	 */
	static OfficeRef Load(ItemFactory &factory, uint32 officeID);

	/**
	 * Spawns office.
	 *
	 * @param[in] factory
	 * @param[in] data Item data for Office
	 * @return Pointer to new Office object; NULL if failed
	 */
	static OfficeRef Spawn(ItemFactory &factory, ItemData& data);
	
	void Delete();
	static void ValidateAddItem( EVEItemFlags flag, InventoryItemRef item, Client* c );
	double GetCapacity( EVEItemFlags flag) const;

	uint32 GetOfficeFolder() const { return m_officeFolderID; }
	
	const ItemType &    type() const { return static_cast<const ItemType &>(InventoryItem::type()); }

protected:
	Office(
		ItemFactory &_factory,
		uint32 _officeID,
		// InventoryItem stuff:
		const ItemType &_type,
		const ItemData &_data
		);

	/*
	 * Member functions:
	 */
	using InventoryItem::_Load;

	// Template loader:
	template<class _Ty>
	static RefPtr<_Ty> _LoadItem(ItemFactory &factory, uint32 officeID,
		// InventoryItem stuff:
		const ItemType &type, const ItemData &data)
	{
		// check it's a office
		if( type.id() != 27 )
		{
			_log( ITEM__ERROR, "Trying to load %s as Office.", type.group().name().c_str() );
			return RefPtr<_Ty>();
		}

		return _Ty::template _LoadOffice<_Ty>( factory, officeID, type, data );
	}

	// Actual loading stuff:
	template<class _Ty>
	static RefPtr<_Ty> _LoadOffice(ItemFactory &factory, uint32 officeID,
		const ItemType &type, const ItemData &data
	);

	bool _Load();
    static uint32 _Spawn(ItemFactory &factory, ItemData &data);

	uint32 inventoryID() const { return itemID(); }
	PyRep *GetItem() const { return GetItemRow(); }
	
	void AddItem( InventoryItemRef item );
	void RemoveItem( InventoryItemRef item );

	uint32 m_officeFolderID;
};

 #endif /* !__OFFICE__H__INCL__ */
 
 