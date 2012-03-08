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

#ifndef __OFFICEFOLDER__H__INCL__
#define __OFFICEFOLDER__H__INCL__

// TODO: We may need to create StructureTypeData and StructureType classes just as Ship.h/Ship.cpp
// has in order to load up type data specific to structures.  For now, the generic ItemType class is used.

/**
 * InventoryItem which represents OfficeFolder.
 */
class OfficeFolder
: public InventoryItem,
  public InventoryEx
{
	friend class InventoryItem;	// to let it construct us
public:
	/**
	 * Loads OfficeFolder from DB
	 *
	 * @param[in] factory
	 * @param[in] officeFolderID ID of folder to load.
	 * @return Pointer to OfficeFolder object; NULL if failed.
	 */
	static OfficeFolderRef Load(ItemFactory &factory, uint32 officeFolderID);
	/**
	 * Spawns new OfficeFolder.
	 *
	 * @param[in] factory
	 * @param[in] data Item data for OfficeFolder.
	 * @return Pointer to new OfficeFolder object; NULL if failed.
	 */
	static OfficeFolderRef Spawn(ItemFactory &factory, ItemData &data);

	/*
	 * Primary public interface:
	 */
	uint32 GetOfficeSlotsLeft() const;
	OfficeRef InstallOffice( uint32 corporationID );
	void UninstallOffice( uint32 officeID );
	void Delete();
	double GetCapacity(EVEItemFlags flag) const;

	/*
	 * _ExecAdd validation interface:
	 */
	static void ValidateAddItem(EVEItemFlags flag, InventoryItemRef item, Client *c);

	/*
	 * Public fields:
	 */
	const ItemType &    type() const { return static_cast<const ItemType &>(InventoryItem::type()); }

protected:
	OfficeFolder(
		ItemFactory &_factory,
		uint32 _officeFolderID,
		// InventoryItem stuff:
		const ItemType &_itemType,
		const ItemData &_data
	);

	/*
	 * Member functions
	 */
	using InventoryItem::_Load;

	// Template loader:
	template<class _Ty>
	static RefPtr<_Ty> _LoadItem(ItemFactory &factory, uint32 officeFolderID,
		// InventoryItem stuff:
		const ItemType &type, const ItemData &data)
	{
		// check if it's a structure
        if( type.id() != 26 )
		{
			_log( ITEM__ERROR, "Trying to load %s as OfficeFolder.", type.category().name().c_str() );
			return RefPtr<_Ty>();
		}
		//// cast the type
		//const ItemType &itemType = static_cast<const ItemType &>( type );

		// no additional stuff

		return _Ty::template _LoadOfficeFolder<_Ty>( factory, officeFolderID, type, data );
	}

	// Actual loading stuff:
	template<class _Ty>
	static RefPtr<_Ty> _LoadOfficeFolder(ItemFactory &factory, uint32 officeFolderID,
		// InventoryItem stuff:
		const ItemType &itemType, const ItemData &data
	);

	bool _Load();

	static uint32 _Spawn(ItemFactory &factory,
		// InventoryItem stuff:
		ItemData &data
	);

	uint32 inventoryID() const { return itemID(); }
	PyRep *GetItem() const { return GetItemRow(); }
};

#endif /* !__OFFICEFOLDER__H__INCL__ */


